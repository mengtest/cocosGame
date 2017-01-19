#include "PauseScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

PauseScene::PauseScene()
{
}

PauseScene::~PauseScene()
{
}

Scene* PauseScene::createScene()
{
	auto scene = Scene::create();
	auto layer = PauseScene::create();
	scene->addChild(layer);
	return scene;
}

bool PauseScene::init()
{
	this->resume();
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 170))){
		return false;
	}
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vector<MenuItem*> menuItems;

	auto resumeGame = Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 124);
	resumeGame->setColor(Color3B(255, 255, 255));
	auto toMainMenu = Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 124);
	toMainMenu->setColor(Color3B(255, 255, 255));
	menuItems.pushBack(MenuItemLabel::create(resumeGame,
		CC_CALLBACK_1(PauseScene::menuCloseCallback, this)));
	menuItems.pushBack(MenuItemLabel::create(toMainMenu,
		CC_CALLBACK_1(PauseScene::goToMainMenu, this)));

	auto menu = Menu::createWithArray(menuItems);
	menu->alignItemsInColumns(1, 1);
	this->addChild(menu, 1);

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			PauseScene::resumeGame();
			break;
		case EventKeyboard::KeyCode::KEY_BACKSPACE:
			Director::getInstance()->end();
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, menu);

	darkness = Sprite::create("images/map/map.png");
	darkness->setAnchorPoint(Vec2(0, 0));
	darkness->setPosition(Vec2(0, 0));
	darkness->setColor(Color3B(0, 0, 0));
	darkness->setOpacity(0);
	this->addChild(darkness, 100, "darkness");
	return true;
}

void PauseScene::menuCloseCallback(Ref* pSender)
{
	PauseScene::resumeGame();
}

void PauseScene::goToMainMenu(Ref* pSender) {
	//resumeGame();
	//auto action = Sequence::create(DelayTime::create(fadeTime), CallFunc::create(std::bind(&PauseScene::innerGoToMainMenu, this)), NULL);
	//this->runAction(action);
	//auto fadeIn = FadeIn::create(fadeTime);
	//darkness->runAction(fadeIn);
	
	innerGoToMainMenu();
}

void PauseScene::innerGoToMainMenu() {
	resumeGame();
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void PauseScene::resumeGame() {
	Director::getInstance()->resume();
}
