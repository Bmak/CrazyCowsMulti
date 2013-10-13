#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

class MainScene : public cocos2d::CCLayerColor
{
public:
	MainScene();
	~MainScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
    // instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);
    
	void setViewPointCenter(CCPoint position);


	void registerWithTouchDispatcher();

	void setPlayerPosition(CCPoint position);

	bool ccTouchBegan(CCTouch *touch, CCEvent *event);

	void ccTouchEnded(CCTouch *touch, CCEvent *event);

	// implement the "static node()" method manually
	CREATE_FUNC(MainScene);
    
protected:
	CCTMXTiledMap * _testmap;
	cocos2d::CCArray *_targets;
	cocos2d::CCArray *_projectiles;
	int _projectilesDestroyed;

	CCTMXTiledMap * _tileMap;
	CCTMXLayer * _trees;
	CCTMXLayer * _rock;
	CCTMXLayer * _sand;

	CCTMXLayer * _meta;

	CCSprite *_player;

	CCPoint tileCoordForPosition(CCPoint position);

};

#endif  // __MAIN_SCENE_H__
