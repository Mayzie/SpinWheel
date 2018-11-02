#include "SceneGameAuto.h"

USING_NS_CC;

bool SceneGameAuto::init() {
	if(!SceneGameAbstract::init()) {
		throw -1;
	}

	SceneGameAbstract::changeParent(this);

	this->m_tapLeft = Sprite::createWithSpriteFrameName("TapCircle.png");
	this->m_tapRight = Sprite::createWithSpriteFrameName("TapCircle.png");

	this->m_tapLeft->setColor(Color3B::WHITE);
	this->m_tapRight->setColor(Color3B::WHITE);

	this->m_tapLeft->setAnchorPoint(Vec2(0.0f, 0.0f));
	this->m_tapRight->setAnchorPoint(Vec2(1.0f, 0.0f));

	this->m_tapLeft->setPosition(Vec2(35.0f, 100.0f));
	this->m_tapRight->setPosition(Vec2(SceneGameAbstract::m_visibleSize.width - 35.0f, 100.0f));

	this->m_tapLeft->setOpacity(0);
	this->m_tapRight->setOpacity(0);

	this->addChild(this->m_tapLeft, 10);
	this->addChild(this->m_tapRight, 10);

	SceneGameAbstract::throwCannonball();

	this->scheduleUpdate();

	return true;
}

void SceneGameAuto::update(float delta) {
	SceneGameAbstract::update(delta);

	if(SceneGameAbstract::getCannonBallsSize() > 0) {
		this->rotateWheel();

		Node *topCannonBall = std::get<0>(SceneGameAbstract::m_cannonBalls[0]);
		Rect ballBoundingBox = utils::getCascadeBoundingBox(topCannonBall);
		Rect wheelBounds = SceneGameAbstract::m_wheelBounds;

		if(SceneGameAbstract::m_wheelBounds.intersectsRect(ballBoundingBox)) {
			SceneGameAbstract::m_cannonBalls.erase(SceneGameAbstract::m_cannonBalls.begin());
		}
	}
}

void SceneGameAuto::rotateWheel() {
	int currentColourIndex = SceneGameAbstract::getCurrentColourIndex();
	int newColourIndex = std::get<1>(SceneGameAbstract::m_cannonBalls[0]);

	if(currentColourIndex == newColourIndex) {
		return;
	}

	float rotationDegrees = (ROTATION_DEGREES * ((modulo(currentColourIndex + 1, NUM_COLOURS)) == newColourIndex ? -1 : 1));
	if(rotationDegrees < 0) {
		// Display tap left
		if(this->m_tapLeft) {
			if(this->m_tapLeftAction)
				this->m_tapLeft->stopAction(this->m_tapLeftAction);

			this->m_tapLeft->setOpacity(255);
			this->m_tapLeftAction = this->m_tapLeft->runAction(FadeOut::create(NODE_BALL_THROW_INTERVAL));
		}
	} else if(rotationDegrees > 0) {
		// Display tap right
		if(this->m_tapRight) {
			if(this->m_tapRightAction)
				this->m_tapRight->stopAction(this->m_tapRightAction);

			this->m_tapRight->setOpacity(255);
			this->m_tapRightAction = this->m_tapRight->runAction(FadeOut::create(NODE_BALL_THROW_INTERVAL));
		}
	}

	SceneGameAbstract::rotateWheel(rotationDegrees, newColourIndex);
}
