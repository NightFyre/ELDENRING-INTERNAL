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

void MainThread()
{
    //  MAIN LOOP
    while (g_DX12->bRunning)
    {
        if (GamePadGetKeyState(XINPUT_GAMEPAD_RIGHT_THUMB | XINPUT_GAMEPAD_LEFT_THUMB))
            g_DX12->m_ShowMenu ^= 1;

        std::this_thread::sleep_for(1ms);
        std::this_thread::yield();
    }

    FreeLibraryAndExitThread(g_Module, 0);
}

void init()
{
    ///  STRUCTS, HOOKS & VARIABLES
    g_GameVariables     = std::make_unique<GameVariables>();
    g_GameFunctions     = std::make_unique<GameFunctions>();

    //  WAIT FOR USER INPUT
    while (!GamePadGetKeyState(XINPUT_GAMEPAD_RIGHT_THUMB | XINPUT_GAMEPAD_LEFT_THUMB))
          Sleep(60);

    g_Menu              = std::make_unique<Menu>();
    g_DX12              = std::make_unique<DX12_Base>();
    g_DX12->bRunning    = g_DX12->InitializeWindowContext(L"ELDEN RING™");
    if (!g_DX12->bRunning)
        return;

    g_Styles            = std::make_unique<Styles>();
    g_Hooking           = std::make_unique<Hooking>();
    g_Hooking->Hook();

    MainThread();
}