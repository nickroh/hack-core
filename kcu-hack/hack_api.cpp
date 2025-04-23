#include <iostream>
#include <windows.h>
#include <direct.h>
#include "receiver.h"

using namespace std;

DWORD WINAPI ShowMessageBoxLoop(LPVOID lpParam) {
    while (true) {
        cout << "INJECTED DLL" << endl;
        MessageBox(NULL, L"Injected DLL is running!", L"Info", MB_OK);
        Sleep(5000);  // Wait for 5 seconds before showing again
    }
    return 0;
}

DWORD WINAPI runReceiver(LPVOID lpParam) {
    Receiver* receiver = new Receiver();
    receiver->start();
    return 0;
}


//// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // The DLL is loaded, and we can initialize resources, if needed.
        // You could call any hack functions from here, or let the user trigger them through other means
        HANDLE hThread = CreateThread(NULL, 0, runReceiver, NULL, 0, NULL); // 
        if (hThread) {
            CloseHandle(hThread);
        }
    }

    return TRUE; // Successfully loaded
}

