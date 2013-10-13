#include "MainScene.h"
#include "SimpleAudioEngine.h"

#include "cocos2d.h"

using namespace cocos2d;

MainScene::~MainScene() {
	if (_targets) {
		_targets->release();
		_targets = NULL;
	}

	if (_projectiles) {
		_projectiles->release();
		_projectiles = NULL;
	}

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

MainScene::MainScene() :
		_targets(NULL), _projectiles(NULL), _projectilesDestroyed(0) {
}

CCScene* MainScene::scene() {
	CCScene * scene = NULL;
	do {
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		MainScene *layer = MainScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init() {
	bool bRet = false;
	do {
		CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(255,255,255,255) ));

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png",
				"CloseSelected.png", this,
				menu_selector(MainScene::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);

		// Place the menu item bottom-right conner.
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		pCloseItem->setPosition(
				ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
						origin.y + pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);


		_tileMap = new CCTMXTiledMap();
		_tileMap->initWithTMXFile("level.tmx");
		_trees = _tileMap->layerNamed("trees");
		_rock = _tileMap->layerNamed("rock");
		_sand = _tileMap->layerNamed("sand");

		_meta = _tileMap->layerNamed("meta");
		//_meta->setVisible(false);

		this->addChild(_tileMap);

		CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Resp");

		if (objectGroup == NULL) {
			CCLog("tile map has no objects object layer");
			return false;
		}

		CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");

		int x = ((CCString) *spawnPoint->valueForKey("x")).intValue();
		int y = ((CCString) *spawnPoint->valueForKey("y")).intValue();

		_player = new CCSprite();
		_player->initWithFile("Player.png");
		_player->setPosition(ccp(x,y));

		this->addChild(_player);
		this->setViewPointCenter(_player->getPosition());

		this->setTouchEnabled(true);

		bRet = true;
	} while (0);

	return bRet;
}

void MainScene::menuCloseCallback(CCObject* pSender) {
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

void MainScene::setViewPointCenter(CCPoint position) {

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int x = MAX(position.x, winSize.width/2);
	int y = MAX(position.y, winSize.height/2);
	x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
}

void MainScene::registerWithTouchDispatcher() {
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
			this, 0, true);
}

bool MainScene::ccTouchBegan(CCTouch *touch, CCEvent *event) {
	return true;
}

void MainScene::setPlayerPosition(CCPoint position)
{
    CCPoint tileCoord = this->tileCoordForPosition(position);
    int tileGid = _meta->tileGIDAt(tileCoord);

    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            if (collision && (collision->compare("true") == 0)) {
                return;
            }
        }
    }
    _player->setPosition(position);
}

void MainScene::ccTouchEnded(CCTouch *touch, CCEvent *event) {
	CCPoint touchLocation = touch->getLocationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	touchLocation = this->convertToNodeSpace(touchLocation);

	CCPoint playerPos = _player->getPosition();
	CCPoint diff = ccpSub(touchLocation, playerPos);

	if (abs(diff.x) > abs(diff.y)) {
		if (diff.x > 0) {
			playerPos.x += _tileMap->getTileSize().width;
		} else {
			playerPos.x -= _tileMap->getTileSize().width;
		}
	} else {
		if (diff.y > 0) {
			playerPos.y += _tileMap->getTileSize().height;
		} else {
			playerPos.y -= _tileMap->getTileSize().height;
		}
	}

	// safety check on the bounds of the map
	if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width)
			&& playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height)
				&& playerPos.y >= 0 && playerPos.x >= 0) {
		this->setPlayerPosition(playerPos);
	}

	this->setViewPointCenter(_player->getPosition());
}

CCPoint MainScene::tileCoordForPosition(CCPoint position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y)
    		/ _tileMap->getTileSize().height;
    return ccp(x, y);
}
