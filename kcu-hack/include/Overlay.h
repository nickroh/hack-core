// Overlay.h
#ifndef OVERLAY_H
#define OVERLAY_H

#include <algorithm>
#include "geometry.h"

class Overlay {
public:

    Overlay();   // Constructor
    ~Overlay();  // Destructor

    bool EnableBoxOverlay();

private:

    // converts in game 3d coordinate to monitor 2d coordinate
    bool WorldToScreen(vec pos, vec& screen, float matrix[16], int windowWidth, int windowHeight);

};

#endif // OVERLAY_H
