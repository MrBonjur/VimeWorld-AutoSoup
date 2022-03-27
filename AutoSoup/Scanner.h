#pragma once
#include <Windows.h>

class Scanner {
public:
	static char* ScanIn(const char* pattern, char* mask, char* begin, unsigned int size);
	static char* ScanEx(const char* pattern, char* mask, char* begin, char* end, HANDLE hProc);
};