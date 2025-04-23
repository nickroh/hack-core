#ifndef HACKCORE_H  // Include guard to prevent multiple inclusions
#define HACKCORE_H

#include <Windows.h>
#include "Overlay.h"
#include "AimBot.h"

// HackCore class definition
class HackCore {
public:
    // Default constructor
    HackCore();  // Default constructor

    // Destructor
    ~HackCore();  // Destructor

    // Public data members (variables)

    // Public methods
    void setHP(int newHP);            // Set HP
    void setArmor(int newArmor);      // Set Armor
    void setAmmo(int newAmmo);        // Set Ammo
    void setRecoil(int newRecoil);    // Set Recoil
    void setESP(int enable);
    void setAimBot(int enable, float scale);
private:

    bool initialize();
    uintptr_t baseAddr;
    uintptr_t playerEntitiy;

    int playerCnt = 0;
    uintptr_t* otherPlayers;

    Overlay* overlay;
    AimBot* aimbot;
    
};

#endif // HACKCORE_H#pragma once
