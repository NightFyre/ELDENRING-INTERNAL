#pragma once
#include "Common.hpp"
#include "Tabs/Main.h"
#include "Tabs/Character.h"
#include "Tabs/Entities.h"
#include "Tabs/Visuals.h"
#include "Tabs/Debug.h"

namespace ER
{	
	class Menu
	{
	public:	//	MEMBERS
		float										size[2];
		float										pos[2];
		float										maxWidth;
		float										maxHeight;
		const char*									title			= "HEXINTON";
		const char*									m_MenuFooter	= "ELDEN RING - STEAM DECK";
		bool										bShowMenu;
		bool										bShowDemo;
		bool										bShowStyleEditor;


		//	Cheat Controllers
		bool										bInfiniteHealth;
		bool										bInfiniteMana;
		bool										bInfiniteStamina;
	
	public:	//	FUNCTIONS
		void										Draw();
		void										DrawMenu();
		void										loops();

	public:	//	CONSTRUCTOR
		Menu()  noexcept = default;
		~Menu() noexcept = default;
		Menu(Menu const&) = delete;
		Menu(Menu&&) = delete;
		Menu& operator=(Menu const&) = delete;
		Menu& operator=(Menu&&) = delete;
	};
	inline std::unique_ptr<Menu> g_Menu;

	class DX12_Base
	{
	private:
		typedef HRESULT(APIENTRY* Present12) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		Present12 oPresent = NULL;

		typedef void(APIENTRY* ExecuteCommandLists)(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
		ExecuteCommandLists oExecuteCommandLists = NULL;

		typedef HRESULT(APIENTRY* ResizeTarget)(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters);
		ResizeTarget oResizeTarget = NULL;

		struct _FrameContext 
		{
			ID3D12CommandAllocator*					CommandAllocator;
			ID3D12Resource*							Resource;
			D3D12_CPU_DESCRIPTOR_HANDLE				DescriptorHandle;
		};
		WNDCLASSEXA									WindowClass;
		HWND										WindowHwnd;
		HWND										m_GameWindow{};
		IDXGISwapChain*								m_Swapchain{};
		IDXGISwapChain3*							m_Swapchain3{};
		DXGI_SWAP_CHAIN_DESC						m_SwapDesc{};
		ID3D12Device*								m_Device{};
		ID3D12DescriptorHeap*						m_DescriptorHeap{};
		ID3D12DescriptorHeap*						m_rtvDescriptorHeap{};
		ID3D12CommandAllocator**					m_CommandAllocator;
		ID3D12GraphicsCommandList*					m_CommandList{};
		ID3D12CommandQueue*							m_CommandQueue{};
		ID3D12Resource**							m_BackBuffer;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>	m_RenderTargets;
		uint64_t									m_BuffersCounts = -1;
		_FrameContext*								m_FrameContext{}; 
		uint64_t*									MethodsTable = NULL;

	public:
		bool										m_Init{};
		uint64_t									m_OldWndProc{};
		bool										m_ShowMenu	= TRUE;
		bool										bRunning	= FALSE;
		bool										bKillswitch = FALSE;

	public:
		bool										InitializeWindow(std::string windowName);
		bool										FindGameWindow(std::string title, HWND& out);
		bool										ObtainDevice(IDXGISwapChain* pSwapChain);
		void										InitializeImGui(ID3D12Device* pDevice);
		void										Overlay();
		void										ResetRenderState();
		void										DestroyWindowContext();
		void										DestroyImGuiContext();

	public:	//	Hook Helpers [Uses Minhook]
		bool										CreateHook(uint16_t Index, void** Original, void* Function);
		bool										DisableHook(uint16_t Index);
		void										EnableAllHooks();
		void										DisableAllHooks();

	public:	//	Hooked Functions in Target Process
		static LRESULT								WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static HRESULT APIENTRY						HookPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		static void									HookExecuteCommandLists(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
		static HRESULT APIENTRY						HookResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters);

	public:	//	CONSTRUCTOR
		explicit DX12_Base() = default;
		~DX12_Base() noexcept;
		DX12_Base(DX12_Base const&) = delete;
		DX12_Base(DX12_Base&&) = delete;
		DX12_Base& operator=(DX12_Base const&) = delete;
		DX12_Base& operator=(DX12_Base&&) = delete;
	};
	inline std::unique_ptr<DX12_Base> g_DX12;

	class Styles 
	{
	public:	//	MEMBERS

	public:	//	FUNCTIONS
		void										InitStyle();


	public:	//	CONSTRUCTOR
		explicit Styles() = default;
		Styles(Styles const&) = delete;
		Styles(Styles&&) = delete;
		Styles& operator=(Styles const&) = delete;
		Styles& operator=(Styles&&) = delete;
	};
	inline std::unique_ptr<Styles> g_Styles;
}