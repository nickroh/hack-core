#include <iostream>
#include <conio.h> // For _getch()
#include <windows.h>
#include <tlhelp32.h> // Never include Win32 headers before <windows.h>
#include "constants.h"
#include "kcu_dll.h"
#include "dll_inject.h"
#include <direct.h>

#pragma comment(lib, "Kernel32.lib")

using namespace std;

void hp_hack();
void recoil_hack();
void curr_ammo_hack();
void reserved_ammo_hack();
void dll_test();
DWORD check_pid(const wstring& processName);
uintptr_t get_base_address(DWORD pid, const wstring& moduleName);
DWORD getProcessID(const wstring& processName);

wstring get_current_directory() {
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        // Convert char[] (cwd) to std::wstring
        return std::wstring(cwd, cwd + strlen(cwd));
    }
    else {
        // Error getting current directory
        return L"Error getting current directory";
    }
}

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
            wstring targetProcess = L"ac_client.exe";
            DWORD pid = check_pid(targetProcess);
        }
        else if (ch == 'm') {
            wstring targetProcess = L"ac_client.exe";
            DWORD pid = check_pid(targetProcess);
            uintptr_t baseAddress = get_base_address(pid, targetProcess);
            cout << "Base address for ac_client.exe 0x" << hex << baseAddress << "\n";
        }
        else if (ch == 'i') {
            wstring targetProcess = L"Notepad.exe"; // 메모장에 인젝트
            DWORD pid = check_pid(targetProcess);
            cout << "TESTING DLL INJECTION!\n";

            wstring dll_name = get_current_directory() + L"\\kcu_dll.dll";
            wcout << dll_name << "\n";

            dll_injection(pid, dll_name);

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

// method that changes hp value
void hp_hack() {
    uintptr_t playerAddress = 0; // var that stores address of player
        
    // getting process info
    wstring targetProcess = L"ac_client.exe";
    DWORD pid = check_pid(targetProcess);
    
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
    cout << 1;

}

// TODO: implement ammo HACK
void curr_ammo_hack() {

}

// TODO: implement ammo HACK
void reserved_ammo_hack() {

}



