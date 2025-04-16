// Overlay.h
#ifndef OVERLAY_H
#define OVERLAY_H

#include <algorithm>
#include <Windows.h>
#include "geometry.h"
#include "Memory.h"
#include "OpenGL.h"
#include "util.h"

class Overlay {
public:

    Overlay();   // Constructor
    ~Overlay();  // Destructor

    bool SetBoxOverlay(bool enable);

    static OpenGL openGL;
private:
    Memory::Trampoline* trampoline;

    HMODULE hOpenGL; // opengl module
    void* target; // targetfunction pointer
    BYTE* gateway; // gateway address of trampoline
    BYTE originalBytes[5];

    bool activated = false;

    // converts in game 3d coordinate to monitor 2d coordinate
    bool WorldToScreen(vec pos, vec& screen, float matrix[16], int windowWidth, int windowHeight);

    using tSwapBuffers = BOOL(WINAPI*)(HDC);
    static tSwapBuffers oSwapBuffers;

    static void drawRect(Rect rect, int color);
    static BOOL WINAPI hkSwapBuffers(HDC hdc) {
        OpenGL openGL;
        Rect rect = Rect(100, 100, 20, 20);
        openGL.SetupOrtho();
        openGL.DrawRect(rect, openGL.GREEN);
        openGL.RestoreGL();
        return oSwapBuffers(hdc);
    }
};


#endif // OVERLAY_H
