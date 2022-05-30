#include "stdafx.h"
#include "detours.h"
#include "lazyimporter.h"

VOID Main()
{
    MH_Initialize();
    SettingsHelper::LoadDefaultConfig();

    if (!Util::Initialize()) {
        return;
    }

    if (!offsets::Initialize()) {
        return;
    }
    if (!hooks::Initialize()) {
        return;
    }

    if (!rend::Initialize()) {
        return;
    }
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) 
{
    if (reason == DLL_PROCESS_ATTACH) 
    {
      iat(DisableThreadLibraryCalls)(hModule);
      Main();
    }

    return TRUE;
}