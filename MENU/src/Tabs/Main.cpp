#pragma once
#include "../../include/Tabs/Main.h"
#include "../../include/D3DRenderer.hpp"
using namespace ER::Tabs;

/// <summary>
/// PLAYER OPTIONS
/// </summary>
void Main::Reset() {

}

void Main::Draw() 
{
    //  Initize so that we can set and store ImGui Content Positions
    ImGuiWindow*    window = GImGui->CurrentWindow;
    ImVec2          ToolLocation;

    //---------------------------------------------------------------------------------------------------

    ImGui::SeparatorText("CHEATS");
    ImGui::Checkbox("INFINITE HEALTH",  &g_Menu->bInfiniteHealth);
    ImGui::Checkbox("INFINITE MANA",    &g_Menu->bInfiniteMana);
    ImGui::Checkbox("INFINITE STAMINA", &g_Menu->bInfiniteStamina);

    ImGui::SeparatorText("ABOUT");
    ImGui::Text("ELDEN RING STEAM DECK");
    ImGui::Text("BUILD VERSION: 0.0.1");
    ImGui::Text("BUILD DATE: 7/23/2023");
    ImGui::Text("GAME VERSION: 1.09.1");

    //---------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text(g_Menu->m_MenuFooter);
}