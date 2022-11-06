#pragma once
#include "Common.hpp"
#include "Memory.hpp"
#include "GameDataMan.hpp"
namespace ER {

	struct Alliance {
		int None = 0;
		int Player = 1;
		int Enemy = 6;
		int Boss = 7;
		int Decoy = 10;
		int Friendly = 26;
		int Object = 30;
	};

	//	PLAYER
	struct pObject {
		char pad_0000[8];	//0x0000
		int LOCALID;	//0x0008
		char pad_000C[84];	//0x000C
		int PARAMID;	//0x0060
		char pad_0064[8];	//0x0064
		char ALLIANCE;	//0x006C
		char pad_006D[7];	//0x006D
		unsigned __int16 GLOBALID;	//0x0074
		char pad_0076[282];	//0x0076
		unsigned __int64 EntObjectPTR;	//0x0190
	};	//Size: 0x0198

	struct pChrData {
		char pad_0000[312];	//0x0000
		int Health;	//0x0138
		int MaxHealth;	//0x013C
		int BaseMaxHealth;	//0x0140
		int unk;	//0x0144
		int Mana;	//0x0148
		int MaxMan;	//0x014C
		int BaseMaxMana;	//0x0150
		int Stamina;	//0x0154
		int MaxStamina;	//0x0158
		int BaseMaxStamina;	//0x015C
	};	//Size: 0x0160

	struct pChrPhysics {
		char pad_0000[84];	//0x0000
		float Azimuth;	//0x0054
		char pad_0058[24];	//0x0058
		Vector3 Position;	//0x0070
	};	//Size: 0x007C

	struct pChrFall {
		char pad_0000[6404];	//0x0000
		bool DrawSkeleton;	//0x1904
	};	//Size: 0x1905

	//	ENTITIES
	struct EntObject {
		char pad_0000[8];	//0x0000
		int LOCALID;	//0x0008
		char pad_000C[84];	//0x000C
		int PARAMID;	//0x0060
		char pad_0064[8];	//0x0064
		char ALLIANCE;	//0x006C
		char pad_006D[7];	//0x006D
		unsigned __int16 GLOBALID;	//0x0074
		char pad_0076[282];	//0x0076
		unsigned __int64 EntObjectPTR;	//0x0190
	};	//Size: 0x0198
	
	struct ChrData {
		char pad_0000[312]; // 0x0000
		int Health;	//0x0138
		int MaxHealth;	//0x013C
		int BaseMaxHealth;	//0x0140
		int unk;	//0x0144
		int Mana;	//0x0148
		int MaxMan;	//0x014C
		int BaseMaxMana;	//0x0150
		int Stamina;	//0x0154
		int MaxStamina;	//0x0158
		int BaseMaxStamina;	//0x015C
	};	//Size: 0x0160

	struct ChrTimeAct {
		char pad_0000[64]; // 0x0000
		int32_t Animation; // 0x0040
	};	//Size: 0x0040

	struct ChrPhysics {
		char pad_0000[84];	//0x0000
		float Azimuth;	//0x0054
		char pad_0058[24];	//0x0058
		Vector3 Position;	//0x0070
	};	//Size: 0x007C

	struct ChrFall {
		char pad_0000[6404];	//0x0000
		bool DrawSkeleton;	//0x1904
	};	//Size: 0x1905

	class WorldCharMan
	{
	public:
		int count = 0;
		int arraySIZE = 0;
		bool m_isValid{}; 

		int validEnts_count = 0;		//	WorldCharUpdate
		int entwndw_count = 0;			//	Menu:: EntityWindow
		int entwndw_count2 = 0;			//	Menu:: EntityWindow

		Alliance Char_Faction{};
		uintptr_t Base{};
		uintptr_t Ptr{};
		uintptr_t LastPtr{};
		uintptr_t PlayerPTR;

		uint32_t Begin;
		uint32_t Finish;

		///	Resolve PointerChain
		pObject* pEntityObjectBase{};		// {};
		pChrData* pCharData{};				// {};		//	*(uintptr_t*)EntityObjectBase->EntObjectPTR + 0x0;
		pChrPhysics* pCharPhysics{};		// {};		//	*(uintptr_t*)EntityObjectBase->EntObjectPTR + 0x68;
		pChrFall* pCharFall{};				// {};		//	*(uintptr_t*)EntityObjectBase->EntObjectPTR + 0x70;

		EntObject* EntityObjectBase[1000];		// {};
		ChrData* CharData[1000];				// {};		//	*(uintptr_t*)EntityObjectBase->EntObjectPTR + 0x0;
		ChrTimeAct* CharTimeAct[1000];			// {};		//	*(uintptr_t*)CharData->CharTimeActModulePtr;
		ChrPhysics* CharPhysics[1000];			// {};		//	*(uintptr_t*)EntityObjectBase->EntObjectPTR + 0x68;
		ChrFall* CharFall[1000];				// {};		//	*(uintptr_t*)EntityObjectBase->EntObjectPTR + 0x70;

		explicit WorldCharMan();
		~WorldCharMan() noexcept = default;

		WorldCharMan(WorldCharMan const&) = delete;
		WorldCharMan(WorldCharMan&&) = delete;
		WorldCharMan& operator=(WorldCharMan const&) = delete;
		WorldCharMan& operator=(WorldCharMan&&) = delete;
		
		void Init();
		bool Update();
		bool Valid();

		//	CUSTOM FUNCTIONS
		void ESP_SKELETON(float distance);
		void dbg_SKELETON(bool ENABLED);
		void stallENTS();
		void d_StallEnts(float DISTANCE);
		void killENTS();
	};
	inline std::unique_ptr<WorldCharMan> g_WorldCharMan;
}