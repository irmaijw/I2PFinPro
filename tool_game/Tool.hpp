#ifndef TOOL_HPP
#define TOOL_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;

class Tool: public Engine::Sprite {
protected:
    int price;
    float coolDown;
    float reload = 0;
    float rotateRadian = 2 * ALLEGRO_PI;
    Sprite imgBase;
    std::list<Tool*>::iterator lockedToolIterator;
    PlayScene* getPlayScene();

public:
    bool Enabled = true;
    bool Preview = false;
    Tool(std::string imgBase, std::string imgTool, float x, float y, float radius, int price, float coolDown);
    void Draw() const override;
	int GetPrice() const;
};
#endif // TOOL_HPP
