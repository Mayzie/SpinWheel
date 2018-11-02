#ifndef SCENE_GAME_ABSTRACT_H
#define SCENE_GAME_ABSTRACT_H

#include <vector>
#include <tuple>
#include <random>

#include "cocos2d.h"

/*
 * Precompiler definitions
 */
#define RANDOM_0_1 ((float) rand() / (float) RAND_MAX)  // Random floating point value between 0 and 1.

#define NUM_SEGMENTS (5)  // Number of segments the wheel should have.
#define NUM_COLOURS NUM_SEGMENTS
#define ROTATION_DEGREES (360.0f / NUM_SEGMENTS)  // How many degrees each tap action should rotate by.
#define ROTATION_DURATION (ROTATION_DEGREES / 480.0f)  // How long each rotation action should take.

#define NODE_WHEEL_SCALE_FACTOR (1.25f)  // How much to scale the wheel node by. ToDo: Fix the image so that it is scaled by this amount, rather than doing it in code.
#define NODE_CANNON_SCALE_FACTOR (2.75f)  // How much to scale the cannon node by. ToDo: Same as above.

#define NODE_BALL_THROW_INTERVAL (0.475f)  // How often new cannonballs are created.
#define NODE_BALL_MOVE_DURATION (1.5f)  // How long it takes a cannonball to get from the cannon to the wheel.

#define BALL_SCALE_MIN (1.0f - 0.25f)  // Smallest size to scale a cannonball to.
#define BALL_SCALE_MAX (1.0f + 0.15f)  // Largest size to scale a cannonball to.
#define BALL_SCALE_DURATION (0.15f)  // How long a single scale action of a cannonball should take.
#define BALL_SCALE_INTERVAL (0.2f * RANDOM_0_1)  // How long to wait before running a scaling animation again on the cannonball.

#define BACKGROUND_NODE_SIZE (15.0f)  // How big each background tile is.
#define BACKGROUND_MOVE_SIZE (BACKGROUND_NODE_SIZE * 16)  // The distance each background row will move downwards per second.
#define BACKGROUND_COLOUR_VALUES_SIZE (18)  // How many (total) possible background colour values there will be.
#define BACKGROUND_DEFAULT_COLOUR_VALUE (cocos2d::Color4B(59, 154, 255, 255))  // The colour the whole background should be.

inline int modulo(int, int);

class SceneGameAbstract : public cocos2d::LayerColor {
public:
	virtual bool init();
	// Passes the time since last loop as argument.
	virtual void update(float);

	CREATE_FUNC(SceneGameAbstract)
protected:
	// Controls wheel rotation - degrees to rotate by + colour change.
	static void rotateWheel(float, int);
	static void throwCannonball();

	static void changeParent(SceneGameAbstract *);

	static int getCannonBallsSize();
	static void resetCannonBalls();

	static int getCurrentColourIndex();

	static bool getWheelRotating();

	// Colour index, ball node.
	static void onThrowFinished(cocos2d::Node *);
	static void updateColour(int);
	static void setWheelRotating(bool);

	static void setSceneParent(SceneGameAbstract *);
	static SceneGameAbstract *getSceneParent();

	static std::vector<cocos2d::Color3B> m_wheelBallColourValues;
	static cocos2d::Node *m_wheel;
	static cocos2d::Rect m_wheelBounds;

	static cocos2d::Node *m_cannon;
	static std::vector<std::tuple<cocos2d::Node *, int, cocos2d::Node *>> m_cannonBalls;

	static cocos2d::Size m_visibleSize;
	static cocos2d::Vec2 m_origin;
private:
	//inline void initialiseCommonMemory();
	//inline int random();

	static std::vector<cocos2d::Node *> m_backgroundNodeRows;
	static std::vector<cocos2d::Color3B> m_backgroundColourValues;

	// Used for determining which colour ball to throw.
	static std::mt19937 m_randomGenerator;
	static std::uniform_int_distribution<int> *m_randomDistribution;

	static int m_lastColourIndex;
	static int m_currentColourIndex;

	static bool m_initialised;

	static SceneGameAbstract *m_parent;
	static cocos2d::Action *m_actions;

	// Rotating action
	static bool m_wheelRotating;
	static float m_rotationDegrees;
	static float m_rotationStartAngle;  //
	static float m_rotationElapsedTime;
	static bool m_updatedColour;
	static int m_newColourIndex;
};

#endif
