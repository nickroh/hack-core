#include <iostream>
#include <windows.h>
#include <tlhelp32.h> 
#include "constants.h"
#include <direct.h>
#include "util.h"
#include "code_patch.h"

#pragma comment(lib, "Kernel32.lib")

using namespace std;

DWORD WINAPI ShowMessageBoxLoop(LPVOID lpParam) {
    while (true) {
        cout << "INJECTED DLL" << endl;
        MessageBox(NULL, L"Injected DLL is running!", L"Info", MB_OK);
        Sleep(5000);  // Wait for 5 seconds before showing again
    }
    return 0;
}

//// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // The DLL is loaded, and we can initialize resources, if needed.
        // You could call any hack functions from here, or let the user trigger them through other means
        cout << "DLL Injected successfully!" << endl;
        CreateThread(NULL, 0, ShowMessageBoxLoop, NULL, 0, NULL);
        // Example: Automatically perform HP hack when DLL is loaded
    }

    return TRUE; // Successfully loaded
}

