#pragma once
#include "../../include/Tabs/Visuals.h"
#include "../../include/Console.hpp"
#include "../../include/Menu.hpp"
using namespace ER::Tabs;

void Visuals::Reset() 
{

}

void Visuals::Draw() 
{
    //  Initize so that we can set and store ImGui Content Positions
    ImGuiWindow* window = GImGui->CurrentWindow;
    ImVec2 ToolLocation;
    //  ImGui::Text("[VISUALS]", TRUE, ImColor(0, 255, 255, 200));
    ImGui::Text("[VISUALS]");
    ImGui::Spacing();
    ImGui::Separator();

    //---------------------------------------------------------------------------------------------------



    //---------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    ImGui::Separator();
    std::string footer = g_Menu->m_MenuFooter + g_Console->GetTimeString();
    ImGui::Text(footer.c_str());
}

void Visuals::DrawOptions()
{

}