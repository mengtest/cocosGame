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
};

