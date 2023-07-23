#include "include/Common.hpp"
#include "include/D3DRenderer.hpp"
#include "include/Hooking.hpp"
#include "include/Menu.hpp"
#include "include/GameDataMan.hpp"
#include "include/WorldCharMan.hpp"
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")
using namespace ER;
void MainThread();

bool GamePadGetKeyState(WORD combinationButtons)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    DWORD result = XInputGetState(0, &state);
    if (result == ERROR_SUCCESS)
    {
        if ((state.Gamepad.wButtons & combinationButtons) == combinationButtons)
            return true;
    }
    return false;
}

/// <summary>
/// SEPERATE THREAD for g_WorldCharMan->Update();
/// </summary>
/// // this will remain here
void UpdateThread()
{
    //  This seperate thread was created for the sole purpose of updating the WorldCharMan Entity Struct
    //  Optimizations still need to be made as crashing can and does still occur during load screens and other instances
    //  Nullptrs tend to be the underlying cause.
    while (g_Running) {
        if (g_WorldCharMan->Update())
            g_WorldCharMan->count = NULL;
        std::this_thread::sleep_for(5s);
        std::this_thread::yield();
    }
    return;
}

void BackgroundWorker()
{
    while (g_Running) {
        g_Menu->bgLoops();
        std::this_thread::sleep_for(1ms);
        std::this_thread::yield();
    }
    return;
}

void init()
{
    ///  STRUCTS, HOOKS & VARIABLES
    g_GameVariables     = std::make_unique<GameVariables>();
    g_GameFunctions     = std::make_unique<GameFunctions>();

    //  g_GameFunctions->FMVSkip(g_GameVariables->m_ModuleBase);
    //  g_GameFunctions->UnlockFPS(g_GameVariables->m_ModuleBase);

    //  WAIT FOR USER INPUT
    //  while (GetAsyncKeyState(VK_INSERT) == NULL)
    //      Sleep(60);
    //  system("cls");

    g_Menu              = std::make_unique<Menu>();
    g_D3DRenderer       = std::make_unique<D3DRenderer>();
    g_Styles            = std::make_unique<Styles>();
    g_Hooking           = std::make_unique<Hooking>();
    g_Hooking->Hook();
    //  g_GameDataMan   = std::make_unique<GameDataMan>();
    //  g_WorldCharMan  = std::make_unique<WorldCharMan>();

    MainThread();
}

void MainThread()
{
    //  CREATE WorldCharMan Update Thread
    //  std::thread WCMUpdate(UpdateThread);
    //  std::thread BGWorker(BackgroundWorker);

    //  MAIN LOOP
    while (g_Running)
    {
        if (GamePadGetKeyState(XINPUT_GAMEPAD_A | XINPUT_GAMEPAD_B))
            g_GameVariables->m_ShowMenu = !g_GameVariables->m_ShowMenu;

        if (GetAsyncKeyState(VK_INSERT) & 1) {
            g_GameVariables->m_ShowMenu = !g_GameVariables->m_ShowMenu;
            //  g_GameFunctions->PauseGameplay(g_GameVariables->m_ModuleBase, g_GameVariables->m_ShowMenu);
        }

        if (GetAsyncKeyState(VK_DELETE) & 1) {
            g_GameVariables->m_ShowMenu = FALSE;
            g_Running = FALSE;
        }

        std::this_thread::sleep_for(3ms);
        std::this_thread::yield();
    }

    //  EXIT THREAD
    //  g_GameFunctions->PauseGameplay(g_GameVariables->m_ModuleBase, FALSE);    //  Due to unhooking requiring the menu being shown ... the game will remain paused unless we revert that here
    //  BGWorker.join();
    //  WCMUpdate.join();
    std::this_thread::sleep_for(500ms);
    FreeLibraryAndExitThread(g_Module, 0);
}