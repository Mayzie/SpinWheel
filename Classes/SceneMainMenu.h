#ifndef SCENE_MAIN_MENU_H
#define SCENE_MAIN_MENU_H

#include "cocos2d.h"
#include "SceneGameAbstract.h"

#define BACKGROUND_MAINMENU_COLOUR_VALUE (cocos2d::Color4B(0, 0, 0, 127))

class SceneMainMenu : cocos2d::LayerColor {
public:
	static cocos2d::Scene *createScene();

	virtual bool init();

	void setAutoChildLayer(SceneGameAbstract *);

	CREATE_FUNC(SceneMainMenu)
private:
	bool onTouchBegan(cocos2d::Touch *, cocos2d::Event *);
	void onTouchEnded(cocos2d::Touch *, cocos2d::Event *);

	SceneGameAbstract *m_autoLayer;
};

#endif
