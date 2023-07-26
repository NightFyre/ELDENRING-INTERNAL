#pragma once
#include "Common.hpp"
#include "Tabs/Main.h"
#include "Tabs/Character.h"
#include "Tabs/Entities.h"
#include "Tabs/Visuals.h"
#include "Tabs/Debug.h"

//	CLEANING IS REQUIRED
namespace ER
{

	class Menu
	{
	public:
		const char* m_MenuFooter = "ELDEN RING - STEAM DECK";

		void Draw();
		void DrawMenu();

		//	INITIALIZE CLASS
		Menu()  noexcept = default;
		~Menu() noexcept = default;
		Menu(Menu const&) = delete;
		Menu(Menu&&) = delete;
		Menu& operator=(Menu const&) = delete;
		Menu& operator=(Menu&&) = delete;
	};
	inline std::unique_ptr<Menu> g_Menu;
};