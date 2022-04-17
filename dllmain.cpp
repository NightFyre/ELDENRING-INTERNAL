// dllmain.cpp : Defines the entry point for the DLL application.
#include "Common.hpp"
#include "D3DRenderer.hpp"
#include "Hooking.hpp"
#include "Menu.hpp"
#include "GameDataMan.hpp"
#include "WorldCharMan.hpp"
#include "Console.hpp"
#include <map>

void MainThread()
{
    
    using namespace ER;

    //  STRUCTS, HOOKS & VARIABLES
    g_Console = std::make_unique<Console>();
    g_Console->printdbg("[+] ELDEN RING INTERNAL (PREVIEW)\n", TRUE, g_Console->color.yellow);
    g_Console->printdbg("[+] BUILD VERSION: alpha-0.0.3\n", TRUE, g_Console->color.yellow);
    g_Console->printdbg("[+] BUILD DATE: 4/15/22\n", TRUE, g_Console->color.yellow);
    g_Console->printdbg("[+] Created bv NightFyre & NBOTT42\n\n", TRUE, g_Console->color.yellow);\

    g_Console->printdbg("[!] THIS IS A PREVIEW BUILD\n", TRUE, g_Console->color.red);
    g_Console->printdbg("[!] PLEASE DON'T INJECT UNTIL YOU HAVE LOADED YOUR SAVE\n\n", TRUE, g_Console->color.red);
    g_Console->printdbg("[+] PRESS [INSERT] TO INJECT MENU\n", FALSE);
    g_GameVariables = std::make_unique<GameVariables>();
    g_GameFunctions = std::make_unique<GameFunctions>();

    //  HIDE CONSOLE
    //g_GameVariables->m_ShowConsole = FALSE;
    //::ShowWindow(GetConsoleWindow(), SW_HIDE);

    //  FMV SKIP NEW TEST
    //  AOB: 48 8B 90 ? ? ? ? 48 85 D2 74 07 C6
    Patch((BYTE*)g_GameVariables->m_ModuleBase + 0x0AB01EE, (BYTE*)"\xE9\x1C\x00\x00\x00", 5);
    g_Console->printdbg("[+] FMV::Skip ; PATCH APPLIED\n", TRUE, g_Console->color.green);

    //  UNLOCK FPS LIMIT
    Patch((BYTE*)g_GameVariables->m_ModuleBase + 0x1963CF7, (BYTE*)"\xC7\x45\xEF\x00\x00\x00\x00", 7);
    g_Console->printdbg("[+] FPS:: LIMIT REMOVED\n\n", TRUE, g_Console->color.green);


    //  WAIT FOR USER INPUT
    while (GetAsyncKeyState(VK_INSERT) == NULL)
        Sleep(60);
    system("cls");


    g_Menu = std::make_unique<Menu>();
    g_D3DRenderer = std::make_unique<D3DRenderer>();
    g_Hooking = std::make_unique<Hooking>();
    g_Hooking->Hook();
    g_GameDataMan = std::make_unique<GameDataMan>();
    g_WorldCharMan = std::make_unique<WorldCharMan>();

    uintptr_t ViewMatrix = g_GameFunctions->p2addy(g_GameVariables->m_ModuleBase + 0x03C61588, { 0x60, 0x60, 0x420 });
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

