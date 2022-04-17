#include "WorldCharMan.hpp"
#include "Console.hpp"

namespace ER {
	WorldCharMan::WorldCharMan()
	{
		Init();
	}

	void WorldCharMan::Init()
	{
		g_Console->printdbg("[+] WorldCharMan::Init STARTED\n", TRUE, g_Console->color.yellow);
		auto Sig = Signature("48 8B 05 ? ? ? ? 48 85 C0 74 0F 48 39 88 ? ? ? ? 75 06 89 B1 5C 03 00 00 0F 28 05 ? ? ? ? 4C 8D 45 E7").Scan().Add(3).Rip().As<uint64_t>();

		if (!Sig) {
			g_Console->printdbg("[!] WorldCharMan::Init - FAILED; {Sig}\n\n", TRUE, g_Console->color.red);
			return;
		}
		Base = Sig;

		g_Console->printdbg("[+] WorldCharMan::Init FINISHED\n\n", TRUE, g_Console->color.green);
		Update();
	}

	void WorldCharMan::Update()
	{
		g_Console->printdbg("[+] WorldCharMan::Update STARTED\n", TRUE, g_Console->color.yellow);

		auto BasePtr = RPM<uint64_t>(Base) + 0x18F10;

		if (!BasePtr) {
			g_Console->printdbg("[!] WorldCharMan::Update - FAILED; {BasePTR}\n\n", TRUE, g_Console->color.red);
			return;
		}
		Ptr = BasePtr;

		//	Resolve Base
		if (!Base || !Ptr) {
			g_Console->printdbg("[!] WorldCharMan::Update - FAILED; {Base || Ptr}\n\n", TRUE, g_Console->color.red);
			count = 0;
			Init();
		}
		LastPtr = Ptr;
		
		if (g_Console->verbose) {
			printf("[+] Base: %llX\n", Base);
			printf("[+] Ptr: %llX\n", Ptr);
		}

		//	Establish Size of Array
		Begin = RPM<uint32_t>(Ptr);
		Finish = RPM<uint32_t>(Ptr + 0x8);
		arraySIZE = (Finish - Begin) / 8;
		if (arraySIZE == NULL)
		{
			g_Console->printdbg("[!] WorldCharMan::Update - FAILED; {array}\n\n", TRUE, g_Console->color.red);	// weak point
			Init();
		}

		// Get Player Data
		PlayerPTR = (RPM<uintptr_t>(g_GameVariables->m_ModuleBase + 0x03A2ED50));
		if (PlayerPTR == NULL)
		{
			g_Console->printdbg("[!] WorldCharMan::Update - FAILED; {PlayerPTR}\n\n", TRUE, g_Console->color.red);
			Init();
		}
		pEntityObjectBase = (pObject*)(RPM<uintptr_t>(PlayerPTR) + 0);
		pCharData = (pChrData*)RPM<uintptr_t>(pEntityObjectBase->EntObjectPTR + 0x0);
		pCharFall = (pChrFall*)RPM<uintptr_t>(pEntityObjectBase->EntObjectPTR + 0x28);
		pCharPhysics = (pChrPhysics*)RPM<uintptr_t>(pEntityObjectBase->EntObjectPTR + 0x68);

		for (int i = 0; i <= arraySIZE - 1; i = i + 1) {
			EntityObjectBase[i] = (EntObject*)*(uintptr_t*)(RPM<uintptr_t>(Ptr) + i * 8);		//EntityBranch
			CharData[i] = (ChrData*)RPM<uintptr_t>(EntityObjectBase[i]->EntObjectPTR + 0x0);
			CharFall[i] = (ChrFall*)RPM<uintptr_t>((uintptr_t)EntityObjectBase[i]->EntObjectPTR + 0x28);
			CharPhysics[i] = (ChrPhysics*)RPM<uintptr_t>((uintptr_t)EntityObjectBase[i]->EntObjectPTR + 0x68);
			count++;

			//	VERBOSE CONSOLE LOG
			if (g_Console->verbose) {
				printf("EntityObjectBase: %p\n", EntityObjectBase);
				printf("CharData: %p\n", CharData[i]);
				printf("CharFall: %p\n", CharFall[i]);
				printf("CharPhsyics: %p\n", CharPhysics[i]);
				printf("---------------------------------------------\n\n");
			}
		}
		g_Console->printdbg("[+] WorldCharMan::Update FINISHED\n\n", TRUE, g_Console->color.green);
	}

	bool WorldCharMan::Valid()
	{
		return (Base != 0 && Ptr != 0);
	}
}
