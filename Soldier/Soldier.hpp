#pragma once
#include "Engine/Sprite.hpp"

class Soldier : public Engine::Sprite {
public:
    float moveSpeed;
    Soldier(float x, float y, float moveSpeed = 200.0f, float scale = 0.5f);
    void Update(float deltaTime) override;
    void Move(int dx, int dy, float deltaTime);
};