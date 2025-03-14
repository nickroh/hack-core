#include <iostream>
#include <conio.h> // For _getch()
#include <windows.h>
#include <tlhelp32.h> // Never include Win32 headers before <windows.h>
#include "constants.h"
#include "util.h"

using namespace std;

DWORD getProcessID(const wstring& processName) {
    DWORD pid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        cerr << "Failed to create snapshot." << endl;
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (processName == pe32.szExeFile) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return pid;
}

DWORD check_pid(const wstring& process_name) {
    wstring targetProcess = process_name;
    DWORD pid = getProcessID(targetProcess);

    if (pid) {
        wcout << "Process " << targetProcess << " found with PID: " << pid << endl;
        return pid;
    }
    else {
        wcout << "Process " << targetProcess << " not found." << endl;
        return 0;
    }
}

uint32_t get_base_address(DWORD pid, const wstring& moduleName) {
    uint32_t baseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        cerr << "Failed to create module snapshot." << endl;
        return 0;
    }

    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &me32)) {
        do {
            if (moduleName == me32.szModule) {
                baseAddress = (uint32_t)me32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &me32));
    }

    CloseHandle(hSnapshot);
    return baseAddress;
}
