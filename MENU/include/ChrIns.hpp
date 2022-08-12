#pragma once
#include "Common.hpp"
#include "ChrModules.hpp"

namespace ER {

#pragma pack(1)
	class ChrIns
	{
	public:
		uint64_t _;
		uint32_t LocalId; // 0x8
		char pad[0x54]; // 0xC
		uint32_t ParamId; // 0x60
		uint32_t pad2[2]; // 0x64
		uint8_t TeamId; // 0x6C
		char pad3[0x7]; // 0x6D
		uint32_t GlobalId; // 0x74
		char pad4[0x118]; // 0x78
		ChrModules* Modules; // 0x190

		uint32_t HP()
		{
			return Modules->UnitData->Health;
		}

		uint32_t MaxHP()
		{
			return Modules->UnitData->MaxHealth;
		}

		Vector3 GetPos() {
			return Modules->Physics->Pos;
		}

		Vector3 SetPos(Vector3 In) {
			Modules->Physics->Pos = In;
		}

		float Distance(Vector3 Other)
		{
			return GetPos().DistTo(Other);
		}
	};
	static_assert(sizeof(ChrIns) == 0x198);
#pragma pack(pop)
}