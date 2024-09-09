#include "../include/Game.hpp"
#include "../include/Menu.hpp"
#include "../include/Console.hpp"
#include "../include/D3DRenderer.hpp"

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

	void GameFunctions::FMVSkip(uintptr_t addr)
	{
		///  FMV SKIP NEW TEST
		//  STATIC ADDRESS!! NEEDS sig function applied!!
		//  AOB: 48 8B 90 ? ? ? ? 48 85 D2 74 07 C6
		//  OLD | 0x0A9415E |	0x0A8FA5E | 0x0A8FB4E
		using namespace ER;
		Patch((BYTE*)addr + g_GameVariables->offsets.ptr_FMV_SKIP, (BYTE*)"\xE9\x1C\x00\x00\x00", 5);
		g_Console->printdbg("[+] FMV's SKIPPED\n", Console::Colors::green);
	}

	void GameFunctions::UnlockFPS(uintptr_t addr)
	{
		using namespace ER;
		Patch((BYTE*)addr + g_GameVariables->offsets.ptr_UNLOCK_FPS, (BYTE*)"\xC7\x45\xEF\x00\x00\x00\x00", 7);  // AOB 2 | OLD 0x1944B27
		g_Console->printdbg("[+] FPS LIMIT REMOVED\n\n", Console::Colors::green);
	}

	void GameFunctions::PauseGameplay(uintptr_t addr, bool ACTIVE)
	{
		switch (ACTIVE) {
			case (TRUE): Patch((BYTE*)addr + g_GameVariables->offsets.ptr_PAUSE_GAME, (BYTE*)"\x85", 1); break;
			case (FALSE): Patch((BYTE*)addr + g_GameVariables->offsets.ptr_PAUSE_GAME, (BYTE*)"\x84", 1); break;
		}	
	}

	int count = 0;
	void GameFunctions::ESP(float distance)
	{
		//	//  Update Entity Info
		//	if (!g_WorldCharMan->m_isValid) {
		//		g_Menu->bESP = FALSE;
		//		g_Console->printdbg("[+] MENU:: ESP; OFF {WorldCharMan::Update ; FAILED}\n", Console::Colors::red);
		//		return;
		//	}
		//	
		//	if (g_WorldCharMan->pCharData->Health == NULL) {
		//		g_Menu->bESP = FALSE;
		//		g_Console->printdbg("[+] MENU:: ESP; OFF {Health is NULL}\n", Console::Colors::red);
		//		return;
		//	}
		//	
		//	///  Filter Entity Results
		//	Vector2 vecScreen;
		//	uintptr_t ViewMatrix = g_GameFunctions->p2addy(g_GameVariables->m_ModuleBase + g_GameVariables->offsets.ptr_NBOTT_W2S, { 0x60, 0x60, 0x420 });
		//	Vector2 Size = { ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y };
		//	Vector2 pos = { (Size.x / 2), (Size.y / 2) };
		//	ImVec2 DrawPosition = { pos.x, 0 };
		//	
		//	memcpy(&g_Menu->Matrix, (BYTE*)ViewMatrix, sizeof(g_Menu->Matrix));
		//	for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {
		//	
		//		//  COMPARE WITH PLAYER
		//		if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase) continue;
		//	
		//		//  POSITION CHECK
		//		if (g_WorldCharMan->CharPhysics[i]->Position == g_WorldCharMan->pCharPhysics->Position) continue;
		//	
		//		//  ANIMATION CHECK
		//		if (g_WorldCharMan->CharTimeAct[i]->Animation < 0) continue;
		//	
		//		//  HEALTH CHECK
		//		if (g_WorldCharMan->CharData[i]->Health == NULL)
		//		{
		//			//  ADDITIOONAL PATCH TO REVERT SKELETON DRAWING IF ENTITY DIES
		//			if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
		//				g_WorldCharMan->CharFall[i]->DrawSkeleton = NULL;
		//			continue;
		//		}
		//	
		//		float DistanceToEntity = g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->CharPhysics[i]->Position, g_WorldCharMan->pCharPhysics->Position);
		//	
		//		///  DRAW SKELETON DISTANCE
		//		if (distance != NULL) {
		//			if (DistanceToEntity <= distance)
		//			{
		//				if (g_WorldCharMan->CharFall[i]->DrawSkeleton == NULL)
		//					g_WorldCharMan->CharFall[i]->DrawSkeleton = 1;
		//			}
		//			else if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
		//				g_WorldCharMan->CharFall[i]->DrawSkeleton = NULL;
		//		}
		//	
		//		std::string EntityCount = "ENTITIES: " + std::to_string(g_WorldCharMan->count);
		//		std::string EntityDistance = std::to_string(g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->CharPhysics[i]->Position, g_WorldCharMan->pCharPhysics->Position));
		//		if (g_D3DRenderer->WorldToScreen(g_WorldCharMan->CharPhysics[i]->Position, vecScreen, g_Menu->Matrix, Size.x, Size.y)) {
		//			ImGui::GetBackgroundDrawList()->AddText(ImVec2(5, 5), ImColor(255, 0, 0, 255), EntityCount.c_str());
		//			ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecScreen.x, vecScreen.y), ImColor(255, 0, 0, 255), EntityDistance.c_str());
		//			ImGui::GetBackgroundDrawList()->AddLine(DrawPosition, ImVec2(vecScreen.x, vecScreen.y), ImColor(255, 255, 255), 0.3f);
		//			count++;
		//		}
		//	}
		//	g_WorldCharMan->count = count;
		//	count = NULL;
	}

	void GameFunctions::dbg_ESP()
	{
		//	if (!g_WorldCharMan->m_isValid) {
		//		g_Menu->m_dbgMatrixWnd = FALSE;
		//		g_Console->printdbg("[+] MENU:: ENT WINDOW; OFF {WorldCharMan::Update ; FAILED}\n", Console::Colors::red);
		//		return;
		//	}
		//	
		//	if (g_WorldCharMan->pCharData->Health == NULL) {
		//		g_Menu->m_dbgMatrixWnd = FALSE;
		//		g_Console->printdbg("[+] MENU:: ENT WINDOW; OFF {Health is NULL}\n", Console::Colors::red);
		//		return;
		//	}
		//	
		//	///  Filter Entity Results
		//	int count = 0;
		//	Vector2 vecScreen;
		//	Vector2 pos = { ImGui::GetMainViewport()->GetCenter().x, ImGui::GetMainViewport()->GetCenter().y };
		//	for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {
		//	
		//		//  COMPARE WITH PLAYER
		//		if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase)
		//			continue;
		//	
		//		//  HEALTH CHECK
		//		if (g_WorldCharMan->CharData[i]->Health == NULL)
		//			continue;
		//	
		//		//  POSITION CHECK
		//		if (g_WorldCharMan->CharPhysics[i]->Position == g_WorldCharMan->pCharPhysics->Position)
		//			continue;
		//	
		//		///  DRAW
		//		if (g_D3DRenderer->WorldToScreen(g_WorldCharMan->CharPhysics[i]->Position, vecScreen, g_Menu->ViewMatrix, ImGui::GetWindowWidth(), ImGui::GetWindowHeight())) {
		//			if (g_Menu->dbg_ENT_RGB) {
		//				ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecScreen.x, vecScreen.y), ImColor(g_Menu->dbg_RAINBOW), std::to_string(count).c_str());
		//				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos.x, pos.y + 960), ImVec2(vecScreen.x, vecScreen.y), ImColor(g_Menu->dbg_RAINBOW), 0.3f);
		//			}
		//			else {
		//				ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecScreen.x, vecScreen.y), ImColor(0, 0, 255, 255), std::to_string(count).c_str());
		//				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos.x, pos.y + 960), ImVec2(vecScreen.x, vecScreen.y), ImColor(255, 255, 255), 0.3f);
		//			}
		//			count++;
		//		}
		//	}
		//	g_WorldCharMan->count = NULL;
		//	count = NULL;
	}

	void GameFunctions::Barrier(float distance)
	{
		//	if (!g_WorldCharMan->m_isValid) {
		//		m_BARRIER = FALSE;
		//		g_Console->printdbg("[+] MENU:: BARRIER; OFF {WorldCharMan::Update ; FAILED}\n", Console::Colors::red);
		//		return;
		//	}
		//	
		//	if (g_WorldCharMan->pCharData->Health == NULL) {
		//		m_BARRIER = FALSE;
		//		g_Console->printdbg("[+] MENU:: BARRIER; OFF {Health is NULL}\n", Console::Colors::red);
		//		return;
		//	}
		//	
		//	if (distance == NULL) return;
		//	
		//	//  LOOP ARRAY
		//	for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1)
		//	{
		//		//  FILTERS
		//		if (g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) > distance) continue; //  DISTANCE
		//		if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase) continue;                                       //  PLAYER OBJECT CHECK
		//		if (g_WorldCharMan->CharData[i]->Health == NULL) continue;                                                                                          //  HEALTH CHECK
		//		if (g_WorldCharMan->CharTimeAct[i]->Animation < 0) continue;                                                                                        //  ANIMATION CHECK
		//	
		//	
		//		if (g_WorldCharMan->EntityObjectBase[i]->ALLIANCE == g_WorldCharMan->Char_Faction.Enemy
		//			|| g_WorldCharMan->EntityObjectBase[i]->ALLIANCE == (int)48
		//			|| g_WorldCharMan->EntityObjectBase[i]->ALLIANCE == (int)51)
		//		{
		//			if (g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) <= distance)          //  Begin Health Drain
		//			{
		//				g_WorldCharMan->CharData[i]->Health = g_WorldCharMan->CharData[i]->Health - 1;                                                                  //  HEALTH - 1
		//				ImGui::GetBackgroundDrawList()->AddText(ImVec2(g_Menu->PrintToScreen.posTHREE), ImColor(255, 255, 255, 255), "ENTITY IN BARRIER!");             //  TEXT ON SCREEN
		//			}
		//			if (g_GameFunctions->GetDistanceTo3D_Object(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) <= (float)1)          //  KILL ENTITY
		//				g_WorldCharMan->CharData[i]->Health = 0;                                                                                                        //  HEALTH = 0
		//		}
		//	}
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