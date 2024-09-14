#pragma once
#include "SDK.h"

/**
 * Name: Elden Ring
 * Version: ~
 * Engine: ~
*/

#pragma pack(push, 0x01)
namespace HEXINTON
{

	GameManager**			CGlobals::GGameMan;
	GameDataManager**		CGlobals::GGameDataMan;
	WorldCharacterManager**	CGlobals::GWorldCharMan;

	//---------------------------------------------------------------------------------------------------
	// 
	//	----------	[SECTION] GLOBALS
	//
	//---------------------------------------------------------------------------------------------------

	//	GameMan: 48 8B 05 ?? ?? ?? ?? 80 B8 ?? ?? ?? ?? 0D 0F94 C0 C3
	//	GameDataMan: 48 8B 05 ?? ?? ?? ?? 48 85 C0 74 05 48 8B 40 58 C3 C3
	//	WorldChrMan: 48 8B 05 ?? ?? ?? ?? 48 85 C0 74 0F 48 39 88
    bool InitSdk() { return InitSdk("EldenRing.exe", 0x3D69938, 0x3D5DF58, 0x3D65FA8); }
	bool InitSdk(const std::string& moduleName, unsigned int gGameMan, unsigned int gGameDataMan, unsigned int gWorldCharMan)
	{
		auto mBaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName.c_str()));
		if (!mBaseAddress)
			return FALSE;

		CGlobals::GGameMan			= reinterpret_cast<GameManager**>(mBaseAddress + gGameMan);
		CGlobals::GGameDataMan		= reinterpret_cast<GameDataManager**>(mBaseAddress + gGameDataMan);
		CGlobals::GWorldCharMan		= reinterpret_cast<WorldCharacterManager**>(mBaseAddress + gWorldCharMan);    
		
		if (!CGlobals::GGameMan || !CGlobals::GGameDataMan || !CGlobals::GWorldCharMan)
			return FALSE;
		
		return TRUE;
	}

	void ShutdownSdk()
	{
		CGlobals::GGameMan			= nullptr;
		CGlobals::GGameDataMan		= nullptr;
		CGlobals::GWorldCharMan		= nullptr;
	}

	unsigned int GetVtblOffset(void* czInstance, const char* dwModule)
	{
		uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(NULL);
		return ((*(unsigned int*)czInstance) - moduleBase);
	}

	int GetVtblIndex(void* fncPtr, void* vTblAddr) { return (((__int64)fncPtr - (__int64)vTblAddr) / sizeof(void*)) - 1; }
}
#pragma pack(pop)