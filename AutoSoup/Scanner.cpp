#include "Scanner.h"

char* Scanner::ScanIn(const char* pattern, char* mask, char* begin, unsigned int size)
{
	unsigned int patternLength = strlen(mask);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(begin + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (begin + i);
		}
	}
	return (char*)"error";
}

char* Scanner::ScanEx(const char* pattern, char* mask, char* begin, char* end, HANDLE hProc)
{
	char* currentChunk = begin;
	char* match = nullptr;
	SIZE_T bytesRead;

	while (currentChunk < end)
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQueryEx(hProc, currentChunk, &mbi, sizeof(mbi))) {

			return (char*)"error";
		}

		if (mbi.RegionSize > INT_MAX) {
			currentChunk = currentChunk + mbi.RegionSize;
		}
		else
		{
			//char* buffer = (char*) VirtualAlloc(nullptr, mbi.RegionSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			char* buffer = new char[mbi.RegionSize];
			if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)
			{

				DWORD oldprotect;
				if (VirtualProtectEx(hProc, mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldprotect))
				{

					ReadProcessMemory(hProc, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead);
					VirtualProtectEx(hProc, mbi.BaseAddress, mbi.RegionSize, oldprotect, &oldprotect);
					char* internalAddress = ScanIn(pattern, mask, buffer, bytesRead);
					if (internalAddress != (char*)"error")
					{
						uintptr_t offsetFromBuffer = internalAddress - buffer;
						match = currentChunk + offsetFromBuffer;
						delete[] buffer;
						//VirtualFree(buffer);
						break;
					}
				}
			}
			currentChunk = currentChunk + mbi.RegionSize;
			//VirtualFree(buffer);
			delete[] buffer;
		}
	}
	return match;
}
