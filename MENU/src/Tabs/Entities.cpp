#pragma once
#include "../../include/Tabs/Entities.h"
#include "../../include/Menu.hpp"
#include "../../include/GameDataMan.hpp"
#include "../../include/WorldCharMan.hpp"


//#include "../../include/Game.hpp"
//
//#include "../../include/D3DRenderer.hpp"

using namespace ER::Tabs;

/// <summary>
/// PLAYER OPTIONS
/// </summary>
void Entities::Reset() {

}

void Entities::Draw() {
    //  Initize so that we can set and store ImGui Content Positions
    ImGuiWindow* window = GImGui->CurrentWindow;
    ImVec2 ToolLocation;
    ImGui::Text("[ENTITY STATS]", TRUE, ImColor(0, 255, 255, 200));
    ImGui::Spacing();
    ImGui::Separator();

    //---------------------------------------------------------------------------------------------------


    if (!g_GameDataMan->Valid()) {
        ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] GAME DATA UPDATE ERROR");
        return;
    }

    if (!g_WorldCharMan->Valid()) {
        ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] ENTITY UPDATE ERROR");
        return;
    }

    if (!g_WorldCharMan->m_isValid) {
        ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] UPDATE ERROR");
        return;
    }

    if (ImGui::Button("KILL ALL ENTITIES", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
        g_WorldCharMan->killENTS();
    }
    ImGui::Spacing();

    //  Store all entity coords in an array of Vector3 Floats
    //  Flush data when boolean is false
    if (ImGui::Button("FREEZE ALL ENTITIES", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
        if (!g_Menu->f_TOGGLE)
            g_WorldCharMan->stallENTS();
        else if (g_Menu->f_TOGGLE) {
            //  Flush Entity Info
            for (int i = 0; i <= 999 - 1; i = i + 1) {
                g_Menu->storedPOS[i].x = NULL;
                g_Menu->storedPOS[i].y = NULL;
                g_Menu->storedPOS[i].z = NULL;
            }
            g_Menu->f_TOGGLE = FALSE;
        }
    }
    ImGui::Spacing();

    if (ImGui::Button("TELEPORT ALL TO CROSSHAIR", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
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

    ImGui::Checkbox("FREEZE CLOSE ENTITIES", &g_Menu->f_TOGGLE);
    ImGui::Spacing();
    ImGui::Checkbox("Health Drain Barrier", &g_GameFunctions->m_BARRIER);
    if (g_GameFunctions->m_BARRIER) {
        ImGui::SameLine();
        ImGui::Text("  |  DISTANCE: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(75);
        ImGui::SliderFloat("##", &g_GameFunctions->m_BARRIER_DISTANCE, 0, 50, "%.2f");
    }
    ImGui::Separator();

    if (ImGui::Checkbox("DISPLAY ENTITY ARRAY", &g_Menu->m_dbgEntityWnd)) {
        if (!g_Menu->m_dbgEntityWnd)
            g_WorldCharMan->count = NULL;
    }


    //---------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text(g_Menu->m_MenuFooter);
}