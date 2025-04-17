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

    using tSwapBuffers = BOOL(WINAPI*)(HDC);
private:
    Memory::Trampoline* trampoline;
    OpenGL* openGL;

    HMODULE hOpenGL; // opengl module
    void* target; // targetfunction pointer
    BYTE* gateway; // gateway address of trampoline
    BYTE originalBytes[5];

    bool activated = false;

    // converts in game 3d coordinate to monitor 2d coordinate
    bool WorldToScreen(vec pos, vec& screen, float matrix[16], int windowWidth, int windowHeight);

    static void drawRect(Rect rect, int color);

};


#endif // OVERLAY_H
