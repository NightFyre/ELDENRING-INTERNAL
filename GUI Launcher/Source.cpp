#include "helper.hpp"
#include "DXWindow.hpp"
#include "Launcher.hpp"

//  MAIN FUNCTION
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    using namespace ERLauncher;
    g_LauncherVariables = std::make_unique<LauncherVariables>();
    g_LauncherVariables->hInstance = hInstance;
    g_DXwndw = std::make_unique<DXWindow>();
    g_Launcher = std::make_unique<Launcher>();

    while (g_Running)
    {
        g_DXwndw->UpdateWindow();
        std::this_thread::sleep_for(1ms);
        std::this_thread::yield();
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    g_DXwndw->CleanupDeviceD3D();
    ::DestroyWindow(g_DXwndw->hwnd);
    ::UnregisterClass(g_DXwndw->wc.lpszClassName, g_DXwndw->wc.hInstance);
    return 0;
}