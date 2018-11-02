#ifndef SCENE_GAME_AUTO_H
#define SCENE_GAME_AUTO_H

#include "cocos2d.h"
#include "SceneGameAbstract.h"

class SceneGameAuto : public SceneGameAbstract {
public:
	virtual bool init();
	// Passes the time since last loop as argument.
	virtual void update(float);

	CREATE_FUNC(SceneGameAuto)
protected:
	virtual void rotateWheel();
private:
	cocos2d::Sprite *m_tapLeft;
	cocos2d::Sprite *m_tapRight;

	cocos2d::Action *m_tapLeftAction;
	cocos2d::Action *m_tapRightAction;
};

#endif
