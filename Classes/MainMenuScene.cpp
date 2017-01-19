#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

MainMenuScene::MainMenuScene()
{
}

MainMenuScene::~MainMenuScene()
{
}

Scene* MainMenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenuScene::init()
{
	if (!LayerColor::initWithColor(Color4B(157, 181, 173, 255))){
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::Sprite* back = Sprite::create("images/map/background_menu.png");
	back->setAnchorPoint(Vec2(0, 0));
	back->setPosition(Vec2(0, 0));
	this->addChild(back, 0, "back");

	Vector<MenuItem*> menuItems;

	auto startGame = Label::createWithTTF("New Game", "fonts/Marker Felt.ttf", 124);
	startGame->setColor(Color3B(255, 255, 255));

	auto endGame = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 124);
	endGame->setColor(Color3B(255, 255, 255));

	menuItems.pushBack(MenuItemLabel::create(startGame,
		CC_CALLBACK_1(MainMenuScene::startNewGame, this)));

	menuItems.pushBack(MenuItemLabel::create(endGame,
		CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this)));


	auto menu = Menu::createWithArray(menuItems);
	menu->alignItemsInColumns(1, 1);
	this->addChild(menu, 1);

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			close();
			break;
		case EventKeyboard::KeyCode::KEY_BACKSPACE:
			close();
			break;
		case EventKeyboard::KeyCode::KEY_ENTER:
			startNewGame(this);
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			startNewGame(this);
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, menu);

	back2 = Sprite::create("images/map/map.png");
	back2->setAnchorPoint(Vec2(0, 0));
	back2->setPosition(Vec2(0, 0));
	back2->setColor(Color3B(0, 0, 0));
	this->addChild(back2, 100, "back2");
	auto fadeIn = FadeOut::create(fadeTime);
	back2->runAction(fadeIn);

	return true;
}

void MainMenuScene::startNewGame(Ref* pSender) {
	if (!startingGame) {
		auto fadeIn = FadeIn::create(fadeTime);
		back2->runAction(fadeIn);
		auto startGameAction = Sequence::create(DelayTime::create(fadeTime), CallFunc::create(std::bind(&MainMenuScene::innerStartNewGame, this)), NULL);
		this->runAction(startGameAction);
		startingGame = true;
	}
}

void MainMenuScene::innerStartNewGame() {
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
	close();
}

void MainMenuScene::innerClose() {
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainMenuScene::close() {
	if (!startingGame && !endingGame) {
		auto fadeIn = FadeIn::create(fadeTime);
		back2->runAction(fadeIn);
		auto endGameAction = Sequence::create(DelayTime::create(fadeTime), CallFunc::create(std::bind(&MainMenuScene::innerClose , this)), NULL);
		this->runAction(endGameAction);
		endingGame = true;
	}
}
