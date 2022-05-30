#pragma once

#define DISCORD Discord::Instance()

class Discord
{

public:

    static uintptr_t GetDiscordModuleBase();

    bool CreateHook(uintptr_t originalPresent, uintptr_t hookFunction, uintptr_t pOriginal);

    bool EnableHook(uintptr_t pTarget, bool toggle);

    bool EnableHookQue();

    short SafeKeyState(int vKey);

    short SetCursorPos(int x, int y);

    bool GetCursorPos(LPPOINT lpPoint);

    HCURSOR SetCursor(HCURSOR hCursor);

    bool HookFunction(uintptr_t sig, uintptr_t function, uintptr_t ints);

    static Discord& Instance()
    {
        static Discord handle;
        return handle;
    }
};
