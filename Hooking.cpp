#include "Hooking.hpp"
#include "D3DRenderer.hpp"
#include "Memory.hpp"
#include "Menu.hpp"

// TEST
#include "WorldCharMan.hpp"
namespace ER {

	//	Get Function Address
	__int64 Hooking::GetAddr(INT64 addr)
	{
		if (g_GameVariables->m_ModuleBase == NULL)
			g_GameVariables->m_ModuleBase = (uintptr_t)GetModuleHandleA(NULL);
		return (__int64)g_GameVariables->m_ModuleBase + addr;
	}


	//	MinHook
	void Hooking::HookInput(uint64_t a1, uint64_t a2)
	{
		if (g_GameVariables->m_ShowMenu)
			return;

		reinterpret_cast<decltype(&HookInput)>(g_Hooking->m_OriginalInputHandler)(a1, a2);
	}

	void Hooking::HookW2S(INT64 a1, INT64 a2, INT64 a3, void* a4)
	{
		if (g_Menu->dbgPrintW2S) {
			printf("### FUNCTION CALLED : WorldToScreen\n");
			printf("| a1 :  %llX\n", a1);
			printf("| a2 :  %llX\n", a2);
			printf("| a3 :  %llX\n", a3);
			printf("| a4 :  %p\n", a4);
			printf("-----------------------------------\n\n");
			g_Menu->dbgPrintW2S = FALSE;
		}
		g_Hooking->TRUE_W2S = a3;
		reinterpret_cast<decltype(&HookW2S)>(g_Hooking->m_OriginalW2SHandler)(a1, a2, a3, a4);
	}

	void Hooking::FindHooks()
	{
		m_InputHandler = Signature("48 8B C4 41 56 48 81 EC ? ? ? ? 48 C7 40 ? ? ? ? ? 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 0F 29 70 E8 0F 29 78 D8 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 50 ?").Scan().As<uint64_t>();
		m_W2SHandler = GetAddr(g_Menu->ptr_W2S_FUNCTION);
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

		///	MH_CreateHook(); Params
		// LPVOID pTarget, 
		// LPVOID pDetour, 
		// LPVOID *ppOriginal

		if (m_InputHandler)
			MH_CreateHook((LPVOID)m_InputHandler, &HookInput, &m_OriginalInputHandler);
		
		if (m_W2SHandler)
			MH_CreateHook((LPVOID)m_W2SHandler, &HookW2S, &m_OriginalW2SHandler);

		MH_EnableHook(MH_ALL_HOOKS);
	}

	void Hooking::Unhook()
	{
		g_D3DRenderer->Unhook();
		MH_RemoveHook(MH_ALL_HOOKS);
	}
}