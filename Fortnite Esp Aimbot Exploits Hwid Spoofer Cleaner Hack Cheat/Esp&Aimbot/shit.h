#pragma once
#include "imports.h"

#pragma region colorsforconsole

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

#pragma endregion colorsforconsole

namespace items
{

	//handle for console output to change color
	HANDLE ConsoleHandle = 0;

	//check if process exists by getting pid
	bool processexists(const wchar_t* processname)
	{
		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE) {
			return false;
		}

		PROCESSENTRY32W entry = { 0 };
		entry.dwSize = sizeof(entry);
		if (Process32First(snapshot, &entry)) {
			do {
				if (_wcsicmp(entry.szExeFile, processname) == 0) {
					return true;
					break;
				}
			} while (Process32Next(snapshot, &entry));
		}

		CloseHandle(snapshot);

		return false;
	}
}