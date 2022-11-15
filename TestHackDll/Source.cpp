#include <iostream>
#include <Windows.h>
const char* stringFormat = "%s";

//am rn to lazy to format everything.

//start of structure
class world_debug
{
public:
	const char* worldShotDebugText = "World debug: A shot has gone off!\n";
	const char* playerHealthDecText = "Player debug: player health has decreased!\n";
};

world_debug c_WorldDebug;
DWORD worldShotDebugJmpBack;
DWORD playerHealthDecJmpBack;
//end of structure

//thanks to guided hacking 
bool Hook(void* toHook, void* ourFunc, int len)
{
	if (len < 5)
	{
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	DWORD relativeAddress = ((DWORD)ourFunc - (DWORD)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;
	
	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);

	return true;
}

//yes i know your turning dyslexic by reading this
void __declspec(naked) WorldShotDebug()
{
	__asm
	{
		inc eax //This will make every enemy have unlimited ammo
		mov [ecx+24], eax
		mov al, 01
		mov eax, stringFormat
		mov ebx, c_WorldDebug.worldShotDebugText;
		push eax
		push ebx
		call printf
		pop ebx
		pop eax
		jmp[worldShotDebugJmpBack]
	}
}
void __declspec(naked) PlayerHealthDebug()
{
	__asm
	{
		fstp dword ptr[esi + 0x00000644]
		mov eax, stringFormat
		mov ebx, c_WorldDebug.playerHealthDecText;
		push eax
		push ebx
		call printf
		pop ebx
		pop eax
		jmp[playerHealthDecJmpBack]
	}
}

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