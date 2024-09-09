#pragma once
#include "SDK.h"

/**
 * Name: Elden Ring
 * Version: ~
 * Engine: ~
*/

#pragma pack(push, 0x01)
namespace HEXINTON
{

	// --------------------------------------------------
	// # WorldCharacterManager
	// --------------------------------------------------
	LocalPlayer* WorldCharacterManager::GetLocalPlayer() 
	{
		LocalPlayer* result = nullptr;
		if (this == nullptr)
			return result;

		if (this->pLocalPlayer != nullptr)
			result = this->pLocalPlayer;

		return result;
	}

	PlayerInstance* WorldCharacterManager::GetLocalPlayerInstance()
	{
		PlayerInstance* result = nullptr;
		if (this == nullptr)
			return result;

		auto localPlayer = this->GetLocalPlayer();
		if (localPlayer != nullptr)
			result = localPlayer->GetPlayerInstance();

		return result;
	}

	CharacterModules* WorldCharacterManager::GetLocalPlayerCharModules()
	{
		CharacterModules* result = nullptr;
		if (this == nullptr)
			return result;

		auto playerInstance = this->GetLocalPlayerInstance();
		if (playerInstance != nullptr)
			result = playerInstance->GetCharacterModules();

		return result;
	}

	CSCharData* WorldCharacterManager::GetLocalPlayerCharData()
	{
		CSCharData* result = nullptr;
		if (this == nullptr)
			return result;

		auto charModules = this->GetLocalPlayerCharModules();
		if (charModules != nullptr)
			result = charModules->GetCharData();

		return result;
	}

	CSCharPhysics* WorldCharacterManager::GetLocalPlayerCharPhysics()
	{
		CSCharPhysics* result = nullptr;
		if (this == nullptr)
			return result;

		auto charModules = this->GetLocalPlayerCharModules();
		if (charModules != nullptr)
			result = charModules->GetCharPhysics();

		return result;
	}

	bool WorldCharacterManager::GetCharacterArray(std::vector<PlayerInstance*>* pOut)
	{
		std::vector<PlayerInstance*> result;

		auto count = this->pEntArrayEnd - this->pEntArrayBegin / 8;
		auto p = *(long long*)this->pEntArrayBegin;
		for (int i = 0; i < count; i++)
		{
			auto pEnt = p + (PlayerInstance*)(i * 0x8);
			if (pEnt <= 0)
				continue;

			result.push_back(pEnt);
		}
	
		*pOut = result;

		return result.size() > 0;
	}

	// --------------------------------------------------
	// # LocalPlayer
	// --------------------------------------------------
	PlayerInstance* LocalPlayer::GetPlayerInstance() 
	{ 
		PlayerInstance* result = nullptr;

		if (this->pPlayer != nullptr)
			result = this->pPlayer;

		return result;
	}

	// --------------------------------------------------
	// # PlayerInstance
	// --------------------------------------------------
	CharacterModules* PlayerInstance::GetCharacterModules() 
	{ 
		CharacterModules* result = nullptr;

		if (this->pCharModules != nullptr)
			result = this->pCharModules;

		return result;
	}
	
	int32_t	PlayerInstance::GetCharType() { return this->CharacterType; }
	int32_t	PlayerInstance::GetTeam() { return this->TeamType; }
	void PlayerInstance::SetCharType(const int32_t newType) { this->CharacterType = newType; }
	void PlayerInstance::SetTeam(const int32_t newTeam) { this->TeamType = newTeam; }

	bool PlayerInstance::GetHP(int32_t& outHealth)
	{
		bool result = FALSE;

		// Get Modules
		CharacterModules* modules = this->pCharModules;
		if (modules == nullptr)
			return result;

		// Obtain Unit Data
		CSCharData* charData = modules->GetCharData();
		if (charData == nullptr)
			return result;

		//	Get Current Health
		outHealth = charData->GetHealth();
		return TRUE;
	}

	void PlayerInstance::SetHP(int32_t value)
	{
		bool result = FALSE;

		// Get Modules
		CharacterModules* modules = this->pCharModules;
		if (modules == nullptr)
			return;

		// Obtain Unit Data
		CSCharData* charData = modules->GetCharData();
		if (charData == nullptr)
			return;

		//	Get Current Health
		charData->SetHealth(value);
	}

	bool PlayerInstance::GetPos(Vector3& out)
	{
		bool result = FALSE;

		// Get Modules
		CharacterModules* modules = this->pCharModules;
		if (modules == nullptr)
			return result;

		// Obtain physics
		CSCharPhysics* physics = modules->GetCharPhysics();
		if (physics == nullptr)
			return result;

		out = physics->GetLocalPosition();
		return TRUE;
	}

	void PlayerInstance::SetPos(Vector3 In)
	{
		bool result = FALSE;

		// Get Modules
		CharacterModules* modules = this->pCharModules;
		if (modules == nullptr)
			return;

		// Obtain physics
		CSCharPhysics* physics = modules->GetCharPhysics();
		if (physics == nullptr)
			return;

		physics->SetLocalPosition(In);
	}

	float PlayerInstance::GetDistance(Vector3 Other)
	{
		Vector3 cPos;
		GetPos(cPos);
		return cPos.DistTo(Other);
	}


	// --------------------------------------------------
	// # CSCharModules
	// --------------------------------------------------
	CSCharData* CharacterModules::GetCharData() 
	{
		CSCharData* result = nullptr;

		if (this->pCharData != nullptr)
			result = this->pCharData;

		return result;
	}

	CSCharPhysics* CharacterModules::GetCharPhysics() 
	{
		CSCharPhysics* result = nullptr;

		if (this->pCharPhysics != nullptr)
			result = this->pCharPhysics;

		return result;
	}

	CSPlayerDamage* CharacterModules::GetPlayerDamage() 
	{
		CSPlayerDamage* result = nullptr;

		if (this->pPlayerDamage != nullptr)
			result = this->pPlayerDamage;

		return result;
	}

	// --------------------------------------------------
	// # CSCharData
	// --------------------------------------------------
	int32_t CSCharData::GetHealth() { return this->CurrentHealth; }
	int32_t CSCharData::GetMaxHealth() { return this->MaxHealth; }
	void CSCharData::SetHealth(const int32_t& value) { this->CurrentHealth = value; }
	int32_t CSCharData::GetMana() { return this->CurrentMana; }
	int32_t CSCharData::GetMaxMana() { return this->MaxMana; }
	void CSCharData::SetMana(const int32_t& value) { this->CurrentMana = value; }
	int32_t CSCharData::GetStamina() { return this->CurrentStamina; }
	int32_t CSCharData::GetMaxStamina() { return this->MaxStamina; }
	void CSCharData::SetStamina(const int32_t& value) { this->CurrentStamina = value; }

	// --------------------------------------------------
	// # CSCharPhysics
	// --------------------------------------------------
	Vector3 CSCharPhysics::GetLocalPosition() { return this->RelativePosition; }
	void CSCharPhysics::SetLocalPosition(Vector3 newPosition) { this->RelativePosition = newPosition; }
	void CSCharPhysics::ToggleGravity() { this->bNoGravity ^= 1; }
}
#pragma pack(pop)