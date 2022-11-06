#include "../include/Hooking.hpp"
#include "../include/D3DRenderer.hpp"
#include "../include/Memory.hpp"
#include "../include/Menu.hpp"

// TEST
#include "../include/WorldCharMan.hpp"
namespace ER {

	//	MinHook
	void Hooking::HookInput(uint64_t a1, uint64_t a2)
	{
		if (g_GameVariables->m_ShowMenu)
			return;

		reinterpret_cast<decltype(&HookInput)>(g_Hooking->m_OriginalInputHandler)(a1, a2);
	}

	void Hooking::FindHooks()
	{
		m_InputHandler = Signature("48 8B C4 41 56 48 81 EC ? ? ? ? 48 C7 40 ? ? ? ? ? 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 0F 29 70 E8 0F 29 78 D8 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 50 ?").Scan().As<uint64_t>();
	}

	Hooking::Hooking()
	{
		MH_Initialize();
	}

	Hooking::~Hooking()
	{
		MH_RemoveHook(MH_ALL_HOOKS);
	}

	void Hooking::Hook()
	{
		g_D3DRenderer->Hook();

		FindHooks();

		if (m_InputHandler)
			MH_CreateHook((LPVOID)m_InputHandler, &HookInput, &m_OriginalInputHandler);

		MH_EnableHook(MH_ALL_HOOKS);
	}

	void Hooking::Unhook()
	{
		g_D3DRenderer->Unhook();
		MH_RemoveHook(MH_ALL_HOOKS);
	}
}