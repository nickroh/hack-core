// Overlay.cpp
#include "Overlay.h"
#include "constants.h"
#include "OpenGL.h"
#include "Memory.h"
#include "Overlay.h"
#include "geometry.h"
#include "constants.h"
#include "player.h"
#include "AimBot.h"
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
    aimbot = new AimBot();

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
    if (aimbot) {
        delete aimbot;
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

bool Overlay::setAimBot(bool enable, float offset) {
    aimbotActivated = enable;
    scale = offset;
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
    int gameMode = *reinterpret_cast<int*>(base + Offsets::GameMode);

    Player local(localPlayer, myTeam);
    //std::vector<Player> players;

    for (int i = 1; i < playerCount; i++) {

        uintptr_t playerPtr = entityList[i];

        if (!playerPtr) continue;
        DWORD vtable = *reinterpret_cast<DWORD*>(playerPtr);
        if (vtable != 0x54D07C && vtable != 0x54D0A4) continue;

        Player p(playerPtr, myTeam);
        //players.push_back(p);

        vec foot;
        vec head;
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        if (!WorldToScreen(p.getPosition(), foot, matrixPtr, viewport[2], viewport[3]) || !WorldToScreen(p.getHead(), head, matrixPtr, viewport[2], viewport[3])) {
            continue;
        } 

        float scale = (foot.y - head.y) / PLAYER_HEIGHT;
        float width = PLAYER_WIDTH * scale;

        Rect rect = Rect(head.x - width / 2, head.y, width, (foot.y - head.y));
        const GLubyte* color;
        if (!p.isEnemy() && isTeamGame(gameMode)) {
            color = openGL->GREEN;
        }
        else {
            color = openGL->RED;
            if (aimbotActivated && !p.isDead()) {
                aimbot->addPlayer(p);
            }
        }

        if (!p.isDead()) {
            openGL->Outline(rect, 3, color);
        }

    }
    cnt++;
    openGL->RestoreGL();

    if (aimbotActivated) {
        aimbot->calculateBestTarget(local, scale);
        aimbot->clear();
    }
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

    return true;
}
