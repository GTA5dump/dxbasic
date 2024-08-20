// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

void regLog() {
    if (!AttachConsole(GetCurrentProcessId())) {
        AllocConsole();
    }
    SetConsoleTitleW(L"Hax");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        g_hModule = hModule;
        g_hModuleHandle = CreateThread(nullptr, NULL, [](LPVOID module_) -> DWORD {
            regLog();
            g_render.start_render();
            printf("Init render\n");
            while (g_Running) {
                if (GetAsyncKeyState(VK_END)) {
                    g_Running = false;
                }
                std::this_thread::sleep_for(10ms);
            }
            g_render.stop_render();
            fclose(stdout);
            FreeConsole();
            FreeLibraryAndExitThread(g_hModule, g_dThreadID);
            return 0;
        }, hModule, g_dThreadID, nullptr);
    }
    return TRUE;
}