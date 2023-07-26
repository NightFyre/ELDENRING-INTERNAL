#include "initialize.hpp"
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);
    
    if (!g_Module)
        g_Module = hModule;

    switch (ul_reason_for_call) 
    {
        case (DLL_PROCESS_ATTACH):  DisableThreadLibraryCalls(hModule); CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init, g_Module, NULL, NULL); break;
        case (DLL_PROCESS_DETACH):  g_KillSwitch = TRUE; break;
    }

    return TRUE;
}