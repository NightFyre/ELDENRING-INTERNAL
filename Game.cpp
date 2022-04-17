#include "Game.hpp"
#include "ImGui/imgui.h"

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

	uintptr_t GameFunctions::p2addy(uintptr_t PTR, std::vector<unsigned int> OFFSETS)
	{
		uintptr_t addr = PTR;
		for (unsigned int i = 0; i < OFFSETS.size(); i++) {
			addr = *(uintptr_t*)addr;
			addr += OFFSETS[i];
		}
		return addr;
	}

	float GameFunctions::GetDistanceToObject(Vector3 POS, Vector3 POS2)
	{
		float distance = (POS.x - POS2.x) + (POS.y - POS2.y) + (POS.z - POS2.z);
		return (distance);
	}

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

		if (init && text[0])
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 5), ImColor(255, 255, 255, 255), text);
	}

	//  CREDIT: techiew
	// Replaces the memory at a given address with newBytes. Performs memory comparison with originalBytes to stop unintended memory modification.
	static constexpr int MASKED = 0xffff;
	bool GameFunctions::Replace(uintptr_t address, std::vector<uint16_t> originalBytes, std::vector<uint8_t> newBytes)
	{
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
		//Log("Bytes at address: %s", bufferString.c_str());

		std::string newBytesString = "";
		for (size_t i = 0; i < newBytes.size(); i++)
		{
			std::stringstream stream;
			stream << "0x" << std::hex << (unsigned int)newBytes[i];
			std::string byte = stream.str();
			newBytesString.append(byte + " ");
		}
		//Log("New bytes: %s", newBytesString.c_str());

		if (memcmp(&buffer[0], &truncatedOriginalBytes[0], buffer.size()) == 0)
		{
			//Log("Bytes match");
			memcpy((void*)address, &newBytes[0], newBytes.size());
			//Log("Patch applied");
			return true;
		}
		else
		{
			//RaiseError("Bytes do not match!");
		}
		return false;
	}
}