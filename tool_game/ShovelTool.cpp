#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "ShovelTool.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int ShovelTool::Price = 50;
ShovelTool::ShovelTool(float x, float y) : Tool("play/tower-base.png", "play/shovel.png", x, y, 200, Price, 0.5) {
	// Move center downward, since we the ShovelTool head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}
