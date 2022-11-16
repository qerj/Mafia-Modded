#include "Hacks.h"


DWORD worldShotDebugJmpBack;
DWORD playerHealthDecJmpBack;
const char* stringFormat = "%s";

//start of structure
class world_debug
{
public:
	const char* worldShotDebugText = "World debug: A shot has gone off!\n";
	const char* playerHealthDecText = "Player/NPC debug: someone has been shot!\n";
};

world_debug c_WorldDebug;

//yes i know your turning dyslexic by reading this
void __declspec(naked) WorldShotDebug()
{
	__asm
	{
		inc eax //This will make every enemy have unlimited ammo
		mov[ecx + 24], eax
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