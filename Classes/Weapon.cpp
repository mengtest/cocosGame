#include "Weapon.h"


Weapon::Weapon() : _ammoFrames(NULL)
{
	_ammoFrames = __Array::create();
	_ammoFrames->retain();
}

Weapon::~Weapon()
{
	if (_ammoFrames)
	{
		_ammoFrames->release();
		_ammoFrames = NULL;
	}
}

void Weapon::makeLazerPistol() {
	this->ammo = 0;
	this->shootMode = 0;
	this->maxBulletsNumber = 6;
	this->currentBulletsNumber = maxBulletsNumber;
	this->reloadTime = 1.4f;
	this->blockShootingTime = 0.13f;
	this->reloadBarOffsetX = 102;
	this->reloadBarOffsetY = 57;
	this->gunOffsetX = 102;
	this->gunOffsetY = 2;
	this->gunFireOffsetX = 90;
	this->gunFireOffsetY = 10;
	this->bulletOffsetX = 35;
	this->bulletOffsetY = 15;
	this->damage = 50;
	this->frameWidth = 130;
	this->frameHeight = 100;
	this->gunFireRemoveDelay = 0.08f;
	this->bulletUri = new String("images/bullet.png");
	this->gunFireUri = new String("images/player/gun/fire.png");
	this->_ammoFrames->addObject(new String("images/player/gun/gun.png"));
	this->_ammoFrames->addObject(new String("images/player/gun/gun_2.png"));
}

void Weapon::makeLazerGun() {
	this->damage = 40;
	this->ammo = 1;
	this->maxBulletsNumber = 24;
	this->currentBulletsNumber = maxBulletsNumber;
	this->reloadTime = 2.7f;
	this->shootMode = 1;
	this->blockShootingTime = 0.12f;
	this->reloadBarOffsetX = 102;
	this->reloadBarOffsetY = 72;
	this->gunOffsetX = 102;
	this->gunOffsetY = 2;
	this->gunFireOffsetX = 110;
	this->gunFireOffsetY = 10;
	this->bulletOffsetX = 55;
	this->bulletOffsetY = 10;
	this->frameWidth = 130;
	this->frameHeight = 100;
	this->gunFireRemoveDelay = 0.05f;
	this->bulletUri = new String("images/player/gun/bullet_3.png");
	this->gunFireUri = new String("images/player/gun/fire_2.png");
	this->_ammoFrames->addObject(new String("images/player/gun/gun_3.png"));
	this->_ammoFrames->addObject(new String("images/player/gun/gun_4.png"));
}
