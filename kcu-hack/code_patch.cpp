#include <iostream>
#include <conio.h> // For _getch()
#include <windows.h>
#include <tlhelp32.h> // Never include Win32 headers before <windows.h>
#include "constants.h"
#include "util.h"

using namespace std;

void code_patch(int option = 0) {
    uint32_t y_recoil_addr = 0; // var that stores address of player
    char opcode[] = "\x90\x90\x90\x90\x90";
    char original[] = "\xF3\x0F\x11\x56\x38";
    // getting process info
    wstring targetProcess = L"ac_client.exe";
    DWORD pid = check_pid(targetProcess);

    uint32_t baseAddress = get_base_address(pid, targetProcess);
    HANDLE TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

    // get the address of pointer pointing player address
    y_recoil_addr = baseAddress + Offsets::RecoilASMADDR;


    if (option) {
        WriteProcessMemory(TargetProcess, (BYTE*)(y_recoil_addr), &original, sizeof(char) * 5, NULL);
    }
    else {
        WriteProcessMemory(TargetProcess, (BYTE*)(y_recoil_addr), &opcode, sizeof(char) * 5, NULL);
    }
    

    cout << "removed y recoil" << "\n";
}
