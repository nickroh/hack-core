#include <iostream>
#include <conio.h> // For _getch()
#include <windows.h>
#include <tlhelp32.h> // Never include Win32 headers before <windows.h>
#include <fstream>
#include "constants.h"
#include "util.h"

using namespace std;

void Log(const string& message) {
    ofstream logFile("C:\\Users\\roh51\\Desktop\\overlay_log.txt", ios_base::app); // Open in append mode
    if (logFile.is_open()) {
        logFile << message << endl;
    }
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

bool isTeamGame(int gameMode) {
    return gameMode == 0 || gameMode == 4 || gameMode == 5 || gameMode == 7
        || gameMode == 11 || gameMode == 13 || gameMode == 14 || gameMode == 16
        || gameMode == 17 || gameMode == 20 || gameMode == 21;
}

bool WorldToScreen(vec pos, vec& screen, float matrix[16], int windowWidth, int windowHeight) {
    Vec4 clipCoords;

    clipCoords.x = matrix[0] * pos.x + matrix[4] * pos.y + matrix[8] * pos.z + matrix[12];
    clipCoords.y = matrix[1] * pos.x + matrix[5] * pos.y + matrix[9] * pos.z + matrix[13];
    clipCoords.z = matrix[2] * pos.x + matrix[6] * pos.y + matrix[10] * pos.z + matrix[14];
    clipCoords.w = matrix[3] * pos.x + matrix[7] * pos.y + matrix[11] * pos.z + matrix[15];

    // if it's inside or behind you
    if (clipCoords.w < 0.1f)
    {
        return false;
    }

    Vec3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    // modify the screen cordinates passed by reference
    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

    return true;
}
