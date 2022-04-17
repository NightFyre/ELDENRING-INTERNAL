#include "Console.hpp"
FILE* stream;
HANDLE fConsole; 

namespace ER {

	Console::Console() {
		Init();
	}

	void Console::Init() {
		AllocConsole();
		freopen_s(&stream, "CONOUT$", "w", stdout);
		fConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTitleA("ELDEN RING DEBUG CONSOLE");

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

		return;
	}

	void Console::printdbg(const char* Text, bool err, int color)
	{
		if (err) {
			SetConsoleTextAttribute(fConsole, color);
			printf(Text);
			SetConsoleTextAttribute(fConsole, 15);
		}
		else {
			printf(Text);
		}
		return;
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
