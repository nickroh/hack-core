#pragma once

#include <cstdint>

namespace Offsets {
    // Base Addresses
    constexpr uintptr_t LocalPlayer = 0x0017E0A8; // ac_client.exe + 0x0017E0A8
    constexpr uintptr_t EntityList = 0x18AC04;   // ac_client.exe + 0x18AC04
    constexpr uintptr_t FOV = 0x18A7CC;   // ac_client.exe + 0x18A7CC
    constexpr uintptr_t PlayerCount = 0x18AC0C;   // ac_client.exe + 0x18AC0C

    // Player Position Offsets
    constexpr uintptr_t PositionX = 0x2C;
    constexpr uintptr_t PositionY = 0x30;
    constexpr uintptr_t PositionZ = 0x28;

    // Head Position Offsets
    constexpr uintptr_t HeadX = 0x4;
    constexpr uintptr_t HeadY = 0xC;
    constexpr uintptr_t HeadZ = 0x8;

    // Player Camera Offsets
    constexpr uintptr_t CameraX = 0x34;
    constexpr uintptr_t CameraY = 0x38;

    // Ammo Offsets
    constexpr uintptr_t AssaultRifleAmmo = 0x140;
    constexpr uintptr_t SMGAmmo = 0x138;
    constexpr uintptr_t SniperAmmo = 0x13C;
    constexpr uintptr_t ShotgunAmmo = 0x134;
    constexpr uintptr_t PistolAmmo = 0x12C;
    constexpr uintptr_t GrenadeAmmo = 0x144;
    constexpr uintptr_t ReservedAmmoRifle = 0x11C; // not tested out if applicable to other types of weapon


    // Fire Rate Offsets
    constexpr uintptr_t FastFireAR = 0x164;
    constexpr uintptr_t FastFireSniper = 0x160;
    constexpr uintptr_t FastFireShotgun = 0x158;

    // Other Player Offsets
    constexpr uintptr_t AutoShoot = 0x204;
    constexpr uintptr_t Health = 0xEC;
    constexpr uintptr_t Armor = 0xF0;
    constexpr uintptr_t PlayerName = 0x205;

    constexpr uintptr_t RecoilASMADDR = 0xC2EC3;

    // RECOIL PATTERN
    //constexpr uintptr_t 
    //F3 0F 11 56 38
}
