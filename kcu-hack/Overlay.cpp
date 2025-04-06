// Overlay.cpp
#include "Overlay.h"
#include "geometry.h"
#include <iostream>
#include <algorithm>
#include <windows.h>



Overlay::Overlay() {
    std::cout << "Overlay created." << std::endl;
}

Overlay::~Overlay() {
    std::cout << "Overlay destroyed." << std::endl;
}

bool Overlay::WorldToScreen(vec pos, vec& screen, float matrix[16], int windowWidth, int windowHeight) {
    Vec4 clipCoords;

    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.w = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];

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
