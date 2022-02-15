#include <ostream>
#include <Windows.h>
#pragma comment(linker, "/ENTRY:Entry")


void Entry()
{
    HMODULE hMod = LoadLibraryA("sc.dll");
    if (hMod == nullptr) {
        OutputDebugStringA("Failed to load dll");
    }
    //while (1)
    //{

    //}
}