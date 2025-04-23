#include "AimBot.h"
#include "util.h"
#include "constants.h"
#include "geometry.h"
#include "player.h"
#include <vector>
#include <Windows.h>
#include <cstdint>
#include <cmath>
#include <limits>
#include <algorithm>
#include <sstream>
#include <gl\GL.h>

AimBot::AimBot() {
    // Initialization code here (if any)
    ac_client = GetModuleHandle(L"ac_client.exe");
    base = reinterpret_cast<uintptr_t>(ac_client);
    localPlayer = *reinterpret_cast<uintptr_t*>(base + Offsets::LocalPlayer);
    glGetIntegerv(GL_VIEWPORT, viewport);
}

AimBot::~AimBot() {
    // Cleanup code here (if any)
}

// only add enemy no ally
void AimBot::addPlayer(Player p) {
    players.push_back(p);
}

void AimBot::clear() {
    players.clear();
}

void AimBot::calculateBestTarget(Player local, float scale) {
    Player* target = nullptr;
    float bestPriority = FLT_MAX;
    float* matrixPtr = reinterpret_cast<float*>(base + Offsets::ViewMatrix);
    for (Player& p : players) {
        vec screen;
        if (!WorldToScreen(p.getPosition(), screen, matrixPtr, viewport[2], viewport[3])) {continue;}

        float centerX = viewport[2] / 2.0f;
        float centerY = viewport[3] / 2.0f;

        float dist2D = sqrtf((screen.x - centerX) * (screen.x - centerX) + (screen.y - centerY) * (screen.y - centerY));

        float maxFOVPixels = 150.0f;
        if (dist2D > maxFOVPixels) continue;

        float worldDistance = local.getPosition().Distance(p.getPosition());
        float priority = dist2D + worldDistance * 0.5f; // 조준점 + 거리 기반 가중치
        if (priority < bestPriority) {
            bestPriority = priority;
            target = &p;
        }
    }

    if (!target) {
        return;
    }
    else {
        AimAtTarget(CalcAngle(local.getPosition(), target->getPosition()), scale);
    }

}

void AimBot::AimAtTarget(const Vector3& targetAngle, float scale) {
    float smooth = clamp(scale, 0.0f, 100.0f) / 100.0f;

    float x = *(float*)(localPlayer + Offsets::CameraX);
    float y = *(float*)(localPlayer + Offsets::CameraY);

    *(float*)(localPlayer + Offsets::CameraX) = x + (targetAngle.x - x) * smooth;
    *(float*)(localPlayer + Offsets::CameraY) = y + (targetAngle.y - y) * smooth;
}

vec AimBot::CalcAngle(const Vector3& from, const Vector3& to) {
    Vector3 delta = from - to;
    Vector3 angles;
    angles.x = atanf(delta.x / delta.y) * -57.2957795f;
    angles.y = atanf(delta.z / sqrtf(delta.x * delta.x + delta.y * delta.y)) * -57.2957795f;
    angles.z = 0.f;

    if (delta.y < 0.0f) angles.x += 180.0f;
    while (angles.x < 0.f) angles.x += 360.f;
    while (angles.x >= 360.f) angles.x -= 360.f;
    angles.y = clamp(angles.y, -90.0f, 90.0f);

    return angles;
}

