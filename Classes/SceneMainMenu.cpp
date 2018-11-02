#include "SceneMainMenu.h"
#include "SceneGameAuto.h"
#include "SceneGameUser.h"

USING_NS_CC;

Scene *SceneMainMenu::createScene() {
	auto scene = Scene::create();

	auto autoLayer = SceneGameAuto::create();
	auto menuLayer = SceneMainMenu::create();

	menuLayer->setAutoChildLayer(autoLayer);

	CCLOG("CCLog: Adding autoLayer child. Pointer: 0x%08x.", autoLayer);
	scene->addChild(autoLayer, 1);
	CCLOG("CCLog: Adding menuLayer child.  Pointer: 0x%08x.", menuLayer);
	if(menuLayer) {
		scene->addChild(menuLayer, 2);
		CCLOG("CCLog: Added.");
	}

	return scene;
}

bool SceneMainMenu::init() {
	if(!LayerColor::initWithColor(BACKGROUND_MAINMENU_COLOUR_VALUE)) {
		CCLOG("CCLog: Couldn't init with colour.");
		return false;
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(SceneMainMenu::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SceneMainMenu::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

bool SceneMainMenu::onTouchBegan(Touch *touch, Event *event) {
	return true;  // Captured.
}

void SceneMainMenu::onTouchEnded(Touch *touch, Event *event) {
	Scene *gameUserScene = SceneGameUser::createScene(this->m_autoLayer);

	if(gameUserScene) {
		Director::getInstance()->pushScene(gameUserScene);
	}
}

void SceneMainMenu::setAutoChildLayer(SceneGameAbstract *child) {
	this->m_autoLayer = child;
}
