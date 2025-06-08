#include "Soldier.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"

Soldier::Soldier(float x, float y, float moveSpeed, float scale)
    : Engine::Sprite("soldier-1.png", x, y, 0, 0, scale, scale), moveSpeed(moveSpeed) {}

void Soldier::Update(float deltaTime) {
    Engine::Sprite::Update(deltaTime);
}

void Soldier::Move(int dx, int dy, float deltaTime) {
    Position.x += dx * moveSpeed * deltaTime;
    Position.y += dy * moveSpeed * deltaTime;
}