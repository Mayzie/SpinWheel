#ifndef SCENE_GAME_USER_H
#define SCENE_GAME_USER_H

#include "cocos2d.h"
#include "SceneGameAbstract.h"

class SceneGameUser : public SceneGameAbstract {
public:
	static cocos2d::Scene *createScene(SceneGameAbstract *);

	virtual bool init();
	// Passes the time since last loop as argument.
	virtual void update(float);

	void setAutoParent(SceneGameAbstract *);

	CREATE_FUNC(SceneGameUser)
protected:
	virtual void rotateWheel();
//	virtual void throwCannonball();
private:
	bool onTouchBegan(cocos2d::Touch *, cocos2d::Event *);
	void onTouchEnded(cocos2d::Touch *, cocos2d::Event *);

	int m_score;
	cocos2d::Label *m_scoreText;

	std::vector<cocos2d::Touch *> m_touches;

	SceneGameAbstract *m_parent;
};

#endif
