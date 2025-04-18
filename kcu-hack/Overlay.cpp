// Overlay.cpp
#include "Overlay.h"
#include "constants.h"
#include "OpenGL.h"
#include "Memory.h"
#include "Overlay.h"
#include "geometry.h"
#include "constants.h"
#include "player.h"
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <sstream>
#include "util.h"

Overlay::tSwapBuffers oSwapBuffers = nullptr;
Overlay* g_overlay = nullptr;

BOOL WINAPI hkSwapBuffers(HDC hdc) {

    uintptr_t base = reinterpret_cast<uintptr_t>(g_overlay->ac_client);
    int playerCount = *reinterpret_cast<int*>(base + Offsets::PlayerCount);
    uintptr_t* entityList = *reinterpret_cast<uintptr_t**>(base + Offsets::EntityList);
    uintptr_t localPlayer = *reinterpret_cast<uintptr_t*>(base + Offsets::LocalPlayer);
    int myTeam = *reinterpret_cast<int*>(localPlayer + Offsets::Team);

    std::vector<Player> players;

    for (int i = 1; i < playerCount; i++) {
        uintptr_t playerPtr = entityList[i];

        if (!playerPtr) continue;
        DWORD vtable = *reinterpret_cast<DWORD*>(playerPtr);
        if (vtable != 0x54D07C && vtable != 0x54D0A4) continue;

        Player p(playerPtr, myTeam);
        players.push_back(p);
    }

    OpenGL openGL;
    Rect rect = Rect(100, 100, 20, 20);
    openGL.SetupOrtho();
    openGL.DrawRect(rect, openGL.GREEN);
    openGL.RestoreGL();
    return oSwapBuffers(hdc);
}

Overlay::Overlay() {
    std::cout << "Overlay created." << std::endl;
    g_overlay = this;
    ac_client = GetModuleHandle(L"ac_client.exe");
    hOpenGL = GetModuleHandleA("opengl32.dll");
    if (!hOpenGL) {
        std::cerr << "[-] Failed to get handle\n";
        return;
    }
    target = GetProcAddress(hOpenGL, "wglSwapBuffers");
    if (!target) {
        std::cerr << "[-] Failed to find function\n";
        return;
    }
    else {
        std::cout << "func detected" << std::endl;
    }
    trampoline = new Memory::Trampoline();
    if (!trampoline) {
        std::cerr << "[-] Failed to instantiate Trampoline()\n";
        return;
    }
    openGL = new OpenGL();
}

Overlay::~Overlay() {
    if (trampoline) {
        // 후킹 해제
        if (activated) {
            trampoline->UnhookFunction(reinterpret_cast<BYTE*>(target), originalBytes, 5);
        }
        delete trampoline;
    }
    if (openGL) {
        delete openGL;
    }
    std::cout << "Overlay destroyed." << std::endl;
}

void Overlay::testESP() {
    uintptr_t base = reinterpret_cast<uintptr_t>(ac_client);  // assume ac_client is already set
    int playerCount = *reinterpret_cast<int*>(base + Offsets::PlayerCount);
    uintptr_t* entityList = *reinterpret_cast<uintptr_t**>(base + Offsets::EntityList);

    uintptr_t localPlayer = *reinterpret_cast<uintptr_t*>(base + Offsets::LocalPlayer);
    int myTeam = *reinterpret_cast<int*>(localPlayer + Offsets::Team);

    for (int i = 1; i < playerCount; ++i) {
        uintptr_t playerPtr = entityList[i];
        if (!playerPtr) continue;

        DWORD vtable = *reinterpret_cast<DWORD*>(playerPtr);
        if (vtable != 0x54D07C && vtable != 0x54D0A4) continue;

        Player p(playerPtr, myTeam);
        vec pos = p.getPosition();

        std::stringstream ss;
        ss << "Player " << i << " -> Pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ") team: " << p.getTeam();
        Log(ss.str());
    }
}

bool Overlay::activateESP(bool enable) {
    if (enable && !activated) {

        //target = reinterpret_cast<void*>(oSwapBuffers);// Save hook target for unhooking
        BYTE* dst = reinterpret_cast<BYTE*>(&hkSwapBuffers);


        // Backup original bytes before hooking
        memcpy(originalBytes, target, 5);

        // Create trampoline and store original function pointer
        oSwapBuffers = reinterpret_cast<tSwapBuffers>(
            trampoline->CreateTrampoline((BYTE*)target, dst, 5)
            );

        activated = true;
    }
    else if (!enable && activated) {
        // Restore original bytes
        trampoline->UnhookFunction(reinterpret_cast<BYTE*>(target), originalBytes, 5);

        // Restore original function pointer
        oSwapBuffers = reinterpret_cast<tSwapBuffers>(target);
        activated = false;
    }

    return true;
}

bool Overlay::WorldToScreen(vec pos, vec& screen, float matrix[16], int windowWidth, int windowHeight) {
    Vec4 clipCoords;

    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    if (clipCoords.w < 0.1f) {
        return false;
    }

    vec3 NDC;
    
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = (windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

    return true;
}
