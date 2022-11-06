#pragma once
#include "../../include/Tabs/Debug.h"
#include "../../include/Menu.hpp"
#include "../../include/D3DRenderer.hpp"
#include "../../include/GameDataMan.hpp"
#include "../../include/WorldCharMan.hpp"
using namespace ER::Tabs;

/// <summary>
/// PLAYER OPTIONS
/// </summary>
void Debug::Reset() {

}

void Debug::Draw() {
    //  Initize so that we can set and store ImGui Content Positions
    ImGuiWindow* window = GImGui->CurrentWindow;
    ImVec2 ToolLocation;
    ImGui::TextCentered("[ABOUT]", TRUE, ImColor(0, 255, 255, 200));
    ImGui::Spacing();
    ImGui::Separator();

    //---------------------------------------------------------------------------------------------------


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

    if (ImGui::Toggle("DRAW ALL SKELETONS", &g_Menu->m_dbgSkeleton))
        g_WorldCharMan->dbg_SKELETON(g_Menu->m_dbgSkeleton);
    if (g_Menu->m_dbgSkeleton) {
        ImGui::SameLine();
        std::string paired = (" | " + std::to_string(g_WorldCharMan->validEnts_count));
        ImGui::Text(paired.c_str());
    }

    if (ImGui::Toggle("DEBUG ESP", &g_Menu->m_dbgMatrixWnd))
        g_Console->LogEvent("[+] MENU:: DEBUG ESP; ", g_Menu->m_dbgMatrixWnd);

    ImGui::Separator();
    ImGui::Toggle("RGB THEME", &g_Menu->dbg_RAINBOW_THEME);
    ImGui::Spacing();
    if (ImGui::Toggle("SHOW CONSOLE | ", &g_GameVariables->m_ShowConsole)) {
        if (g_GameVariables->m_ShowConsole)
            ShowWindow(GetConsoleWindow(), SW_SHOW);
        else if (!g_GameVariables->m_ShowConsole)
            ShowWindow(GetConsoleWindow(), SW_HIDE);
    }
    ImGui::SameLine();
    ImGui::Checkbox("Verbose Logging", &g_Console->verbose);
    ImGui::Spacing();
    //ImGui::Toggle("SHOW IMGUI::DEMO WINDOW", &g_GameVariables->m_ShowDemo);
    ImGui::Separator();

    if (ImGui::Button("CALL W2S FUNCTION", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
        g_Menu->dbgPrintW2S = TRUE;

    if (ImGui::Button("CALL WorldCharMan Update", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
        g_WorldCharMan->Update();

    if (ImGui::Button("INITIALIZE STYLE", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
        g_Styles->InitStyle();
    ImGui::Separator();
    if (ImGui::Button("UNLOAD DLL")) g_KillSwitch = TRUE;
    ImGui::Spacing();
    if (ImGui::Button("QUIT GAME", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
        TerminateProcess(g_GameVariables->m_GameHandle, EXIT_SUCCESS);


    //---------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    ImGui::Separator();
    std::string footer = g_Menu->m_MenuFooter + g_Console->GetTimeString();
    ImGui::TextCentered(footer.c_str());
}