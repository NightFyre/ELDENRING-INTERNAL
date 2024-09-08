#pragma once
#include "helper.hpp"

namespace ERLauncher {
	

	class DXWindow 
	{
	public:
		void SetWindowStyle(LONG style);
		void SetWindowFocus(HWND window);
		ImVec2 GetScreenSize();
		ImVec2 GetWindowSize();
		ImVec2 SetWindowPos();
		ImVec2 GetWindowPos();
		HWND GetWindowHandle();
		ID3D11Device* GetD3DDevice();
		IDXGISwapChain* GetSwapChain();
		ID3D11DeviceContext* GetDeviceContext();
		ID3D11RenderTargetView* GetRTV();

	public:
		void UpdateWindow();

	public:
		explicit DXWindow();
		~DXWindow();

	private:
		HWND m_hwnd{ 0 };
		WNDCLASSEX m_wc{ };
		ID3D11Device* m_pd3dDevice{ nullptr };
		IDXGISwapChain* m_pSwapChain{ nullptr };
		ID3D11DeviceContext* m_pd3dDeviceContext{ nullptr };
		ID3D11RenderTargetView* m_mainRenderTargetView{ nullptr };
		ImVec2 m_szScreen{ 0.0f, 0.0f };
		ImVec2 m_szWndw{ 0.f, 0.f };
		ImVec2 m_posWndw{ 0.f, 0.f };

	private:
		bool CreateDeviceD3D(HWND hWnd);
		void CleanupDeviceD3D();
		void CreateRenderTarget();
		void CleanupRenderTarget();

	private:
		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};

	class Styles {

	};
	inline std::unique_ptr<DXWindow> g_DXwndw;
	inline std::unique_ptr<Styles> g_Styles;
}