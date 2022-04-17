// dllmain.cpp : Defines the entry point for the DLL application.
#include "Common.hpp"
#include "D3DRenderer.hpp"
#include "Hooking.hpp"
#include "Menu.hpp"
#include "GameDataMan.hpp"
#include "WorldCharMan.hpp"
#include "Console.hpp"

uintptr_t p22addy(uintptr_t PTR, std::vector<unsigned int> OFFSETS)
{
    uintptr_t addr = PTR;
    for (unsigned int i = 0; i < OFFSETS.size(); i++) {
        addr = *(uintptr_t*)addr;
        addr += OFFSETS[i];
    }
    return addr;
}

void MainThread()
{
    using namespace ER;

    //  STRUCTS, HOOKS & VARIABLES
    g_Console = std::make_unique<Console>();
    g_GameVariables = std::make_unique<GameVariables>();
    g_GameDataMan = std::make_unique<GameDataMan>();
    g_WorldCharMan = std::make_unique<WorldCharMan>();
    g_Menu = std::make_unique<Menu>();
    g_D3DRenderer = std::make_unique<D3DRenderer>();
    g_Hooking = std::make_unique<Hooking>();
    g_Hooking->Hook();

    uintptr_t ViewMatrix = p22addy(g_GameVariables->m_ModuleBase + 0x03C61588, { 0x60, 0x60, 0x420 });
    printf("ViewMatrixAddress: %llX\n", ViewMatrix);

    //  MAIN LOOP
    while (g_Running)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            g_GameVariables->m_ShowMenu = !g_GameVariables->m_ShowMenu;
            if (g_GameVariables->m_ShowMenu)
                g_Menu->InitStyle();
        }
        if (GetAsyncKeyState(VK_DELETE) & 1) {
            g_GameVariables->m_ShowMenu = FALSE;
            g_Running = FALSE;
        }
        std::this_thread::sleep_for(3ms);
        std::this_thread::yield();
    }

    //  EXIT THREAD
    g_Console->Free();
    std::this_thread::sleep_for(500ms);
    g_Hooking->Unhook();
    std::this_thread::sleep_for(500ms);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

    using namespace ER;
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        g_Module = hModule;
        _beginthread((_beginthread_proc_type)MainThread, 0, NULL);
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        g_Hooking->Unhook();
        FreeLibraryAndExitThread(hModule, TRUE);
    }
    return TRUE;
}

