#pragma once

#include <cstdint>

namespace Offsets {
    // Base Addresses
    constexpr uint32_t LocalPlayer = 0x0017E0A8; // ac_client.exe + 0x0017E0A8
    constexpr uint32_t EntityList = 0x18AC04;   // ac_client.exe + 0x18AC04
    constexpr uint32_t FOV = 0x18A7CC;   // ac_client.exe + 0x18A7CC
    constexpr uint32_t PlayerCount = 0x18AC0C;   // ac_client.exe + 0x18AC0C

    // Player Position Offsets
    constexpr uint32_t PositionX = 0x2C;
    constexpr uint32_t PositionY = 0x30;
    constexpr uint32_t PositionZ = 0x28;

    // Head Position Offsets
    constexpr uint32_t HeadX = 0x4;
    constexpr uint32_t HeadY = 0xC;
    constexpr uint32_t HeadZ = 0x8;

    // Player Camera Offsets
    constexpr uint32_t CameraX = 0x34;
    constexpr uint32_t CameraY = 0x38;

    // Ammo Offsets
    constexpr uint32_t AssaultRifleAmmo = 0x140;
    constexpr uint32_t SMGAmmo = 0x138;
    constexpr uint32_t SniperAmmo = 0x13C;
    constexpr uint32_t ShotgunAmmo = 0x134;
    constexpr uint32_t PistolAmmo = 0x12C;
    constexpr uint32_t GrenadeAmmo = 0x144;
    constexpr uint32_t ReservedAmmoRifle = 0x11C; // not tested out if applicable to other types of weapon


    // Fire Rate Offsets
    constexpr uint32_t FastFireAR = 0x164;
    constexpr uint32_t FastFireSniper = 0x160;
    constexpr uint32_t FastFireShotgun = 0x158;

    // Other Player Offsets
    constexpr uint32_t AutoShoot = 0x204;
    constexpr uint32_t Health = 0xEC;
    constexpr uint32_t Armor = 0xF0;
    constexpr uint32_t PlayerName = 0x205;

    constexpr uint32_t RecoilASMADDR = 0xC2EC3;

    constexpr uint32_t Team = 0x030C;
    constexpr uint32_t Dead = 0x0318;

    constexpr uint32_t v_bot = 0x54D07C;
    constexpr uint32_t v_user = 0x540DA4;

    // RECOIL PATTERN
    //constexpr uint32_t 
    //F3 0F 11 56 38
}
