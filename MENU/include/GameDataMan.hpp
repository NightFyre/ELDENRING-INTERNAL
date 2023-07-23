#pragma once
#include "Common.hpp"
#include "Memory.hpp"

namespace ER {

#pragma pack(1)
	class FaceData {
	public:
		uint32_t FaceModelId;
		uint32_t HairModelId;
		uint32_t EyeModelId;
		uint32_t EyebrowModelId;
		uint32_t BeardModelId;
		uint32_t AccModelId;
		uint32_t DecalModelId;
		uint32_t EyelashModelId;
	};
	static_assert(sizeof(FaceData) == 0x20);
#pragma pack(pop)

#pragma pack(1)
	struct ChrStats {
	public:
		uint32_t Vigor; // 0x0
		uint32_t Mind; // 0x4
		uint32_t Endurance; // 0x8
		uint32_t Strength; // 0xC
		uint32_t Dexterity; // 0x10
		uint32_t Intelligence; // 0x14
		uint32_t Faith; // 0x18
		uint32_t Arcane; // 0x1C
		// 0x20
	};
	static_assert(sizeof(ChrStats) == 0x20);
#pragma pack(pop)

#pragma pack(1)
	struct ChrAsm {
	public:
		uint8_t ArmStyle; // 0x328 (GameData + 0x328)
		uint8_t pad[3]; // 0x329
		uint32_t CurrentWepSlotOffsetL; // 0x32C
		uint32_t CurrentWepSlotOffsetR; // 0x330
		uint32_t CurrentWepSlotOffsetLArrow; // 0x334
		uint32_t CurrentWepSlotOffsetRArrow; // 0x338
		uint32_t CurrentWepSlotOffsetLBolt; // 0x33C
		uint32_t CurrentWepSlotOffsetRBolt; // 0x340
		char pad2[0x44]; // 0x344
		uint32_t Accessory1; // 0x388
		uint32_t Accessory2; // 0x38C
		uint32_t Accessory3; // 0x390
		uint32_t Accessory4; // 0x394
		uint32_t Accessory5; // 0x398
		uint32_t PrimaryWeaponL; // 0x39C
		uint32_t PrimaryWeaponR; // 0x3A0
		uint32_t SecondaryWeaponL; // 0x3A4
		uint32_t SecondaryWeaponR; // 0x3A8
		uint32_t TertiaryWeaponL; // 0x3AC
		uint32_t TertiaryWeaponR; // 0x3B0
		uint32_t PrimaryArrow; // 0x3B4
		uint32_t PrimaryBolt; // 0x3B8
		uint32_t SecondaryArrow; // 0x3BC
		uint32_t SecondaryBolt; // 0x3C0
		uint32_t TertiaryArrow; // 0x3C4
		uint32_t TertiaryBolt; // 0x3C8
		uint32_t Helmet; // 0x3CC
		uint32_t Chest; // 0x3D0
		uint32_t Gloves; // 0x3D4
		uint32_t Legs; // 0x3D8
		uint32_t Hair; // 0x3DC
	};
	static_assert(sizeof(ChrAsm) == 0xB8);
#pragma pack(pop)

#pragma pack(1)
	class ChrGameData {
	public:
		char pad[0x10]; // 0x0
		uint32_t Health; // 0x10
		uint32_t MaxHealth; // 0x14
		uint32_t BaseMaxHealth; // 0x18
		uint32_t Mana; // 0x1C
		uint32_t MaxMana; // 0x20
		uint32_t BaseMaxMana; // 0x24
		uint32_t unk; // 0x28
		uint32_t Stamina; // 0x2C
		uint32_t MaxStamina; // 0x30
		uint32_t BaseMaxStamina; // 0x34
		uint32_t unk_; // 0x38
		ChrStats Stats; // 0x3C
		int pad2[2]; // 0x58
		uint32_t RuneArc; // 0x64
		uint32_t Level; // 0x68
		uint32_t CurrentRunes; // 0x6C
		uint32_t TotalRunes; // 0x70
		int unk2; // 0x74
		uint32_t Immunity1; // 0x78
		uint32_t Immunity2; // 0x7C
		uint32_t Robustness; // 0x80
		uint32_t Vitality; // 0x84
		uint32_t Robustness2; // 0x88
		uint32_t Focus; // 0x8C
		uint32_t Focus2; // 0x90
		int pad3[2]; // 0x94
		char Name[19]; // 0x9C
		char pad4[0xF]; // 0xAF
		PlayerGender Gender; // 0xBE
		PlayerClass Class; // 0xBF
		uint8_t Appearance; // 0xC0
		uint8_t unk3; // 0xC1
		uint8_t Voice; // 0xC2
		uint8_t StartingGift; // 0xC3
		char pad5[0x3B]; // 0xC4;
		uint8_t GreaterRuneState; // 0xFF
		byte pad6[0x228]; // 0x100
		ChrAsm ChrAsm; // 0x328
	};
	static_assert(sizeof(ChrGameData) == (0x328 + sizeof(ChrAsm)));
#pragma pack(pop)

	class GameDataMan
	{
	public:
		uintptr_t Base{};
		uintptr_t Ptr{};
		uintptr_t LastPtr{};
		ChrGameData* GameData{}; // 0x8
		uint32_t PlayTimeMS{}; // 0xA0
		uint32_t NGPlus{}; // 0x120

		explicit GameDataMan();
		~GameDataMan() noexcept = default;

		GameDataMan(GameDataMan const&) = delete;
		GameDataMan(GameDataMan&&) = delete;
		GameDataMan& operator=(GameDataMan const&) = delete;
		GameDataMan& operator=(GameDataMan&&) = delete;

		void Init();
		void Update();

		bool Valid();
	};
	inline std::unique_ptr<GameDataMan> g_GameDataMan;

}