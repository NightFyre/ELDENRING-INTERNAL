#include "DXWindow.hpp"
#include "Launcher.hpp"
#include "resource.h"
#include <dwmapi.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ERLauncher 
{

    MARGINS gMargin;
	DXWindow::DXWindow()
	{
        m_szWndw = { 280.f , 200.f };
        m_szScreen.x = GetSystemMetrics(SM_CXSCREEN);
        m_szScreen.y = GetSystemMetrics(SM_CYSCREEN);
        m_posWndw = { m_szScreen.x / 2 - m_szWndw.x / 2 , m_szScreen.y / 2 - m_szWndw.y / 2};

        m_wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), 
            LoadIcon(g_LauncherVariables->hInstance, MAKEINTRESOURCE(IDI_ICON1)), 
            NULL, NULL, NULL, 
            _T("Dear ImGui Example Launcher"), 
            LoadIcon(g_LauncherVariables->hInstance, MAKEINTRESOURCE(IDI_APPLICATION)) 
        };
        ::RegisterClassEx(&m_wc);
        //  m_hwnd = ::CreateWindow(m_wc.lpszClassName, _T("ELDEN RING LAUNCHER"), WS_BORDER, m_posWndw.x, m_posWndw.y, m_szWndw.x, m_szWndw.y, NULL, NULL, m_wc.hInstance, NULL);  //  debug window
        m_hwnd = ::CreateWindow(m_wc.lpszClassName, _T("ELDEN RING LAUNCHER"), WS_EX_TOPMOST | WS_POPUP, m_posWndw.x, m_posWndw.y, m_szWndw.x, m_szWndw.y, NULL, NULL, m_wc.hInstance, NULL);
        SetLayeredWindowAttributes(m_hwnd, 0, 255, LWA_ALPHA);
        gMargin = { 0, 0, static_cast<int>(m_szScreen.x), static_cast<int>(m_szScreen.y) };
        DwmExtendFrameIntoClientArea(m_hwnd, &gMargin);

        if (!CreateDeviceD3D(m_hwnd)) 
        {
            CleanupDeviceD3D();
            ::UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
            return;
        }
        ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
        ::UpdateWindow(m_hwnd);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
        io.IniFilename = NULL;                                      // Disable Ini File
        
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowTitleAlign = ImVec2(.5f, .5f);

        ImGui_ImplWin32_Init(m_hwnd);
        ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext);
	}

    DXWindow::~DXWindow()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
        DestroyWindow(m_hwnd);
        UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
    }

    void DXWindow::UpdateWindow()
    {
        static float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                g_LauncherVariables->m_LauncherRunning = FALSE;
        }

        if (!g_LauncherVariables->m_LauncherRunning) 
            g_Running = FALSE;

        ::SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        g_Launcher->Draw(&g_LauncherVariables->m_LauncherRunning);

        ImGui::Render();
        m_pd3dDeviceContext->OMSetRenderTargets(1, &m_mainRenderTargetView, NULL);
        m_pd3dDeviceContext->ClearRenderTargetView(m_mainRenderTargetView, (float*)clearColor);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        m_pSwapChain->Present(1, 0);
    }

    bool DXWindow::CreateDeviceD3D(HWND hWnd)
    {
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
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext) != S_OK)
            return false;

        CreateRenderTarget();

        return true;
    }

    void DXWindow::CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void DXWindow::CleanupDeviceD3D()
    {
        CleanupRenderTarget();

        if (m_pSwapChain)
        {
            m_pSwapChain->Release();
            m_pSwapChain = nullptr;
        }

        if (m_pd3dDeviceContext)
        {
            m_pd3dDeviceContext->Release();
            m_pd3dDeviceContext = nullptr;
        }

        if (m_pd3dDevice)
        {
            m_pd3dDevice->Release();
            m_pd3dDevice = nullptr;
        }
    }

    void DXWindow::CleanupRenderTarget()
    {
        if (m_mainRenderTargetView)
        {
            m_mainRenderTargetView->Release();
            m_mainRenderTargetView = nullptr;
        }
    }

    void DXWindow::SetWindowStyle(LONG flags) { SetWindowLong(m_hwnd, GWL_EXSTYLE, flags); }

    void DXWindow::SetWindowFocus(HWND window)
    {
        SetForegroundWindow(window);
        SetActiveWindow(window);
    }

    ImVec2 DXWindow::GetScreenSize() { return m_szScreen; }

    ImVec2 DXWindow::GetWindowPos() { return m_posWndw; }

    ImVec2 DXWindow::GetWindowSize() { return m_szWndw; }

    HWND DXWindow::GetWindowHandle() { return m_hwnd; }

    ID3D11Device* DXWindow::GetD3DDevice() { return m_pd3dDevice; }

    IDXGISwapChain* DXWindow::GetSwapChain() { return m_pSwapChain; }

    ID3D11DeviceContext* DXWindow::GetDeviceContext() { return m_pd3dDeviceContext; }

    ID3D11RenderTargetView* DXWindow::GetRTV() { return m_mainRenderTargetView; }

    LRESULT WINAPI DXWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
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
}