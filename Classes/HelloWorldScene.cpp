#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

HelloWorld::~HelloWorld()
{
	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}

	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}
}

HelloWorld::HelloWorld(): _targets(NULL), _projectiles(NULL) {}

bool HelloWorld::init()
{
	_targets = CCArray::create();
	_targets->retain();
	_projectiles = CCArray::create();
	_projectiles->retain();

	if (!LayerColor::initWithColor(Color4B(157, 181, 173, 255))){
		return false;
	}
    




	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"images/ui/menu.png",
		"images/ui/menu.png",
		CC_CALLBACK_1(HelloWorld::menuOpenMenuCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + visibleSize.height - closeItem->getContentSize().height / 2));

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	this->schedule(schedule_selector(HelloWorld::gameLogic), spawnEnemyFrequency);

	this->setTouchEnabled(true);
	this->addTouchListener();
	this->schedule(schedule_selector(HelloWorld::update));

	Device::setAccelerometerEnabled(true);
	auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	label = Label::createWithTTF("Hello, Glenda!", "fonts/Marker Felt.ttf", 24);
	label->setColor(Color3B(0, 0, 0));

	label->setPosition(Vec2(visibleSize.width / 2, label->getContentSize().height / 2));
	//this->addChild(label, 2);

	double temp = 145;
	double temp2 = 56;
	double temp3 = 56;
	char buffer1[100];
	sprintf(buffer1, "x:%f\ny:%f\nz:%f\nplayer.x:%f\nplayer.y:%f", temp, temp2, temp3, player->getPosition().x, player->getPosition().y);
	acceleration->setString(buffer1);
	acceleration->setPosition(Vec2(acceleration->getContentSize().width, visibleSize.height - acceleration->getContentSize().height));

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
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





	//HelloWorld::acceleration = Label::createWithTTF("acceleration", "fonts/Marker Felt.ttf", 24);
	//acceleration -> setColor(Color3B(0, 0, 0));

	//this->addChild(acceleration, 1);
    
	Size winSize = CCDirector::sharedDirector()->getWinSize();

	player = Sprite::create("images/player/1.png");
	player->setScale(1);
	player->setPosition(Vec2(0 , winSize.height / 2));
	this->addChild(player, 0, "player");

	//2 * player->getContentSize().width

	//CCFiniteTimeAction* actionMove =
	//	CCMoveTo::create((float)2,
	//	ccp(2 * player->getContentSize().width, player->getPosition().y));
	//CCFiniteTimeAction* actionMoveDone =
	//	CCCallFuncN::create(this,
	//	CC_CALLFUNCN_SELECTOR(HelloWorld::realInit));
	//player->runAction(CCSequence::create(actionMove,
	//	actionMoveDone, NULL));

    return true;
}

//void HelloWorld::realInit(CCNode* sender) {
//
//
//}

void HelloWorld::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){

	//char buffer[100];
	//sprintf(buffer, "x:%f\ny:%f\nz:%f\nplayer.x:%f\nplayer.y:%f", acc->x, acc->y, acc->z, player->getPosition().x, player->getPosition().y);
	//acceleration->setString(buffer);
	Director *director = Director::getInstance();
	auto viewSize = director->getWinSize();
	float minSize = MIN(viewSize.width, viewSize.height);
	if (!movementStarted) {
		startYOffset = acc->y;
		startXOffset = acc->x;
		//char buffer1[100];
		//sprintf(buffer1, "startYOffset:%f", startYOffset);
		//label->setString(buffer1);
	}
	double targetSpeedY = (acc->y - startYOffset) * minSize * 2;
	double targetSpeedX = (acc->x - startXOffset) * minSize * 2;
	eaterPointsPerSecY = (eaterPointsPerSecY * .8f) + (targetSpeedY * .2f);
	eaterPointsPerSecX = (eaterPointsPerSecX * .8f) + (targetSpeedX * .2f);

	movementStarted = true;
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){

	for (auto& touch : touches) {
		if (touch) {
			shoot();
		}
	}
}

void HelloWorld::shoot() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::create("images/bullet.png");
	projectile->setPosition(ccp(player->getPositionX() + player->getContentSize().width - projectile->getContentSize().width * 2, player->getPositionY() - projectile->getContentSize().height / 2));
	projectile->setScale(1);
	projectile->setTag(2);
	_projectiles->addObject(projectile);

	this->addChild(projectile);

	int realX = winSize.width + (projectile->getContentSize().width / 2);
	//float ratio = (float)offset.y / (float)offset.x;
	int realY = projectile->getPosition().y;
	CCPoint realDest = ccp(realX, realY);

	int offRealX = realX - projectile->getPosition().x;
	int offRealY = realY - projectile->getPosition().y;
	float length = sqrtf((offRealX*offRealX) + (offRealY*offRealY));
	float velocity = 600 / 1; // 600 пикселей / 1 секунда
	float realMoveDuration = length / velocity;

	auto actionMove = MoveTo::create(realMoveDuration, realDest);
	projectile->runAction(actionMove);
}

void HelloWorld::addMonster(){

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *monster = CCSprite::create("images/mobs/worm1.png");

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(2);

	animFrames.pushBack(SpriteFrame::create("images/mobs/worm1.png", Rect(0, 0, 54, 50)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/worm2.png", Rect(0, 0, 54, 50)));

	// create the animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	Animate* animate = Animate::create(animation);

	// run it and repeat it forever
	monster -> runAction(RepeatForever::create(animate));

	monster -> setScale(1);

	//CCSprite* monster = CCSprite::create("player.png");

	// определим появление монстра вдоль оси Y

	int monsterHeight = monster->getContentSize().height;
	int monsterWidth = monster->getContentSize().width;
	int minY = monsterHeight / 2;
	int maxY = winSize.height - monsterHeight / 2;
	int rangeY = maxY - minY;
	int actualY = (rand() % rangeY) + minY;
	CCLOG("%i", actualY);

	// создаем монстров немного за экраном по правому краю и в случайной позиции Y,
	// рассчитанной выше
	monster->setPosition(ccp(winSize.width + monsterWidth / 2, actualY));

	monster->setTag(1);
	_targets->addObject(monster);

	this->addChild(monster);

	// считаем скорость монстров
	int minDuration = 7;
	int maxDuration = 10;
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() % rangeDuration) + minDuration;

	//auto actionMove = MoveTo::create((float)actualDuration,	ccp(0 - monsterWidth / 2, actualY));
	//auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
	//// создаем действия
	//CCFiniteTimeAction* actionMove = CCMoveTo::actionWithDuration((float)actualDuration, ccp(0 - monsterWidth / 2, actualY));
	//CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget(this, callfuncN_selector(HelloWorld::spriteMoveFinished));

	//monster->runAction(actionMove);

	CCFiniteTimeAction* actionMove =
		CCMoveTo::create((float)actualDuration,
		ccp(0 - monster->getContentSize().width / 2, actualY));
	CCFiniteTimeAction* actionMoveDone =
		CCCallFuncN::create(this,
		CC_CALLFUNCN_SELECTOR(HelloWorld::spriteMoveFinished));
	monster->runAction(CCSequence::create(actionMove,
		actionMoveDone, NULL));
}

void HelloWorld::gameLogic(float dt){
	addMonster();
}

void HelloWorld::update(float dt)
{
	float newX = 0;
	float newY = 0;
	if (movementStarted) {
		float diffY = (eaterPointsPerSecY * dt)*1.3f;
		float diffX = (eaterPointsPerSecX * dt)*1.3f;
		newY = player->getPosition().y + diffY;
		newX = player->getPosition().x + diffX;
	}
	if (wPressed) {
		newY = player->getPosition().y + playerSpeed;
	}
	if (sPressed) {
		newY = player->getPosition().y - playerSpeed;
	}
	if (dPressed) {
		newX = player->getPosition().x + playerSpeed;
	}
	if (aPressed) {
		newX = player->getPosition().x - playerSpeed;
	}
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if (newY < player->getContentSize().height || newY > winSize.height - player->getContentSize().height) {
		newY = player->getPosition().y;
	}
	if (newX < player->getContentSize().width || newX > winSize.width - player->getContentSize().width) {
		newX = player->getPosition().x;
	}
	player->setPosition(newX, newY);
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
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			CCRect targetRect = CCRectMake(
				target->getPosition().x - (target->getContentSize().width / 2),
				target->getPosition().y - (target->getContentSize().height / 2),
				target->getContentSize().width,
				target->getContentSize().height);

			if (projectileRect.intersectsRect(targetRect))
			{
				targetsToDelete->addObject(target);
				projectilesToDelete->addObject(projectile);
			}
		}
	}

	CCARRAY_FOREACH(targetsToDelete, jt)
	{
		CCSprite *target = dynamic_cast<CCSprite*>(jt);
		_targets->removeObject(target);
		this->removeChild(target, true);
	}

	CCARRAY_FOREACH(projectilesToDelete, it)
	{
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}

	projectilesToDelete->release();
	targetsToDelete->release();
}

void HelloWorld::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	this -> removeChild(sprite, true);

	if (sprite->getTag() == 1)  // target
	{
		_targets->removeObject(sprite, true);
	}
	else if (sprite->getTag() == 2) // projectile
	{
		_projectiles->removeObject(sprite, true);
	}
}

void HelloWorld::menuOpenMenuCallback(Ref* pSender) {
	openMenu();
}

void HelloWorld::openMenu() {
	auto scene = MenuScreen::createScene();
	Director::getInstance()->replaceScene(scene);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
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
