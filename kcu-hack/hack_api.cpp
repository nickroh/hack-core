#include <iostream>
#include <windows.h>
#include <tlhelp32.h> 
#include "constants.h"
#include "kcu_dll.h"
#include "dll_inject.h"
#include <direct.h>
#include "util.h"
#include "code_patch.h"

#pragma comment(lib, "Kernel32.lib")

using namespace std;

//// Function declarations
extern "C" __declspec(dllexport) void dll_hp_hack(int input);
extern "C" __declspec(dllexport) void dll_recoil_hack(int option);
extern "C" __declspec(dllexport) void dll_rifle_ammo_hack(int input);
extern "C" __declspec(dllexport) void dll_armor_hack(int input);
extern "C" __declspec(dllexport) bool dll_check_ac();


//// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // The DLL is loaded, and we can initialize resources, if needed.
        // You could call any hack functions from here, or let the user trigger them through other means
        cout << "DLL Injected successfully!" << endl;

        // Example: Automatically perform HP hack when DLL is loaded
        dll_hp_hack(100);
    }

    return TRUE; // Successfully loaded
}

// Your hack implementations
bool dll_check_ac() {
    wstring targetProcess = L"ac_client.exe";
    DWORD pid = check_pid(targetProcess);

    if (pid) {
        return true;
    }
    else {
        return false;
    }
}

void dll_hp_hack(int input = 100) {
    uint32_t playerAddress = 0;
    wstring targetProcess = L"ac_client.exe";
    DWORD pid = check_pid(targetProcess);

    uint32_t baseAddress = get_base_address(pid, targetProcess);
    HANDLE TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

    uint32_t playerEntitiy = baseAddress + Offsets::LocalPlayer;
    ReadProcessMemory(TargetProcess, (void*)(playerEntitiy), &playerAddress, sizeof(int), 0);

    uint32_t hp = playerAddress + Offsets::Health;

    int bugHp = input;
    WriteProcessMemory(TargetProcess, (BYTE*)(hp), &bugHp, sizeof(int), NULL);

    cout << "HP changed to 300 successfully " << "\n";
}

void dll_recoil_hack(int option = 1) {
    code_patch(option);
}

void dll_rifle_ammo_hack(int input = 30) {
    uint32_t playerAddress = 0;
    wstring targetProcess = L"ac_client.exe";
    DWORD pid = check_pid(targetProcess);

    uint32_t baseAddress = get_base_address(pid, targetProcess);
    HANDLE TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

    uint32_t playerEntitiy = baseAddress + Offsets::LocalPlayer;
    ReadProcessMemory(TargetProcess, (void*)(playerEntitiy), &playerAddress, sizeof(int), 0);

    uint32_t ara = playerAddress + Offsets::AssaultRifleAmmo;
    int bugAmmo = input;

    WriteProcessMemory(TargetProcess, (BYTE*)(ara), &bugAmmo, sizeof(int), NULL);

    cout << "Assault Rifle Ammo changed to 300 successfully " << "\n";
}

void dll_armor_hack(int input = 50) {
    uint32_t playerAddress = 0;
    wstring targetProcess = L"ac_client.exe";
    DWORD pid = check_pid(targetProcess);

    uint32_t baseAddress = get_base_address(pid, targetProcess);
    HANDLE TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

    uint32_t playerEntitiy = baseAddress + Offsets::LocalPlayer;
    ReadProcessMemory(TargetProcess, (void*)(playerEntitiy), &playerAddress, sizeof(int), 0);

    uint32_t armor = playerAddress + Offsets::Armor;
    int bugArmor = input;

    WriteProcessMemory(TargetProcess, (BYTE*)(armor), &bugArmor, sizeof(int), NULL);

    cout << "Armor changed to 300 successfully " << "\n";
}

