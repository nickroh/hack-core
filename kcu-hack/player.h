#pragma once
#include "geometry.h"

class Player {
public:
    Player(uintptr_t p, int myTeam);

    vec getHead();
    vec getPosition();
    int getTeam();
    
    bool isDead();
    bool isEnemy();

private:
    vec head;
    vec position;
    int team;
    int enemy;
    bool dead;
};
