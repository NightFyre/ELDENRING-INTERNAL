#pragma once
#include "../../include/Tabs/Main.h"
#include "../../include/Game.hpp"
#include "../../include/Menu.hpp"
#include "../../include/D3DRenderer.hpp"
using namespace ER::Tabs;

/// <summary>
/// PLAYER OPTIONS
/// </summary>
void Main::Reset() {

}

void Main::Draw() {
    //  Initize so that we can set and store ImGui Content Positions
    ImGuiWindow* window = GImGui->CurrentWindow;
    ImVec2 ToolLocation;
    ImGui::Text("[ABOUT]", TRUE, ImColor(0, 255, 255, 200));
    ImGui::Spacing();
    ImGui::Separator();

    //---------------------------------------------------------------------------------------------------


    ImGui::Text("ELDEN RING STEAM DECK");
    ImGui::Text("BUILD VERSION: 0.0.1");
    ImGui::Text("BUILD DATE: 7/23/2023");
    ImGui::Text("GAME VERSION: 1.09.1");


    //---------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text(g_Menu->m_MenuFooter);
}