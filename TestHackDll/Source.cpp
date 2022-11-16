#include <iostream>
#include <Windows.h>

#include "Hooks.h"
#include "Hacks.h"


extern DWORD worldShotDebugJmpBack;
extern DWORD playerHealthDecJmpBack;
//end of structure


DWORD WINAPI MainThread(LPVOID param)
{
	//start of struct
	int worldDebugPlayerShotHookLenght = 6;
	DWORD hookAddressWorldDebugPlayerShot = 0x00609E64;
	worldShotDebugJmpBack = 0x00609E6A;//hookAddress + hookLenght;
	//end of struct
	int playerDebugHealthDecreaseHookLenght = 6;
	DWORD hookAddressPlayerDebugHealthDecrease = 0x005766FE;
	playerHealthDecJmpBack = 0x00576704;

	Hook((void*)hookAddressWorldDebugPlayerShot, WorldShotDebug, worldDebugPlayerShotHookLenght);
	Hook((void*)hookAddressPlayerDebugHealthDecrease, PlayerHealthDebug, playerDebugHealthDecreaseHookLenght);

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	while (1)
	{
		if (GetAsyncKeyState(VK_END)) { break; }
		
		Sleep(50);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread((HMODULE)param, 0);
}

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0);
	}

	return TRUE;
}