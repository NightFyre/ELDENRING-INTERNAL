#pragma once
#include "Common.hpp"
namespace ER
{
	struct Offsets {
		///	OFFSETS
		//	QWORDS
		int ptr_DEBUG_FLAGS = 0x3C4FB40;			//	OLD 0x03C23F78;	//	0x03C0C1C8;	//	DEBUG FLAGS
		int ptr_NBOTT_W2S = 0x3C482C8;		//	OLD 0x03C1C6E8;	// 0x03C04828;	//	NBOTT WorldToScreen qword_ptr
		int ptr_PLAYER_DATA = 0x3A18D80;	//	OLD 0x039F1D70;	//	0x039DAD60 	//	Player Data qword_ptr
		int ptr_W2S_FUNCTION = 0x0753CE0;	//	OLD 0x07527A0;	//  W2S Function	|	AOB			|	1.0.3 = 0x0752890	

		///  FMV SKIP NEW TEST
		//  STATIC ADDRESS!! NEEDS sig function applied!!
		//  AOB: 48 8B 90 ? ? ? ? 48 85 D2 74 07 C6
		//  OLD | 0x0A9415E |	0x0A8FA5E | 0x0A8FB4E
		int ptr_FMV_SKIP = 0x0A9970E;

		///  UNLOCK FPS LIMIT 
		//  STATIC ADDRESS!! NEEDS sig function applied!!
		//  ptr_SET_FPS: C7 ? ? 89 88 88 3C EB ? 89 ? 18 EB ? 89 ? 18 C7	|	AOB + 0x3	|	1.0.3 = E07F4F + 0x3 = 0xE07F52
		//  ptr_UNLOCK_FPS: C7 ? EF 3C 00 00 00 C7 ? F3 01 00 00 00			|   OLD 0x1944A37
		int ptr_SET_FPS = 0x0DFA6C2;	// 0x0DF0952;		//	OLD 0x0DE8B72;	//  FPS				|	AOB + 0x3	|	1.0.3 = E07F4F + 0x3 = 0xE07F52
		int ptr_UNLOCK_FPS = 0x1958797;

		//  STATIC ADDRESS!! NEEDS sig function applied!!
		//  Pause Game AOB: 0f 84 ? ? ? ? c6 83 ? ? 00 00 00 48 8d ? ? ? ? ? 48 89 ? ? 89 (Add 1byte)
		//  OLD | 0x0A81336 | 0x0A7CCD6 | 0xA7CDC6
		int ptr_PAUSE_GAME = 0x0A868F6;
	};
	
	class GameVariables
	{
	public:
		Offsets offsets;

		explicit GameVariables();
		~GameVariables() noexcept = default;
		GameVariables(GameVariables const&) = delete;
		GameVariables(GameVariables&&) = delete;
		GameVariables& operator=(GameVariables const&) = delete;
		GameVariables& operator=(GameVariables&&) = delete;

		//	Dx & ImGui
		int m_GamePid{};
		HANDLE m_GameHandle{};
		HWND m_GameWindow{};
		int m_GameWidth;
		int m_GameHeight;
		LPCSTR m_GameTitle;
		LPCSTR m_ClassName;
		LPCSTR m_GamePath;
		uintptr_t m_ModuleBase;		// OBTAIN MODULE BASE
		IDXGISwapChain3* m_Swapchain{};

		//	MENU RELATED
		bool m_ShowMenu = TRUE;
		bool m_ShowConsole{};
		bool m_ShowDemo{};
	};

	class GameFunctions
	{
	public:
		explicit GameFunctions();
		~GameFunctions() noexcept = default;
		GameFunctions(GameFunctions const&) = delete;
		GameFunctions(GameFunctions&&) = delete;
		GameFunctions& operator=(GameFunctions const&) = delete;
		GameFunctions& operator=(GameFunctions&&) = delete;
		
		bool m_BARRIER = FALSE;
		float m_BARRIER_DISTANCE = 0;

		void FMVSkip(uintptr_t addr);
		void UnlockFPS(uintptr_t addr);
		void PauseGameplay(uintptr_t addr, bool ACTIVE);
		float GetDistanceTo3D_Object(Vector3 POS, Vector3 POS2);
		void Barrier(float distance);

		//	Memory Functions (needs to be moved elsewhere)
		uintptr_t p2addy(uintptr_t PTR, std::vector<unsigned int> OFFSETS);
		bool Replace(uintptr_t address, std::vector<uint16_t> originalBytes, std::vector<uint8_t> newBytes);
	};

	inline std::unique_ptr<GameVariables> g_GameVariables;
	inline std::unique_ptr<GameFunctions> g_GameFunctions;
}