#include "D3DRenderer.hpp"
#include "Game.hpp"
#include "Hooking.hpp"
#include "Menu.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//  RAINBOW THEME OPTION
float HSV_RAINBOW_SPEED = 0.001;
static float HSV_RAINBOW_HUE = 0;
/// <summary>
/// 
/// </summary>
/// <param name="saturation"></param>
/// <param name="value"></param>
/// <param name="opacity"></param>
/// // PENDING RELOCATION TO d3drenderer.cpp
void SV_RAINBOW(float saturation, float value, float opacity)
{
	using namespace ER;
	HSV_RAINBOW_HUE -= HSV_RAINBOW_SPEED;
	if (HSV_RAINBOW_HUE < -1.f) HSV_RAINBOW_HUE += 1.f;
	for (int i = 0; i < 860; i++)
	{
		float hue = HSV_RAINBOW_HUE + (1.f / (float)860) * i;
		if (hue < 0.f) hue += 1.f;
		g_Menu->dbg_RAINBOW = ImColor::HSV(hue, (saturation / 255), (value / 255), (opacity / 255));
	}
}

namespace ER 
{

    static uint64_t* MethodsTable = NULL;

	bool D3DRenderer::Hook()
	{
		if (InitHook()) {
			CreateHook(54, (void**)&oExecuteCommandLists, HookExecuteCommandLists);
			CreateHook(140, (void**)&oPresent, HookPresent);
			CreateHook(146, (void**)&oResizeTarget, HookResizeTarget);
			return 1;
		}
		return 0;
	}

	void D3DRenderer::Unhook()
	{
		SetWindowLongPtr(g_GameVariables->m_GameWindow, GWLP_WNDPROC, (LONG_PTR)m_OldWndProc);
		DisableAll();
	}

    bool D3DRenderer::InitHook()
    {
		if (InitWindow() == false) {
			return false;
		}

		HMODULE D3D12Module = GetModuleHandle("d3d12.dll");
		HMODULE DXGIModule = GetModuleHandle("dxgi.dll");
		if (D3D12Module == NULL || DXGIModule == NULL) {
			DeleteWindow();
			return false;
		}

		void* CreateDXGIFactory = GetProcAddress(DXGIModule, "CreateDXGIFactory");
		if (CreateDXGIFactory == NULL) {
			DeleteWindow();
			return false;
		}

		IDXGIFactory* Factory;
		if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&Factory) < 0) {
			DeleteWindow();
			return false;
		}

		IDXGIAdapter* Adapter;
		if (Factory->EnumAdapters(0, &Adapter) == DXGI_ERROR_NOT_FOUND) {
			DeleteWindow();
			return false;
		}

		void* D3D12CreateDevice = GetProcAddress(D3D12Module, "D3D12CreateDevice");
		if (D3D12CreateDevice == NULL) {
			DeleteWindow();
			return false;
		}

		ID3D12Device* Device;
		if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&Device) < 0) {
			DeleteWindow();
			return false;
		}

		D3D12_COMMAND_QUEUE_DESC QueueDesc;
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = 0;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.NodeMask = 0;

		ID3D12CommandQueue* CommandQueue;
		if (Device->CreateCommandQueue(&QueueDesc, __uuidof(ID3D12CommandQueue), (void**)&CommandQueue) < 0) {
			DeleteWindow();
			return false;
		}

		ID3D12CommandAllocator* CommandAllocator;
		if (Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator) < 0) {
			DeleteWindow();
			return false;
		}

		ID3D12GraphicsCommandList* CommandList;
		if (Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList) < 0) {
			DeleteWindow();
			return false;
		}

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = WindowHwnd;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* SwapChain;
		if (Factory->CreateSwapChain(CommandQueue, &SwapChainDesc, &SwapChain) < 0) {
			DeleteWindow();
			return false;
		}

		MethodsTable = (uint64_t*)::calloc(150, sizeof(uint64_t));
		memcpy(MethodsTable, *(uint64_t**)Device, 44 * sizeof(uint64_t));
		memcpy(MethodsTable + 44, *(uint64_t**)CommandQueue, 19 * sizeof(uint64_t));
		memcpy(MethodsTable + 44 + 19, *(uint64_t**)CommandAllocator, 9 * sizeof(uint64_t));
		memcpy(MethodsTable + 44 + 19 + 9, *(uint64_t**)CommandList, 60 * sizeof(uint64_t));
		memcpy(MethodsTable + 44 + 19 + 9 + 60, *(uint64_t**)SwapChain, 18 * sizeof(uint64_t));
		Sleep(1000);
		
		//	INIT NOTICE
		Beep(300, 300);

		MH_Initialize();
		Device->Release();
		Device = NULL;
		CommandQueue->Release();
		CommandQueue = NULL;
		CommandAllocator->Release();
		CommandAllocator = NULL;
		CommandList->Release();
		CommandList = NULL;
		SwapChain->Release();
		SwapChain = NULL;
		DeleteWindow();
		return true;
    }

	void D3DRenderer::Overlay(IDXGISwapChain* pSwapChain)
	{
		if (m_CommandQueue == nullptr)
			return;

		ID3D12CommandQueue* pCmdQueue = this->m_CommandQueue;

		IDXGISwapChain3* pSwapChain3 = nullptr;
		DXGI_SWAP_CHAIN_DESC sc_desc;
		pSwapChain->QueryInterface(IID_PPV_ARGS(&pSwapChain3));
		if (pSwapChain3 == nullptr)
			return;

		pSwapChain3->GetDesc(&sc_desc);

		if (!m_Init)
		{
			UINT bufferIndex = pSwapChain3->GetCurrentBackBufferIndex();
			ID3D12Device* pDevice;
			if (pSwapChain3->GetDevice(IID_PPV_ARGS(&pDevice)) != S_OK)
				return;

			m_BuffersCounts = sc_desc.BufferCount;

			m_RenderTargets.clear();

			{
				D3D12_DESCRIPTOR_HEAP_DESC desc = {};
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				desc.NumDescriptors = 1;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				if (pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_DescriptorHeap)) != S_OK)
				{
					pDevice->Release();
					pSwapChain3->Release();
					return;
				}
			}
			{
				D3D12_DESCRIPTOR_HEAP_DESC desc = {};
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				desc.NumDescriptors = m_BuffersCounts;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc.NodeMask = 1;
				if (pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvDescriptorHeap)) != S_OK)
				{
					pDevice->Release();
					pSwapChain3->Release();
					m_DescriptorHeap->Release();
					return;
				}

				SIZE_T rtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
				m_CommandAllocator = new ID3D12CommandAllocator * [m_BuffersCounts];
				for (int i = 0; i < m_BuffersCounts; ++i)
				{
					m_RenderTargets.push_back(rtvHandle);
					rtvHandle.ptr += rtvDescriptorSize;
				}
			}

			for (UINT i = 0; i < sc_desc.BufferCount; ++i)
			{
				if (pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator[i])) != S_OK)
				{
					pDevice->Release();
					pSwapChain3->Release();
					m_DescriptorHeap->Release();
					for (UINT j = 0; j < i; ++j)
					{
						m_CommandAllocator[j]->Release();
					}
					m_rtvDescriptorHeap->Release();
					delete[]m_CommandAllocator;
					return;
				}
			}

			if (pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator[0], NULL, IID_PPV_ARGS(&m_CommandList)) != S_OK ||
				m_CommandList->Close() != S_OK)
			{
				pDevice->Release();
				pSwapChain3->Release();
				m_DescriptorHeap->Release();
				for (UINT i = 0; i < m_BuffersCounts; ++i)
					m_CommandAllocator[i]->Release();
				m_rtvDescriptorHeap->Release();
				delete[]m_CommandAllocator;
				return;
			}

			m_BackBuffer = new ID3D12Resource * [m_BuffersCounts];
			for (UINT i = 0; i < m_BuffersCounts; i++)
			{
				pSwapChain3->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffer[i]));
				pDevice->CreateRenderTargetView(m_BackBuffer[i], NULL, m_RenderTargets[i]);
			}

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.IniFilename = NULL;

			g_GameVariables->m_GameWindow = FindWindow("ELDEN RING™", NULL);

			ImGui_ImplWin32_Init(g_GameVariables->m_GameWindow);
			ImGui_ImplDX12_Init(pDevice, m_BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM, NULL,
				m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());
			ImGui_ImplDX12_CreateDeviceObjects();
			ImGui::GetIO().ImeWindowHandle = g_GameVariables->m_GameWindow;
			m_OldWndProc = SetWindowLongPtr(g_GameVariables->m_GameWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

			m_Init = true;

			pDevice->Release();
		}


		SV_RAINBOW(169, 169, 200);	//	RGB THEME
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::GetIO().MouseDrawCursor = g_GameVariables->m_ShowMenu;

		g_Menu->Draw();

		ImGui::EndFrame();

		UINT bufferIndex = pSwapChain3->GetCurrentBackBufferIndex();

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_BackBuffer[bufferIndex];
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		m_CommandAllocator[bufferIndex]->Reset();
		m_CommandList->Reset(m_CommandAllocator[bufferIndex], NULL);
		m_CommandList->ResourceBarrier(1, &barrier);
		m_CommandList->OMSetRenderTargets(1, &m_RenderTargets[bufferIndex], FALSE, NULL);
		m_CommandList->SetDescriptorHeaps(1, &m_DescriptorHeap);

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_CommandList);

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		m_CommandList->ResourceBarrier(1, &barrier);
		m_CommandList->Close();

		pCmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&m_CommandList);

		pSwapChain3->Release();
	}

	HRESULT APIENTRY D3DRenderer::HookResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters)
	{
		g_D3DRenderer->ResetRenderState();
		return g_D3DRenderer->oResizeTarget(_this, pNewTargetParameters);
	}

	LRESULT D3DRenderer::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui::GetCurrentContext())
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

		return CallWindowProc((WNDPROC)g_D3DRenderer->m_OldWndProc, hWnd, msg, wParam, lParam);
	}

    HRESULT APIENTRY D3DRenderer::HookPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
    {
		if (g_KillSwitch) {
			g_Hooking->Unhook();
			g_D3DRenderer->oPresent(pSwapChain, SyncInterval, Flags);
			g_Running = FALSE;
			return 0;

		}
		g_D3DRenderer->Overlay(pSwapChain);
		return g_D3DRenderer->oPresent(pSwapChain, SyncInterval, Flags);

    }

	void D3DRenderer::HookExecuteCommandLists(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) {
		if (!g_D3DRenderer->m_CommandQueue)
			g_D3DRenderer->m_CommandQueue = queue;

		g_D3DRenderer->oExecuteCommandLists(queue, NumCommandLists, ppCommandLists);
	}

	bool D3DRenderer::Init(IDXGISwapChain3* swapChain)
	{
		m_Swapchain = swapChain;
		if (SUCCEEDED(m_Swapchain->GetDevice(__uuidof(ID3D12Device), (void**)&m_Device))) {
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			io.IniFilename = NULL;

			DXGI_SWAP_CHAIN_DESC Desc;
			m_Swapchain->GetDesc(&Desc);
			Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			Desc.OutputWindow = g_GameVariables->m_GameWindow;
			Desc.Windowed = ((GetWindowLongPtr(g_GameVariables->m_GameWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

			m_BuffersCounts = Desc.BufferCount;
			m_FrameContext = new _FrameContext[m_BuffersCounts];

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
			DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			DescriptorImGuiRender.NumDescriptors = m_BuffersCounts;
			DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			if (m_Device->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&m_DescriptorHeap)) != S_OK)
				return 0;

			ID3D12CommandAllocator* Allocator;
			if (m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
				return 0;

			for (size_t i = 0; i < m_BuffersCounts; i++) {
				m_FrameContext[i].CommandAllocator = Allocator;
			}

			if (m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&m_CommandList)) != S_OK || m_CommandList->Close() != S_OK)
				return 0;

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
			DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			DescriptorBackBuffers.NumDescriptors = m_BuffersCounts;
			DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DescriptorBackBuffers.NodeMask = 1;

			if (m_Device->CreateDescriptorHeap(&DescriptorBackBuffers, IID_PPV_ARGS(&m_rtvDescriptorHeap)) != S_OK)
				return 0;

			const auto RTVDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

			for (size_t i = 0; i < m_BuffersCounts; i++) {
				ID3D12Resource* pBackBuffer = nullptr;
				m_FrameContext[i].DescriptorHandle = RTVHandle;
				m_Swapchain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
				m_Device->CreateRenderTargetView(pBackBuffer, nullptr, RTVHandle);
				m_FrameContext[i].Resource = pBackBuffer;
				RTVHandle.ptr += RTVDescriptorSize;
			}

			g_GameVariables->m_GameWindow = FindWindow("ELDEN RING™", NULL);
			m_OldWndProc = SetWindowLongPtrA(g_GameVariables->m_GameWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui_ImplWin32_Init(g_GameVariables->m_GameWindow);
			ImGui_ImplDX12_Init(m_Device, m_BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM, NULL, m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());
			ImGui_ImplDX12_CreateDeviceObjects();
			ImGui::GetIO().ImeWindowHandle = g_GameVariables->m_GameWindow;
			m_Device->Release();
            return 1;
		}

        return 0;

	}

	bool D3DRenderer::InitWindow() {

		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = DefWindowProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = GetModuleHandle(NULL);
		WindowClass.hIcon = NULL;
		WindowClass.hCursor = NULL;
		WindowClass.hbrBackground = NULL;
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = "MJ";
		WindowClass.hIconSm = NULL;
		RegisterClassEx(&WindowClass);
		WindowHwnd = CreateWindow(WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
		if (WindowHwnd == NULL) {
			return false;
		}
		return true;
	}

	void D3DRenderer::ResetRenderState()
	{
		if (m_Init)
		{
			m_DescriptorHeap->Release();
			for (UINT i = 0; i < m_BuffersCounts; ++i)
			{
				m_CommandAllocator[i]->Release();
				m_BackBuffer[i]->Release();
			}
			m_rtvDescriptorHeap->Release();
			delete[]m_CommandAllocator;
			delete[]m_BackBuffer;

			ImGui_ImplDX12_Shutdown();
			//Windows_Hook::Inst()->resetRenderState();
			SetWindowLongPtr(g_GameVariables->m_GameWindow, GWLP_WNDPROC, (LONG_PTR)m_OldWndProc);
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			m_Init = false;
		}
	}

	bool D3DRenderer::DeleteWindow() {
		DestroyWindow(WindowHwnd);
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		if (WindowHwnd != NULL) {
			return false;
		}
		return true;
	}

    bool D3DRenderer::CreateHook(uint16_t Index, void** Original, void* Function) {
        void* target = (void*)MethodsTable[Index];
        if (MH_CreateHook(target, Function, Original) != MH_OK || MH_EnableHook(target) != MH_OK) {
            return false;
        }
        return true;
    }

    void D3DRenderer::DisableHook(uint16_t Index) {
        MH_DisableHook((void*)MethodsTable[Index]);
    }

    void D3DRenderer::DisableAll() {
		DisableHook(MethodsTable[54]);
		DisableHook(MethodsTable[140]);
		DisableHook(MethodsTable[146]);
    }

	D3DRenderer::~D3DRenderer() noexcept
	{
		Unhook();
	}


	//	GUIDED HACKING TEMPLATE
	bool D3DRenderer::WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight)
	{
		//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
		Vector4 clipCoords;
		clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
		clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
		clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
		clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

		if (clipCoords.w < 0.1f)
			return false;

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		Vector3 NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
		screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
		return true;
	}
}