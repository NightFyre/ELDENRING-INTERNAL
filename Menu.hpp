#pragma once
#include "Common.hpp"

namespace ER {

	class Menu
	{
	public:
		Menu()  noexcept = default;
		~Menu() noexcept = default;

		Menu(Menu const&) = delete;
		Menu(Menu&&) = delete;
		Menu& operator=(Menu const&) = delete;
		Menu& operator=(Menu&&) = delete;

		void InitStyle();
		void Draw();

	private:
		bool m_StyleInit{};
	};

	inline std::unique_ptr<Menu> g_Menu;

};