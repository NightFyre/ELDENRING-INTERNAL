// dllmain.cpp : Defines the entry point for the DLL application.
#include "include/Common.hpp"
#include "include/D3DRenderer.hpp"
#include "include/Hooking.hpp"
#include "include/Menu.hpp"
#include "include/GameDataMan.hpp"
#include "include/WorldCharMan.hpp"
#include "include/Console.hpp"

/// TEMPORARY FUNCTION SPACE, All functions here will be placed elsewhere eventually. I do not like having dllmain being so cluttered.



/// <summary>
/// SEPERATE THREAD for g_WorldCharMan->Update();
/// </summary>
/// // this will remain here
void BackgroundWorker()
{
    //  This seperate thread was created for the sole purpose of updating the WorldCharMan Entity Struct
    //  Optimizations still need to be made as crashing can and does still occur during load screens and other instances
    //  Nullptrs tend to be the underlying cause.
    using namespace ER;
    while (g_Running) {
        if (g_WorldCharMan->Update())
            g_WorldCharMan->count = NULL;
        std::this_thread::sleep_for(5s);
        std::this_thread::yield();
    }
    return;
}

void MainThread()
{
    using namespace ER;

    ///  STRUCTS, HOOKS & VARIABLES
    g_Console = std::make_unique<Console>();
    g_Console->InitializeConsole("ELDEN RING INTERNAL - DEBUG");
    g_Console->printdbg("[+] ELDEN RING INTERNAL (PREVIEW)\n", Console::Color::yellow);
    g_Console->printdbg("[+] BUILD VERSION: alpha-0.0.5\n", Console::Color::yellow);
    g_Console->printdbg("[+] BUILD DATE: 5/3/22\n", Console::Color::yellow);
    g_Console->printdbg("[+] Created bv NightFyre & NBOTT42\n\n", Console::Color::yellow);
    g_Console->printdbg("[!] THIS IS A PREVIEW BUILD\n", Console::Color::red);
    g_Console->printdbg("[!] PLEASE DON'T INJECT UNTIL YOU HAVE REACHED THE MAIN MENU\n\n", Console::Color::red);
    g_GameVariables = std::make_unique<GameVariables>();
    g_GameFunctions = std::make_unique<GameFunctions>();
    ///  HIDE CONSOLE
    //g_GameVariables->m_ShowConsole = FALSE;
    //::ShowWindow(GetConsoleWindow(), SW_HIDE);
    g_GameFunctions->FMVSkip(g_GameVariables->m_ModuleBase);
    g_GameFunctions->UnlockFPS(g_GameVariables->m_ModuleBase);

    g_Console->printdbg("alpha-0.0.4 CHANGE-LOG:\n- WorldCharMan::Update Function Changed\n- WorldCharMan::Update Call Frequency INCREASED\n- Menu:: Included New Functions\n- Draw Skeleton Distance Updated\n- Freeze Entities Test\n\n", Console::Color::teal);
    g_Console->printdbg("[+] PRESS [INSERT] TO SHOW/HIDE MENU\n\n", Console::Color::DEFAULT);

    //  WAIT FOR USER INPUT
    while (GetAsyncKeyState(VK_INSERT) == NULL)
        Sleep(60);
    system("cls");

    g_Menu = std::make_unique<Menu>();
    g_D3DRenderer = std::make_unique<D3DRenderer>();
    g_Styles = std::make_unique<Styles>();
    g_Hooking = std::make_unique<Hooking>();
    g_Hooking->Hook();
    g_GameDataMan = std::make_unique<GameDataMan>();
    g_WorldCharMan = std::make_unique<WorldCharMan>();

    //uintptr_t ViewMatrix = g_GameFunctions->p2addy(g_GameVariables->m_ModuleBase + 0x03C04828, { 0x60, 0x60, 0x420 });
    //printf("ViewMatrixAddress: %llX\n", ViewMatrix);

    //  CREATE WorldCharMan Update Thread
    std::thread WCMUpdate(BackgroundWorker);

    //  MAIN LOOP
    while (g_Running)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            g_GameVariables->m_ShowMenu = !g_GameVariables->m_ShowMenu;
            g_GameFunctions->PauseGameplay(g_GameVariables->m_ModuleBase, g_GameVariables->m_ShowMenu);
        }

        if (GetAsyncKeyState(VK_DELETE) & 1) {
            g_GameVariables->m_ShowMenu = FALSE;
            g_Running = FALSE;
        }

        std::this_thread::sleep_for(3ms);
        std::this_thread::yield();
    }

    //  EXIT THREAD
    g_GameFunctions->PauseGameplay(g_GameVariables->m_ModuleBase, FALSE);    //  Due to unhooking requiring the menu being shown ... the game will remain paused unless we revert that here
    WCMUpdate.join();
    g_Console->Free();
    std::this_thread::sleep_for(500ms);
    FreeLibraryAndExitThread(g_Module, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    using namespace ER;
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        g_Module = hModule;
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, g_Module, NULL, NULL);
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
        g_KillSwitch = TRUE;
    return TRUE;
}