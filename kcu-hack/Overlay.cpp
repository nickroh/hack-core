// Overlay.cpp
#include "Overlay.h"
#include "OpenGL.h"
#include "Memory.h"
#include "Overlay.h"
#include "geometry.h"
#include <iostream>
#include <algorithm>
#include <windows.h>
#include "util.h"

Overlay::tSwapBuffers oSwapBuffers = nullptr;

BOOL WINAPI hkSwapBuffers(HDC hdc) {
    OpenGL openGL;
    Rect rect = Rect(100, 100, 20, 20);
    openGL.SetupOrtho();
    openGL.DrawRect(rect, openGL.GREEN);
    openGL.RestoreGL();
    return oSwapBuffers(hdc);
}

Overlay::Overlay() {
    std::cout << "Overlay created." << std::endl;
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
    Log("at least overlay inited");
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


bool Overlay::SetBoxOverlay(bool enable) {
    if (enable && !activated) {
        Log("entered");

        //target = reinterpret_cast<void*>(oSwapBuffers);// Save hook target for unhooking
        BYTE* dst = reinterpret_cast<BYTE*>(&hkSwapBuffers);


        // Backup original bytes before hooking
        //memcpy(originalBytes, src, 5);

        Log("initing Trampoline");
        // Create trampoline and store original function pointer
        oSwapBuffers = reinterpret_cast<tSwapBuffers>(
            trampoline->CreateTrampoline((BYTE*)target, dst, 5)
            );

        Log("Trampoline created");
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
