#include "player.h"
#include "geometry.h"
#include "constants.h"

// Constructor to initialize the player with a pointer and team ID
Player::Player(uintptr_t p, int myTeam) {
    //position = Vec3(*(reinterpret_cast<float*>(p + Offsets::PositionX)),
    //    *(reinterpret_cast<float*>(p + Offsets::PositionY)),
    //    *(reinterpret_cast<float*>(p + Offsets::PositionZ)));

    position = Vec3(
        *(reinterpret_cast<float*>(p + Offsets::PositionX)),
        *(reinterpret_cast<float*>(p + Offsets::PositionY)),
        *(reinterpret_cast<float*>(p + Offsets::PositionZ))
        );
    head = Vec3(
        *(reinterpret_cast<float*>(p + Offsets::HeadX)),
        *(reinterpret_cast<float*>(p + Offsets::HeadY)),
        *(reinterpret_cast<float*>(p + Offsets::HeadZ))
    );
    team = *(reinterpret_cast<int*>(p + Offsets::Team));
    enemy = (myTeam != team);
    dead = *(reinterpret_cast<bool*>(p + Offsets::Dead));
}
vec Player::getPosition() {
    return position;
}
vec Player::getHead() {
    return head;
}
int Player::getTeam() {
    return team;
}

bool Player::isDead() {
    return dead;
}
bool Player::isEnemy() {
    return enemy;
}

