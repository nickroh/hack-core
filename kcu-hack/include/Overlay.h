// Overlay.h
#ifndef OVERLAY_H
#define OVERLAY_H

#include <algorithm>
#include <Windows.h>
#include "geometry.h"
#include "Memory.h"
#include "OpenGL.h"
#include "util.h"

const int VIRTUAL_SCREEN_WIDTH = 800;
const int VIRTUAL_SCREEN_HEIGHT = 600;
const int GAME_UNIT_MAGIC = 400;

const float PLAYER_HEIGHT = 5.25f;
const float PLAYER_WIDTH = 2.0f;
const float EYE_HEIGHT = 4.5f;

const float PLAYER_ASPECT_RATIO = PLAYER_HEIGHT / PLAYER_WIDTH;

class Overlay {
public:

    Overlay();   // Constructor
    ~Overlay();  // Destructor
    HMODULE ac_client;

    void testESP();
    void materializeESP();
    bool activateESP(bool enalbe);
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
