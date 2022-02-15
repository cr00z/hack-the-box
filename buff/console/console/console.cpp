// console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
    char* path = "c:\\windows\\tasks\\sc.dll";
    HMODULE hMod = LoadLibraryA(path);
    if (hMod == nullptr)
    {
        cout << "Failed to load dll " << path << endl;
        char* path2 = "sc.dll";
        HMODULE hMod = LoadLibraryA(path2);
    }
    while (1) {}
	return 0;
}

