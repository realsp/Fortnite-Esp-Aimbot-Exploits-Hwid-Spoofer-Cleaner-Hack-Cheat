#pragma once

#include <vector>

class Helper
{
public:

    static uintptr_t PatternScan(uintptr_t moduleAdress, const char* signature);

    static void OpenConsole();
};
