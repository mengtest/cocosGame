#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

GameScene::GameScene() : _targets(NULL), _projectiles(NULL), _deadTargets(NULL) {}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

GameScene::~GameScene()
{
	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}

	if (_deadTargets)
	{
		_deadTargets->release();
		_deadTargets = NULL;
	}

	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}
}

bool GameScene::init()
{
	if (!LayerColor::initWithColor(Color4B(157, 181, 173, 255))){
		return false;
	}
	initArrays();
	initMenuButton();
	this->schedule(schedule_selector(GameScene::gameLogic), spawnEnemyFrequency);
	this->schedule(schedule_selector(GameScene::moveEnemies), enemySpeed);
	this->schedule(schedule_selector(GameScene::moveBullets), bulletSpeed);
	this->schedule(schedule_selector(GameScene::update));
	this->setTouchEnabled(true);
	this->addTouchListener();
	initMap();
	createPlayer();
	createGun();
	addKeyboardEventListener();
	addAccelarationEventListener();

	//acceleration = Label::createWithTTF("acceleration", "fonts/Marker Felt.ttf", 24);
	//char buffer1[100];
	//sprintf(buffer1, "player.x:%f\nplayer.y:%f", player->getPosition().x, player->getPosition().y);
	//acceleration->setString(buffer1);
	//acceleration->setPosition(Vec2(acceleration->getContentSize().width, 2 * acceleration->getContentSize().height));
	//this->addChild(acceleration, 1, "acceleration");

	return true;
}

void GameScene::initMap() {
	cocos2d::Sprite* back = Sprite::create("images/map/map.png");
	back->setAnchorPoint(Vec2(0, 0));
	back->setPosition(Vec2(0, 0));
	this->addChild(back, 0, "back");

	cocos2d::Sprite* light = Sprite::create("images/map/light_2.png");
	light->setAnchorPoint(Vec2(0, 0));
	light->setPosition(Vec2(0, 0));
	this->addChild(light, 1, "light");

	cocos2d::Sprite* shadow = Sprite::create("images/map/shadow.png");
	shadow->setAnchorPoint(Vec2(0, 0));
	shadow->setPosition(Vec2(0, 0));
	this->addChild(shadow, 10, "shadow");
}

void GameScene::createPlayer() {

	Size winSize = CCDirector::sharedDirector()->getWinSize();
	player = Sprite::create("images/player/idle/0001.png");
	player->setScale(1);
	int posX = (player->getContentSize().width * 2) / pixelSize;
	posX *= pixelSize;
	int posY = (winSize.height / 2) / pixelSize;
	posY *= pixelSize;
	player->setPosition(Vec2(posX + 2, posY + 2));


	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(2);

	animFrames.pushBack(SpriteFrame::create("images/player/idle/0001.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/player/idle/0002.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/player/idle/0003.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/player/idle/0004.png", Rect(0, 0, 125, 125)));

	// create the animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.14f);
	Animate* animate = Animate::create(animation);

	// run it and repeat it forever
	player->runAction(RepeatForever::create(animate));

	std::string mapKey0 = "player";
	playerMap.insert(mapKey0, player);

	Sprite* hero_shadow = Sprite::create("images/player/hero_shadow.png");
	hero_shadow->setPosition(Vec2(playerMap.at("player")->getPosition().x, playerMap.at("player")->getPosition().y));
	playerMap.insert("hero_shadow", hero_shadow);

	this->addChild(playerMap.at("hero_shadow"), 2, "hero_shadow");
	this->addChild(playerMap.at("player"), 3, "player");
}

void GameScene::createGun() {

	Size winSize = CCDirector::sharedDirector()->getWinSize();
	Sprite* gun = Sprite::create("images/player/gun/gun.png");
	gun->setScale(1);
	gun->setPosition(Vec2(playerMap.at("player")->getPosition().x + gunOffsetX, playerMap.at("player")->getPosition().y + gunOffsetY));


	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(2);

	animFrames.pushBack(SpriteFrame::create("images/player/gun/gun.png", Rect(0, 0, 130, 100)));
	animFrames.pushBack(SpriteFrame::create("images/player/gun/gun_2.png", Rect(0, 0, 130, 100)));

	// create the animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.28f);
	Animate* animate = Animate::create(animation);

	// run it and repeat it forever
	gun->runAction(RepeatForever::create(animate));
	std::string mapKey0 = "gun";
	playerMap.insert(mapKey0, gun);

	this->addChild(playerMap.at("gun"), 3, "gun");
}

void GameScene::initMenuButton() {

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menuButton = MenuItemImage::create(
		"images/ui/menu_button.png",
		"images/ui/menu_button.png",
		CC_CALLBACK_1(GameScene::menuOpenMenuCallback, this));
	float menuButtonScale = 1;
	menuButton->setScale(menuButtonScale);
	menuButton->setPosition(Vec2(origin.x + visibleSize.width - menuButtonScale * menuButton->getContentSize().width / 2,
		origin.y + visibleSize.height - menuButtonScale * menuButton->getContentSize().height / 2));

	auto menu = Menu::create(menuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 40, "menuButton");

}

void GameScene::initArrays() {
	_targets = CCArray::create();
	_targets->retain();
	_deadTargets = CCArray::create();
	_deadTargets->retain();
	_projectiles = CCArray::create();
	_projectiles->retain();
}

void GameScene::addKeyboardEventListener() {

	auto eventListener = EventListenerKeyboard::create();

	//int newPosX;
	//int newPosY;

	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_BACKSPACE:
			Director::getInstance()->end();
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			openMenu();
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			shoot();
			break;
		case EventKeyboard::KeyCode::KEY_A:
			aPressed = true;
			dPressed = false;
			break;
		case EventKeyboard::KeyCode::KEY_D:
			dPressed = true;
			aPressed = false;
			break;
		case EventKeyboard::KeyCode::KEY_W:
			wPressed = true;
			sPressed = false;
			break;
		case EventKeyboard::KeyCode::KEY_S:
			sPressed = true;
			wPressed = false;
			break;
			//case EventKeyboard::KeyCode::KEY_A:
			//	newPosX = player->getPosition().x - pixelSize;
			//	player->setPositionX(newPosX);
			//	CCLOG("player->getPosition().x = %f", player->getPosition().x);
			//	CCLOG("player->getPosition().y = %f", player->getPosition().y);
			//	break;
			//case EventKeyboard::KeyCode::KEY_D:
			//	newPosX = player->getPosition().x + pixelSize;
			//	player->setPositionX(newPosX);
			//	CCLOG("player->getPosition().x = %f", player->getPosition().x);
			//	CCLOG("player->getPosition().y = %f", player->getPosition().y);
			//	break;
			//case EventKeyboard::KeyCode::KEY_W:
			//	newPosY = player->getPosition().y + pixelSize;
			//	player->setPositionY(newPosY);
			//	CCLOG("player->getPosition().x = %f", player->getPosition().x);
			//	CCLOG("player->getPosition().y = %f", player->getPosition().y);
			//	break;
			//case EventKeyboard::KeyCode::KEY_S:
			//	newPosY = player->getPosition().y - pixelSize;
			//	player->setPositionY(newPosY);
			//	CCLOG("player->getPosition().x = %f", player->getPosition().x);
			//	CCLOG("player->getPosition().y = %f", player->getPosition().y);
			//	break;
		}
	};

	eventListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_A:
			aPressed = false;
			break;
		case EventKeyboard::KeyCode::KEY_D:
			dPressed = false;
			break;
		case EventKeyboard::KeyCode::KEY_W:
			wPressed = false;
			break;
		case EventKeyboard::KeyCode::KEY_S:
			sPressed = false;
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, player);
}

void GameScene::addAccelarationEventListener() {

	Device::setAccelerometerEnabled(true);
	auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void GameScene::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){

	Director *director = Director::getInstance();
	auto viewSize = director->getWinSize();
	float minSize = MIN(viewSize.width, viewSize.height);
	if (!movementStarted) {
		startYOffset = acc->y;
		startXOffset = acc->x;
	}
	double targetSpeedY = (acc->y - startYOffset) * minSize * 2;
	double targetSpeedX = (acc->x - startXOffset) * minSize * 2;
	eaterPointsPerSecY = (eaterPointsPerSecY * .8f) + (targetSpeedY * .2f);
	eaterPointsPerSecX = (eaterPointsPerSecX * .8f) + (targetSpeedX * .2f);

	movementStarted = true;
}

void GameScene::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){

	for (auto& touch : touches) {
		if (touch) {
			shoot();
		}
	}
}

void GameScene::shoot() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::create("images/bullet.png");
	int posX = playerMap.at("gun")->getPositionX() + bulletOffsetX;
	posX /= pixelSize;
	posX *= pixelSize;
	int posY = playerMap.at("gun")->getPositionY() + bulletOffsetY;
	posY /= pixelSize;
	posY *= pixelSize;
	projectile->setPosition(ccp(posX, posY));
	projectile->setScale(1);
	projectile->setTag(2);
	_projectiles->addObject(projectile);

	this->addChild(projectile, 2);

	auto gunFire = CCSprite::create("images/player/gun/fire.png");
	gunFire->setPosition(playerMap.at("gun")->getPosition().x + gunFireOffsetX, playerMap.at("gun")->getPosition().y + gunFireOffsetY);
	this->addChild(gunFire, 2, "gunFire");
	auto action = Sequence::create(DelayTime::create(0.07), CallFunc::create(std::bind(&GameScene::removeCorpse, this, gunFire)), NULL);
	this->runAction(action);

	//int realX = winSize.width + (projectile->getContentSize().width / 2);
	//float ratio = (float)offset.y / (float)offset.x;
	//int realY = projectile->getPosition().y;
	//CCPoint realDest = ccp(realX, realY);

	//int offRealX = realX - projectile->getPosition().x;
	//int offRealY = realY - projectile->getPosition().y;
	//float length = sqrtf((offRealX*offRealX) + (offRealY*offRealY));
	//float velocity = 600 / 1; // 600 пикселей / 1 секунда
	//float realMoveDuration = length / velocity;

	//auto actionMove = MoveTo::create(realMoveDuration, realDest);
	//projectile->runAction(actionMove);
}

void GameScene::addMonster(){

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *monster = CCSprite::create("images/mobs/demon.png");
	CCSprite *monster_shadow = CCSprite::create("images/mobs/demon_shadow.png");

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(2);

	animFrames.pushBack(SpriteFrame::create("images/mobs/walk/1.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/walk/2.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/walk/3.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/walk/4.png", Rect(0, 0, 125, 125)));

	// create the animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.28f);
	Animate* animate = Animate::create(animation);

	// run it and repeat it forever
	monster -> runAction(RepeatForever::create(animate));
	
	monster->setScale(1);

	//CCSprite* monster = CCSprite::create("player.png");

	// определим появление монстра вдоль оси Y

	int monsterHeight = monster->getContentSize().height;
	int monsterWidth = monster->getContentSize().width;
	int minY = monsterHeight / 2;
	int maxY = winSize.height - monsterHeight / 2;
	int rangeY = maxY - minY;
	int actualY = (rand() % rangeY) + minY;
	actualY /= pixelSize;
	actualY *= pixelSize;
	actualY += 2;
	int actualX = winSize.width + monsterWidth / 2;
	actualX /= pixelSize;
	actualX *= pixelSize;
	actualX += 2;
	//CCLOG("%i", actualY);

	// создаем монстров немного за экраном по правому краю и в случайной позиции Y,
	// рассчитанной выше
	monster->setPosition(Vec2(winSize.width + monsterWidth / 2, actualY));
	monster_shadow->setPosition(Vec2(monster->getPositionX(), monster->getPositionY()));

	monster->setTag(1);

	Monster *monsterObj = new Monster();
	monsterObj->spritesMap.insert("monster", monster);
	monsterObj->spritesMap.insert("monster_shadow", monster_shadow);
	_targets->addObject(monsterObj);

	this->addChild(monster_shadow, 1);
	this->addChild(monster, 1);
	

	// считаем скорость монстров
	int minDuration = 12;
	int maxDuration = 14;
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() % rangeDuration) + minDuration;

	//auto actionMove = MoveTo::create((float)actualDuration,	ccp(0 - monsterWidth / 2, actualY));
	//auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveFinished));
	//// создаем действия
	//CCFiniteTimeAction* actionMove = CCMoveTo::actionWithDuration((float)actualDuration, ccp(0 - monsterWidth / 2, actualY));
	//CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget(this, callfuncN_selector(GameScene::spriteMoveFinished));

	//monster->runAction(actionMove);

	//CCFiniteTimeAction* actionMove =
	//	CCMoveTo::create((float)actualDuration,
	//	ccp(0 - monster->getContentSize().width / 2, actualY));
	//CCFiniteTimeAction* actionMoveDone =
	//	CCCallFuncN::create(this,
	//	CC_CALLFUNCN_SELECTOR(GameScene::spriteMoveFinished));
	//monster->runAction(CCSequence::create(actionMove,
	//	actionMoveDone, NULL));
}

void GameScene::gameLogic(float dt){
	addMonster();
}

void GameScene::moveEnemies(float dt){
	CCObject* jt = NULL;
	CCARRAY_FOREACH(_targets, jt)
	{
		Monster *currentMonster = dynamic_cast<Monster*>(jt);
		
		for ( auto key : currentMonster->spritesMap.keys() ) {
			CCSprite *target = currentMonster->spritesMap.at(key);
			target->setPositionX(target->getPosition().x - pixelSize);
		}
	}
}

void GameScene::moveBullets(float dt){
	CCObject* jt = NULL;
	CCARRAY_FOREACH(_projectiles, jt)
	{
		CCSprite *target = dynamic_cast<CCSprite*>(jt);
		target->setPositionX(target->getPosition().x + pixelSize);
	}
}

void GameScene::update(float dt)
{
	checkTouch();
	int newX = 0;
	int newY = 0;
	//char buffer1[100];
	//sprintf(buffer1, "player.x:%f\nplayer.y:%f", newX, newY);
	//acceleration->setString(buffer1);
	if (movementStarted) {
		float diffY = (eaterPointsPerSecY * dt)*1.3f;
		float diffX = (eaterPointsPerSecX * dt)*1.3f;
		int intDiffX = 0;
		int intDiffY = 0;
		if (diffX < 0) {
			intDiffX = int((diffX - 4) / pixelSize);
		}
		else {
			intDiffX = int((diffX + 4) / pixelSize);
		}
		intDiffX *= pixelSize;
		if (diffY < 0) {
			intDiffY = int((diffY - 4) / pixelSize);
		}
		else {
			intDiffY = int((diffY + 4) / pixelSize);
		}
		intDiffY *= pixelSize;
		newX = player->getPosition().x + intDiffX;
		newY = player->getPosition().y + intDiffY;
	}
	if (wPressed) {
		newY = player->getPosition().y + pixelSize;
	}
	if (sPressed) {
		newY = player->getPosition().y - pixelSize;
	}
	if (dPressed) {
		newX = player->getPosition().x + pixelSize;
	}
	if (aPressed) {
		newX = player->getPosition().x - pixelSize;
	}
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if (newY < player->getContentSize().height / 2 || newY > winSize.height - player->getContentSize().height / 2) {
		newY = player->getPosition().y;
	}
	if (newX < player->getContentSize().width / 2 || newX > winSize.width - player->getContentSize().width / 2) {
		newX = player->getPosition().x;
	}
	//player->setPosition(newX, newY);
	playerMap.at("player")->setPosition(newX, newY);
	playerMap.at("gun")->setPosition(newX + gunOffsetX, newY + gunOffsetY);
	playerMap.at("hero_shadow")->setPosition(newX, newY);
	if (this->getChildByName("gunFire")) {
		this->getChildByName("gunFire")->setPosition(playerMap.at("gun")->getPosition().x + gunFireOffsetX, playerMap.at("gun")->getPosition().y + gunFireOffsetY);
	}
	CCArray *projectilesToDelete = CCArray::create();
	projectilesToDelete->retain();
	CCArray* targetsToDelete = CCArray::create();
	targetsToDelete->retain();
	CCObject* it = NULL;
	CCObject* jt = NULL;

	CCARRAY_FOREACH(_projectiles, it)
	{
		CCSprite *projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
			projectile->getPosition().x - (projectile->getContentSize().width / 2),
			projectile->getPosition().y - (projectile->getContentSize().height / 2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);


		CCARRAY_FOREACH(_targets, jt)
		{
			Monster *currentMonster = dynamic_cast<Monster*>(jt);
			CCSprite *target = currentMonster->spritesMap.at("monster");
			CCRect targetRect = CCRectMake(
				target->getPosition().x - (target->getContentSize().width / 2) + 100,
				target->getPosition().y - (target->getContentSize().height / 2) + 50,
				target->getContentSize().width - 100,
				target->getContentSize().height - 100);

			if (projectileRect.intersectsRect(targetRect))
			{
				targetsToDelete->addObject(currentMonster);
				projectilesToDelete->addObject(projectile);
			}
		}
	}


	CCARRAY_FOREACH(targetsToDelete, jt)
	{
		Monster *currentMonster = dynamic_cast<Monster*>(jt);
		currentMonster->hitsToShot--;
		CCSprite *target = currentMonster->spritesMap.at("monster");
		if (currentMonster->hitsToShot < 1) {
			CCSprite *monster_shadow = currentMonster->spritesMap.at("monster_shadow");
			//CCArray* targets = CCArray::create();

			for (auto key : currentMonster->spritesMap.keys()) {
				auto action = Sequence::create(DelayTime::create(2), CallFunc::create(std::bind(&GameScene::removeCorpse, this, currentMonster->spritesMap.at(key))), NULL);
				this->runAction(action);
			}

			//_deadTargets->addObject(target);
			_targets->removeObject(currentMonster);
			//this->removeChild(target, true);
			int temp = cocos2d::RandomHelper::random_int(0, 1);
			if (temp) {
				target->setTexture("images/mobs/demon_dead.png");
			}
			else {
				target->setTexture("images/mobs/demon_dead_2.png");
			}

			target->stopAllActions();


			//sprite1pos = CGPoint.ccp((winSize.width / 2 + winSize.width / 2), 0);

			//now the bezier config declaration


			auto jump = JumpBy::create(0.5, Vec2(20, 0), 30, 1);

			auto seq = Sequence::create(jump, nullptr);


			CCFiniteTimeAction* actionMove1 =
				CCMoveTo::create((float)1,
				ccp(target->getPosition().x + 30, target->getPosition().y));
			CCFiniteTimeAction* actionMove2 =
				CCMoveTo::create((float)1,
				ccp(target->getPosition().x + 30, target->getPosition().y));
			CCFiniteTimeAction* actionMove3 =
				CCMoveTo::create((float)0.5,
				ccp(target->getPosition().x + 30, target->getPosition().y));
			if (temp) {
				target->runAction(seq);
				monster_shadow->runAction(actionMove3);
			}
			else {
				target->runAction(actionMove1);
				monster_shadow->runAction(actionMove2);
			}

			auto fadeOutAction1 = FadeOut::create(1.0f);
			auto fadeOutAction2 = FadeOut::create(1.0f);
			/*auto fadeOut = Sequence::create(DelayTime::create(1), CallFunc::create(std::bind(&FadeOut::create, this, 1.0f)), NULL);*/
			auto fadeOut1 = Sequence::create(DelayTime::create(1), fadeOutAction1, NULL);
			auto fadeOut2 = Sequence::create(DelayTime::create(1), fadeOutAction2, NULL);
			////auto fadeOut = FadeOut::create(2.0f);

			target->runAction(fadeOut1);
			monster_shadow->runAction(fadeOut2);
		}
		else {
			//target->setTexture("images/mobs/demon_dead.png");
			target->setColor(ccc3(0,0,0));
			auto action = Sequence::create(DelayTime::create(0.1), CallFunc::create(std::bind(&GameScene::restoreEnemy, this, target)), NULL);
			this->runAction(action);
		}
	}

	CCARRAY_FOREACH(projectilesToDelete, it)
	{
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}

	projectilesToDelete->release();
	targetsToDelete->release();


	CCARRAY_FOREACH(_targets, it) {
		Monster *currentMonster = dynamic_cast<Monster*>(it);
		CCSprite *target = currentMonster->spritesMap.at("monster");
		if (currentMonster->spritesMap.at("monster")->getPositionX() < 0) {
			CCSprite *monster_shadow = currentMonster->spritesMap.at("monster_shadow");
			//CCArray* targets = CCArray::create();

			for (auto key : currentMonster->spritesMap.keys()) {
				removeCorpse(currentMonster->spritesMap.at(key));
			}

			_targets->removeObject(currentMonster);
		}
	}
	CCARRAY_FOREACH(_projectiles, it) {
		if (it == NULL) {
			CCLOG("1");
		}
		else {
			Sprite *bullet = dynamic_cast<CCSprite*>(it);
			if (bullet->getPositionX() > winSize.width + bullet->getContentSize().width) {
				_projectiles->removeObject(bullet);
				this->removeChild(bullet);
			}
		}
	}
}

void GameScene::restoreEnemy(Sprite* target) {
	target->setColor(ccc3(255, 255, 255));
}

void GameScene::removeCorpse(Sprite* target) {
	this->removeChild(target, true);
}

void GameScene::spriteMoveFinished(CCNode* sender)
{

}

void GameScene::menuOpenMenuCallback(Ref* pSender) {
	openMenu();
}

void GameScene::checkTouch() {
	if (!this->isTouchEnabled()) {
		this->setTouchEnabled(true);
		this->_eventDispatcher->resumeEventListenersForTarget(this->getChildByName("menuButton"));
		this->_eventDispatcher->resumeEventListenersForTarget(this->getChildByName("player"));
		this->setKeyboardEnabled(true);
		removeChildByTag(PAUSE_SCENE_TAG);
	}
}

void GameScene::openMenu() {
	Director::getInstance()->pause();
	this->setTouchEnabled(false);
	this->setKeyboardEnabled(false);
	this->_eventDispatcher->pauseEventListenersForTarget(this->getChildByName("menuButton"));
	this->_eventDispatcher->pauseEventListenersForTarget(this->getChildByName("player"));
	auto scene = PauseScene::createScene();
	scene->setTag(PAUSE_SCENE_TAG);
	addChild(scene, 50);
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}
