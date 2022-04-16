#pragma once
#include "Common.hpp"

namespace ER {

	enum PlayerClass : uint8_t {
		Vagabond = 0,
		Warrior = 1,
		Hero = 2,
		Bandit = 3,
		Astrologer = 4,
		Prophet = 5,
		Confessor = 6,
		Samurai = 7,
		Prisoner = 8,
		Wretch = 9
	};

	enum PlayerGender : uint8_t {
		Female = 0,
		Male = 1
	};

}