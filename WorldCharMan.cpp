#include "WorldCharMan.hpp"
#include "Console.hpp"
#include "ImGui/imgui.h"
#include "Menu.hpp"
#include "D3DRenderer.hpp"

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
			return;
			//Init();
		}

		// Get Player Data
		PlayerPTR = (RPM<uintptr_t>(g_GameVariables->m_ModuleBase + 0x03A2ED50));
		if (PlayerPTR == NULL)
		{
			g_Console->printdbg("[!] WorldCharMan::Update - FAILED; {PlayerPTR}\n\n", TRUE, g_Console->color.red);
			return;
			//Init();
		}

		//	CRASH PROTECTION ??
		if ((pObject*)(RPM<uintptr_t>(PlayerPTR) + 0) == NULL)
			return;
		pEntityObjectBase = (pObject*)(RPM<uintptr_t>(PlayerPTR) + 0);

		if ((pChrData*)RPM<uintptr_t>(pEntityObjectBase->EntObjectPTR + 0x0) == NULL)
			return;
		pCharData = (pChrData*)RPM<uintptr_t>(pEntityObjectBase->EntObjectPTR + 0x0);
		
		//	HEALTH CHECK
		if (pCharData->Health == NULL)
			return;

		if ((pChrFall*)RPM<uintptr_t>(pEntityObjectBase->EntObjectPTR + 0x28) == NULL)
			return;
		pCharFall = (pChrFall*)RPM<uintptr_t>(pEntityObjectBase->EntObjectPTR + 0x28);
		
		if ((pChrPhysics*)RPM<uintptr_t>(pEntityObjectBase->EntObjectPTR + 0x68) == NULL)
			return;
		pCharPhysics = (pChrPhysics*)RPM<uintptr_t>(pEntityObjectBase->EntObjectPTR + 0x68);
		

		/// LOOP
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

		//	ORIGINAL
		return (Base != 0 && Ptr != 0);

		//	constant check if world char man is valid
		//	WorldCharMan resets on death
		//if (!g_WorldCharMan->pCharData->Health)
		//	return FALSE;
	}

	//  DEBUG DRAW SKELETON (useful for turning all off in case of bug)
	void WorldCharMan::dbg_SKELETON(bool ENABLED)
	{
		if (!g_GameDataMan->Valid())
			return;

		if (!g_WorldCharMan->Valid())
			return;

		g_WorldCharMan->Update();

		if (ENABLED) {
			for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {
				//  HEALTH FLAG
				if (g_WorldCharMan->CharData[i]->Health != 0) {
					//  DRAW
					if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 0)
						g_WorldCharMan->CharFall[i]->DrawSkeleton = 1;
				}
				else if (g_WorldCharMan->CharFall[i]->DrawSkeleton = 1)
					g_WorldCharMan->CharFall[i]->DrawSkeleton = 0;
			}
			g_Console->printdbg("[+] DRAW ALL SKELETONS : ON\n", TRUE, g_Console->color.green);
		}
		else {
			for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {
				if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
					g_WorldCharMan->CharFall[i]->DrawSkeleton = 0;
			}
			g_Console->printdbg("[+] DRAW ALL SKELETONS : OFF\n", TRUE, g_Console->color.red);
		}
		g_WorldCharMan->count = NULL;
		return;
	}

	//  DISTANCE BASED ESP SKELETON
	void WorldCharMan::ESP_SKELETON(float distance)
	{
		if (distance == NULL)
			return;

		if (!g_GameDataMan->Valid())
			return;

		if (!g_WorldCharMan->Valid())
			return;

		g_WorldCharMan->Update();

		for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {

			if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase)
				continue;

			if (g_WorldCharMan->CharData[i]->Health != NULL)
			{

				if (g_WorldCharMan->CharPhysics[i]->Position.x <= (g_WorldCharMan->pCharPhysics->Position.x + distance) && (g_WorldCharMan->pCharPhysics->Position.x - distance)
					&& g_WorldCharMan->CharPhysics[i]->Position.y <= (g_WorldCharMan->pCharPhysics->Position.y + 7) && (g_WorldCharMan->pCharPhysics->Position.y - 7)
					&& g_WorldCharMan->CharPhysics[i]->Position.z <= (g_WorldCharMan->pCharPhysics->Position.z + distance) && (g_WorldCharMan->pCharPhysics->Position.z - distance))
				{
					if (g_WorldCharMan->CharFall[i]->DrawSkeleton == NULL)
						g_WorldCharMan->CharFall[i]->DrawSkeleton = 1;
				}
				else if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
					g_WorldCharMan->CharFall[i]->DrawSkeleton = NULL;

				//if (GetDistanceToObject(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) > NULL 
				//    || GetDistanceToObject(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) <= distance)
				//{
				//    if (g_WorldCharMan->CharFall[i]->DrawSkeleton == NULL)
				//        g_WorldCharMan->CharFall[i]->DrawSkeleton = 1;
				//}
				//else if (g_WorldCharMan->CharFall[i]->DrawSkeleton == 1)
				//    g_WorldCharMan->CharFall[i]->DrawSkeleton = NULL;
			}
			else if (g_WorldCharMan->CharFall[i]->DrawSkeleton != NULL)
				g_WorldCharMan->CharFall[i]->DrawSkeleton = NULL;
		}
	}

	//	FREEZE ENTITIES
	void WorldCharMan::stallENTS()
	{
		//  Obtain entity information
		g_WorldCharMan->Update();

		if (!g_Menu->f_TOGGLE) {

			// Store entity position information
			for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1)
			{
				if (g_WorldCharMan->CharPhysics[i]->Position == g_WorldCharMan->pCharPhysics->Position) continue;

				//  DISTANCE
				if (g_GameFunctions->GetDistanceToObject(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) == 0
					|| g_GameFunctions->GetDistanceToObject(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) >= g_Menu->s_drawDistance)
					continue;

				// PUSH ENTITY COORDS INTO ARRAY
				g_Menu->storedPOS[i] = g_WorldCharMan->CharPhysics[i]->Position;
			}
			g_Menu->f_TOGGLE = TRUE;
		}

		//  FREEZE ENTITIES
		for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {
			if (g_WorldCharMan->CharPhysics[i]->Position != g_WorldCharMan->pCharPhysics->Position)
				g_WorldCharMan->CharPhysics[i]->Position = g_Menu->storedPOS[i];
		}
	}

	//  DISTANCE BASED ENTITY FREEZING
	void WorldCharMan::d_StallEnts(float DISTANCE)
	{
		g_WorldCharMan->Update();

		if (DISTANCE == 0)
			DISTANCE = 5;
		//  LOOP ENTITY ARRAY
		for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1)
		{
			//  BEGIN FILTERING ENTITIES
			if (g_WorldCharMan->CharData[i]->Health == NULL) continue;                                              //  HEALTH CHECK
			if (g_WorldCharMan->CharPhysics[i]->Position == g_WorldCharMan->pCharPhysics->Position) continue;       //  COMPARE POSITION WITH PLAYER
			if (g_GameFunctions->GetDistanceToObject(g_WorldCharMan->pCharPhysics->Position, g_WorldCharMan->CharPhysics[i]->Position) > DISTANCE) continue;

			//  PUSH ENTITY OBJECT INTO THE ARRAY
			g_Menu->ents[i] = g_GameFunctions->p2addy((uintptr_t)g_WorldCharMan->EntityObjectBase[i] + 0x190, { 0x68, 0x70 });
		}


		//  OBTAIN SIZE OF ENTS OBJECT
		int size = sizeof(g_Menu->ents) / sizeof(g_Menu->ents[0]);
		int count = 0;
		if (size != NULL)
		{
			//  POINTER CHAIN BULLSHIT
			for (int i = 0; i < size - 1; i = i + 1) {
				//  Get Distance
				if (g_GameFunctions->GetDistanceToObject(g_WorldCharMan->pCharPhysics->Position, g_Menu->distFREEZE[i]) > DISTANCE)
				{
					//  RELEASE OBJECT
					g_Menu->ents[i] = NULL;
					g_Menu->distFREEZE[i] = Vector3(0, 0, 0);
					continue;
				}

				if (g_Menu->ents[i] != NULL)
				{
					g_Menu->distFREEZE[i] = Vector3(*(float*)g_Menu->ents[i], *(float*)g_Menu->ents[i] + 4, *(float*)g_Menu->ents[i] + 8);
					count++;
				}
			}
		}

		//  FREEZE ENTITY
		for (int i = 0; i < count - 1; i = i + 1)
			if (g_Menu->ents[i] != NULL)
				Vector3(*(float*)g_Menu->ents[i], *(float*)g_Menu->ents[i] + 4, *(float*)g_Menu->ents[i] + 8) = g_Menu->distFREEZE[i];
			else continue;
	}

	//  GENOCIDE
	void WorldCharMan::killENTS()
	{
		if (!g_GameDataMan->Valid()) {
			g_Console->printdbg("[+] VALIDATION FAILED : {GameDataMan} . . .\n", TRUE, g_Console->color.red);
			return;
		}
		g_WorldCharMan->Update();

		int count = 0;
		for (int i = 0; i <= g_WorldCharMan->arraySIZE - 1; i = i + 1) {

			//  COMPARE OBJECT POINTERS
			if ((uintptr_t)g_WorldCharMan->EntityObjectBase[i] == (uintptr_t)g_WorldCharMan->pEntityObjectBase)
				continue;

			//  PLAYER POSITION FILTER
			if (Vector3(g_WorldCharMan->CharPhysics[i]->Position) == Vector3(g_WorldCharMan->pCharPhysics->Position))
				continue;

			if (g_WorldCharMan->CharData[i]->Health != NULL)
				g_WorldCharMan->CharData[i]->Health = NULL;
			count++;
		}
		printf("KILLED: %d ENTITIES\n", count);
		count = NULL;
		g_WorldCharMan->count = NULL;
		return;
	}
}
