#include "../include/GameDataMan.hpp"

namespace ER {

	GameDataMan::GameDataMan()
	{
		Init();
	}

	void GameDataMan::Init()
	{
		if (!Base) // Base should never change.
		{
			auto Sig = Signature("48 8B 05 ? ? ? ? 48 85 C0 74 05 48 8B 40 58 C3 C3").Scan().Add(3).Rip().As<uint64_t>();

			if (!Sig) {
				return;
			}

			Base = Sig;
		}

		auto BasePtr = RPM<uint64_t>(Base);

		if (!BasePtr) {
			return;
		}

		Ptr = BasePtr;
		LastPtr = Ptr;

		Update();
	}

	void GameDataMan::Update()
	{
		if (!Base || !Ptr) {
			Init();
			return;
		}

		if (LastPtr != RPM<uint64_t>(Base)) {
			Ptr = 0;
			LastPtr = 0;
			return;
		}

		LastPtr = Ptr;

		GameData = (ChrGameData*)RPM<uint64_t>(Ptr + 0x8);
		PlayTimeMS = RPM<uint32_t>(Ptr + 0xA0);
		NGPlus = RPM<uint32_t>(Ptr + 0x120);
	}

	bool GameDataMan::Valid()
	{
		return (Base != 0 && Ptr != 0);
	}
}
