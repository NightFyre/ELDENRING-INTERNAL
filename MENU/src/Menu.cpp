#include "../include/Menu.hpp"
#include "../include/Console.hpp"
#include "../include/Hooking.hpp"
#include "../include/D3DRenderer.hpp"

namespace ER {

    //  MAIN MENU LOOP
    void Menu::Draw()
    {
        // MAIN WINDOW
        if (g_GameVariables->m_ShowMenu) 
        {

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

            // STYLE
            g_Styles->InitStyle();

            DrawMenu();
            ImGui::End();

            //  IMGUI DEMO WINDOW
            if (g_GameVariables->m_ShowDemo)
                ImGui::ShowDemoWindow();

            //  ENTITY WINDOW
            //  if (m_dbgEntityWnd)
            //      DrawEntityWindow();
            //  
            //  //  DBG MATRIX WINDOW
            //  if (m_dbgMatrixWnd)
            //      DrawMatrixHelper();
        }
        else 
        {
            if (m_dbgEntityWnd)
                m_dbgEntityWnd = FALSE;

            if (m_dbgMatrixWnd)
                m_dbgMatrixWnd = FALSE;
        }

        // OTHER DRAWING THINGS
        {
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

            //  DRAW DISTANCE
            if (s_drawDistance != NULL) {
                std::string text = "Draw Distance: " + std::to_string(s_drawDistance);
                if (dbg_RAINBOW_THEME)
                    ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 15), ImColor(g_Menu->dbg_RAINBOW), text.c_str());
                else
                    ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 15), ImColor(255, 255, 255, 255), text.c_str());
            }
        }

        // ESP STUFF
        //  {
        //      //  ESP
        //      if (bESP)
        //          g_GameFunctions->ESP(s_drawDistance);
        //  
        //      //  DEBUG ESP
        //      if (m_dbgMatrixWnd)
        //          g_GameFunctions->dbg_ESP();
        //  
        //      //  DISTANCE BASED SKELTON DRAW (visuals tab)
        //      if (s_draw)
        //          g_WorldCharMan->ESP_SKELETON(s_drawDistance);
        //  }
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

            if (ImGui::BeginTabItem("PLAYER"))
            {
                Tabs::Character::Draw();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("ENTITY OPTIONS"))
            {
                Tabs::Entities::Draw();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("VISUALS"))
            {
                Tabs::Visuals::Draw();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("DEBUG"))
            {
                Tabs::Debug::Draw();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    void Menu::DrawEntityWindow()
    {
        //  int count = 0;
        //  std::string grace = "GRACE SIGHT ";
        //  std::string entity = "ENTITY OBJECT ";
        //  std::string empty_space = "   ";
        //  ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
        //  if (!ImGui::Begin("ENTITY ARRAY", &m_dbgEntityWnd, 96)) {
        //  
        //      //  HANDLE EXCEPTION
        //      if (!g_GameDataMan->Valid()) {
        //          ImGui::End();
        //          m_dbgEntityWnd = FALSE;
        //          return;
        //      }
        //  
        //      if (!g_WorldCharMan->Valid()) {
        //          ImGui::End();
        //          m_dbgEntityWnd = FALSE;
        //          return;
        //      }
        //  }
        //  if (g_WorldCharMan->m_isValid) !m_dbgEntityWnd;
        //  
        //  ImGui::Text("GRACE SIGHTS: ");
        //  ImGui::SameLine();
        //  ImGui::Text(std::to_string(g_WorldCharMan->entwndw_count).c_str());
        //  ImGui::Text("ENTITIES: ");
        //  ImGui::SameLine();
        //  ImGui::Text(std::to_string(g_WorldCharMan->entwndw_count2).c_str());
        //  ImGui::Separator();
        //  ImGui::Spacing();
        //  
        //  //  Grace Locations ??
        //  if (ImGui::CollapsingHeader("GRACE LOCATIONS"))
        //  {
        //      ImGui::Spacing();
        //      for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1)
        //      {
        //          if (g_WorldCharMan->CharTimeAct[i]->Animation < NULL) continue;
        //          if (g_WorldCharMan->EntityObjectBase[i]->ALLIANCE != g_WorldCharMan->Char_Faction.None) continue;
        //          count++;
        //          std::string OBJECT_TEXT = grace + std::to_string(count) + empty_space;
        //  
        //          ImGui::PushID(i);
        //          ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
        //          if (ImGui::CollapsingHeader((OBJECT_TEXT).c_str()))
        //          {
        //              ImGui::Text("SIGHT OF GRACE INFO");
        //              ImGui::Spacing();
        //              ImGui::Text("LOCAL ID: ");
        //              ImGui::SameLine();
        //              ImGui::Text(std::to_string(g_WorldCharMan->EntityObjectBase[i]->LOCALID).c_str());
        //              ImGui::Text("PARAM ID: ");
        //              ImGui::SameLine();
        //              ImGui::Text(std::to_string(g_WorldCharMan->EntityObjectBase[i]->PARAMID).c_str());
        //              ImGui::Text("ALLIANCE: ");
        //              ImGui::SameLine();
        //              ImGui::Text(std::to_string(g_WorldCharMan->EntityObjectBase[i]->ALLIANCE).c_str());
        //              ImGui::Text("GLOBAL ID: ");
        //              ImGui::SameLine();
        //              ImGui::Text(std::to_string(g_WorldCharMan->EntityObjectBase[i]->GLOBALID).c_str());
        //              ImGui::Text("ANIMATION: ");
        //              ImGui::SameLine();
        //              ImGui::Text(std::to_string(g_WorldCharMan->CharTimeAct[i]->Animation).c_str());
        //              ImGui::Text("DISTANCE: ");
        //              ImGui::SameLine();
        //              ImGui::Text(std::to_string(g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position)).c_str());
        //              if (ImGui::Button("TELEPORT TO GRACE", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
        //                  g_WorldCharMan->pCharPhysics->Position = Vector3(g_WorldCharMan->CharPhysics[i]->Position.x, g_WorldCharMan->CharPhysics[i]->Position.y + 2, g_WorldCharMan->CharPhysics[i]->Position.z);
        //                  g_Console->printdbg("[+] MENU::EntityList; TELEPORT TO SIGHT OF GRACE\n", Console::Colors::green);
        //                  printf("COORDINATES:\nPosX: %f\nPosY: %f\nPosZ: %f\n", g_WorldCharMan->CharPhysics[i]->Position.x,
        //                      g_WorldCharMan->CharPhysics[i]->Position.y, g_WorldCharMan->CharPhysics[i]->Position.z);
        //              }
        //              ImGui::Spacing();
        //              ImGui::Separator();
        //          }
        //          ImGui::PopID();
        //          ImGui::Spacing();
        //      }
        //  }
        //  g_WorldCharMan->entwndw_count = count;
        //  count = NULL;
        //  
        //  ImGui::Spacing();
        //  ImGui::Separator();
        //  ImGui::Spacing();
        //  
        //  //  ENTITIES
        //  for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1)
        //  {
        //      if (g_WorldCharMan->CharTimeAct[i]->Animation < NULL) continue;
        //      if (g_WorldCharMan->EntityObjectBase[i]->ALLIANCE == g_WorldCharMan->Char_Faction.None) continue;
        //      count++;
        //      std::string OBJECT_TEXT = entity + std::to_string(count) + empty_space;
        //  
        //      ImGui::PushID(i);
        //      ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
        //      if (ImGui::CollapsingHeader((OBJECT_TEXT).c_str()))
        //      {
        //          ImGui::Text("ENTITY INFO");
        //          ImGui::Spacing();
        //          ImGui::Text("LOCAL ID: ");
        //          ImGui::SameLine();
        //          ImGui::Text(std::to_string(g_WorldCharMan->EntityObjectBase[i]->LOCALID).c_str());
        //          ImGui::Text("PARAM ID: ");
        //          ImGui::SameLine();
        //          ImGui::Text(std::to_string(g_WorldCharMan->EntityObjectBase[i]->PARAMID).c_str());
        //          ImGui::Text("ALLIANCE: ");
        //          ImGui::SameLine();
        //          ImGui::Text(std::to_string(g_WorldCharMan->EntityObjectBase[i]->ALLIANCE).c_str());
        //          ImGui::Text("GLOBAL ID: ");
        //          ImGui::SameLine();
        //          ImGui::Text(std::to_string(g_WorldCharMan->EntityObjectBase[i]->GLOBALID).c_str());
        //          ImGui::Text("ANIMATION: ");
        //          ImGui::SameLine();
        //          ImGui::Text(std::to_string(g_WorldCharMan->CharTimeAct[i]->Animation).c_str());
        //          ImGui::Text("DISTANCE: ");
        //          ImGui::SameLine();
        //          ImGui::Text(std::to_string(g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position)).c_str());
        //          ImGui::Separator();
        //          if (ImGui::Button("KILL TARGET", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
        //              g_WorldCharMan->CharData[i]->Health = 0;
        //          ImGui::Spacing();
        //  
        //          if (ImGui::Button("TELEPORT TO TARGET", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
        //              g_WorldCharMan->pCharPhysics->Position = g_WorldCharMan->CharPhysics[i]->Position;
        //              g_Console->printdbg("[+] MENU::EntityList; TELEPORT TO TARGET\n", Console::Colors::green);
        //              printf("COORDINATES:\nPosX: %f\nPosY: %f\nPosZ: %f\n", g_WorldCharMan->CharPhysics[i]->Position.x,
        //                  g_WorldCharMan->CharPhysics[i]->Position.y, g_WorldCharMan->CharPhysics[i]->Position.z);
        //          }
        //          ImGui::Spacing();
        //  
        //          if (ImGui::Button("TELEPORT TO CROSSHAIR", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
        //              g_WorldCharMan->CharPhysics[i]->Position = g_WorldCharMan->pCharPhysics->Position;
        //              g_Console->printdbg("[+] MENU::EntityList; TELEPORT TO CROSSHAIR\n", Console::Colors::green);
        //              printf("COORDINATES:\nPosX: %f\nPosY: %f\nPosZ: %f\n", g_WorldCharMan->CharPhysics[i]->Position.x,
        //                  g_WorldCharMan->CharPhysics[i]->Position.y, g_WorldCharMan->CharPhysics[i]->Position.z);
        //          }
        //          ImGui::Spacing();
        //  
        //          ImGui::Text("FREEZE TARGET: ");
        //          ImGui::SameLine();
        //          if (ImGui::Toggle("##FREEZE TARGET", &m_dbgFreezeTarget)) {
        //              g_Console->printdbg("[+] MENU::EntityList; FREEZE TARGET {not implemented}\n", Console::Colors::red);
        //          }
        //          ImGui::Spacing();
        //  
        //  
        //          ImGui::Text("DRAW SKELETON: ");
        //          ImGui::SameLine();
        //          ImGui::Toggle("##DrawSkele", &g_WorldCharMan->CharFall[i]->DrawSkeleton);
        //          ImGui::Spacing();
        //  
        //          ImGui::Text("HEALTH:  ");
        //          ImGui::SameLine();
        //          ImGui::InputInt("##Health", &g_WorldCharMan->CharData[i]->Health, 1, 10);
        //  
        //          ImGui::Text("MANA:    ");
        //          ImGui::SameLine();
        //          ImGui::InputInt("##Mana", &g_WorldCharMan->CharData[i]->Mana, 1, 10);
        //  
        //          ImGui::Text("STAMINA: ");
        //          ImGui::SameLine();
        //          ImGui::InputInt("##Stamina", &g_WorldCharMan->CharData[i]->Stamina, 1, 10);
        //  
        //          ImGui::Text("PosX:    ");
        //          ImGui::SameLine();
        //          ImGui::InputFloat("##PosX", &g_WorldCharMan->CharPhysics[i]->Position.x, 1.0f, 10.0f);
        //  
        //          ImGui::Text("PosY:    ");
        //          ImGui::SameLine();
        //          ImGui::InputFloat("##PosY", &g_WorldCharMan->CharPhysics[i]->Position.y, 1.0f, 10.0f);
        //  
        //          ImGui::Text("PosZ:    ");
        //          ImGui::SameLine();
        //          ImGui::InputFloat("##PosZ", &g_WorldCharMan->CharPhysics[i]->Position.z, 1.0f, 10.0f);
        //          ImGui::Spacing();
        //          ImGui::Separator();
        //      }
        //      ImGui::PopID();
        //      ImGui::Spacing();
        //  }
        //  g_WorldCharMan->entwndw_count2 = count;
        //  ImGui::End();
    }

    void Menu::DrawMatrixHelper()
    {
        //  ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
        //  if (!ImGui::Begin("MATRIX FLOATS", &m_dbgMatrixWnd, 96)) {
        //      ImGui::End();
        //      return;
        //  }
        //  
        //  for (int i = 0; i <= 15; i = i + 1)
        //  {
        //      ImGui::PushID(i);
        //      if (i <= 9)
        //      {
        //          std::string a = "MATRIX  [" + std::to_string(i) + "]: ";
        //          ImGui::Text(a.c_str());
        //      }
        //      else if (i > 9)
        //      {
        //          std::string a = "MATRIX [" + std::to_string(i) + "]: ";
        //          ImGui::Text(a.c_str());
        //      }
        //      std::string b = "##MATRIX [" + std::to_string(i) + "]: ";
        //  
        //  
        //      //ImGui::Text(a.c_str());
        //      ImGui::SameLine();
        //      ImGui::SliderFloat(b.c_str(), &ViewMatrix[i], -30.0f, 30.0f, "%.2f");
        //      ImGui::SameLine();
        //      if (ImGui::Button("+"))
        //          ViewMatrix[i] += .10f;
        //      ImGui::SameLine();
        //      if (ImGui::Button("-"))
        //          ViewMatrix[i] -= .10f;
        //      ImGui::SameLine();
        //      if (ImGui::Button("RESET"))
        //          ViewMatrix[i] = 0.0f;
        //      ImGui::PopID();
        //  }
        //  
        //  if (ImGui::Button("RESET ALL", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
        //  {
        //      for (int i = 0; i <= 15; i = i + 1)
        //          ViewMatrix[i] = 0;
        //  }
        //  
        //  ImGui::Separator();
        //  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
        //  if (ImGui::Button("RAINBOW ESP", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
        //  {
        //      g_Menu->dbg_ENT_RGB = !g_Menu->dbg_ENT_RGB;
        //      g_Console->LogEvent("[+] MENU:: DEBUG ESP WINDOW:: RGB LINES ; ", dbg_ENT_RGB);
        //  }
        //  ImGui::PopStyleColor();
        //  ImGui::End();
    }

    void Menu::bgLoops()
    {
        //  //  FREEZE ENTITIES
        //  if (f_TOGGLE)
        //  {
        //      g_WorldCharMan->stallENTS();
        //      //d_StallEnts(s_drawDistance);
        //      if (g_WorldCharMan->pCharData->Health == NULL) {
        //          f_TOGGLE = FALSE;
        //          for (int i = 0; i < sizeof(storedPOS) / sizeof(storedPOS[0]); i = i + 1)
        //              storedPOS[i] = Vector3(0, 0, 0);
        //          g_Console->printdbg("[+] MENU:: FREEZE ENTITIES; OFF {Health is NULL}\n\n", Console::Colors::red);
        //      }
        //  }
        //  
        //  if (g_GameFunctions->m_BARRIER)
        //      g_GameFunctions->Barrier(g_GameFunctions->m_BARRIER_DISTANCE);
        //  
        //  //  CUSTOM FPS LIMIT 
        //  //  CREDIT: techiew
        //  if (c_FPS != f_FPS)
        //  {
        //      uintptr_t RESET_FPS = g_GameVariables->m_ModuleBase + g_GameVariables->offsets.ptr_SET_FPS;      //  FPS ADDRESS
        //      int x = *(int*)RESET_FPS;   //  READ CURRENT FPS VALUE
        //      char oBytes[sizeof x];      //  CONVERT TO BYTE ARRAY
        //      std::copy(reinterpret_cast<const char*>(reinterpret_cast<const void*>(&x)), reinterpret_cast<const char*>(reinterpret_cast<const void*>(&x)) + sizeof x, oBytes);
        //  
        //      //  ORIGINAL BYTES & PATCH BYTES
        //      std::vector<uint16_t> oBytes2 = { (unsigned short)oBytes[0], (unsigned short)oBytes[1], (unsigned short)oBytes[2], (unsigned short)oBytes[3] }; //= arrayOfByte; //{ 0x0, 0x0, 0x0, 0x0 };
        //      std::vector<uint8_t> bytes(4, 0x90);
        //  
        //      //  RESET CURRENT FPS
        //      c_FPS = f_FPS;
        //      float frames = (1000 / f_FPS) / 1000;
        //      memcpy(&bytes[0], &frames, 4);
        //      if (g_GameFunctions->Replace(RESET_FPS, oBytes2, bytes)) {
        //          printf("CUSTOM FPS: %f\n", f_FPS);
        //      }
        //      else
        //          g_Console->printdbg("[!] MENU:: CUSTOM FPS ; failed\n", Console::Colors::green);
        //  }
    }
}