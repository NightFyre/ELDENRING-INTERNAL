#include "Menu.hpp"
#include "ImGui/imgui.h"
#include "GameDataMan.hpp"
#include "WorldCharMan.hpp"
#include "Console.hpp"

bool m_BOXES = FALSE;
bool debugDRAW = FALSE;
bool m_dbgSkeleton = FALSE;
bool m_dbgHitBoxes = FALSE;
bool m_dbgCrosshair = FALSE;
bool dbg_FPS = FALSE;
bool m_dbgFreezeTarget = FALSE;
bool bESP = FALSE;

//  WINDOWS
bool m_dbgEntityWnd = FALSE;
bool m_dbgMatrixWnd = FALSE;

//  
float Matrix[16];
float ViewMatrix[16];

/// pDrawList
//ImDrawList* pDrawList;
//ImDrawList* DrawList = ImGui::GetWindowDrawList();
//ImColor Orange = ImColor(255, 84, 0, 255);
//void DrawMisc(ImDrawList* DrawList)
//{
//    //Get Window size will crash
//    auto& displaySize = ImGui::GetIO().DisplaySize;
//
//    //Where we will draw our text on the screen.
//    float WindowX = 0, WindowY = 20;
//
//    //Allowing us to clean up a bit.
//    ImVec2 TextLocationToDraw(WindowX, WindowY);
//
//    //Get our drawlist to draw in.
//    pDrawList = DrawList;
//
//    //Allow some memory allocation so we can store our strings.
//    char buffer[500];
//
//    //Draw our string.
//    //sprintf(buffer, "NBOTT42 - Elden Ring Rekage 0.01 Beta");
//    //pDrawList->AddRect(ImVec2(25, 25), ImVec2(25, 25), ImColor(255, 0, 0, 0));
//
//    //Clear the buffer to add other text.
//    ZeroMemory(buffer, 500);
//    return;
//}
//
//void BoxesAreGud()
//{
//    DrawMisc(DrawList);
//    return;
//}

uintptr_t p2addy(uintptr_t PTR, std::vector<unsigned int> OFFSETS)
{
    uintptr_t addr = PTR;
    for (unsigned int i = 0; i < OFFSETS.size(); i++) {
        addr = *(uintptr_t*)addr;
        addr += OFFSETS[i];
    }
    return addr;
}

namespace ER {

    void FPS()
    {
        static bool init = false;
        static char text[64] = "FPS: unknown";
        static int frame = 0;
        static clock_t time = clock();
        frame++;
        if (clock() - time >= 1000)
        {
            memset(text, 0, sizeof(text));
            sprintf(text, "FPS: %i\n", frame);
            frame = 0;
            time = clock();
            init = true;
        }

        if (init && text[0])
            ImGui::GetBackgroundDrawList()->AddText(ImVec2(25, 25), ImColor(255, 255, 255, 255), text);
    }

    bool WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight)
    {
        //Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
        Vector4 clipCoords;
        clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
        clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
        clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
        clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

        if (clipCoords.w < 0.1f)
            return false;

        //perspective division, dividing by clip.W = Normalized Device Coordinates
        Vector3 NDC;
        NDC.x = clipCoords.x / clipCoords.w;
        NDC.y = clipCoords.y / clipCoords.w;
        NDC.z = clipCoords.z / clipCoords.w;

        screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
        screen.y = (windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
        return true;
    }

    void ESP()
    {
        if (g_WorldCharMan->pCharData->Health == 0)
            return;

        //  Update Entity Info
        g_WorldCharMan->Update();

        ///  Filter Entity Results
        int count = 0;
        Vector2 vecScreen;
        uintptr_t ViewMatrix = p2addy(g_GameVariables->m_ModuleBase + 0x03C61588, { 0x60, 0x60, 0x420 });
        memcpy(&Matrix, (BYTE*)ViewMatrix, sizeof(Matrix));
        for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {

            //  COMPARE WITH PLAYER
            if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase)
                continue;

            //  HEALTH CHECK
            if (g_WorldCharMan->CharData[i]->Health == NULL)
                continue;

            //  POSITION CHECK
            if (g_WorldCharMan->CharPhysics[i]->Position == g_WorldCharMan->pCharPhysics->Position)
                continue;

            if (WorldToScreen(g_WorldCharMan->CharPhysics[i]->Position, vecScreen, Matrix, ImGui::GetWindowWidth(), ImGui::GetWindowHeight())) {
                ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecScreen.x, vecScreen.y), ImColor(255, 0, 0, 255), std::to_string(count).c_str());
                count++;
            }
        }
        g_WorldCharMan->count = NULL;
        count = NULL;
    }

    //  DEBUG ESP WINDOW
    void DrawEntInfo()
    {
        //  Update Entity Info
        g_WorldCharMan->Update();

        ///  Filter Entity Results
        int count = 0;
        Vector2 vecScreen;
        for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {
            
            //  COMPARE WITH PLAYER
            if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase)
                continue;

            //  HEALTH CHECK
            if (g_WorldCharMan->CharData[i]->Health == NULL)
                continue;

            //  POSITION CHECK
            if (g_WorldCharMan->CharPhysics[i]->Position == g_WorldCharMan->pCharPhysics->Position)
                continue;

            //  DRAW
            if (WorldToScreen(g_WorldCharMan->CharPhysics[i]->Position, vecScreen, ViewMatrix, ImGui::GetWindowWidth(), ImGui::GetWindowHeight())) {
                ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecScreen.x, vecScreen.y), ImColor(0, 0, 255, 255), std::to_string(count).c_str());
                count++;
            }
        }
        g_WorldCharMan->count = NULL;
        count = NULL;
    }

    //  CAUSES CRASH
    void drawSKELETON(bool ENABLED)
    {
        if (!g_GameDataMan->Valid())
            return;
        g_WorldCharMan->Update();
        if (ENABLED) {
            for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {
                if (g_WorldCharMan->CharData[i]->Health != 0) {
                    if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 0)
                        g_WorldCharMan->CharFall[i]->DrawSkeleton = 1;
                }
            }
            g_Console->printdbg("[+] DRAW SKELETONS : ON\n", TRUE, g_Console->color.green);
        } 
        else {
            for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {
                if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
                    g_WorldCharMan->CharFall[i]->DrawSkeleton = 0;
            }
            g_Console->printdbg("[+] DRAW SKELETONS : OFF\n", TRUE, g_Console->color.red);
        }
        g_WorldCharMan->count = NULL;
        return;

        //  OLD
        /// OBTAIN SIZE OF ARRAY 
        //  - p = Valueof(WorldCharManager)
        //  - begin = Valueof(p + 0x18F10)
        //  - finish = Valueof( p + 0x18F18)
        //  - p = Valueof(begin+count*8)
        //uintptr_t basePOINTER = *(uintptr_t*)(g_GameVariables->m_ModuleBase + 0x3C64E38);
        //uintptr_t bADDR = *(uintptr_t*)(basePOINTER + 0x18F10);
        //unsigned int begin = *(unsigned int*)(basePOINTER + 0x18F10);
        //unsigned int finish = *(unsigned int*)(basePOINTER + 0x18F18);
        //int SIZE = (finish - begin) / 8;
        //
        ///// LOOP ARRAY
        //for (int i = 0; i <= SIZE - 1; i = i + 1)
        //{
        //    //  CURRENT ENTITY
        //    uintptr_t EntityArrayPointer = *(uintptr_t*)(bADDR + i * 8);
        //    uintptr_t BASE4 = EntityArrayPointer + 0x190;
        //
        //    //  BRANCH FOR SKELETON
        //    uintptr_t brSKELETON = *(uintptr_t*)BASE4 + 0x28;
        //    uintptr_t DebugSkeleton = *(uintptr_t*)brSKELETON + 0x1904;          //  SKELETON BOOL ADDRESS
        //
        //    //  BRANCH FOR HEALTH
        //    uintptr_t brHP = *(uintptr_t*)BASE4 + 0x0;
        //    uintptr_t HEALTH = *(uintptr_t*)brHP + 0x138;
        //
        //    //  BRANCH FOR COORDS
        //    uintptr_t brCOORDS = *(uintptr_t*)BASE4 + 0x68;
        //    uintptr_t PlayerPosX = *(uintptr_t*)brCOORDS + 0x70;
        //    uintptr_t PlayerPosY = *(uintptr_t*)brCOORDS + 0x74;
        //    uintptr_t PlayerPosZ = *(uintptr_t*)brCOORDS + 0x78;
        //
        //    //  WRITE
        //    if (*(int*)DebugSkeleton == 0)
        //        *(int*)DebugSkeleton = 1;
        //    else
        //        *(int*)DebugSkeleton = 0;
        //}
        //return;
    }    
    
    //  GENOCIDE
    void killENTS()
    {
        if (!g_GameDataMan->Valid()) {
            g_Console->printdbg("[+] VALIDATION FAILED : {GameDataMan} . . .\n", TRUE, g_Console->color.red);
            return;
        }
        g_WorldCharMan->Update();

        int count = 0;
        for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {

            //  COMPARE OBJECT POINTERS
            if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase)
                continue;

            //  PLAYER POSITION FILTER
            if (Vector3(g_WorldCharMan->CharPhysics[i]->Position) == Vector3(g_WorldCharMan->pCharPhysics->Position))
                continue;

            if (g_WorldCharMan->CharData[i]->Health != NULL)
                g_WorldCharMan->CharData[i]->Health = NULL;
            count++;
        }
        printf("KILLED: %d ENTITIES\n", count);
        count = NULL;
        g_WorldCharMan->count = NULL;
        return;
    }

    //  CHAR STATS
    void CharacterStats()
    {
        if (!g_GameDataMan->Valid())
            return;
        if (!g_WorldCharMan->Valid())
            return;

        ImGui::Text("CHARATER ATTRIBUTES");
        ImGui::Spacing();

        ImGui::Text("Runes:        ");
        ImGui::SameLine();
        ImGui::InputInt("##Runes", (int*)&g_GameDataMan->GameData->CurrentRunes);

        ImGui::Text("Level:        ");
        ImGui::SameLine();
        ImGui::InputInt("##Level", (int*)&g_GameDataMan->GameData->Level);

        ImGui::Text("Vigor:        ");
        ImGui::SameLine();
        ImGui::InputInt("##Vigor", (int*)&g_GameDataMan->GameData->Stats.Vigor);

        ImGui::Text("Mind:         ");
        ImGui::SameLine();
        ImGui::InputInt("##Mind", (int*)&g_GameDataMan->GameData->Stats.Mind);

        ImGui::Text("Endurance:    ");
        ImGui::SameLine();
        ImGui::InputInt("##Endurance", (int*)&g_GameDataMan->GameData->Stats.Endurance);

        ImGui::Text("Strength:     ");
        ImGui::SameLine();
        ImGui::InputInt("##Strength", (int*)&g_GameDataMan->GameData->Stats.Strength);

        ImGui::Text("Dexterity:    ");
        ImGui::SameLine();
        ImGui::InputInt("##Dexterity", (int*)&g_GameDataMan->GameData->Stats.Dexterity);

        ImGui::Text("Intelligence: ");
        ImGui::SameLine();
        ImGui::InputInt("##Intelligence", (int*)&g_GameDataMan->GameData->Stats.Intelligence);

        ImGui::Text("Faith:        ");
        ImGui::SameLine();
        ImGui::InputInt("##Faith", (int*)&g_GameDataMan->GameData->Stats.Faith);

        ImGui::Text("Arcane:       ");
        ImGui::SameLine();
        ImGui::InputInt("##Arcane", (int*)&g_GameDataMan->GameData->Stats.Arcane);

        ImGui::Separator();
        ImGui::Text("CHARACTER STATS");
        ImGui::Spacing();


        ImGui::Text("Health        ");
        ImGui::SameLine();
        ImGui::InputInt("##Health", &g_WorldCharMan->pCharData->Health);

        ImGui::Text("Mana          ");
        ImGui::SameLine();
        ImGui::InputInt("##Mana", &g_WorldCharMan->pCharData->Mana);

        ImGui::Text("Stamina       ");
        ImGui::SameLine();
        ImGui::InputInt("##Stamina", &g_WorldCharMan->pCharData->Stamina);

        ImGui::Text("Azimuth       ");
        ImGui::SameLine();
        ImGui::InputFloat("##Azimuth", &g_WorldCharMan->pCharPhysics->Azimuth);

        ImGui::Text("PosX          ");
        ImGui::SameLine();
        ImGui::InputFloat("##PosX", &g_WorldCharMan->pCharPhysics->Position.x);

        ImGui::Text("PosY          ");
        ImGui::SameLine();
        ImGui::InputFloat("##PosY", &g_WorldCharMan->pCharPhysics->Position.y);

        ImGui::Text("PosZ          ");
        ImGui::SameLine();
        ImGui::InputFloat("##PosZ", &g_WorldCharMan->pCharPhysics->Position.z);

    }

    //  ENTITY TAB
    void EntityStats()
    {
        if (!g_GameDataMan->Valid()) {
            g_Console->printdbg("[!] GameDataMan::Valid - FAILED; {EntityStats}", TRUE, g_Console->color.red);
            return;
        }

        if (!g_WorldCharMan->Valid()) {
            g_Console->printdbg("[!] WorldCharMan::Valid - FAILED; {EntityStats}", TRUE, g_Console->color.red);
            return;
        }

        if (ImGui::Button("KILL ALL ENTITIES", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
            g_Console->printdbg("[+] MENU:: KILL ALL ENTITIES\n", TRUE, g_Console->color.green);
            killENTS();
        }
        ImGui::Spacing();

        if (ImGui::Button("FREEZE ALL ENTITIES", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
            g_Console->printdbg("[!] MENU:: FREEZE ALL ENTITIES [NOT-IMPLEMENTED]\n", TRUE, g_Console->color.red);
        }
        ImGui::Spacing();

        if (ImGui::Button("TELEPORT ALL TO CROSSHAIR", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
            g_Console->printdbg("[!] MENU:: TELEPORT ALL TO CROSSHAIR\n", TRUE, g_Console->color.red);
            g_WorldCharMan->Update();

            for (int i = 0; i < g_WorldCharMan->arraySIZE - 1; i++) {
                g_WorldCharMan->CharPhysics[i]->Position = g_WorldCharMan->pCharPhysics->Position;
            }
            printf("TELEPORTED: %d ENTITIES\n", g_WorldCharMan->arraySIZE);
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Toggle("ESP", &bESP)) {
            if (bESP)
                g_Console->printdbg("[+] MENU:: ESP; ON\n", TRUE, g_Console->color.green);
            else
                g_Console->printdbg("[+] MENU:: ESP; OFF\n", TRUE, g_Console->color.red);
        }
        ImGui::Spacing();

        if (ImGui::Toggle("DRAW HITBOXES", &m_dbgHitBoxes))
        {
            if (m_dbgHitBoxes) {
                *(int8_t*)(g_GameVariables->m_ModuleBase + 0x3C68EF0 + 0xF) = 1;
                g_Console->printdbg("[+] MENU:: HIT-BOXES; ON\n", TRUE, g_Console->color.green);
            }
            else {
                *(int8_t*)(g_GameVariables->m_ModuleBase + 0x3C68EF0 + 0xF) = 0;
                g_Console->printdbg("[+] MENU:: HIT-BOXES; OFF\n", TRUE, g_Console->color.red);
            }
        }
        ImGui::Spacing();

        if (ImGui::Toggle("DISPLAY ENTITY ARRAY", &m_dbgEntityWnd)) {
            if (m_dbgEntityWnd) {
                g_Console->printdbg("[+] MENU::EntityArray; OPEN\n", TRUE, g_Console->color.green);
                g_WorldCharMan->Update();
            }
            else {
                g_Console->printdbg("[+] MENU::EntityArray; CLOSED\n", TRUE, g_Console->color.red);
                g_WorldCharMan->count = NULL;
            }
        }
    }

    //  DEBUG
    void DebugOptions()
    {
        if (!g_GameDataMan->Valid())
            return;

        ///  DEBUG POINTER CHAINS
        ImGui::Text("DEBUG POINTER CHAINS");
        ImGui::Spacing();
        ImGui::Text("WorldCharMan Base: %llX", (uintptr_t)g_WorldCharMan->Base);
        ImGui::Text("ptr: %llX", (uintptr_t)g_WorldCharMan->Ptr);
        ImGui::Text("begin: %u", g_WorldCharMan->Begin);
        ImGui::Text("finish: %u", g_WorldCharMan->Finish);
        ImGui::Text("size: %d", g_WorldCharMan->arraySIZE);
        ImGui::Text("count: %d", g_WorldCharMan->count);
        ImGui::Spacing();
        ImGui::Text("EntityObjectBase: %llX", (uintptr_t)g_WorldCharMan->EntityObjectBase[0]);
        ImGui::Text("pEntityObject: %llX", (uintptr_t)g_WorldCharMan->EntityObjectBase[0]->EntObjectPTR);
        ImGui::Text("CharData: %llX", (uintptr_t)g_WorldCharMan->CharData[0]);
        ImGui::Text("CharFall: %llX", (uintptr_t)g_WorldCharMan->CharFall[0]);
        ImGui::Text("CharPhysics: %llX", (uintptr_t)g_WorldCharMan->CharPhysics[0]);
        ImGui::Separator();

        ImGui::Toggle("DRAW FPS", &dbg_FPS);
        ImGui::Toggle("DRAW CROSSHAIR", &m_dbgCrosshair);
        if (ImGui::Toggle("DRAW SKELETON", &m_dbgSkeleton)) {
            if (m_dbgSkeleton)
                drawSKELETON(TRUE);
            else
                drawSKELETON(FALSE);
        }
        if (ImGui::Toggle("DEBUG ESP", &m_dbgMatrixWnd)) {
            if (m_dbgMatrixWnd) {
                g_Console->printdbg("[+] MENU:: ESP; ON\n", TRUE, g_Console->color.green);
                m_BOXES = TRUE;
            }
            else if (!m_dbgMatrixWnd) {
                g_Console->printdbg("[+] MENU:: ESP; OFF\n", TRUE, g_Console->color.red);
                m_BOXES = FALSE;
            }
        }
        ImGui::Separator();

        ImGui::Toggle("SHOW CONSOLE", &g_GameVariables->m_ShowConsole);
        ImGui::Checkbox("Verbose Logging", &g_Console->verbose);
        ImGui::Separator();

        if (ImGui::Button("INITIALIZE STYLE", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            g_Menu->InitStyle();
        ImGui::Separator();

        if (ImGui::Button("QUIT GAME", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            TerminateProcess(g_GameVariables->m_GameHandle, EXIT_SUCCESS);
    }

    void Menu::Draw()
    {
        // MAIN WINDOW
        if (g_GameVariables->m_ShowMenu) { 
            IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");
            ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
            if (!ImGui::Begin("ELDEN RING INTERNAL DEBUG", &g_GameVariables->m_ShowMenu, 96))
            {
                ImGui::End();
                return;
            }

            if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("MAIN"))
                {
                    CharacterStats();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("ENTITY OPTIONS"))
                {
                    EntityStats();
                    ImGui::EndTabItem();
                }
                 
                if (ImGui::BeginTabItem("DEBUG"))
                {
                    DebugOptions();
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }

        //  ENTITY ARRAY WINDOW
        if (m_dbgEntityWnd && g_GameVariables->m_ShowMenu)
        {
            ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
            if (!ImGui::Begin("ENTITY ARRAY", &m_dbgEntityWnd, 96)) {

                //  HANDLE EXCEPTION
                if (!g_GameDataMan->Valid()) {
                    ImGui::End();
                    m_dbgEntityWnd = FALSE;
                    return;
                }                
                
                if (!g_WorldCharMan->Valid()) {
                    ImGui::End();
                    m_dbgEntityWnd = FALSE;
                    return;
                }
            }

            for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1)
            {
                ImGui::PushID(i);
                //  ENTITY OBJECT HEADER
                std::string a = "ENTITY OBJECT ";
                std::string b = std::to_string(i);
                std::string c = a + b;
                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                if (ImGui::CollapsingHeader(c.c_str()))
                {
                    if (ImGui::Button("KILL TARGET", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
                        g_WorldCharMan->CharData[i]->Health = 0;
                    ImGui::Spacing();

                    if (ImGui::Button("TELEPORT TO TARGET", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
                        g_WorldCharMan->pCharPhysics->Position = g_WorldCharMan->CharPhysics[i]->Position;
                        g_Console->printdbg("[+] MENU::EntityList; TELEPORT TO TARGET\n", TRUE, g_Console->color.green);
                        printf("COORDINATES:\nPosX: %f\n PosY: %f\n PosZ: %f\n", g_WorldCharMan->CharPhysics[i]->Position.x,  
                            g_WorldCharMan->CharPhysics[i]->Position.y, g_WorldCharMan->CharPhysics[i]->Position.z);
                    }
                    ImGui::Spacing();

                    if (ImGui::Button("TELEPORT TO CROSSHAIR", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
                        g_WorldCharMan->CharPhysics[i]->Position = g_WorldCharMan->pCharPhysics->Position;
                        g_Console->printdbg("[+] MENU::EntityList; TELEPORT TO CROSSHAIR\n", TRUE, g_Console->color.green);
                        printf("COORDINATES:\nPosX: %f\n PosY: %f\n PosZ: %f\n", g_WorldCharMan->CharPhysics[i]->Position.x,
                            g_WorldCharMan->CharPhysics[i]->Position.y, g_WorldCharMan->CharPhysics[i]->Position.z);
                    }
                    ImGui::Spacing();
                    
                    ImGui::Text("FREEZE TARGET: ");
                    ImGui::SameLine();
                    if (ImGui::Toggle("##FREEZE TARGET", &m_dbgFreezeTarget)) {
                        g_Console->printdbg("[+] MENU::EntityList; FREEZE TARGET\n", TRUE, g_Console->color.red);
                    }
                    ImGui::Spacing();


                    ImGui::Text("DRAW SKELETON: ");
                    ImGui::SameLine();
                    ImGui::Toggle("##DrawSkele", &g_WorldCharMan->CharFall[i]->DrawSkeleton);
                    ImGui::Spacing();

                    ImGui::Text("HEALTH:  ");
                    ImGui::SameLine();
                    ImGui::InputInt("##Health", &g_WorldCharMan->CharData[i]->Health, 1, 10);

                    ImGui::Text("MANA:    ");
                    ImGui::SameLine();
                    ImGui::InputInt("##Mana", &g_WorldCharMan->CharData[i]->Mana, 1, 10);

                    ImGui::Text("STAMINA: ");
                    ImGui::SameLine();
                    ImGui::InputInt("##Stamina", &g_WorldCharMan->CharData[i]->Stamina, 1, 10);

                    ImGui::Text("PosX:    ");
                    ImGui::SameLine();
                    ImGui::InputFloat("##PosX", &g_WorldCharMan->CharPhysics[i]->Position.x, 1.0f, 10.0f);

                    ImGui::Text("PosY:    ");
                    ImGui::SameLine();
                    ImGui::InputFloat("##PosY", &g_WorldCharMan->CharPhysics[i]->Position.y, 1.0f, 10.0f);

                    ImGui::Text("PosZ:    ");
                    ImGui::SameLine();
                    ImGui::InputFloat("##PosZ", &g_WorldCharMan->CharPhysics[i]->Position.z, 1.0f, 10.0f);
                    ImGui::Spacing();
                    ImGui::Separator();
                }
                ImGui::PopID();
                ImGui::Spacing();
            }

            ImGui::End();
        }
        else if (!g_GameVariables->m_ShowMenu) {
            !m_dbgEntityWnd;
        }

        //  DEBUG MATRIX WINDOW
        if (m_dbgMatrixWnd && g_GameVariables->m_ShowMenu) {
            ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
            if (!ImGui::Begin("MATRIX FLOATS", &m_dbgMatrixWnd, 96)) {
                ImGui::End();
                return;
            }

            for (int i = 0; i <= 15; i = i + 1)
            {
                ImGui::PushID(i);
                std::string a = "MATRIX [" + std::to_string(i) + "]: ";
                std::string b = "##MATRIX [" + std::to_string(i) + "]: ";
                ImGui::Text(a.c_str());
                ImGui::SameLine();
                ImGui::SliderFloat(b.c_str(), &ViewMatrix[i], -30.0f, 30.0f, "%.2f");
                ImGui::SameLine();
                if (ImGui::Button("+"))
                    ViewMatrix[i] += .10f;
                ImGui::SameLine();
                if (ImGui::Button("-"))
                    ViewMatrix[i] -= .10f;
                ImGui::SameLine();
                if (ImGui::Button("RESET"))
                    ViewMatrix[i] = 0.0f;
                ImGui::PopID();
            }

            if (ImGui::Button("RESET ALL", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            {
                for (int i = 0; i <= 15; i = i + 1)
                    ViewMatrix[i] = 0;
            }
            
            ImGui::End();
        }
        else if (!g_GameVariables->m_ShowMenu) {
            !m_dbgMatrixWnd;
            !m_BOXES;
        }

        //  DRAW FPS
        if (dbg_FPS)
            FPS();

        //  DEBUG DRAW CROSSHAIR
        if (m_dbgCrosshair)
            ImGui::GetBackgroundDrawList()->AddCircle(ImGui::GetMainViewport()->GetCenter(), 25.0f, IM_COL32(255, 255, 255, 255), 100, 1.0f);

        //  SHOW DEBUG CONSOLE
        if (g_GameVariables->m_ShowConsole)
            ::ShowWindow(GetConsoleWindow(), SW_SHOW);
        else
            ::ShowWindow(GetConsoleWindow(), SW_HIDE);

        //  ESP
        if (bESP)
            ESP();

        //  DEBUG ESP
        if (m_BOXES)
            DrawEntInfo();
    }

    void Menu::InitStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(0.75f, 0.75f, 0.29f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.27f, 0.00f, 0.63f, 0.43f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.74f, 0.74f, 0.29f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
        colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.90f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.WindowPadding = ImVec2(8.00f, 8.00f);
        style.FramePadding = ImVec2(5.00f, 2.00f);
        style.CellPadding = ImVec2(6.00f, 6.00f);
        style.ItemSpacing = ImVec2(6.00f, 6.00f);
        style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
        style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        style.IndentSpacing = 25;
        style.ScrollbarSize = 15;
        style.GrabMinSize = 10;
        style.WindowBorderSize = 1;
        style.ChildBorderSize = 1;
        style.PopupBorderSize = 1;
        style.FrameBorderSize = 1;
        style.TabBorderSize = 1;
        style.WindowRounding = 7;
        style.ChildRounding = 4;
        style.FrameRounding = 3;
        style.PopupRounding = 4;
        style.ScrollbarRounding = 9;
        style.GrabRounding = 3;
        style.LogSliderDeadzone = 4;
        style.TabRounding = 4;
        m_StyleInit = true;

        g_Console->printdbg("[+] MENU STYLE INITIALIZED\n", TRUE, g_Console->color.green);
    }
}