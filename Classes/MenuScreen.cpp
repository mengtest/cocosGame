#include "MenuScreen.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

MenuScreen::MenuScreen()
{
}

MenuScreen::~MenuScreen()
{
}

Scene* MenuScreen::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScreen::create();
	scene->addChild(layer);
	return scene;
}

bool MenuScreen::init()
{
	if (!LayerColor::initWithColor(Color4B(157, 181, 173, 255))){
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vector<MenuItem*> menuItems;

	auto startGame = Label::createWithTTF("New Game", "fonts/Marker Felt.ttf", 24);
	startGame->setColor(Color3B(255, 255, 255));

	auto endGame = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 24);
	endGame->setColor(Color3B(255, 255, 255));

	menuItems.pushBack(MenuItemLabel::create(startGame,
		CC_CALLBACK_1(MenuScreen::startNewGame, this)));
	
	menuItems.pushBack(MenuItemLabel::create(endGame,
		CC_CALLBACK_1(MenuScreen::menuCloseCallback, this)));
	

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
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, menu);

}

void MenuScreen::startNewGame(Ref* pSender) {
	auto scene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MenuScreen::menuCloseCallback(Ref* pSender)
{
	close();
}

void MenuScreen::close() {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
