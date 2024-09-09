#pragma once

/**
 * Name: Elden Ring
 * Version: ~
 * Engine: ~
*/

#pragma once
#pragma pack(push, 0x01)
namespace HEXINTON
{
	enum PlayerClass : uint8_t 
    {
		Vagabond = 0,
		Warrior,
		Hero,
		Bandit,
		Astrologer,
		Prophet,
		Confessor,
		Samurai,
		Prisoner,
		Wretch,
	};

	enum PlayerGender : uint8_t 
    {
		Female = 0,
		Male,
	};
}
#pragma pack(pop)