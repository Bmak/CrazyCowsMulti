#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "CCLayerPanZoom.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;


class MainScene : public CCLayerPanZoom
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

	void setPlayerPosition(CCPoint position);

	/*
	void ccTouchesBegan(CCSet * pTouches,CCEvent * pEvent);

	void ccTouchesMoved(CCSet *touches, CCEvent *pEvent);

	void ccTouchesEnded(CCSet * pTouches,CCEvent * pEvent);
	 */

	// implement the "static node()" method manually
	CREATE_FUNC(MainScene);
    
protected:
	CCTMXTiledMap * _testmap;

	CCTMXTiledMap * _tileMap;
	CCTMXLayer * _trees;
	CCTMXLayer * _rock;
	CCTMXLayer * _sand;

	CCTMXLayer * _meta;

	CCSprite *_player;

	CCPoint tileCoordForPosition(CCPoint position);

	bool _multiTouchState;

	void playerMove(CCTouch *touch);

	CCPoint * _currTouchLocation;

};

#endif  // __MAIN_SCENE_H__
