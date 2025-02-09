#include <iostream>
#include <conio.h> // For _getch()
#include <windows.h>
#include <tlhelp32.h> // Never include Win32 headers before <windows.h>
#include "constants.h"
#include "kcu_dll.h"

#pragma comment(lib, "Kernel32.lib")

using namespace std;

void hp_hack();
void recoil_hack();
void curr_ammo_hack();
void reserved_ammo_hack();
void dll_test();
DWORD check_pid();
uintptr_t get_base_address(DWORD pid, const wstring& moduleName);
DWORD getProcessID(const wstring& processName);

int main() {
    char ch;
    cout << "Press 'q' to print 'Welcome message'.\n"
        << "Press 'g' to get pid.\n"
        << "Press '0' to quit.\n"
        << "Press 'h' to hack hp.\n"
        << "Press 'm' to get base addr.\n";

    while (true) {
        ch = _getch(); // Get character input without pressing Enter

        if (ch == '0') {
            cout << "Exiting...\n";
            break;
        }
        else if (ch == 'q') {
            cout << "NUTRIA CHEAT ACTIAVTED!!\n";
        }
        else if (ch == 'g') {
            DWORD pid = check_pid();
        }
        else if (ch == 'm') {
            DWORD pid = check_pid();
            wstring targetProcess = L"ac_client.exe";
            uintptr_t baseAddress = get_base_address(pid, targetProcess);
            cout << "Base address for ac_client.exe 0x" << std::hex << baseAddress << "\n";
        }
        else if (ch == 'h') {
            hp_hack();
        }

    }

    return 0;
}

void dll_test() {
    PrintComment();
}

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

DWORD check_pid() {
    wstring targetProcess = L"ac_client.exe";
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

uintptr_t get_base_address(DWORD pid, const wstring& moduleName) {
    uintptr_t baseAddress = 0;
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
                baseAddress = (uintptr_t)me32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &me32));
    }

    CloseHandle(hSnapshot);
    return baseAddress;
}

// method that changes hp value
void hp_hack() {
    uintptr_t playerAddress = 0; // var that stores address of player
        
    // getting process info
    DWORD pid = check_pid(); 
    wstring targetProcess = L"ac_client.exe";
    uintptr_t baseAddress = get_base_address(pid, targetProcess);
    HANDLE TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

    // get the address of pointer pointing player address
    uintptr_t playerEntitiy = baseAddress + Offsets::LocalPlayer;

    // read the value of pointer
    ReadProcessMemory(TargetProcess, (void*)(playerEntitiy), &playerAddress, sizeof(uintptr_t), 0);

    // calculate address of hp based on offset
    uintptr_t hp = playerAddress + Offsets::Health;
    cout << "player addr" << std::hex <<  playerAddress << "\n";
    // hp to set
    int bugHp = 300;

    // set hp to bugHp
    WriteProcessMemory(TargetProcess, (BYTE*)(hp), &bugHp, sizeof(int), NULL);

    // print some message 그래야 보기좋음 ㅇㅇ
    cout << "HP changed to 300 successfully " << "\n";
}

// TODO: implement RECOIL HACK
void recoil_hack() {

}

// TODO: implement ammo HACK
void curr_ammo_hack() {

}

// TODO: implement ammo HACK
void reserved_ammo_hack() {

}



