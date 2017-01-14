#pragma once
#include "cocos2d.h"

using namespace cocos2d;
class Monster : public Ref
{
public:
	Monster();
	~Monster();

	Map<std::string, Sprite*> spritesMap;

	int hitsToShot = 3;
	int hitBoxXOffset = 100;
	int hitBoxYOffset = 50;

	std::string getDeadTexture();
};

