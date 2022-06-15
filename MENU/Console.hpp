#pragma once
#include "Common.hpp"
namespace ER {

	struct Colors {
		int dark_blue = 1;
		int dark_green = 2;
		int dark_teal = 3;
		int dark_red = 4;
		int dark_pink = 5;
		int dark_yellow = 6;
		int dark_white = 7;
		int dark_gray = 8;
		int blue = 9;
		int green = 10;
		int teal = 11;
		int red = 12;
		int pink = 13;
		int yellow = 14;
		int white = 15;
	};

	class Console {
	public:

		Colors color{};
		bool verbose = false;

		explicit Console();
		~Console() noexcept = default;
		Console(Console const&) = delete;
		Console(Console&&) = delete;
		Console& operator=(Console const&) = delete;
		Console& operator=(Console&&) = delete;
		
		void Init();
		void printdbg(const char* Text, bool err, int color = {});
		void Free();
	};
	inline std::unique_ptr<Console> g_Console;
}