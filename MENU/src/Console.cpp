#include "../include/Console.hpp"
FILE* stream;
HANDLE fConsole; 

namespace ER {

	Console::Console() {
		return;
	}

	void Console::InitializeConsole(const char* ConsoleName) {
		AllocConsole();
		g_Handle = GetStdHandle(STD_OUTPUT_HANDLE);
		g_hWnd = GetConsoleWindow();
		freopen_s(&stream_in, "CONIN$", "r", stdin);
		freopen_s(&stream_out, "CONOUT$", "w", stdout);
		freopen_s(&stream_error, "CONOUT$", "w", stderr);
		SetConsoleTitleA(ConsoleName);
		ShowWindow(g_hWnd, SW_SHOW);
		printdbg("[+] Console::Initialized\n", Console::Colors::DEFAULT);
	}

	void Console::printdbg(const char* Text, Colors Color, ...)
	{
		SetConsoleTextAttribute(g_Handle, Color);
		va_list arg;
		va_start(arg, Color);
		vfprintf(stream_out, Text, arg);
		va_end(arg);
		SetConsoleTextAttribute(g_Handle, Colors::DEFAULT);
	}

	void Console::scandbg(const char* Text, ...)
	{
		va_list arg;
		va_start(arg, Text);
		vfscanf(stream_in, Text, arg);
		va_end(arg);
	}

	//  LOG EVENT FUNCTION
	//  ONLY USE FOR ON|OFF DEBUG PRINTS
	//<EXAMPLE>
	//  TEXT: "[+] MENU:: ESP ; "
	//  FLAG: bESP
	//</EXAMPLE>
	//  OUTPUT: [+] MENU:: ESP ; [<FLAG RESULT>]
	void Console::LogEvent(std::string TEXT, bool FLAG)
	{
		std::string output;
		int color;
		switch (FLAG) {
		case (TRUE):	output = " [ON]\n"; color = Colors::green; break;
		case (FALSE):	output = " [OFF]\n"; color = Colors::red; break;
		}
		std::string append = TEXT + output;
		g_Console->printdbg(append.c_str(), static_cast<Colors>(color));
	}

	std::string Console::GetTimeString()
	{
		auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm;
		localtime_s(&now_tm, &now_c);
		std::stringstream ss{};
		ss << std::put_time(&now_tm, "%Y-%m-%d");
		return ss.str();
	}

	void Console::Free()
	{
		//  FREE CONSOLE
		if (stream != NULL)
			fclose(stream);
		FreeConsole();
		return;
	}
}
