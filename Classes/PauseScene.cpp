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
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 170))){
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vector<MenuItem*> menuItems;

	auto resumeGame = Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 24);
	resumeGame->setColor(Color3B(255, 255, 255));
	menuItems.pushBack(MenuItemLabel::create(resumeGame,
		CC_CALLBACK_1(PauseScene::menuCloseCallback, this)));

	auto menu = Menu::createWithArray(menuItems);
	menu->alignItemsInColumns(1);
	this->addChild(menu, 1);

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			PauseScene::resumeGame();
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, menu);

}

void PauseScene::menuCloseCallback(Ref* pSender)
{
	PauseScene::resumeGame();
}

void PauseScene::resumeGame() {
	Director::getInstance()->resume();
	this->removeFromParentAndCleanup(true);
}
