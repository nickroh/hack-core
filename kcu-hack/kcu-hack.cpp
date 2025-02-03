// kcu-hack.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h> // For _getch()
#include <windows.h>
#include <tlhelp32.h> // Never include Win32 headers before <windows.h>

#pragma comment(lib, "Kernel32.lib")

using namespace std;

void hp_hack();
DWORD check_pid();
uintptr_t get_base_address(DWORD pid, const wstring& moduleName);
DWORD getProcessID(const wstring& processName);

int main() {
    char ch;
    cout << "Press 'q' to print 'Welcome message'.\n"
        << "Press 'g' to get pid.\n"
        << "Press '0' to quit.\n"
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
            cout << "Base address for ac_client.exe " << baseAddress << "\n";
        }
    }

    return 0;
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
void hp_hack() {

}

