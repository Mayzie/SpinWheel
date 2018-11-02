#include "SceneGameUser.h"

USING_NS_CC;


Scene *SceneGameUser::createScene(SceneGameAbstract *autoScene) {
	auto scene = Scene::create();

	auto layer = SceneGameUser::create();

	layer->setAutoParent(autoScene);

	scene->addChild(layer, 1);

	return scene;
}

bool SceneGameUser::init() {
	if(!SceneGameAbstract::init()) {
		throw -1;
	}

	SceneGameAbstract::changeParent(this);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(SceneGameUser::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SceneGameUser::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	SceneGameAbstract::resetCannonBalls();
	SceneGameAbstract::throwCannonball();

	this->scheduleUpdate();

	return true;
}

void SceneGameUser::update(float delta) {
	SceneGameAbstract::update(delta);

	this->rotateWheel();

	if(SceneGameAbstract::m_cannonBalls.size() > 0) {
		Node *cannonBall = std::get<0>(SceneGameAbstract::m_cannonBalls[0]);
		Rect ballBoundingBox = utils::getCascadeBoundingBox(cannonBall);

		if(SceneGameAbstract::m_wheelBounds.intersectsRect(ballBoundingBox)) {
			int ballColour = std::get<1>(SceneGameAbstract::m_cannonBalls[0]);

			if(ballColour != SceneGameAbstract::getCurrentColourIndex()) {
				Director::getInstance()->pause();

				// ToDo: Save score, show ad.
			} else {
				SceneGameAbstract::m_cannonBalls.erase(SceneGameAbstract::m_cannonBalls.begin());
				this->m_score++;

				// ToDo: Update score text.
			}
		}
	}
}

void SceneGameUser::rotateWheel() {
	if(this->m_touches.size() > 0) {
		auto touchLocation = (this->m_touches[0])->getLocation();

		float rotationDegrees;
		int newColourIndex;

		if(touchLocation.x <= SceneGameAbstract::m_visibleSize.width / 2) {
			rotationDegrees = -ROTATION_DEGREES;
			newColourIndex = modulo(SceneGameAbstract::getCurrentColourIndex() + 1, NUM_COLOURS);
		} else {
			rotationDegrees = ROTATION_DEGREES;
			newColourIndex = modulo(SceneGameAbstract::getCurrentColourIndex() - 1, NUM_COLOURS);
		}

		SceneGameAbstract::rotateWheel(rotationDegrees, newColourIndex);
	}
}

bool SceneGameUser::onTouchBegan(Touch *touch, Event *event) {
	auto director = Director::getInstance();
	if(director->isPaused()) {
		// ToDo: Set ...

		SceneGameAbstract::resetCannonBalls();
		SceneGameAbstract::changeParent(this->m_parent);
		director->resume();
		director->popScene();
	} else {
		this->m_touches.push_back(touch);

		if(!SceneGameAbstract::getWheelRotating()) {
			this->rotateWheel();
		}
	}

	return true;
}

void SceneGameUser::onTouchEnded(Touch *touch, Event *event) {
	for(auto it = this->m_touches.begin(); it != this->m_touches.end(); ++it) {
		if((*it) == touch) {
			this->m_touches.erase(it);

			break;
		}
	}
}


void SceneGameUser::setAutoParent(SceneGameAbstract *parent) {
	this->m_parent = parent;
	//SceneGameAbstract::setParent(parent);
	//SceneGameAbstract::m_parent = parent;
}

