#pragma once

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);
	void update(float) override;
private:
	cocos2d::Label* label;
	cocos2d::Sprite* bg;
	cocos2d::Sprite* sprite;
	cocos2d::Sprite* newSnake[1200];
};