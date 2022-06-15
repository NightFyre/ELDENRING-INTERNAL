#include "Game.hpp"
#include "Menu.hpp"
#include "Console.hpp"

namespace ER {

	GameVariables::GameVariables()
	{

		m_GamePid = GetCurrentProcessId();
		m_GameHandle = GetCurrentProcess();
		m_GameWindow = FindWindow("ELDEN RING™", NULL);
		m_ModuleBase = (uintptr_t)GetModuleHandle(NULL);

		RECT TempRect;
		GetWindowRect(m_GameWindow, &TempRect);
		m_GameWidth = TempRect.right - TempRect.left;
		m_GameHeight = TempRect.bottom - TempRect.top;

		char TempTitle[MAX_PATH];
		GetWindowText(m_GameWindow, TempTitle, sizeof(TempTitle));
		m_GameTitle = TempTitle;

		char TempClassName[MAX_PATH];
		GetClassName(m_GameWindow, TempClassName, sizeof(TempClassName));
		m_ClassName = TempClassName;

		char TempPath[MAX_PATH];
		GetModuleFileNameEx(m_GameHandle, NULL, TempPath, sizeof(TempPath));
		m_GamePath = TempPath;

	}

	GameFunctions::GameFunctions()
	{
		return;
	}

	/// <summary>
	/// 
	/// </summary>
	void GameFunctions::FMVSkip(uintptr_t addr)
	{
		///  FMV SKIP NEW TEST
		//  STATIC ADDRESS!! NEEDS sig function applied!!
		//  AOB: 48 8B 90 ? ? ? ? 48 85 D2 74 07 C6
		//  OLD | 0x0A8FA5E | 0x0A8FB4E
		using namespace ER;
		Patch((BYTE*)addr + 0x0A9415E, (BYTE*)"\xE9\x1C\x00\x00\x00", 5);
		g_Console->printdbg("[+] FMV's SKIPPED\n", TRUE, g_Console->color.green);
	}

	/// <summary>
	/// 
	/// </summary>
	void GameFunctions::UnlockFPS(uintptr_t addr)
	{
		///  UNLOCK FPS LIMIT 
		//  STATIC ADDRESS!! NEEDS sig function applied!!
		//  AOB: C7 ? ? 89 88 88 3C EB ? 89 ? 18 EB ? 89 ? 18 C7 | (Menu.hpp->ptr_SET_FPS)
		//  AOB2: C7 ? EF 3C 00 00 00 C7 ? F3 01 00 00 00   |   OLD 0x1944A37
		using namespace ER;
		Patch((BYTE*)addr + 0x194CCC7, (BYTE*)"\xC7\x45\xEF\x00\x00\x00\x00", 7);  // AOB 2 | OLD 0x1944B27
		g_Console->printdbg("[+] FPS LIMIT REMOVED\n\n", TRUE, g_Console->color.green);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="addr"></param>
	/// <param name="ACTIVE"></param>
	void GameFunctions::PauseGameplay(uintptr_t addr, bool ACTIVE)
	{
		//  STATIC ADDRESS!! NEEDS sig function applied!!
		//  Pause Game AOB: 0f 84 ? ? ? ? c6 83 ? ? 00 00 00 48 8d ? ? ? ? ? 48 89 ? ? 89 (Add 1byte)
		//  OLD | 0A7CCD6 | 0xA7CDC6
		if (ACTIVE)
			Patch((BYTE*)addr + 0x0A81336, (BYTE*)"\x85", 1);
		else if (!ACTIVE)
			Patch((BYTE*)addr + 0x0A81336, (BYTE*)"\x84", 1);
	}

	//	Gets PTR address by resolving a pointer chain
	uintptr_t GameFunctions::p2addy(uintptr_t PTR, std::vector<unsigned int> OFFSETS)
	{
		uintptr_t addr = PTR;
		for (unsigned int i = 0; i < OFFSETS.size(); i++) {
			addr = *(uintptr_t*)addr;
			addr += OFFSETS[i];
		}
		return addr;
	}

	//	Gets distance from Position A to Position B
	float GameFunctions::GetDistanceTo3D_Object(Vector3 POS, Vector3 POS2)
	{
		float x = (POS2.x - POS.x);
		float y = (POS2.y - POS.y);
		float z = (POS2.z - POS.z);
		float distance = std::sqrt(x * x + y * y + z * z);
		return (distance);
	}

	//	Gets current game FPS
	void GameFunctions::FPS()
	{
		//  CREDIT: XBOX360LSBEST
		static bool init = false;
		static char text[64] = "FPS: unknown";
		static int frame = 0;
		static clock_t time = clock();
		frame++;
		if (clock() - time >= 1000)
		{
			memset(text, 0, sizeof(text));
			sprintf(text, "FPS: %i\n", frame);
			frame = 0;
			time = clock();
			init = true;
		}

		if (init && text[0])	//	CASE SWITCH FOR 3rd BOOLEAN
			if (g_Menu->dbg_RAINBOW_THEME)
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 5), ImColor(g_Menu->dbg_RAINBOW), text);
			else if (!g_Menu->dbg_RAINBOW_THEME)
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 5), ImColor(255, 255, 255, 255), text);
	}

	static constexpr int MASKED = 0xffff;
	// TEMP
	// Replaces the memory at a given address with newBytes. Performs memory comparison with originalBytes to stop unintended memory modification.
	bool GameFunctions::Replace(uintptr_t address, std::vector<uint16_t> originalBytes, std::vector<uint8_t> newBytes)
	{
		//  CREDIT: techiew
		std::vector<uint8_t> truncatedOriginalBytes;
		for (auto byte : originalBytes)
		{
			truncatedOriginalBytes.push_back((uint8_t)byte);
		}

		std::string bufferString = "";
		std::vector<uint8_t> buffer(originalBytes.size());
		memcpy(&buffer[0], (void*)address, buffer.size());
		for (size_t i = 0; i < buffer.size(); i++)
		{
			std::stringstream stream;
			stream << "0x" << std::hex << (unsigned int)buffer[i];
			std::string byte = stream.str();
			bufferString.append(byte);
			if (originalBytes[i] == MASKED)
			{
				bufferString.append("?");
				buffer[i] = 0xff;
			}
			bufferString.append(" ");
		}

		std::string newBytesString = "";
		for (size_t i = 0; i < newBytes.size(); i++)
		{
			std::stringstream stream;
			stream << "0x" << std::hex << (unsigned int)newBytes[i];
			std::string byte = stream.str();
			newBytesString.append(byte + " ");
		}

		if (memcmp(&buffer[0], &truncatedOriginalBytes[0], buffer.size()) == 0)
		{
			memcpy((void*)address, &newBytes[0], newBytes.size());
			return true;
		}
		return false;
	}
}