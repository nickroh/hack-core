#include "HackCore.h"  // Include the header file
#include <iostream>     // For console output
#include "util.h">

// Default constructor
HackCore::HackCore() {
    // Initialize all member variables to default values
    std::cout << "HackCore created with default values.\n";
}

// Destructor
HackCore::~HackCore() {
    std::cout << "HackCore destroyed.\n";  // Output when the object is destroyed
}

bool HackCore::initialize() {
    HMODULE hModule = GetModuleHandle(L"ac_client.exe");

    if (hModule) {
        baseAddr = hModule;
    }
    else {
        return false;
    }

    return true;
}

// Set HP method
void HackCore::setHP(int newHP) {

}

// Set Armor method
void HackCore::setArmor(int newArmor) {

}

// Set Ammo method
void HackCore::setAmmo(int newAmmo) {


}

// Set Recoil method
void HackCore::setRecoil(int newRecoil) {

}