#include "Common.hpp"
#include "D3DRenderer.hpp"
#include "Hooking.hpp"
using namespace ELDEN_RING;

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

void CheatThread()
{
    while (g_DX12->bRunning)
    {
        g_Menu->loops();

        std::this_thread::sleep_for(1ms);
        std::this_thread::yield();
    }
}

static int LastTick = NULL;
void MainThread()
{
    while (g_DX12->bRunning)
    {
        if (GamePadGetKeyState(XINPUT_GAMEPAD_RIGHT_THUMB | XINPUT_GAMEPAD_LEFT_THUMB) && ((GetTickCount64() - LastTick) > 350))
        {
            g_DX12->m_ShowMenu ^= 1;
            LastTick = GetTickCount64();
        }
        std::this_thread::sleep_for(1ms);
        std::this_thread::yield();
    }
}

void init()
{
    //  WAIT FOR USER INPUT
    while (!GamePadGetKeyState(XINPUT_GAMEPAD_RIGHT_THUMB | XINPUT_GAMEPAD_LEFT_THUMB) && GetAsyncKeyState(VK_INSERT) == 0)
          Sleep(60);

    g_Menu              = std::make_unique<Menu>();
    g_Styles            = std::make_unique<Styles>();
    g_DX12              = std::make_unique<DX12_Base>();
    g_DX12->bRunning    = g_DX12->InitializeWindow("ELDEN RING™");
    if (g_DX12->bRunning)
    {
        HEXINTON::InitSdk();
        g_Hooking = std::make_unique<Hooking>();
        g_Hooking->Hook();

        std::thread bgWorker(CheatThread);
        
        MainThread();
        
        bgWorker.join();
    }
    HEXINTON::ShutdownSdk();
    FreeLibraryAndExitThread(g_Module, 0);
}