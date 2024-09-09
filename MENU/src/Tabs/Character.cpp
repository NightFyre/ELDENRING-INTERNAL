#pragma once
#include "../../include/Tabs/Character.h"
#include "../../include/Menu.hpp"
#include "../../include/Console.hpp"
using namespace ER::Tabs;

void Character::Reset() 
{

}

void Character::Draw() 
{
    //  Initize so that we can set and store ImGui Content Positions
    ImGuiWindow* window = GImGui->CurrentWindow;
    ImVec2 ToolLocation;
    //  ImGui::TextCentered("[CHARACTER STATS]", TRUE, ImColor(0, 255, 255, 200));
    ImGui::Text("[CHARACTER STATS]");
    ImGui::Spacing();
    ImGui::Separator();

    //---------------------------------------------------------------------------------------------------

    /// 
    //  if (!g_GameDataMan->Valid()) 
    //  {
    //      ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] GAME DATA UPDATE ERROR");
    //      return;
    //  }
    //  
    //  if (!g_WorldCharMan->Valid()) 
    //  {
    //      ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] ENTITY UPDATE ERROR");
    //      return;
    //  }
    //  
    //  if (!g_WorldCharMan->m_isValid) 
    //  {
    //      ImGui::TextColored(ImColor(255, 96, 96, 255), "[+] UPDATE ERROR");
    //      return;
    //  }
    //  
    //  ImGui::Text("CHARATER ATTRIBUTES");
    //  ImGui::Spacing();
    //  
    //  ImGui::Text("Runes:        ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Runes", (int*)&g_GameDataMan->GameData->CurrentRunes);
    //  
    //  ImGui::Text("Level:        ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Level", (int*)&g_GameDataMan->GameData->Level);
    //  
    //  ImGui::Text("Vigor:        ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Vigor", (int*)&g_GameDataMan->GameData->Stats.Vigor);
    //  
    //  ImGui::Text("Mind:         ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Mind", (int*)&g_GameDataMan->GameData->Stats.Mind);
    //  
    //  ImGui::Text("Endurance:    ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Endurance", (int*)&g_GameDataMan->GameData->Stats.Endurance);
    //  
    //  ImGui::Text("Strength:     ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Strength", (int*)&g_GameDataMan->GameData->Stats.Strength);
    //  
    //  ImGui::Text("Dexterity:    ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Dexterity", (int*)&g_GameDataMan->GameData->Stats.Dexterity);
    //  
    //  ImGui::Text("Intelligence: ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Intelligence", (int*)&g_GameDataMan->GameData->Stats.Intelligence);
    //  
    //  ImGui::Text("Faith:        ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Faith", (int*)&g_GameDataMan->GameData->Stats.Faith);
    //  
    //  ImGui::Text("Arcane:       ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Arcane", (int*)&g_GameDataMan->GameData->Stats.Arcane);
    //  
    //  ImGui::Separator();
    //  ImGui::Text("CHARACTER STATS");
    //  ImGui::Spacing();
    //  
    //  
    //  ImGui::Text("Health        ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Health", &g_WorldCharMan->pCharData->Health);
    //  
    //  ImGui::Text("Mana          ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Mana", &g_WorldCharMan->pCharData->Mana);
    //  
    //  ImGui::Text("Stamina       ");
    //  ImGui::SameLine();
    //  ImGui::InputInt("##Stamina", &g_WorldCharMan->pCharData->Stamina);
    //  
    //  ImGui::Text("Azimuth       ");
    //  ImGui::SameLine();
    //  ImGui::InputFloat("##Azimuth", &g_WorldCharMan->pCharPhysics->Azimuth);
    //  
    //  ImGui::Text("PosX          ");
    //  ImGui::SameLine();
    //  ImGui::InputFloat("##PosX", &g_WorldCharMan->pCharPhysics->Position.x);
    //  
    //  ImGui::Text("PosY          ");
    //  ImGui::SameLine();
    //  ImGui::InputFloat("##PosY", &g_WorldCharMan->pCharPhysics->Position.y);
    //  
    //  ImGui::Text("PosZ          ");
    //  ImGui::SameLine();
    //  ImGui::InputFloat("##PosZ", &g_WorldCharMan->pCharPhysics->Position.z);


    //---------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    ImGui::Separator();
    std::string footer = g_Menu->m_MenuFooter + g_Console->GetTimeString();
    ImGui::Text(footer.c_str());
}