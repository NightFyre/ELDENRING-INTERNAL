#include "include/initialize.hpp"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);
    
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        g_Module = hModule;
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(NULL, NULL, MainThread, g_Module, NULL, NULL);
        if (hThread)
            CloseHandle(hThread);
    }

    return TRUE;
}