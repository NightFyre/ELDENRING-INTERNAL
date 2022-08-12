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

		///	TITLE FOR RELEASE BUILD
//		std::cout << R"(                                                                  
//              ,,        ,,                                   ,,                               
//`7MM"""YMM  `7MM      `7MM                     `7MM"""Mq.    db                               
//  MM    `7    MM        MM                       MM   `MM.                                    
//  MM   d      MM   ,M""bMM  .gP"Ya `7MMpMMMb.    MM   ,M9  `7MM  `7MMpMMMb.  .P"Ybmmm         
//  MMmmMM      MM ,AP    MM ,M'   Yb  MM    MM    MMmmdM9     MM    MM    MM :MI  I8           
//  MM   Y  ,   MM 8MI    MM 8M""""""  MM    MM    MM  YM.     MM    MM    MM  WmmmP"           
//  MM     ,M   MM `Mb    MM YM.    ,  MM    MM    MM   `Mb.   MM    MM    MM 8M                
//.JMMmmmmMMM .JMML.`Wbmd"MML.`Mbmmd'.JMML  JMML..JMML. .JMM..JMML..JMML  JMML.YMMMMMb          
//                                                                            6'     dP         
//                                                                            Ybmmmd'           
//
//`7MMF'`7MN.   `7MF'MMP""MM""YMM `7MM"""YMM  `7MM"""Mq.  `7MN.   `7MF'     db      `7MMF'      
//  MM    MMN.    M  P'   MM   `7   MM    `7    MM   `MM.   MMN.    M      ;MM:       MM        
//  MM    M YMb   M       MM        MM   d      MM   ,M9    M YMb   M     ,V^MM.      MM        
//  MM    M  `MN. M       MM        MMmmMM      MMmmdM9     M  `MN. M    ,M  `MM      MM        
//  MM    M   `MM.M       MM        MM   Y  ,   MM  YM.     M   `MM.M    AbmmmqMA     MM      , 
//  MM    M     YMM       MM        MM     ,M   MM   `Mb.   M     YMM   A'     VML    MM     ,M 
//.JMML..JML.    YM     .JMML.    .JMMmmmmMMM .JMML. .JMM..JML.    YM .AMA.   .AMMA..JMMmmmmMMM                                                                                                                              
//)" << std::endl;
		//printdbg("[+] Created bv NightFyre & NBOTT42\n", TRUE, color.yellow);
		//printdbg("[!] PLEASE DONT INJECT UNTIL YOU HAVE LOADED YOUR SAVE\n", TRUE, color.red);
		//printdbg("[+] PRESS [INSERT] TO INJECT MENU\n", FALSE);
	}

	void Console::printdbg(const char* Text, Color Color, ...)
	{
		SetConsoleTextAttribute(g_Handle, Color);
		va_list arg;
		va_start(arg, Color);
		vfprintf(stream_out, Text, arg);
		va_end(arg);
		SetConsoleTextAttribute(g_Handle, Color::DEFAULT);
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
		Color color;
		switch (FLAG) {
		case (TRUE):	output = " [ON]\n"; color = Color::green;
		case (FALSE):	output = " [OFF]\n"; color = Color::red;
		}
		std::string append = TEXT + output;
		g_Console->printdbg(append.c_str(), color);

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
