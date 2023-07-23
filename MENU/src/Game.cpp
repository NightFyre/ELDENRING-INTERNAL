#include "../include/Game.hpp"
#include "../include/Menu.hpp"
#include "../include/WorldCharMan.hpp"
#include "../include/D3DRenderer.hpp"

namespace ER {

	GameVariables::GameVariables()
	{
		m_GamePid		= GetCurrentProcessId();
		m_GameHandle	= GetCurrentProcess();
		m_GameWindow	= FindWindow("ELDEN RING™", NULL);
		m_ModuleBase	= (uintptr_t)GetModuleHandle(NULL);

		RECT TempRect;
		GetWindowRect(m_GameWindow, &TempRect);
		m_GameWidth		= TempRect.right - TempRect.left;
		m_GameHeight	= TempRect.bottom - TempRect.top;

		char TempTitle[MAX_PATH];
		GetWindowText(m_GameWindow, TempTitle, sizeof(TempTitle));
		m_GameTitle		= TempTitle;

		char TempClassName[MAX_PATH];
		GetClassName(m_GameWindow, TempClassName, sizeof(TempClassName));
		m_ClassName		= TempClassName;

		char TempPath[MAX_PATH];
		GetModuleFileNameEx(m_GameHandle, NULL, TempPath, sizeof(TempPath));
		m_GamePath		= TempPath;
	}

	GameFunctions::GameFunctions()
	{
		return;
	}

	void GameFunctions::FMVSkip(uintptr_t addr)
	{
		///  FMV SKIP NEW TEST
		//  STATIC ADDRESS!! NEEDS sig function applied!!
		//  AOB: 48 8B 90 ? ? ? ? 48 85 D2 74 07 C6
		//  OLD | 0x0A9415E |	0x0A8FA5E | 0x0A8FB4E
		using namespace ER;
		Patch((BYTE*)addr + g_GameVariables->offsets.ptr_FMV_SKIP, (BYTE*)"\xE9\x1C\x00\x00\x00", 5);
	}

	void GameFunctions::UnlockFPS(uintptr_t addr)
	{
		using namespace ER;
		Patch((BYTE*)addr + g_GameVariables->offsets.ptr_UNLOCK_FPS, (BYTE*)"\xC7\x45\xEF\x00\x00\x00\x00", 7);  // AOB 2 | OLD 0x1944B27
	}

	void GameFunctions::PauseGameplay(uintptr_t addr, bool ACTIVE)
	{
		switch (ACTIVE) {
			case (TRUE): Patch((BYTE*)addr + g_GameVariables->offsets.ptr_PAUSE_GAME, (BYTE*)"\x85", 1); break;
			case (FALSE): Patch((BYTE*)addr + g_GameVariables->offsets.ptr_PAUSE_GAME, (BYTE*)"\x84", 1); break;
		}	
	}

	void GameFunctions::Barrier(float distance)
	{
		if (!g_WorldCharMan->m_isValid) {
			m_BARRIER = FALSE;
			return;
		}

		if (g_WorldCharMan->pCharData->Health == NULL) {
			m_BARRIER = FALSE;
			return;
		}

		if (distance == NULL) return;

		//  LOOP ARRAY
		for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1)
		{
			//  FILTERS
			if (g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) > distance) continue; //  DISTANCE
			if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase) continue;                                       //  PLAYER OBJECT CHECK
			if (g_WorldCharMan->CharData[i]->Health == NULL) continue;                                                                                          //  HEALTH CHECK
			if (g_WorldCharMan->CharTimeAct[i]->Animation < 0) continue;                                                                                        //  ANIMATION CHECK


			if (g_WorldCharMan->EntityObjectBase[i]->ALLIANCE == g_WorldCharMan->Char_Faction.Enemy
				|| g_WorldCharMan->EntityObjectBase[i]->ALLIANCE == (int)48
				|| g_WorldCharMan->EntityObjectBase[i]->ALLIANCE == (int)51)
			{
				if (g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) <= distance)          //  Begin Health Drain
				{
					g_WorldCharMan->CharData[i]->Health = g_WorldCharMan->CharData[i]->Health - 1;                                                                  //  HEALTH - 1
					ImGui::GetBackgroundDrawList()->AddText(ImVec2(g_Menu->PrintToScreen.posTHREE), ImColor(255, 255, 255, 255), "ENTITY IN BARRIER!");             //  TEXT ON SCREEN
				}
				if (g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) <= (float)1)          //  KILL ENTITY
					g_WorldCharMan->CharData[i]->Health = 0;                                                                                                        //  HEALTH = 0
			}
		}
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