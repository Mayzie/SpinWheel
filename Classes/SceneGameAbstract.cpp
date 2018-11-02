#include "SceneGameAbstract.h"

USING_NS_CC;

bool SceneGameAbstract::m_initialised = false;
Size SceneGameAbstract::m_visibleSize = Size(Vec2(0.0f, 0.0f));
std::vector<Color3B> SceneGameAbstract::m_wheelBallColourValues;
cocos2d::Node *SceneGameAbstract::m_wheel = nullptr;
cocos2d::Node *SceneGameAbstract::m_cannon = nullptr;
std::vector<std::tuple<Node *, int, Node *>> SceneGameAbstract::m_cannonBalls;

Vec2 SceneGameAbstract::m_origin = Vec2(0.0f, 0.0f);
//std::uniform_int_distribution<int> SceneGameAbstract::m_randomDistribution;
std::uniform_int_distribution<int> *SceneGameAbstract::m_randomDistribution = new std::uniform_int_distribution<int>(0, (NUM_SEGMENTS + 1));
std::mt19937 SceneGameAbstract::m_randomGenerator;
bool SceneGameAbstract::m_wheelRotating = false;
Rect SceneGameAbstract::m_wheelBounds;
int SceneGameAbstract::m_lastColourIndex = 0;
int SceneGameAbstract::m_currentColourIndex = 0;

std::vector<Node *> SceneGameAbstract::m_backgroundNodeRows;
std::vector<Color3B> SceneGameAbstract::m_backgroundColourValues;

float SceneGameAbstract::m_rotationDegrees = 0.0f;
float SceneGameAbstract::m_rotationStartAngle = 0.0f;
float SceneGameAbstract::m_rotationElapsedTime = 0.0f;
bool SceneGameAbstract::m_updatedColour = false;
int SceneGameAbstract::m_newColourIndex = 0;

SceneGameAbstract *SceneGameAbstract::m_parent = nullptr;

/**
 * @brief Performs the standard unsigned modulo operation.
 * @param val The dividend.
 * @param mod The divisor.
 * @return (val % mod), translating negative values and excessive values into the range 0..(mod - 1).
 */
inline int modulo(int val, int mod) {
	int ret = val % mod;
	if(ret < 0) {
		return mod + ret;
	} else if(ret > mod) {
		return val - ret;
	}

	return ret;
}

bool SceneGameAbstract::init() {
	CCLOG("CCLog: Inside function '%s'.", __FUNCTION__);
	if(!LayerColor::initWithColor(BACKGROUND_DEFAULT_COLOUR_VALUE)) {
		return false;
	}

	if(!SceneGameAbstract::m_initialised) {
		CCLOG("CCLog: In init.");
		SceneGameAbstract::m_visibleSize = Director::getInstance()->getVisibleSize();
		SceneGameAbstract::m_origin = Director::getInstance()->getVisibleOrigin();

		//SceneGameAbstract::m_randomDistribution = new std::uniform_int_distribution<int>(0, (NUM_SEGMENTS + 1));
		SceneGameAbstract::m_randomGenerator.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());

//		SceneGameAbstract::m_lastColourIndex = 0;
//		SceneGameAbstract::m_currentColourIndex = 0;

		// Set up the background.
		SceneGameAbstract::m_backgroundColourValues.reserve(BACKGROUND_COLOUR_VALUES_SIZE);

		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(59, 154, 255));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(59, 154, 245));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(59, 144, 255));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(59, 144, 245));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(59, 164, 255));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(59, 164, 245));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(49, 154, 255));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(49, 154, 245));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(49, 144, 255));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(49, 144, 245));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(49, 164, 255));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(49, 164, 245));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(69, 154, 255));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(69, 154, 245));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(69, 144, 255));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(69, 144, 245));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(69, 164, 255));
		SceneGameAbstract::m_backgroundColourValues.push_back(Color3B(69, 164, 245));

		for(float y = 0.0f; y < (SceneGameAbstract::m_visibleSize.height + BACKGROUND_NODE_SIZE); y += BACKGROUND_NODE_SIZE) {
			Node *rowNode = Node::create();
			rowNode->setAnchorPoint(Vec2(0.0f, 1.0f));

			for(float x = 0.0f; x < (SceneGameAbstract::m_visibleSize.width); x += BACKGROUND_NODE_SIZE) {
				Sprite *square = Sprite::createWithSpriteFrameName("Pixel.png");

				square->setAnchorPoint(Vec2(0.0f, 0.0f));
				square->setPosition(Vec2(x, 0.0f));
				square->setColor(SceneGameAbstract::m_backgroundColourValues[rand() % BACKGROUND_COLOUR_VALUES_SIZE]);

				rowNode->addChild(square, 1);
			}

			rowNode->setPosition(Vec2(1.75f, y));

			SceneGameAbstract::m_backgroundNodeRows.push_back(rowNode);
		}

		SceneGameAbstract::m_wheelBallColourValues.reserve(NUM_COLOURS);
		SceneGameAbstract::m_wheelBallColourValues.push_back(Color3B(255, 0, 0));
		SceneGameAbstract::m_wheelBallColourValues.push_back(Color3B(0, 255, 0));
		SceneGameAbstract::m_wheelBallColourValues.push_back(Color3B(0, 0, 255));
		SceneGameAbstract::m_wheelBallColourValues.push_back(Color3B(255, 0, 255));
		SceneGameAbstract::m_wheelBallColourValues.push_back(Color3B(0, 255, 255));

		SceneGameAbstract::m_wheel = Node::create();
		for(int i = 0; i < 360; i += ROTATION_DEGREES) {
			auto segment = Sprite::createWithSpriteFrameName("CircleSegment.png");

			segment->setAnchorPoint(Vec2(0.5f, 1.0f));
			segment->setRotation(i);
			segment->setColor(SceneGameAbstract::m_wheelBallColourValues[i / ROTATION_DEGREES]);

			SceneGameAbstract::m_wheel->addChild(segment, 1);
		}

		SceneGameAbstract::m_wheel->setPosition(Vec2(SceneGameAbstract::m_origin.x + SceneGameAbstract::m_visibleSize.width / 2,
													 SceneGameAbstract::m_origin.y + (5 * SceneGameAbstract::m_visibleSize.height) / 8));
		SceneGameAbstract::m_wheel->setScale(NODE_WHEEL_SCALE_FACTOR);
		SceneGameAbstract::m_wheelBounds = utils::getCascadeBoundingBox(SceneGameAbstract::m_wheel);

		SceneGameAbstract::m_cannon = Sprite::createWithSpriteFrameName("Cannon.png");
		SceneGameAbstract::m_cannon->setAnchorPoint(Vec2(0.5f, 0.25f));
		SceneGameAbstract::m_cannon->setPosition(Vec2(SceneGameAbstract::m_origin.x + SceneGameAbstract::m_visibleSize.width / 2, SceneGameAbstract::m_origin.y));
		SceneGameAbstract::m_cannon->setScale(NODE_CANNON_SCALE_FACTOR);

		SceneGameAbstract::m_initialised = true;
	}

	CCLOG("CCLog: Exiting function '%s'.", __FUNCTION__);
	return true;
}

void SceneGameAbstract::changeParent(SceneGameAbstract *newParent) {
	if(SceneGameAbstract::m_initialised) {
		//Director::getInstance()->pause();
		/* Node.retain() and Node.release() need to be used because
		 * removing from parent decreases its reference by 1. */
		SceneGameAbstract::m_wheel->retain();
		SceneGameAbstract::m_cannon->retain();

		if(SceneGameAbstract::m_wheel->getParent())
			SceneGameAbstract::m_wheel->removeFromParentAndCleanup(true);
		if(SceneGameAbstract::m_cannon->getParent())
			SceneGameAbstract::m_cannon->removeFromParentAndCleanup(true);

		for(Node *node : SceneGameAbstract::m_backgroundNodeRows) {
			node->retain();
			if(node->getParent()) {
				node->removeFromParentAndCleanup(true);
			}

			newParent->addChild(node, 1);
			node->release();
		}

		newParent->addChild(SceneGameAbstract::m_wheel, 20);
		newParent->addChild(SceneGameAbstract::m_cannon, 20);

		CCLOG("CCLog: Setting current colour index and rotation to = 0");
		SceneGameAbstract::m_currentColourIndex = 0;
		SceneGameAbstract::m_wheel->setRotation(0.0f);
		SceneGameAbstract::m_wheel->stopAllActions();

		SceneGameAbstract::m_wheel->release();
		SceneGameAbstract::m_cannon->release();

		SceneGameAbstract::m_parent = newParent;
		SceneGameAbstract::setWheelRotating(false);
		//Director::getInstance()->resume();
//		if(SceneGameAbstract::getWheelRotating()) {
//			SceneGameAbstract::m_canFire = false;
//		}
	}
}

void SceneGameAbstract::throwCannonball() {
	if(SceneGameAbstract::m_initialised) {
		SceneGameAbstract::m_parent->unschedule("throwCannonball"); // Leave to parent.

		//if(SceneGameAbstract::m_canFire) {
			auto ballNode = Node::create();
			if(ballNode == nullptr) {
				throw 1;
			}

			//Node *ballNode = Node::create();
			Sprite *ballInlineSprite = Sprite::createWithSpriteFrameName("BallInline.png");
			Sprite *ballOutlineSprite = Sprite::createWithSpriteFrameName("BallOutline.png");

			ballInlineSprite->setScale(BALL_SCALE_MIN);
			ballOutlineSprite->setScale(BALL_SCALE_MIN);

			ballNode->addChild(ballInlineSprite, 1);
			ballNode->addChild(ballOutlineSprite, 0);

			ballNode->setPosition(Vec2(SceneGameAbstract::m_origin.x + SceneGameAbstract::m_visibleSize.width / 2, SceneGameAbstract::m_origin.y));

			int nextColour = (*SceneGameAbstract::m_randomDistribution)(SceneGameAbstract::m_randomGenerator);
			if(nextColour <= 2) {  // Choose the colour to the left.
				SceneGameAbstract::m_lastColourIndex = modulo(SceneGameAbstract::m_lastColourIndex - 1, SceneGameAbstract::m_wheelBallColourValues.size());
			} else if(nextColour >= 4) {  // Choose the colour to the right.
				SceneGameAbstract::m_lastColourIndex = modulo(SceneGameAbstract::m_lastColourIndex + 1, SceneGameAbstract::m_wheelBallColourValues.size());
			} else {
				// Do nothing... colour stays the same.
			}

			ballInlineSprite->setColor(SceneGameAbstract::m_wheelBallColourValues[SceneGameAbstract::m_lastColourIndex]);
			ballOutlineSprite->setColor(SceneGameAbstract::m_wheelBallColourValues[SceneGameAbstract::m_lastColourIndex]);

			Sequence *sequenceMove = Sequence::create(
					EaseQuadraticActionIn::create(MoveTo::create(NODE_BALL_MOVE_DURATION, SceneGameAbstract::m_wheel->getPosition())),
					CallFunc::create(std::bind(&SceneGameAbstract::onThrowFinished, ballNode)),
					nullptr
				);

			auto sequenceGrowShrink = RepeatForever::create(Sequence::create(
					DelayTime::create(BALL_SCALE_INTERVAL),
					ScaleTo::create(BALL_SCALE_DURATION, BALL_SCALE_MAX),
					ScaleTo::create(BALL_SCALE_DURATION, BALL_SCALE_MIN),
					nullptr
				));

			SceneGameAbstract::m_cannonBalls.push_back(std::make_tuple(ballInlineSprite, SceneGameAbstract::m_lastColourIndex, ballNode));

			ballNode->runAction(sequenceMove);
			ballOutlineSprite->runAction(sequenceGrowShrink);

			SceneGameAbstract::m_parent->addChild(ballNode, 10);
		//}

		SceneGameAbstract::m_parent->schedule(std::bind(&SceneGameAbstract::throwCannonball), NODE_BALL_THROW_INTERVAL, "throwCannonball");
	}
}

void SceneGameAbstract::onThrowFinished(Node *ballNode) {
	if(ballNode) {
		ballNode->removeAllChildrenWithCleanup(true);
		ballNode->removeFromParentAndCleanup(true);
	}
}

int SceneGameAbstract::getCannonBallsSize() {
	return SceneGameAbstract::m_cannonBalls.size();
}

void SceneGameAbstract::resetCannonBalls() {
	for(std::tuple<Node *, int, Node *> element : SceneGameAbstract::m_cannonBalls) {
		Node *ballNode = std::get<2>(element);

		if(ballNode) {
			SceneGameAbstract::onThrowFinished(ballNode);
		}
	}

	SceneGameAbstract::m_cannonBalls.clear();
}

int SceneGameAbstract::getCurrentColourIndex() {
	return SceneGameAbstract::m_currentColourIndex;
}

void SceneGameAbstract::updateColour(int newColourIndex) {
	//if(SceneGameAbstract::m_wheelRotating) {
		CCLOG("CCLog: Updating colour to %d.", newColourIndex);
		SceneGameAbstract::m_currentColourIndex = newColourIndex;
		//SceneGameAbstract::m_canFire = true;
	//}
}

void SceneGameAbstract::setWheelRotating(bool rotating) {
	SceneGameAbstract::m_wheelRotating = rotating;
}

void SceneGameAbstract::setSceneParent(SceneGameAbstract *parent) {
	SceneGameAbstract::m_parent = parent;
}

SceneGameAbstract *SceneGameAbstract::getSceneParent() {
	return SceneGameAbstract::m_parent;
}

bool SceneGameAbstract::getWheelRotating() {
	return SceneGameAbstract::m_wheelRotating;
}

void SceneGameAbstract::rotateWheel(float degrees, int colour) {
	if(!SceneGameAbstract::m_wheelRotating) {
		//float actionDuration = ROTATION_DURATION / 2.0f;

		SceneGameAbstract::setWheelRotating(true);
		SceneGameAbstract::m_rotationStartAngle = SceneGameAbstract::m_wheel->getRotation();
		SceneGameAbstract::m_rotationElapsedTime = 0.0f;
		SceneGameAbstract::m_rotationDegrees = degrees;

		/*
		auto rotationSequence = Sequence::create(
				RotateBy::create(actionDuration, degrees / 2.0f),  // Rotate to the halfway point.
				CallFunc::create(std::bind(&SceneGameAbstract::updateColour, colour)),  // Update the active colour.
				RotateBy::create(actionDuration, degrees / 2.0f),  // Rotate the full way.
				CallFunc::create(std::bind(&SceneGameAbstract::setWheelRotating, false)),
				nullptr
			);
			*/

		//SceneGameAbstract::m_wheel->runAction(rotationSequence);
	}
}

void SceneGameAbstract::update(float delta) {
	if(SceneGameAbstract::m_parent != nullptr) {
		// Scroll the background
		for(Node *node : SceneGameAbstract::m_backgroundNodeRows) {
			Vec2 currentPosition = node->getPosition();
			Vec2 newPosition = currentPosition + (Vec2(0.0f, -BACKGROUND_MOVE_SIZE) * delta);

			if(newPosition.y <= -BACKGROUND_NODE_SIZE) {
				newPosition.y += SceneGameAbstract::m_backgroundNodeRows.size() * BACKGROUND_NODE_SIZE;
			}

			node->setPosition(newPosition);
		}

		// Rotate the wheel
		if(SceneGameAbstract::m_wheelRotating) {
			CCLOG("CCLog: We are rotating.");
			// Check if we have finished the rotation.
			if(!((SceneGameAbstract::m_rotationElapsedTime + FLT_EPSILON) >= ROTATION_DURATION)) {
				CCLOG("CCLog: We haven't finished yet.");
				CCLOG("CCLog: Rotation degrees %f.", SceneGameAbstract::m_rotationDegrees);
				CCLOG("CCLog: Rotation duration %f.", ROTATION_DURATION);
				// Check if we have exceeded the halfway mark and proceed to update our new colour index (if we haven't already).
				if(!SceneGameAbstract::m_updatedColour && SceneGameAbstract::m_rotationElapsedTime + FLT_EPSILON >= ROTATION_DURATION / 2.0f) {
					SceneGameAbstract::updateColour(SceneGameAbstract::m_newColourIndex);

					SceneGameAbstract::m_updatedColour = true;
				}

				SceneGameAbstract::m_rotationElapsedTime += delta;
				float updateDelta = MAX(0, MIN(1, SceneGameAbstract::m_rotationElapsedTime / ROTATION_DURATION));
				SceneGameAbstract::m_wheel->setRotation(SceneGameAbstract::m_rotationStartAngle + SceneGameAbstract::m_rotationDegrees * updateDelta);
			} else {
				CCLOG("CCLog: No longer rotating.");
				SceneGameAbstract::m_wheelRotating = false;
			}
		}
	}
}
