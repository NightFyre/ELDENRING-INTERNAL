#pragma once
#include "../../include/Tabs/Visuals.h"
#include "../../include/Menu.hpp"
#include "../../include/WorldCharMan.hpp"
using namespace ER::Tabs;

/// <summary>
/// PLAYER OPTIONS
/// </summary>
void Visuals::Reset() {

}

void Visuals::Draw() {
    //  Initize so that we can set and store ImGui Content Positions
    ImGuiWindow* window = GImGui->CurrentWindow;
    ImVec2 ToolLocation;
    ImGui::Text("[VISUALS]", TRUE, ImColor(0, 255, 255, 200));
    ImGui::Spacing();
    ImGui::Separator();

    //---------------------------------------------------------------------------------------------------


    ImGui::Checkbox("DRAW FPS", &g_Menu->dbg_FPS);
    ImGui::Checkbox("CUSTOM FPS: ", &g_Menu->b_FPS);
    if (g_Menu->b_FPS) {
        g_Menu->dbg_FPS = TRUE;
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat("##", &g_Menu->f_FPS, 0, 999);
        ImGui::SameLine();
        if (ImGui::Button("+"))
            g_Menu->f_FPS = g_Menu->f_FPS + 1;
        ImGui::SameLine();
        if (ImGui::Button("-"))
            g_Menu->f_FPS = g_Menu->f_FPS - 1;
        ImGui::SameLine();
        if (ImGui::Button("RESET"))
            g_Menu->f_FPS = 60;
    }

    ImGui::Text("SKELETON DRAW DISTANCE: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150);
    ImGui::SliderFloat("##DISTANCE: ", &g_Menu->s_drawDistance, 0, 100, "%.1f");


    //---------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text(g_Menu->m_MenuFooter);
}

void Visuals::DrawOptions()
{

}