#include "PurpleSoldier.hpp"

PurpleSoldier::PurpleSoldier(float x, float y, float moveSpeed)
    : Soldier(x, y, moveSpeed, 0.5f) { // 0.2f is 5 times smaller than 1.0f
}