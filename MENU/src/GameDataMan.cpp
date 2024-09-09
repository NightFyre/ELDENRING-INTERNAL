#include "../include/GameDataMan.hpp"

namespace ER {

	GameDataMan::GameDataMan()
	{
		Init();
	}

	void GameDataMan::Init()
	{
		g_Console->printdbg("[+] GameDataMan::Init STARTED\n", Console::Colors::yellow);
		if (!Base) // Base should never change.
		{
			auto Sig = Signature("48 8B 05 ? ? ? ? 48 85 C0 74 05 48 8B 40 58 C3 C3").Scan().Add(3).Rip().As<uint64_t>();

			if (!Sig) {
				g_Console->printdbg("[!] GameDataMan::Init - FAILED; {Sig}\n", Console::Colors::red);
				return;
			}

			Base = Sig;
		}

		auto BasePtr = RPM<uint64_t>(Base);

		if (!BasePtr) {
			g_Console->printdbg("[!] GameDataMan::Init - FAILED; {BasePTR}\n", Console::Colors::red);
			return;
		}

		Ptr = BasePtr;
		LastPtr = Ptr;

		g_Console->printdbg("[+] INITIALIZED GameDataMan\n", Console::Colors::green);
		Update();
	}

	void GameDataMan::Update()
	{
		if (!Base || !Ptr) {
			g_Console->printdbg("[!] GameDataMan::Update - FAILED; {Base || Ptr}\n\n", Console::Colors::red);
			Init();
			return;
		}

		if (LastPtr != RPM<uint64_t>(Base)) {
			Ptr = 0;
			LastPtr = 0;
			g_Console->printdbg("[!] GameDataMan::Update - FAILED; {LastPtr}\n\n", Console::Colors::red);
			return;
		}

		LastPtr = Ptr;

		GameData = (ChrGameData*)RPM<uint64_t>(Ptr + 0x8);
		PlayTimeMS = RPM<uint32_t>(Ptr + 0xA0);
		NGPlus = RPM<uint32_t>(Ptr + 0x120);
		g_Console->printdbg("[+] GameDataMan::update FINISHED\n\n", Console::Colors::green);
	}

	bool GameDataMan::Valid()
	{
		return (Base != 0 && Ptr != 0);
	}
}
