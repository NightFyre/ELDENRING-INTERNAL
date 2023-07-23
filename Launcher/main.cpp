#pragma once

//	Header Garbage
#include <Windows.h>
#include <Psapi.h>
#include <memory>
#include <tchar.h>
#include <string>
#include <thread>
#include <chrono>


//	Dx
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")

// DearImGui
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>


class EldenLauncher 
{
private:

    //  Window
    ID3D11Device*           pd3dDevice{};
    ID3D11DeviceContext*    pd3dDeviceContext{};
    IDXGISwapChain*         pSwapChain{};
    ID3D11RenderTargetView* mainRenderTargetView{};
    WNDCLASSEX              wc{};
    HWND                    hwnd{};
    ImVec4                  clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //  Process Info
    PROCESS_INFORMATION     pInfo;
    STARTUPINFO             sInfo;
    HANDLE                  hProc{};
    uintptr_t               dwModBase{};
    DWORD                   dwPID{};

public:
    bool                    bLauncherRunning{};
    bool                    bShowWindow{};
    bool                    bDemoWindow{};
    bool                    bINJECT{};


public:
    static LRESULT WINAPI   WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void                    InitializeWindow();
    void                    _DestroyWindow();
    bool                    CreateDeviceD3D(HWND hWnd);
    void                    CleanupDeviceD3D();
    void                    CreateRenderTarget();
    void                    CleanupRenderTarget();
    void                    InitStyle();
    void                    RenderUI();
    void                    UpdateWindow();
    bool                    _CreateProcess(const wchar_t* czPath, const wchar_t* czParams, bool bInject = FALSE);
    bool                    _TerminateProcesss();

    //  Constructor
    explicit EldenLauncher();
    ~EldenLauncher() noexcept = default;
    EldenLauncher(EldenLauncher const&) = delete;
    EldenLauncher(EldenLauncher&&) = delete;
    EldenLauncher& operator=(EldenLauncher const&) = delete;
    EldenLauncher& operator=(EldenLauncher&&) = delete;

};
inline std::unique_ptr<EldenLauncher> g_Launcher;


// 
inline std::atomic_bool		g_Running   = TRUE;
using namespace				std::chrono_literals;
int main()
{
    g_Launcher = std::make_unique<EldenLauncher>();
    g_Launcher->InitializeWindow();

    while (g_Running)
    {
        g_Launcher->UpdateWindow();
        std::this_thread::sleep_for(1ms);
        std::this_thread::yield();
    }

    g_Launcher->_DestroyWindow();
    return EXIT_SUCCESS;
}


EldenLauncher::EldenLauncher() {}
void EldenLauncher::InitializeWindow()
{
    wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL),
        NULL, NULL, NULL, NULL,
        _T("Dear ImGui Example Launcher"),
        NULL
    };

    ::RegisterClassEx(&wc);
    hwnd = ::CreateWindow(wc.lpszClassName, _T("ELDEN RING LAUNCHER"), WS_BORDER, 0, 0, 1280, 800, NULL, NULL, wc.hInstance, NULL);
    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }
    ::ShowWindow(hwnd, SW_SHOW);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
    io.IniFilename = NULL;                                      // Disable Ini File
    ImGuiStyle& style = ImGui::GetStyle();                      // Initial Style
    ImGui_ImplWin32_Init(hwnd);                                 // 
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);         // 
    this->InitStyle();
}

void EldenLauncher::_DestroyWindow()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    this->CleanupDeviceD3D();
    ::DestroyWindow(this->hwnd);
    ::UnregisterClass(this->wc.lpszClassName, this->wc.hInstance);
}

bool EldenLauncher::CreateDeviceD3D(HWND wndwHandle)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = wndwHandle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &this->pSwapChain, &this->pd3dDevice, &featureLevel, &this->pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;

}

void EldenLauncher::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    this->pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    this->pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &this->mainRenderTargetView);
    pBackBuffer->Release();
}

void EldenLauncher::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (this->pSwapChain)
    {
        this->pSwapChain->Release();
        this->pSwapChain = NULL;
    }

    if (this->pd3dDeviceContext)
    {
        this->pd3dDeviceContext->Release();
        this->pd3dDeviceContext = NULL;
    }

    if (this->pd3dDevice)
    {
        this->pd3dDevice->Release();
        this->pd3dDevice = NULL;
    }
}

void EldenLauncher::CleanupRenderTarget()
{
    if (!this->mainRenderTargetView)
        return;

    this->mainRenderTargetView->Release();
    this->mainRenderTargetView = NULL;
}

void EldenLauncher::UpdateWindow()
{
    MSG msg;
    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
        {
            g_Running = FALSE;
            return;
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //  RENDER OUR CUSTOM WINDOW HERE
    //  - Passing our boolean to close when done   
    RenderUI();

    // Rendering
    ImGui::Render();
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    pd3dDeviceContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    pd3dDeviceContext->ClearRenderTargetView(mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    pSwapChain->Present(1, 0); // Present with vsync
}

void EldenLauncher::RenderUI()
{
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ 1280, 800 });
    if (ImGui::Begin("ELDEN RING LAUNCHER", NULL, 96 | ImGuiWindowFlags_NoTitleBar) && g_Running)
    {
        if (ImGui::Button("LAUNCH", ImVec2(ImGui::GetContentRegionAvail().x - 12.f, (ImGui::GetContentRegionAvail().y / 2))))
        {
            if (!_CreateProcess(L"eldenring.exe", L"-eac-nop-loaded", FALSE))
            {
                MessageBoxA(NULL, "FAILED TO CREATE PROCESS & INJECT", ERROR, MB_ICONERROR);
                ImGui::End();
                return;
            }
            // _TerminateProcesss();
            // g_Running = FALSE;
        }
        ImGui::Separator();
        if (ImGui::Button("EXIT", ImVec2(ImGui::GetContentRegionAvail().x - 12.f, ImGui::GetContentRegionAvail().y)))
        {
            _TerminateProcesss();
            g_Running = FALSE;
        }
    }
    ImGui::End();
}

void EldenLauncher::InitStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = ImGui::GetStyle().Colors;

    //	STYLE PROPERTIES
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.LogSliderDeadzone = 4;
    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.6000000238418579f;
    style.WindowPadding = ImVec2(16.0f, 16.0f);
    style.WindowRounding = 4.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 4.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 4.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(8.0f, 8.0f);
    style.FrameRounding = 4.0f;
    style.FrameBorderSize = 2.0f;
    style.ItemSpacing = ImVec2(16.0f, 12.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.CellPadding = ImVec2(4.0f, 2.0f);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 24.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabMinSize = 16.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
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
}

bool EldenLauncher::_CreateProcess(const wchar_t* czPath, const wchar_t* czParams, bool bInject)
{
    ZeroMemory(&this->sInfo, sizeof(this->sInfo));
    this->sInfo.cb = sizeof(this->sInfo);
    ZeroMemory(&this->pInfo, sizeof(this->pInfo));

    if (!CreateProcessW(czPath, (LPWSTR)czParams, NULL, NULL, FALSE, NULL, NULL, NULL, &this->sInfo, &this->pInfo))
        return FALSE;

    this->hProc = this->pInfo.hProcess;
    this->dwPID = this->pInfo.dwProcessId;

    if (bInject)
    {
        const char* PATH = "tarnished";
        this->hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, this->dwPID);
        void* addr = VirtualAllocEx(this->hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(this->hProc, addr, PATH, strlen(PATH) + 1, 0);
        HANDLE hThread = CreateRemoteThread(this->hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, addr, 0, 0);
        if (!hThread)
        {
            VirtualFreeEx(this->hProc, addr, 0, MEM_RELEASE);
            return FALSE;
        }
        CloseHandle(hThread);
    }

    return TRUE;
}

bool EldenLauncher::_TerminateProcesss()
{
    bool result = TerminateProcess(this->hProc, EXIT_SUCCESS);
    this->hProc = nullptr;
    this->dwPID = NULL;
    this->dwModBase = NULL;

    return result;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI EldenLauncher::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_Launcher->pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_Launcher->CleanupRenderTarget();
            g_Launcher->pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            g_Launcher->CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}