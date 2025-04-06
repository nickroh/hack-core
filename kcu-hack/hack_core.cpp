#include <iostream>     // For console output
#include "util.h"
#include "constants.h"
#include "hack-core.h"
#include "code_patch.h"

// Default constructor
HackCore::HackCore() {
    // Initialize all member variables to default values
    initialize();
    std::cout << "HackCore created with default values.\n";
}

// Destructor
HackCore::~HackCore() {
    std::cout << "HackCore destroyed.\n";  // Output when the object is destroyed
}

bool HackCore::initialize() {
    HMODULE hModule = GetModuleHandle(L"ac_client.exe");

    if (hModule) {
        baseAddr = (uintptr_t)hModule;
        playerEntitiy = baseAddr + Offsets::LocalPlayer;
    }
    else {
        return false;
    }

    return true;
}

// Set HP method
void HackCore::setHP(int newHP) {
    if (!playerEntitiy) return;

    uintptr_t playerAddress = *(uintptr_t*)playerEntitiy;
    if (!playerAddress) return;

    int* hp = (int*)(playerAddress + Offsets::Health);

    *hp = newHP; 
}

// Set Armor method
void HackCore::setArmor(int newArmor) {
    if (!playerEntitiy) return;

    uintptr_t playerAddress = *(uintptr_t*)playerEntitiy;
    if (!playerAddress) return;

    int* armor = (int*)(playerAddress + Offsets::Armor);

    *armor = newArmor;
}

// Set Ammo method
void HackCore::setAmmo(int newAmmo) {
    if (!playerEntitiy) return;

    uintptr_t playerAddress = *(uintptr_t*)playerEntitiy;
    if (!playerAddress) return;

    int* ammo = (int*)(playerAddress + Offsets::AssaultRifleAmmo);

    *ammo = newAmmo;
}



// Set Recoil method
void HackCore::setRecoil(int newRecoil) {
    code_patch(newRecoil);
}