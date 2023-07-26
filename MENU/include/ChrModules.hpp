#pragma once
#include "Common.hpp"

#pragma pack(push, 1)
class ChrPhysics {
	char pad[0x70];
public:
	Vector3 Pos;
	char pad2[0x157];
	bool GravityEnabled;
};
static_assert(sizeof(ChrPhysics) == 0x1D4);
#pragma pack(pop)

#pragma pack(push, 1)
class UnitData {
public:
	char pad[0x138]; // 0x0
	uint32_t Health; // 0x138
	uint32_t pad2; // 0x13C
	uint32_t MaxHealth; // 0x140
	uint32_t BaseMaxHealth; // 0x144
	uint32_t mana; // 0x148
	uint32_t maxmana; // 0x14C
	uint32_t basemaxmana; // 0x150
	uint32_t stamina; // 0x154
	uint32_t maxstamina; // 0x158
	uint32_t basemaxstamina; // 0x15C
};
static_assert(sizeof(UnitData) == 0x160);
#pragma pack(pop)


#pragma pack(push, 1)
class ChrModules {
public:
	UnitData* UnitData;
	uint64_t actionflag;
	uint64_t behaviorscript;
	uint64_t timeact;
	uint64_t resist;
	uint64_t behavior;
	uint64_t behaviorsync;
	uint64_t ai;
	uint64_t playersuperarmor;
	uint64_t playertoughness;
	uint64_t talk;
	uint64_t event;
	uint64_t playermagic;
	ChrPhysics* Physics;
	uint64_t fall;
	uint64_t playerladder;
	uint64_t actionrequest;
	uint64_t chrthrow;
	uint64_t hitstop;
	uint64_t playerdamage;
	uint64_t playermaterial;
	uint64_t playerknockback;
	uint64_t sfx;
	uint64_t vfx;
	uint64_t behaviordata;
};
#pragma pack(pop)

