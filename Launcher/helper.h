#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <TlHelp32.h>
#include "injector.h"

struct CONSOLE {
	const wchar_t* TITLE = L"Elden Ring | NightFyre";
	HWND WINDOW;
	int SELECTION = NULL;

	void _setWindow(int Width, int Height) {
		_COORD coord;
		coord.X = Width;
		coord.Y = Height;

		_SMALL_RECT Rect;
		Rect.Top = 0;
		Rect.Left = 0;
		Rect.Bottom = Height - 1;
		Rect.Right = Width - 1;

		HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle
		SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size
		SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size
	}

	virtual void allocConsole() {
		SetConsoleTitle(TITLE);
		WINDOW = GetConsoleWindow();
		_setWindow(45, 7);
		_setWindow(45, 7);
	}

	virtual void displayOptions() {
		system("cls");
		printf("[1] OFFLINE w/ EAC Disabled\n");
		printf("[2] OFFLINE w/ EAC Disabled + INJECT\n");
		printf("[3] QUIT\n\n");
	}

	virtual void notImplemented(int Message) {
		SELECTION = NULL;
		system("cls");
		if (Message == 1)
			printf("[!] INVALID SELECTION!\nPlease try again\n");
		if (Message == 2)
			printf("[!] NOT IMPLENTED IN THIS VERSION!\n");
		Sleep(2500);
	}
};
CONSOLE console_T;

struct PROCESS {
	//	STARTUP INFO
	const wchar_t* PATH = L"eldenring.exe";
	const wchar_t* PARAMS = L"-eac-nop-loaded";

	//  PROCESS INFORMATION
	PROCESS_INFORMATION pInfo;
	STARTUPINFO sInfo;
	HANDLE hProc;
	uintptr_t moduleBase;
	DWORD PID;
	DWORD exitCode;
	bool INJECT = FALSE;

	/// <summary>
	/// Creates memory for child process
	/// </summary>
	/// <param name="pInfo">Process Information</param>
	/// <param name="sInfo">Startup Info</param>
	/// <returns>TRUE & Allocated memory</returns>
	virtual bool allocMem(PROCESS_INFORMATION pInfo, STARTUPINFO sInfo) {
		ZeroMemory(&sInfo, sizeof(sInfo));
		sInfo.cb = sizeof(sInfo);
		ZeroMemory(&pInfo, sizeof(pInfo));
		return TRUE;
	}

	DWORD GetProcessID(const wchar_t* procNAME)
	{
		DWORD procId = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32 procEntry;
			procEntry.dwSize = sizeof(procEntry);

			if (Process32First(hSnap, &procEntry))
			{
				do
				{
					if (!_wcsicmp(procEntry.szExeFile, procNAME))
					{
						procId = procEntry.th32ProcessID;
						break;
					}
				} while (Process32Next(hSnap, &procEntry));
			}
		}
		CloseHandle(hSnap);
		return procId;
	}

	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
	{
		uintptr_t modBaseAddr = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPPROCESS, procId);
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);
			if (Module32First(hSnap, &modEntry))
			{
				do
				{
					if (!_wcsicmp(modEntry.szModule, modName))
					{
						modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
						break;
					}
				} while (Module32Next(hSnap, &modEntry));
			}
		}
		CloseHandle(hSnap);
		return modBaseAddr;
	}

	BOOL IsProcessRunning(DWORD pid) {
		HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
		DWORD ret = WaitForSingleObject(process, 0);
		CloseHandle(process);
		return ret == WAIT_TIMEOUT;
	}
};
PROCESS proc_T;

void Launcher(CONSOLE c, PROCESS p, int EAC, bool inject = {});
