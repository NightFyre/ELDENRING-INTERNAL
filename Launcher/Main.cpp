#include "helper.h"

int main()
{
	console_T.allocConsole();

	//  DISPLAY LAUNCH OPTIONS
LABEL_START:
	console_T.displayOptions();

	//	USER INPUT
	std::cin >> console_T.SELECTION;

	//  EXCEPTIONS
	if (console_T.SELECTION <= 0 || console_T.SELECTION >= 4) {
		console_T.notImplemented(1);
		goto LABEL_START;
	}
	else if (console_T.SELECTION == 3) return 0;

	//	INJECT DEBUG BOOL
	if (console_T.SELECTION == 2) {
		///	Set Boolean to true, handle injection post launch AFTER resuming the thread
		proc_T.INJECT = TRUE;
	}

	system("cls");

	//	LAUNCH
	Launcher(console_T, proc_T, console_T.SELECTION, proc_T.INJECT);
}

void Launcher(CONSOLE c, PROCESS p, int EAC, bool INJECT)
{
	//  ALLOC MEMORY
	p.allocMem(p.pInfo, p.sInfo);

	//  CREATE PROCESS
	if (!CreateProcessW(p.PATH, (LPWSTR)p.PARAMS, NULL, NULL, FALSE, 0, NULL, NULL, &p.sInfo, &p.pInfo)) {
		printf("[!] CREATE PROCESS FAILED!\n[!]ERROR CODE: (%d)\n[!]Exiting . . .\n", GetLastError());
		Sleep(5000);
		return;
	}
	printf("[+] Elden Ring Launched\n");
	Sleep(2500);
	
	//	HIDE CONSOLE WINDOW
	ShowWindow(c.WINDOW, SW_HIDE);

	//  PAUSE PROCESS
	SuspendThread(p.pInfo.hThread);

	//  ESTABLISH PROCESS INFO
	p.hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, p.pInfo.dwProcessId);
	p.moduleBase = p.GetModuleBaseAddress(p.pInfo.dwProcessId, p.PATH);

	//  RESUME PROCESS
	ResumeThread(p.pInfo.hThread);

	//  INJECT
	if (INJECT) {
		//	Visit http://www.GuidedHacking.com to learn more. 
		// Broihon has an amazing write up
		if (!Inject(p.pInfo.hProcess)) {
			MessageBoxA(NULL, "FAILED TO INJECT INTO PROCESS!\n\nTROUBLESHOOTING TIPS:\n[1] double check the dll name is 'ER Overlay.dll'\n[2] Make sure you have placed the dll in the correcr path 'ELDEN RING\Game\CUSTOM\ER Overlay.dll'\n", "ERROR", MB_ICONERROR);
			TerminateProcess(p.pInfo.hProcess, EXIT_FAILURE);
			CloseHandle(p.pInfo.hProcess);
			CloseHandle(p.pInfo.hThread);
			return;
		}
	}

	//	KEEPS LAUNCHER RUNNING IN BACKGROUND
	while (p.IsProcessRunning(p.pInfo.dwProcessId))
		Sleep(60);

	// CLOSE PROCESS AND THREAD HANDLES
	CloseHandle(p.pInfo.hProcess);
	CloseHandle(p.pInfo.hThread);
	return;
}
