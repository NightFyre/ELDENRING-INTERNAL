#pragma once
#include "../../include/Tabs/Visuals.h"
#include "../../include/Console.hpp"
#include "../../include/Menu.hpp"
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


    //  ImGui::Toggle("DRAW FPS", &g_Menu->dbg_FPS);
    //  ImGui::Toggle("CUSTOM FPS: ", &g_Menu->b_FPS);
    //  if (g_Menu->b_FPS) {
    //      g_Menu->dbg_FPS = TRUE;
    //      ImGui::SameLine();
    //      ImGui::SetNextItemWidth(50);
    //      ImGui::InputFloat("##", &g_Menu->f_FPS, 0, 999);
    //      ImGui::SameLine();
    //      if (ImGui::Button("+"))
    //          g_Menu->f_FPS = g_Menu->f_FPS + 1;
    //      ImGui::SameLine();
    //      if (ImGui::Button("-"))
    //          g_Menu->f_FPS = g_Menu->f_FPS - 1;
    //      ImGui::SameLine();
    //      if (ImGui::Button("RESET"))
    //          g_Menu->f_FPS = 60;
    //  }

    //  if (ImGui::Toggle("DRAW CROSSHAIR", &g_Menu->m_dbgCrosshair)) g_Console->LogEvent("[+] MENU:: CROSSHAIR ;", g_Menu->m_dbgCrosshair);
    //  if (g_Menu->m_dbgCrosshair)
    //  {
    //      ImGui::Toggle("RGB Crosshair", &g_Menu->m_RGB_CROSSHAIR);
    //  
    //      //  CROSSHAIR COLOR
    //      ImGui::Text("CROSSHAIR COLOR");
    //      ImGui::SameLine();
    //      ImGui::ColorEdit4("CUSTOM##3", (float*)&g_Menu->color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | g_Menu->misc_flags);
    //      ImColor cBuf{ (float)g_Menu->color.x, (float)g_Menu->color.y, (float)g_Menu->color.z };
    //      if (ImColor(g_Menu->dbg_crosshair_color) != ImColor(cBuf))
    //          g_Menu->dbg_crosshair_color = ImColor(cBuf);
    //  
    //      //  SIZE
    //      ImGui::Text("CROSSHAIR SIZE:      ");
    //      ImGui::SameLine();
    //      ImGui::SetNextItemWidth(75);
    //      ImGui::SliderFloat("##SIZE ", &g_Menu->dbg_crosshair_radius, 1, 100, "%.2f");
    //      ImGui::Spacing();
    //  
    //      ImGui::Text("CROSSHAIR SEGMENTS:  ");
    //      ImGui::SameLine();
    //      ImGui::SetNextItemWidth(75);
    //      ImGui::SliderInt("##SIZE2 ", &g_Menu->dbg_crosshair_segments, 0, 12, "%.2f");
    //      ImGui::Spacing();
    //  
    //      ImGui::Text("CROSSHAIR THICKNESS: ");
    //      ImGui::SameLine();
    //      ImGui::SetNextItemWidth(75);
    //      ImGui::SliderFloat("##SIZE3 ", &g_Menu->dbg_crosshair_thickness, 0, 10, "%.2f");
    //      ImGui::Separator();
    //  }

    //  if (ImGui::Toggle("ESP", &g_Menu->bESP)) {
    //      g_Console->LogEvent("[+] MENU:: ESP ; ", g_Menu->bESP);
    //      if (!g_Menu->bESP && !g_Menu->s_draw && g_WorldCharMan->m_isValid)
    //      {
    //          for (int i = 0; i < g_WorldCharMan->arraySIZE - 1; i = i + 1)
    //          {
    //              if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
    //                  g_WorldCharMan->CharFall[i]->DrawSkeleton = 0;
    //          }
    //      }
    //  }

    //  if (ImGui::Toggle("DRAW ESP SKELETON", &g_Menu->s_draw)) {
    //      g_Console->LogEvent("[+] MENU:: ESP SKELETON ; ", g_Menu->s_draw);
    //      if (!g_Menu->s_draw && !g_Menu->bESP && g_WorldCharMan->m_isValid)
    //      {
    //          for (int i = 0; i < g_WorldCharMan->arraySIZE - 1; i = i + 1)
    //          {
    //              if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
    //                  g_WorldCharMan->CharFall[i]->DrawSkeleton = 0;
    //          }
    //      }
    //  }

    ImGui::Text("SKELETON DRAW DISTANCE: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150);
    ImGui::SliderFloat("##DISTANCE: ", &g_Menu->s_drawDistance, 0, 100, "%.1f");

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


    //---------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    ImGui::Separator();
    std::string footer = g_Menu->m_MenuFooter + g_Console->GetTimeString();
    ImGui::Text(footer.c_str());
}

void Visuals::DrawOptions()
{

}