#include "Launcher.hpp"
#include "DXWindow.hpp"

namespace ERLauncher {

	LauncherVariables::LauncherVariables()
	{
        m_LauncherRunning = TRUE;
        m_ShowWindow = TRUE;
        hInstance;
	}

    Launcher::Launcher()
    {
        InitStyle();
    }

    void Launcher::InitStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = ImGui::GetStyle().Colors;
        ImGui::StyleColorsDark();
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.FrameRounding = 1.0f;
        style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
        style.WindowRounding = 4.0f;
        style.FrameBorderSize = 0.75f;

        //	COLORS
        colors[ImGuiCol_Text] = ImVec4(0.75f, 0.75f, 0.29f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.27f, 0.00f, 0.63f, 0.43f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.74f, 0.74f, 0.29f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
        colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.90f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
        return;
    }

    void Launcher::Draw(bool* p_OPEN)
    {
        if (!ImGui::Begin("ELDEN RING LAUNCHER", p_OPEN, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::End();
            return;
        }

        //  LAUNCH OPTIONS
        if (g_LauncherVariables->Proc.PID == NULL)
        {
            //  CREATE PROCESS
            if (ImGui::Button("LAUNCH", g_LauncherVariables->BUTTONS))
            {
                g_LauncherVariables->m_INJECT = FALSE;
                if (!createProcess(g_LauncherVariables->m_INJECT)) {
                    MessageBoxA(NULL, "FAILED TO CREATE PROCESS", ERROR, MB_ICONERROR);
                }
            }

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Spacing();

            //  CREATE PROCESS + INJECT
            if (ImGui::Button("LAUNCH & INJECT", ImVec2(200, 30)))
            {
                g_LauncherVariables->m_INJECT = TRUE;
                if (!createProcess(g_LauncherVariables->m_INJECT)) {
                    MessageBoxA(NULL, "FAILED TO CREATE PROCESS & INJECT", ERROR, MB_ICONERROR);
                }
            }
        }

        //  CLEAN EXIT AFTER LAUNCH
        else {
            clearProcess(FALSE);
            g_Running = FALSE;
        }

        ///  CREATE YOUR EXTERNAL TRAINER HERE
        //else
        //{
        //    g_LauncherVariables->Proc.exitCode = 0;
        //    if (GetExitCodeProcess(g_LauncherVariables->Proc.hProc, &g_LauncherVariables->Proc.exitCode) && g_LauncherVariables->Proc.exitCode == STILL_ACTIVE) {
        //        //  Selecting this will bring back to the launcher main menu as well as terminate the game process
        //        if (ImGui::Button("QUIT GAME", g_LauncherVariables->BUTTONS))
        //            clearProcess(TRUE);
        //        ImGui::Spacing();
        //        ImGui::Spacing();
        //        ImGui::Separator();
        //        ImGui::Spacing();
        //        ImGui::Spacing();
        //        //  Closes the launcher, The game will remain running
        //        if (ImGui::Button("EXIT", g_LauncherVariables->BUTTONS)) {
        //            clearProcess(FALSE);
        //            g_Running = FALSE;
        //        }
        //    }
        //    //  Connection to process has been lost, clear process info and Exit
        //    else {
        //        clearProcess(FALSE);
        //        g_Running = FALSE;
        //    }
        //}

        ImGui::End();
	}

    bool Launcher::createProcess(bool INJECT)
    {
        ZeroMemory(&g_LauncherVariables->Proc.sInfo, sizeof(g_LauncherVariables->Proc.sInfo));
        g_LauncherVariables->Proc.sInfo.cb = sizeof(g_LauncherVariables->Proc.sInfo);
        ZeroMemory(&g_LauncherVariables->Proc.pInfo, sizeof(g_LauncherVariables->Proc.pInfo));
        if (!CreateProcessW(g_LauncherVariables->Proc.PATH, 
            (LPWSTR)g_LauncherVariables->Proc.PARAMS, 
            NULL, NULL, FALSE, 0, NULL, NULL, 
            &g_LauncherVariables->Proc.sInfo, 
            &g_LauncherVariables->Proc.pInfo
        ))
            return FALSE;

        //  OBTAIN PROCESS INFORMATION
        SuspendThread(g_LauncherVariables->Proc.pInfo.hThread);
        ResumeThread(g_LauncherVariables->Proc.pInfo.hThread);
        g_LauncherVariables->Proc.PID = GetProcessId(g_LauncherVariables->Proc.pInfo.hProcess);
        g_LauncherVariables->Proc.hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_LauncherVariables->Proc.PID);
        
        if (INJECT) {
            //  Method for injection . . .
            if (!Inject(g_LauncherVariables->Proc.hProc)) {
                clearProcess(TRUE);
                return FALSE;
            }
        }

        return TRUE;
    }

    void Launcher::clearProcess(bool TERMINATE)
    {
        // Close Process
        if (TERMINATE)
            TerminateProcess(g_LauncherVariables->Proc.hProc, EXIT_SUCCESS);

        //  Clear Info
        CloseHandle(g_LauncherVariables->Proc.pInfo.hThread);
        CloseHandle(g_LauncherVariables->Proc.pInfo.hProcess);
        Sleep(100);

        g_LauncherVariables->Proc.exitCode = NULL;
        g_LauncherVariables->Proc.hProc = NULL;
        g_LauncherVariables->Proc.PID = NULL;
        g_LauncherVariables->Proc.moduleBase = NULL;
        g_LauncherVariables->Proc.pInfo;
        g_LauncherVariables->Proc.sInfo;
        return;
    }

    bool Launcher::Inject(HANDLE Process)
    {
        const char* PATH = "CUSTOM\\ER Overlay.dll";

        void* addr = VirtualAllocEx(Process, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(Process, addr, PATH, strlen(PATH) + 1, 0);
        HANDLE hThread = CreateRemoteThread(Process, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, addr, 0, 0);
        if (!hThread)
        {
            VirtualFreeEx(Process, addr, 0, MEM_RELEASE);
            return FALSE;
        }
        CloseHandle(hThread);
        return TRUE;
    }
}