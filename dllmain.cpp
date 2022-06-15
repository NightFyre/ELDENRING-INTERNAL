// dllmain.cpp : Defines the entry point for the DLL application.
#include "Common.hpp"
#include "D3DRenderer.hpp"
#include "Hooking.hpp"
#include "Menu.hpp"
#include "GameDataMan.hpp"
#include "WorldCharMan.hpp"
#include "Console.hpp"

/// TEMPORARY FUNCTION SPACE

//  RAINBOW THEME OPTION
float HSV_RAINBOW_SPEED = 0.001;
static float HSV_RAINBOW_HUE = 0;
/// <summary>
/// 
/// </summary>
/// <param name="saturation"></param>
/// <param name="value"></param>
/// <param name="opacity"></param>
void SV_RAINBOW(float saturation, float value, float opacity)
{
    using namespace ER;
    HSV_RAINBOW_HUE -= HSV_RAINBOW_SPEED;
    if (HSV_RAINBOW_HUE < -1.f) HSV_RAINBOW_HUE += 1.f;
    for (int i = 0; i < 860; i++)
    {
        float hue = HSV_RAINBOW_HUE + (1.f / (float)860) * i;
        if (hue < 0.f) hue += 1.f;
        g_Menu->dbg_RAINBOW = ImColor::HSV(hue, (saturation / 255), (value / 255), (opacity / 255));
    }
}

/// <summary>
/// SEPERATE THREAD for g_WorldCharMan->Update();
/// </summary>
void BackgroundWorker()
{
    //  This seperate thread was created for the sole purpose of updating the WorldCharMan Entity Struct
    //  Optimizations still need to be made as crashing can and does still occur during load screens and other instances
    //  Nullptrs tend to be the underlying cause.
    using namespace ER;
    while (true) {
        if (g_WorldCharMan->Update())
            g_WorldCharMan->count = NULL;
        std::this_thread::sleep_for(5s);
        std::this_thread::yield();
    }
    return;
}

/// <summary>
/// 
/// </summary>
void FMVSkip()
{
    ///  FMV SKIP NEW TEST
    //  STATIC ADDRESS!! NEEDS sig function applied!!
    //  AOB: 48 8B 90 ? ? ? ? 48 85 D2 74 07 C6
    //  OLD | 0x0A8FB4E
    using namespace ER;
    Patch((BYTE*)g_GameVariables->m_ModuleBase + 0x0A8FA5E, (BYTE*)"\xE9\x1C\x00\x00\x00", 5);
    g_Console->printdbg("[+] FMV's SKIPPED\n", TRUE, g_Console->color.green);
}

/// <summary>
/// 
/// </summary>
void UnlockFPS()
{
    ///  UNLOCK FPS LIMIT 
    //  STATIC ADDRESS!! NEEDS sig function applied!!
    //  AOB: C7 ? ? 89 88 88 3C EB ? 89 ? 18 EB ? 89 ? 18 C7 | (Menu.hpp->ptr_SET_FPS)
    //  AOB2: C7 ? EF 3C 00 00 00 C7 ? F3 01 00 00 00
    using namespace ER;
    Patch((BYTE*)g_GameVariables->m_ModuleBase + 0x1944A37, (BYTE*)"\xC7\x45\xEF\x00\x00\x00\x00", 7);  // AOB 2 | OLD 0x1944B27
    g_Console->printdbg("[+] FPS LIMIT REMOVED\n\n", TRUE, g_Console->color.green);
}

/// <summary>
/// 
/// </summary>
/// <param name="addr"></param>
/// <param name="ACTIVE"></param>
void PauseGameplay(uintptr_t addr, bool ACTIVE)
{
    //  STATIC ADDRESS!! NEEDS sig function applied!!
    //  Pause Game AOB: 0f 84 ? ? ? ? c6 83 ? ? 00 00 00 48 8d ? ? ? ? ? 48 89 ? ? 89
    //  OLD | 0xA7CDC6
    if (ACTIVE)
        Patch((BYTE*)addr + 0x0A7CCD6, (BYTE*)"\x85", 1);
    else if (!ACTIVE)
        Patch((BYTE*)addr + 0x0A7CCD6, (BYTE*)"\x84", 1);
}

/// <summary>
/// 
/// </summary>
void MainThread()
{
    using namespace ER;

    ///  STRUCTS, HOOKS & VARIABLES
    g_Console = std::make_unique<Console>();
    g_Console->printdbg("[+] ELDEN RING INTERNAL (PREVIEW)\n", TRUE, g_Console->color.yellow);
    g_Console->printdbg("[+] BUILD VERSION: alpha-0.0.5\n", TRUE, g_Console->color.yellow);
    g_Console->printdbg("[+] BUILD DATE: 5/3/22\n", TRUE, g_Console->color.yellow);
    g_Console->printdbg("[+] Created bv NightFyre & NBOTT42\n\n", TRUE, g_Console->color.yellow);
    g_Console->printdbg("[!] THIS IS A PREVIEW BUILD\n", TRUE, g_Console->color.red);
    g_Console->printdbg("[!] PLEASE DON'T INJECT UNTIL YOU HAVE REACHED THE MAIN MENU\n\n", TRUE, g_Console->color.red);
    g_GameVariables = std::make_unique<GameVariables>();
    g_GameFunctions = std::make_unique<GameFunctions>();
    ///  HIDE CONSOLE
    //g_GameVariables->m_ShowConsole = FALSE;
    //::ShowWindow(GetConsoleWindow(), SW_HIDE);
    //FMVSkip();
    //UnlockFPS();


    g_Console->printdbg("alpha-0.0.4 CHANGE-LOG:\n- WorldCharMan::Update Function Changed\n- WorldCharMan::Update Call Frequency INCREASED\n- Menu:: Included New Functions\n- Draw Skeleton Distance Updated\n- Freeze Entities Test\n\n", TRUE, g_Console->color.teal);
    g_Console->printdbg("[+] PRESS [INSERT] TO SHOW/HIDE MENU\n\n", FALSE);

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

    //uintptr_t ViewMatrix = g_GameFunctions->p2addy(g_GameVariables->m_ModuleBase + 0x03C04828, { 0x60, 0x60, 0x420 });
    //printf("ViewMatrixAddress: %llX\n", ViewMatrix);

    //  CREATE WorldCharMan Update Thread
    std::thread WCMUpdate(BackgroundWorker);

    //  MAIN LOOP
    while (g_Running)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            g_GameVariables->m_ShowMenu = !g_GameVariables->m_ShowMenu;
            //PauseGameplay(g_GameVariables->m_ModuleBase, g_GameVariables->m_ShowMenu);
        }

        if (GetAsyncKeyState(VK_DELETE) & 1) {
            g_GameVariables->m_ShowMenu = FALSE;
            g_Running = FALSE;
        }

        SV_RAINBOW(169, 169, 200);
        std::this_thread::sleep_for(3ms);
        std::this_thread::yield();
    }

    //  EXIT THREAD
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