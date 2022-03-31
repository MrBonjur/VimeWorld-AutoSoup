#include <windows.h>
#include <string>
#include <iostream>
#include "Scanner.h"

HHOOK _hook;
KBDLLHOOKSTRUCT KeyboardStruct;

HWND get_hwnd = FindWindow(0, L"VimeWorld.ru");
HWND SunAwtFrame = FindWindowEx(get_hwnd, 0, L"SunAwtFrame", NULL);
HWND SunAwtCanvas = FindWindowEx(get_hwnd, 0, L"SunAwtCanvas", NULL);
HWND hwnd = FindWindowEx(SunAwtCanvas, 0, L"LWJGL", NULL);

const char* pattern = "\x14\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x00\x0A\x00\x00\x00\x00\x00\x00\x00";
char* mask = (char*)"xxxxxxxxxxxxxxxxxxxx";
bool is_inv = true;
int slot = 0x32;
bool isShutdown = false;
bool isDebugMode = false;
int trigger_hp = 15;


LRESULT APIENTRY HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_KEYDOWN && nCode >= 0)
	{
		KeyboardStruct = *((KBDLLHOOKSTRUCT*)lParam);
		if (KeyboardStruct.vkCode == 'E')
		{
			slot = '2';
			is_inv = not is_inv;			
		}

		if (KeyboardStruct.vkCode == VK_ESCAPE) is_inv = FALSE;

		if (KeyboardStruct.vkCode == VK_F8) isShutdown = true;			
		
	}
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook()
{
	_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {}
}

void UseSlotLogic() {
	if (slot > 0x39) slot = 0x32;
	PostMessage(hwnd, WM_KEYDOWN, slot, 0);
	PostMessage(hwnd, WM_KEYUP, slot, 0xC01E0001);
	PostMessage(hwnd, WM_RBUTTONDOWN, VK_RBUTTON, 0);
	Sleep(40);
	PostMessage(hwnd, WM_RBUTTONUP, VK_RBUTTON, 0xC01E0001);
	Sleep(10);
	PostMessage(hwnd, WM_KEYDOWN, 0x31, 0);
	PostMessage(hwnd, WM_KEYUP, 0x31, 0xC01E0001);
	slot += 0x1;
	Sleep(100);
}

void DebugMode() {
	AllocConsole();
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	SetConsoleTitleA("Debug console");
}

void AutoSoup(HMODULE hModule)
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetHook, 0, 0, 0);
	int value;	

	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, GetCurrentProcessId());
	Beep(800, 600);

	char* address = Scanner::ScanEx(pattern, mask, (char*)0x0, (char*)0xFFFFFFFFF, phandle);
	ReadProcessMemory(phandle, (void*)address, &value, sizeof(value), 0);

	while (!isShutdown)
	{
		if (value <= 1 || value > 40) {			
			address = Scanner::ScanEx(pattern, mask, (char*)0x0, (char*)0xFFFFFFFFF, phandle);
			while (!address && !isShutdown) {				
				address = Scanner::ScanEx(pattern, mask, (char*)0x0, (char*)0xFFFFFFFFF, phandle);
			}
			ReadProcessMemory(phandle, (void*)address, &value, sizeof(value), 0);
		}
		else {			
			if (value < trigger_hp && !is_inv) {
				UseSlotLogic();
			}
			ReadProcessMemory(phandle, (void*)address, &value, sizeof(value), 0);
		}		
		Sleep(50);
	}
	
	if (isDebugMode) {
		fclose((FILE*)stdin);
		fclose((FILE*)stdout);

		HWND hwnd = GetConsoleWindow();
		FreeConsole();
		PostMessageW(hwnd, WM_CLOSE, 0, 0);
	}
	UnhookWindowsHookEx(_hook);	
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH: {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AutoSoup, hModule, 0, 0);
			break;
		}
	}
	return true;
}

