#pragma once
#include "cocos2d.h"

using namespace cocos2d;
class Weapon : public Ref
{

public:

	__Array *_ammoFrames;

	String* bulletUri;
	String* gunFireUri;

	float gunFireRemoveDelay = 0;
	float ammoAnimSpeed = 0.28f;

	int reloadBarOffsetX = 0;
	int reloadBarOffsetY = 0;
	int currentBulletsNumber = 0;
	int maxBulletsNumber = 0;
	int damage = 0;
	int ammo = 0;
	int shootMode = 0;
	int gunOffsetX = 102;
	int gunOffsetY = 2;
	int gunFireOffsetX = 110;
	int gunFireOffsetY = 10;
	int bulletOffsetX = 35;
	int bulletOffsetY = 15;

	int frameWidth = 0;
	int frameHeight = 0;

	//int currentBulletsNumber = 0;
	//int maxBulletsNumber = 0;

	float reloadTime = 2.0f;
	float blockShootingTime = 0;

	void makeLazerPistol();
	void makeLazerGun();

	Weapon();
	~Weapon();
};

