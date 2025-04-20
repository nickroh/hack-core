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
#include <gl\GL.h>
#include "util.h"

Overlay::tSwapBuffers oSwapBuffers = nullptr;
Overlay* g_overlay = nullptr;

int cnt = 0;

BOOL WINAPI hkSwapBuffers(HDC hdc) {
    g_overlay->materializeESP();
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

void Overlay::materializeESP() {
    openGL->SetupOrtho();

    uintptr_t base = reinterpret_cast<uintptr_t>(g_overlay->ac_client);
    int playerCount = *reinterpret_cast<int*>(base + Offsets::PlayerCount);
    uintptr_t* entityList = *reinterpret_cast<uintptr_t**>(base + Offsets::EntityList);
    uintptr_t localPlayer = *reinterpret_cast<uintptr_t*>(base + Offsets::LocalPlayer);
    int myTeam = *reinterpret_cast<int*>(localPlayer + Offsets::Team);
    float* matrixPtr = reinterpret_cast<float*>(base + Offsets::ViewMatrix);
    float viewMatrix[16];
    memcpy(viewMatrix, matrixPtr, sizeof(float) * 16);

    //std::vector<Player> players;

    for (int i = 1; i < playerCount; i++) {

        uintptr_t playerPtr = entityList[i];

        if (!playerPtr) continue;
        DWORD vtable = *reinterpret_cast<DWORD*>(playerPtr);
        if (vtable != 0x54D07C && vtable != 0x54D0A4) continue;

        Player p(playerPtr, myTeam);
        //players.push_back(p);

        vec screen;
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        if (!WorldToScreen(p.getPosition(), screen, matrixPtr, viewport[2], viewport[3])) {
            continue;
        } 

        std::stringstream ss;
        ss << viewport[2] << ", " << viewport[3] << " input: (x,y,z) " << p.getPosition().x << "," << p.getPosition().y << "," << p.getPosition().z << " => screen coord " << ": " << screen.x << ", " << screen.y << " team: " << p.getTeam();

        if (cnt % 1000 == 0) {
            Log(ss.str());
        }

        float tmp = p.getPosition().Distance(Vec3(matrixPtr[12], matrixPtr[13], matrixPtr[14]));
        float scale = (GAME_UNIT_MAGIC / tmp) * (viewport[2] / VIRTUAL_SCREEN_WIDTH);
        float x = screen.x - scale;
        float y = screen.y - scale * PLAYER_ASPECT_RATIO;
        float width = scale * 2;
        float height = scale * PLAYER_ASPECT_RATIO * 2;

        Rect rect = Rect(screen.x, screen.y, 50, 100);
        openGL->Outline(rect, 5, openGL->GREEN);
    }
    cnt++;
    openGL->RestoreGL();
}

bool Overlay::WorldToScreen(vec pos, vec& screen, float matrix[16], int windowWidth, int windowHeight) {
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


    if (cnt % 1000 == 0) {
        Log("View Matrix Dump:");
        for (int row = 0; row < 4; ++row) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "[ %.4f %.4f %.4f %.4f ]",
                matrix[0 + row], matrix[4 + row],
                matrix[8 + row], matrix[12 + row]);
            Log(buffer);
        }
    }

    if (cnt % 1000 == 0) {
        std::stringstream logStream;
        // Log the clipCoords values
        logStream << "clipCoords: (" << clipCoords.x << ", " << clipCoords.y << ", " << clipCoords.z << ", " << clipCoords.w << ")";
        Log(logStream.str());
        // Log the NDC values
        logStream.str("");  // Clear the log stream
        logStream << "Normalized Device Coordinates: (" << NDC.x << ", " << NDC.y << ", " << NDC.z << ")";
        Log(logStream.str());
        // Log the screen coordinates
        logStream.str("");  // Clear the log stream
        logStream << "Screen coordinates: (" << screen.x << ", " << screen.y << ")";
        Log(logStream.str());
    }

    return true;
}
