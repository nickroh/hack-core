#pragma once
#include "geometry.h"

class Player {
public:
    Player(uintptr_t p, int myTeam);

    vec getPosition();
    int getTeam();
    
    bool isDead();
    bool isEnemy();

    void setPosition(const vec& newPos);
    void setTeam(int newTeam);
    void setDead(bool status);

private:
    vec position;
    int team;
    int enemy;
    bool dead;
};
