#include "../include/Menu.hpp"
#include "../include/GameDataMan.hpp"
#include "../include/WorldCharMan.hpp"
#include "../include/Hooking.hpp"
#include "../include/D3DRenderer.hpp"

namespace ER {

    //  MAIN MENU LOOP
    void Menu::Draw()
    {
        // MAIN WINDOW
        IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");
        if (!ImGui::Begin("ELDEN RING INTERNAL DEBUG", &g_GameVariables->m_ShowMenu, 96))
        {
            ImGui::End();
            return;
        }
        g_Styles->InitStyle();

        DrawMenu();

        //  IMGUI DEMO WINDOW
        if (g_GameVariables->m_ShowDemo)
            ImGui::ShowDemoWindow();

        ImGui::End();
    }

    void Menu::DrawMenu()
    {
        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("MAIN"))
            {
                Tabs::Main::Draw();
                ImGui::EndTabItem();
            }

            //if (ImGui::BeginTabItem("PLAYER"))
            //{
            //    Tabs::Character::Draw();
            //    ImGui::EndTabItem();
            //}

            //if (ImGui::BeginTabItem("DEBUG"))
            //{
            //    Tabs::Debug::Draw();
            //    ImGui::EndTabItem();
            //}

            ImGui::EndTabBar();
        }
    }

    void Menu::bgLoops()
    {
        //  FREEZE ENTITIES
        if (f_TOGGLE)
        {
            g_WorldCharMan->stallENTS();
            //d_StallEnts(s_drawDistance);
            if (g_WorldCharMan->pCharData->Health == NULL) {
                f_TOGGLE = FALSE;
                for (int i = 0; i < sizeof(storedPOS) / sizeof(storedPOS[0]); i = i + 1)
                    storedPOS[i] = Vector3(0, 0, 0);
            }
        }

        if (g_GameFunctions->m_BARRIER)
            g_GameFunctions->Barrier(g_GameFunctions->m_BARRIER_DISTANCE);

        //  CUSTOM FPS LIMIT 
        //  CREDIT: techiew
        if (c_FPS != f_FPS)
        {
            uintptr_t RESET_FPS = g_GameVariables->m_ModuleBase + g_GameVariables->offsets.ptr_SET_FPS;      //  FPS ADDRESS
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
            g_GameFunctions->Replace(RESET_FPS, oBytes2, bytes);
        }
    }
}