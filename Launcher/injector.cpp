#include "injector.h"
bool Inject(HANDLE Process)
{
	const char* PATH = "CUSTOM\\ER Overlay.dll";

	void* addr = VirtualAllocEx(Process, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(Process, addr, PATH, strlen(PATH) + 1, 0);
	HANDLE hThread = CreateRemoteThread(Process, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, addr, 0, 0);
	if (!hThread)
	{
		VirtualFreeEx(Process, addr, 0, MEM_RELEASE);
		return FALSE;
	}
	CloseHandle(hThread);
	return TRUE;
}