#include "Menu.hpp"
#include "ImGui/imgui.h"
#include "GameDataMan.hpp"
#include "WorldCharMan.hpp"
#include "Console.hpp"
#include "Hooking.hpp"
#include "D3DRenderer.hpp"

//  BUG : GAME CRASHES DURING WorldCharan::Update() loop

namespace ER {

    //  LOG EVENT FUNCTION
    //  ONLY USE FOR ON|OFF DEBUG PRINTS
    //<EXAMPLE>
    //  TEXT: "[+] MENU:: ESP ; "
    //  FLAG: bESP
    //</EXAMPLE>
    //  OUTPUT: [+] MENU:: ESP ; [<FLAG RESULT>]
    void LogEvent(std::string TEXT, bool FLAG)
    {
        std::string output;
        int color;
        if (FLAG) {
            output = " [ON]\n";
            color = g_Console->color.green;
        }
        else if (!FLAG) {
            output = " [OFF]\n";
            color = g_Console->color.red;
        }
        std::string append = TEXT + output;
        g_Console->printdbg(append.c_str(), TRUE, color);

    }

    //  LINE + NAME ESP
    void ESP(float distance)
    {
        //  Update Entity Info
        if (!g_WorldCharMan->m_isValid) {
            g_Menu->bESP = FALSE;
            g_Console->printdbg("[+] MENU:: ESP; OFF {WorldCharMan::Update ; FAILED}\n", TRUE, g_Console->color.red);
            return;
        }

        if (g_WorldCharMan->pCharData->Health == NULL) {
            g_Menu->bESP = FALSE;
            g_Console->printdbg("[+] MENU:: ESP; OFF {Health is NULL}\n", TRUE, g_Console->color.red);
            return;
        }

        ///  Filter Entity Results
        int count = 0;
        Vector2 vecScreen;
        uintptr_t ViewMatrix = g_GameFunctions->p2addy(g_GameVariables->m_ModuleBase + g_Menu->ptr_NBOTT_W2S, { 0x60, 0x60, 0x420 });
        //uintptr_t ViewMatrix = g_Hooking->TRUE_W2S;     //CRASH AT MAIN MENU
        Vector2 pos = { ImGui::GetMainViewport()->GetCenter().x, ImGui::GetMainViewport()->GetCenter().y };
        memcpy(&g_Menu->Matrix, (BYTE*)ViewMatrix, sizeof(g_Menu->Matrix));
        for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {

            //  COMPARE WITH PLAYER
            if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase)
                continue;

            //  HEALTH CHECK
            if (g_WorldCharMan->CharData[i]->Health == NULL)
            {
                //  ADDITIOONAL PATCH TO REVERT SKELETON DRAWING IF ENTITY DIES
                if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
                    g_WorldCharMan->CharFall[i]->DrawSkeleton = NULL;
                continue;
            }

            //  POSITION CHECK
            if (g_WorldCharMan->CharPhysics[i]->Position == g_WorldCharMan->pCharPhysics->Position)
                continue;

            ///  DRAW SKELETON 
            //  DISTANCE
            //if (distance != NULL) {
            //    if (g_WorldCharMan->CharPhysics[i]->Position.x <= (g_WorldCharMan->pCharPhysics->Position.x + distance) && (g_WorldCharMan->pCharPhysics->Position.x - distance)
            //        && g_WorldCharMan->CharPhysics[i]->Position.y <= (g_WorldCharMan->pCharPhysics->Position.y + 5) && (g_WorldCharMan->pCharPhysics->Position.y - 5)
            //        && g_WorldCharMan->CharPhysics[i]->Position.z <= (g_WorldCharMan->pCharPhysics->Position.z + distance) && (g_WorldCharMan->pCharPhysics->Position.z - distance))
            //    {
            //        if (g_WorldCharMan->CharFall[i]->DrawSkeleton == NULL)
            //            g_WorldCharMan->CharFall[i]->DrawSkeleton = 1;
            //    }
            //    else if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
            //        g_WorldCharMan->CharFall[i]->DrawSkeleton = NULL;
            //}
              
            if (g_D3DRenderer->WorldToScreen(g_WorldCharMan->CharPhysics[i]->Position, vecScreen, g_Menu->Matrix, ImGui::GetWindowWidth(), ImGui::GetWindowHeight())) {
                ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecScreen.x, vecScreen.y), ImColor(255, 0, 0, 255), std::to_string(count).c_str());
                ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos.x, pos.y + 960), ImVec2(vecScreen.x, vecScreen.y), ImColor(255, 255, 255), 0.3f);
                count++;
            }
        }
        g_WorldCharMan->count = NULL;
        count = NULL;
    }

    //  DEBUG ESP WINDOW
    void dbg_ESP()
    {
        if (!g_WorldCharMan->m_isValid) {
            g_Menu->m_dbgMatrixWnd = FALSE;
            g_Console->printdbg("[+] MENU:: ENT WINDOW; OFF {WorldCharMan::Update ; FAILED}\n", TRUE, g_Console->color.red);
            return;
        }

        if (g_WorldCharMan->pCharData->Health == NULL) {
            g_Menu->m_dbgMatrixWnd = FALSE;
            g_Console->printdbg("[+] MENU:: ENT WINDOW; OFF {Health is NULL}\n", TRUE, g_Console->color.red);
            return;
        }

        ///  Filter Entity Results
        int count = 0;
        Vector2 vecScreen;
        Vector2 pos = { ImGui::GetMainViewport()->GetCenter().x, ImGui::GetMainViewport()->GetCenter().y };
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

            ///  DRAW
            if (g_D3DRenderer->WorldToScreen(g_WorldCharMan->CharPhysics[i]->Position, vecScreen, g_Menu->ViewMatrix, ImGui::GetWindowWidth(), ImGui::GetWindowHeight())) {
                if (g_Menu->dbg_ENT_RGB) {
                    ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecScreen.x, vecScreen.y), ImColor(g_Menu->dbg_RAINBOW), std::to_string(count).c_str());
                    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos.x, pos.y + 960), ImVec2(vecScreen.x, vecScreen.y), ImColor(g_Menu->dbg_RAINBOW), 0.3f);
                }
                else {
                    ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecScreen.x, vecScreen.y), ImColor(0, 0, 255, 255), std::to_string(count).c_str());
                    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos.x, pos.y + 960), ImVec2(vecScreen.x, vecScreen.y), ImColor(255, 255, 255), 0.3f);
                }
                count++;
            }
        }
        g_WorldCharMan->count = NULL;
        count = NULL;
    }

    //  MAIN MENU TAB
    void Menu::AboutTab()
    {
        ImGui::Text("ELDEN RING INTERNAL (PREVIEW)");
        ImGui::Text("BUILD VERSION: alpha-0.0.4");
        ImGui::Text("BUILD DATE: 4/18/2022");
        ImGui::Text("GAME VERSION: 1.04");
    }

    //  CHAR STATS
    void Menu::CharacterStats()
    {
        if (!g_GameDataMan->Valid()) {
            ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] GData::ENTITY ARRAY UPDATE ERROR");
            return;
        }

        if (!g_WorldCharMan->Valid()) {
            ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] CHARACTER UPDATE ERROR");
            return;
        }

        if (!g_WorldCharMan->m_isValid) {
            ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] g_WorldCharMan->m_isValid = FALSE");
            return;
        }

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
    void Menu::EntityStats()
    {
        if (!g_GameDataMan->Valid()) {
            ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] GAME DATA UPDATE ERROR");
            return;
        }

        if (!g_WorldCharMan->Valid()) {
            ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] ENTITY UPDATE ERROR");
            return;
        }

        if (ImGui::Button("KILL ALL ENTITIES", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
            g_Console->printdbg("[+] MENU:: KILL ALL ENTITIES\n", TRUE, g_Console->color.green);
            g_WorldCharMan->killENTS();
        }
        ImGui::Spacing();

        //  Store all entity coords in an array of Vector3 Floats
        //  Flush data when boolean is false
        if (ImGui::Button("FREEZE ALL ENTITIES", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
            g_Console->printdbg("[!] MENU:: FREEZE ALL ENTITIES [BUGGED]\n", TRUE, g_Console->color.dark_yellow);
            if (!f_TOGGLE)
                g_WorldCharMan->stallENTS();
            else {

                //  Flush Entity Info
                for (int i = 0; i <= 999 - 1; i = i + 1) {
                    storedPOS[i].x = NULL;
                    storedPOS[i].y = NULL;
                    storedPOS[i].z = NULL;
                }
                f_TOGGLE = FALSE;
            }
        }
        ImGui::Spacing();

        if (ImGui::Button("TELEPORT ALL TO CROSSHAIR", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
            g_Console->printdbg("[!] MENU:: TELEPORT ALL TO CROSSHAIR\n", TRUE, g_Console->color.red);
            if (g_WorldCharMan->m_isValid)
            {
                for (int i = 0; i < g_WorldCharMan->arraySIZE - 1; i++) {
                    g_WorldCharMan->CharPhysics[i]->Position = g_WorldCharMan->pCharPhysics->Position;
                }
                printf("TELEPORTED: %d ENTITIES\n\n", g_WorldCharMan->arraySIZE);
            }
        }
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Toggle("FREEZE CLOSE ENTITIES", &f_TOGGLE);
        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::Toggle("DISPLAY ENTITY ARRAY", &m_dbgEntityWnd)) {
            LogEvent("[+] MENU:: ENTITY ARRAY ; ", m_dbgEntityWnd);
            if (!m_dbgEntityWnd)
                g_WorldCharMan->count = NULL;
        }
    }

    //  VISUALS TAB
    void Menu::Visuals()
    {
        ImGui::Toggle("DRAW FPS", &dbg_FPS);
        ImGui::Toggle("CUSTOM FPS: ", &b_FPS);
        if (b_FPS) {
            dbg_FPS = TRUE;
            ImGui::SameLine();
            ImGui::SetNextItemWidth(50);
            ImGui::InputFloat("##", &f_FPS, 0, 999);
            ImGui::SameLine();
            if (ImGui::Button("+"))
                f_FPS = f_FPS + 1;
            ImGui::SameLine();
            if (ImGui::Button("-"))
                f_FPS = f_FPS - 1;
            ImGui::SameLine();
            if (ImGui::Button("RESET"))
                f_FPS = 60;
        }

        if (ImGui::Toggle("DRAW CROSSHAIR", &m_dbgCrosshair)) 
            LogEvent("[+] MENU:: CROSSHAIR ;", m_dbgCrosshair);

        if (m_dbgCrosshair)
        {
            ImGui::Toggle("RGB Crosshair", &m_RGB_CROSSHAIR);

            //  CROSSHAIR COLOR
            ImGui::Text("CROSSHAIR COLOR");
            ImGui::SameLine();
            ImGui::ColorEdit4("CUSTOM##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
            ImColor cBuf{ (float)color.x, (float)color.y, (float)color.z };
            if (ImColor(g_Menu->dbg_crosshair_color) != ImColor(cBuf))
                g_Menu->dbg_crosshair_color = ImColor(cBuf);

            //  SIZE
            ImGui::Text("CROSSHAIR SIZE:      ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(75);
            ImGui::SliderFloat("##SIZE ", &g_Menu->dbg_crosshair_radius, 1, 100, "%.2f");
            ImGui::Spacing();

            ImGui::Text("CROSSHAIR SEGMENTS:  ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(75);
            ImGui::SliderInt("##SIZE2 ", &g_Menu->dbg_crosshair_segments, 0, 12, "%.2f");
            ImGui::Spacing();

            ImGui::Text("CROSSHAIR THICKNESS: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(75);
            ImGui::SliderFloat("##SIZE3 ", &g_Menu->dbg_crosshair_thickness, 0, 10, "%.2f");
            ImGui::Separator();
        }

        if (ImGui::Toggle("ESP", &bESP))
            LogEvent("[+] MENU:: ESP ; ", bESP);

        if (ImGui::Toggle("DRAW ESP SKELETON", &s_draw)) {
            if (!s_draw)
            {
                s_drawDistance = 0;
                if (g_WorldCharMan->m_isValid)
                {
                    for (int i = 0; i < g_WorldCharMan->arraySIZE - 1; i = i + 1)
                    {
                        if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
                            g_WorldCharMan->CharFall[i]->DrawSkeleton = 0;
                    }
                }
                else
                    !s_draw;
            }
        }
        if (s_draw)
        {
            ImGui::SameLine();
            ImGui::Text(" : DISTANCE ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(75);
            ImGui::SliderFloat("##DISTANCE: ", &s_drawDistance, 0, 30, "%.1f");
            ImGui::Spacing();
        }

        /// DRAW ALL HIT BOXES
        //if (ImGui::Toggle("DRAW ALL HITBOXES", &m_dbgHitBoxes))
        //{
        //    if (m_dbgHitBoxes) {
        //        *(int8_t*)(g_GameVariables->m_ModuleBase + g_Menu->ptr_DEBUG_FLAGS + 0xF) = 1;
        //        g_Console->printdbg("[+] MENU:: HIT-BOXES; ON\n", TRUE, g_Console->color.green);
        //    }
        //    else {
        //        *(int8_t*)(g_GameVariables->m_ModuleBase + g_Menu->ptr_DEBUG_FLAGS + 0xF) = 0;
        //        g_Console->printdbg("[+] MENU:: HIT-BOXES; OFF\n", TRUE, g_Console->color.red);
        //    }
        //}
    }

    //  DEBUG
    void Menu::DebugOptions()
    {
        if (!g_GameDataMan->Valid()) {
            ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] GData::ENTITY ARRAY UPDATE ERROR");
            return;
        }

        //  DEBUG POINTER CHAIN
        if (!g_WorldCharMan->Valid())
        {
            ImGui::Text("DEBUG POINTER CHAINS");
            ImGui::Spacing();
            ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] WData::ENTITY ARRAY UPDATE ERROR");
            ImGui::Text("WorldCharMan Base: %llX", (uintptr_t)g_WorldCharMan->Base);
            ImGui::Text("EntityObjectBase: %llX", (uintptr_t)g_WorldCharMan->EntityObjectBase[0]);
            ImGui::Separator();
        }
        else
        {
            ///  DEBUG POINTER CHAINS
            ImGui::Text("DEBUG POINTER CHAINS");
            ImGui::Spacing();
            ImGui::Text("WorldCharMan Base: %llX", (uintptr_t)g_WorldCharMan->Base);
            if (g_WorldCharMan->Ptr != NULL) {
                ImGui::Text("ptr: %llX", (uintptr_t)g_WorldCharMan->Ptr);
                ImGui::Text("begin: %u", g_WorldCharMan->Begin);
                ImGui::Text("finish: %u", g_WorldCharMan->Finish);
                ImGui::Text("size: %d", g_WorldCharMan->arraySIZE);
                ImGui::Text("count: %d", g_WorldCharMan->count);
            }
            ImGui::Spacing();

            ImGui::Text("EntityObjectBase: %llX", (uintptr_t)g_WorldCharMan->EntityObjectBase[0]);
            if (g_WorldCharMan->EntityObjectBase[0] != NULL) {
                ImGui::Text("pEntityObject: %llX", (uintptr_t)g_WorldCharMan->EntityObjectBase[0]->EntObjectPTR);
                ImGui::Text("CharData: %llX", (uintptr_t)g_WorldCharMan->CharData[0]);
                ImGui::Text("CharFall: %llX", (uintptr_t)g_WorldCharMan->CharFall[0]);
                ImGui::Text("CharPhysics: %llX", (uintptr_t)g_WorldCharMan->CharPhysics[0]);
            }
            ImGui::Separator();
        }

        if (ImGui::Toggle("DRAW ALL SKELETONS", &m_dbgSkeleton))
            g_WorldCharMan->dbg_SKELETON(m_dbgSkeleton);

        if (ImGui::Toggle("DEBUG ESP", &m_dbgMatrixWnd))
            LogEvent("[+] MENU:: DEBUG ESP; ", m_dbgMatrixWnd);

        ImGui::Separator();
        ImGui::Toggle("RGB THEME", &dbg_RAINBOW_THEME);
        ImGui::Spacing();
        ImGui::Toggle("SHOW CONSOLE | ", &g_GameVariables->m_ShowConsole);
        ImGui::SameLine();
        ImGui::Checkbox("Verbose Logging", &g_Console->verbose);
        ImGui::Spacing();
        ImGui::Toggle("SHOW IMGUI::DEMO WINDOW", &g_GameVariables->m_ShowDemo);
        ImGui::Separator();

        if (ImGui::Button("CALL W2S FUNCTION", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            g_Menu->dbgPrintW2S = TRUE;

        if (ImGui::Button("CALL WorldCharMan Update", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            g_WorldCharMan->Update();

        if (ImGui::Button("INITIALIZE STYLE", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            g_Menu->InitStyle();
        ImGui::Separator();

        if (ImGui::Button("QUIT GAME", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            TerminateProcess(g_GameVariables->m_GameHandle, EXIT_SUCCESS);
    }

    //  MAIN MENU LOOP
    void Menu::Draw()
    {
        // MAIN WINDOW
        if (g_GameVariables->m_ShowMenu) {
            IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");
            if (dbg_RAINBOW_THEME) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(g_Menu->dbg_RAINBOW));
                ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(g_Menu->dbg_RAINBOW));
            }
            if (!ImGui::Begin("ELDEN RING INTERNAL DEBUG", &g_GameVariables->m_ShowMenu, 96))
            {
                ImGui::End();
                return;
            }
            if (dbg_RAINBOW_THEME) {
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
            }
            InitStyle();
            if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("MAIN"))
                {
                    AboutTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("PLAYER"))
                {
                    CharacterStats();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("ENTITY OPTIONS"))
                {
                    EntityStats();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("VISUALS"))
                {
                    Visuals();
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

            if (g_WorldCharMan->m_isValid) !m_dbgEntityWnd;

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
                        printf("COORDINATES:\nPosX: %f\nPosY: %f\nPosZ: %f\n", g_WorldCharMan->CharPhysics[i]->Position.x,
                            g_WorldCharMan->CharPhysics[i]->Position.y, g_WorldCharMan->CharPhysics[i]->Position.z);
                    }
                    ImGui::Spacing();

                    if (ImGui::Button("TELEPORT TO CROSSHAIR", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
                        g_WorldCharMan->CharPhysics[i]->Position = g_WorldCharMan->pCharPhysics->Position;
                        g_Console->printdbg("[+] MENU::EntityList; TELEPORT TO CROSSHAIR\n", TRUE, g_Console->color.green);
                        printf("COORDINATES:\nPosX: %f\nPosY: %f\nPosZ: %f\n", g_WorldCharMan->CharPhysics[i]->Position.x,
                            g_WorldCharMan->CharPhysics[i]->Position.y, g_WorldCharMan->CharPhysics[i]->Position.z);
                    }
                    ImGui::Spacing();

                    ImGui::Text("FREEZE TARGET: ");
                    ImGui::SameLine();
                    if (ImGui::Toggle("##FREEZE TARGET", &m_dbgFreezeTarget)) {
                        g_Console->printdbg("[+] MENU::EntityList; FREEZE TARGET {not implemented}\n", TRUE, g_Console->color.red);
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
                if (i <= 9)
                {   
                    std::string a = "MATRIX  [" + std::to_string(i) + "]: ";
                    ImGui::Text(a.c_str());
                }
                else if (i > 9)
                {
                    std::string a = "MATRIX [" + std::to_string(i) + "]: ";
                    ImGui::Text(a.c_str());
                }
                std::string b = "##MATRIX [" + std::to_string(i) + "]: ";
                
                
                //ImGui::Text(a.c_str());
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

            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
            if (ImGui::Button("RAINBOW ESP", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            {
                g_Menu->dbg_ENT_RGB = !g_Menu->dbg_ENT_RGB;
                LogEvent("[+] MENU:: DEBUG ESP WINDOW:: RGB LINES ; ", dbg_ENT_RGB);
            }
            ImGui::PopStyleColor();
            ImGui::End();
        }
        else if (!g_GameVariables->m_ShowMenu) {
            !m_dbgMatrixWnd;
        }

        //  DRAW FPS
        if (dbg_FPS)
            g_GameFunctions->FPS();

        //  DEBUG DRAW CROSSHAIR
        if (m_dbgCrosshair) {
            if (m_RGB_CROSSHAIR)
                ImGui::GetBackgroundDrawList()->AddCircle(ImGui::GetMainViewport()->GetCenter(), g_Menu->dbg_crosshair_radius, g_Menu->dbg_RAINBOW, g_Menu->dbg_crosshair_segments, g_Menu->dbg_crosshair_thickness);
            else if (!m_RGB_CROSSHAIR)
                ImGui::GetBackgroundDrawList()->AddCircle(ImGui::GetMainViewport()->GetCenter(), g_Menu->dbg_crosshair_radius, ImColor(dbg_crosshair_color), g_Menu->dbg_crosshair_segments, g_Menu->dbg_crosshair_thickness);
        }

        //  SHOW DEBUG CONSOLE
        if (g_GameVariables->m_ShowConsole)
            ::ShowWindow(GetConsoleWindow(), SW_SHOW);
        else
            ::ShowWindow(GetConsoleWindow(), SW_HIDE);

        //  ESP
        if (bESP)
            ESP(s_drawDistance);
        
        //  DEBUG ESP
        if (m_dbgMatrixWnd)
            dbg_ESP();

        //  SKELTON DRAW
        if (s_draw)
            g_WorldCharMan->ESP_SKELETON(s_drawDistance);

        //  DRAW DISTANCE
        if (s_drawDistance != NULL) {
            std::string text = "Draw Distance: " + std::to_string(s_drawDistance);
            if (dbg_RAINBOW_THEME)
                ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 15), ImColor(g_Menu->dbg_RAINBOW), text.c_str());
            else
                ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 15), ImColor(255, 255, 255, 255), text.c_str());
        }

        //  FREEZE ENTITIES
        if (f_TOGGLE)
        {
            g_WorldCharMan->stallENTS();
            //d_StallEnts(s_drawDistance);
            if (g_WorldCharMan->pCharData->Health == NULL) {
                f_TOGGLE = FALSE;
                for (int i = 0; i < sizeof(storedPOS) / sizeof(storedPOS[0]); i = i + 1)
                    storedPOS[i] = Vector3(0, 0, 0);
                g_Console->printdbg("[+] MENU:: FREEZE ENTITIES; OFF {Health is NULL}\n\n", TRUE, g_Console->color.red);
            }
        }

        //  CUSTOM FPS LIMIT 
        //  CREDIT: techiew
        if (c_FPS != f_FPS)
        {
            uintptr_t RESET_FPS = g_GameVariables->m_ModuleBase + ptr_SET_FPS;      //  FPS ADDRESS
            int x = *(int*)RESET_FPS;   //  READ CURRENT FPS VALUE
            char oBytes[sizeof x];      //  CONVERT TO BYTE ARRAY
            std::copy(reinterpret_cast<const char*>(reinterpret_cast<const void*>(&x)), reinterpret_cast<const char*>(reinterpret_cast<const void*>(&x)) + sizeof x, oBytes);

            //  ORIGINAL BYTES & PATCH BYTES
            std::vector<uint16_t> oBytes2 = { (unsigned short)oBytes[0], (unsigned short)oBytes[1], (unsigned short)oBytes[2], (unsigned short)oBytes[3] }; //= arrayOfByte; //{ 0x0, 0x0, 0x0, 0x0 };
            std::vector<uint8_t> bytes(4, 0x90);

            //  RESET CURRENT FPS
            c_FPS = f_FPS;
            float frames = (1000 / f_FPS) / 1000;
            memcpy(&bytes[0], &frames, 4);
            if (g_GameFunctions->Replace(RESET_FPS, oBytes2, bytes)) {
                printf("CUSTOM FPS: %f\n", f_FPS);
            }
            else
                g_Console->printdbg("[!] MENU:: CUSTOM FPS ; failed\n", TRUE, g_Console->color.green);
        }

        if (g_GameVariables->m_ShowDemo)
            ImGui::ShowDemoWindow();
    }

    //  STYLE
    void Menu::InitStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = ImGui::GetStyle().Colors;

        if (g_Menu->dbg_RAINBOW_THEME)
            colors[ImGuiCol_Separator] = g_Menu->dbg_RAINBOW;
        else
            colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.90f);

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
        //colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.90f);
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

        //g_Console->printdbg("[+] MENU STYLE INITIALIZED\n", TRUE, g_Console->color.green);
    }
}