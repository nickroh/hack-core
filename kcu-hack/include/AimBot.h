#ifndef AIMBOT_H
#define AIMBOT_H

#include <Windows.h>
#include <vector>
#include "player.h"
#include "Memory.h"
#include "geometry.h"

class AimBot {
public:
    AimBot();   // Constructor
    ~AimBot();  // Destructor

    void addPlayer(Player p);
    void clear();
    void calculateBestTarget(Player local, float scale);


private:
    template <typename T>
    T clamp(T value, T min, T max) {
        if (value < min) {
            return min;
        }
        else if (value > max) {
            return max;
        }
        else {
            return value;
        }
    }

    HMODULE ac_client;
    uintptr_t base;
    uintptr_t localPlayer;

    int viewport[4];

    vec CalcAngle(const Vector3& from, const Vector3& to);
    void AimAtTarget(const Vector3& targetAngle, float scale);

    int multiplier;
    bool activated;
    std::vector<Player> players;
};

#endif
