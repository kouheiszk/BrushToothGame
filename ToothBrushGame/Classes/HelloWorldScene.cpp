#include "HelloWorldScene.h"

#include "Tooth.h"
#include "ToothManager.h"
#include "Plaque.h"
#include "PlaqueManager.h"
#include "EnemyManager.h"
#include "Boss.h"
#include "HitChecker.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    //this->addChild(label, 1);
    
    // 更新処理の追加
    this->scheduleUpdate();
    
    // タッチ機能の有効化
    m_pTouchEventOneByOne =  EventListenerTouchOneByOne::create();
    m_pTouchEventOneByOne->setSwallowTouches(true);
    m_pTouchEventOneByOne->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegin,this);
    m_pTouchEventOneByOne->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved,this);
    m_pTouchEventOneByOne->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    m_pTouchEventOneByOne->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithFixedPriority(m_pTouchEventOneByOne, 100);
    
    // 歯マネージャーのインスタンス化
    m_pToothManager = ToothManager::create(Vec2(0.0f,visibleSize.height),this);
    m_bHit = false;
    
    m_pEnemyManager = EnemyManager::create(this,10);
    
    // 泡のスプライト生成
    m_pBubbleSprite = Sprite::create("bubble_01.png");
    m_bubblePos = Vec2(0.0f,0.0f);
    m_pBubbleSprite->setPosition(m_bubblePos);
    m_pBubbleSprite->setOpacity(0);
    this->addChild(m_pBubbleSprite);
    
    m_pEnemySprite = Sprite::create("Enemy.png");
    m_pEnemySprite->setPosition(400, 600);
    this->addChild(m_pEnemySprite);
    
    m_pPlaqueSprite = Sprite::create("Plaque.png");
    m_pPlaqueSprite->setPosition(410, 800);
    this->addChild(m_pPlaqueSprite);
    
    m_bEnemyDie = false;
    m_bPlaqueDie = false;
    m_bBossDisp = false;
    m_bMove = false;
    m_touchPos = Point(0.0f,0.0f);
    m_oldTouchPos = m_touchPos;
    m_swipeDirection = SWIPE_DIRECTION_NONE;
    m_oldSwipeDirection = m_swipeDirection;

    this->addChild(Plaque::create(Vec2(64, 64))->getSprite());

     m_pPlaqueManager = PlaqueManager::create(100, this);

    m_pHitChecker = HitChecker::create(m_pEnemyManager, m_pToothManager, m_pPlaqueManager);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float fTime)
{
    // 歯の更新
    m_pToothManager->update();
    m_pPlaqueManager->update();
    
    // 歯を動かすか判定
    if(m_bEnemyDie && m_bPlaqueDie && !m_bMove)
    {
        m_bMove = true;
        
       // m_pToothManager->AddBottomGumPosAndBottomToothPos(Vec2(0,-100));
        //m_pToothManager->AddTopGumPosAndTopToothPos(Vec2(0,100));

        m_pToothManager->moveToothAndGum(1.0f, Vec2(0, 120));

        // ボス出現判定
        if(!m_bBossDisp)
        {
            // ボスの生成
            m_bBossDisp = true;
            m_pBoss = Boss::create(Vec2(350,500));
            m_pBoss->setSpawn(Vec2(350,500));
            this->addChild(m_pBoss->getSprite());
        }
    }
    //敵の更新
    m_pEnemyManager->update();
    //ボスの更新
    if(m_pBoss)
    {
    m_pBoss->update();
    }
}

bool HelloWorld::onTouchBegin(Touch* pTouch,Event* pEvent)
{
    // タッチ座標の取得
    m_touchPos = pTouch->getLocation();
    
    // 泡スプライトの追従
    m_bubblePos = m_touchPos;
    m_pBubbleSprite->setPosition(m_bubblePos);
    m_pBubbleSprite->setOpacity(255);
    
    // エネミースプライトのサイズ取得
    Rect enemyRect = m_pEnemySprite->getBoundingBox();

    // エネミーあたり判定
    if(enemyRect.containsPoint(m_touchPos))
    {
        // エネミーを透明にする
        m_pEnemySprite->setOpacity(0);
        m_bEnemyDie = true;
    }
    
    // ボスが出現しているとき
    if(m_bBossDisp)
    {
        // ボススプライトのサイズ取得
        Rect bossSpriteRect = m_pBoss->getSprite()->getBoundingBox();
        
        // ボス当たり判定
        if(bossSpriteRect.containsPoint(m_touchPos) && !m_pBoss->getDisapper())
        {
            // ボスにダメージ
            m_pBoss->addDamage();
        }
    }
    
    return true;
}

void HelloWorld::onTouchMoved(Touch* pTouch,Event* pEvent)
{
    Point swipVec = Point(0,0);

    // タッチ座標の取得
    m_oldTouchPos = m_touchPos;
    m_touchPos = pTouch->getLocation();


    // 泡スプライトの追従
    m_bubblePos = m_touchPos;
    m_pBubbleSprite->setPosition(m_bubblePos);

    CCLOG("現タッチ位置(%f,%f)",m_touchPos.x,m_touchPos.y);
    CCLOG("旧タッチ位置(%f,%f)",m_oldTouchPos.x,m_oldTouchPos.y);

    float fDistance = m_touchPos.distance(m_oldTouchPos);

    CCLOG("距離(%f)",fDistance);

    // スワイプされているならスワイプ方向の取得
    if(fDistance > SWIPE_PERMISSION_DISTANCE)
    {
        Point::subtract(m_touchPos, m_oldTouchPos, &swipVec);
        float fAngle = atan2f(swipVec.x,swipVec.y);
        CCLOG("角度(%f)",fAngle);

        m_oldSwipeDirection = m_swipeDirection;
        m_swipeDirection = calcSwipeDirection(fAngle);

    }

    else
    {
        m_swipeDirection = SWIPE_DIRECTION_NONE;
    }

    if(m_swipeDirection == SWIPE_DIRECTION_NONE)
    {
        return;
    }

    // 当たり判定領域の算出
    Rect bubbleRect = m_pBubbleSprite->getBoundingBox();
    Rect swipeRect;
    swipeRect.setRect(m_touchPos.x - swipVec.x - bubbleRect.size.width / 2 ,m_touchPos.y - swipVec.y  - bubbleRect.size.height / 2,
                      swipVec.x + bubbleRect.size.width, swipVec.y + bubbleRect.size.height);

    
    // 歯垢スプライトのサイズ取得
    Rect plaqueRect = m_pPlaqueSprite->getBoundingBox();

    // 歯垢当たり判定(はじめに一つだけ出したやつ)
    if(plaqueRect.intersectsRect(swipeRect))
    {
        // 歯垢を透明にする
        m_pPlaqueSprite->setOpacity(0);
        m_bPlaqueDie = true;
    }

    // スワイプ時の当たり判定
    m_pHitChecker->hitCheckSwipe(swipeRect, m_swipeDirection);
}

void HelloWorld::onTouchEnded(Touch* pTouch, Event* pEvent)
{
    // タッチしていないので泡スプライトを透明に
    m_pBubbleSprite->setOpacity(0);
    
    m_swipeDirection = SWIPE_DIRECTION_NONE;
    
    m_bHit = false;
}

void HelloWorld::onTouchCancelled(Touch* pTouch, Event* pEvent)
{
    
}

HelloWorld::SWIPE_DIRECTION HelloWorld::calcSwipeDirection(float fAngle)
{
    fAngle = fAngle * 180 / M_PI;

    if(fAngle <= 45 && fAngle >= -45)
    {
        CCLOG("上方向");
        return SWIPE_DIRECTION_UP;
    }

    if(fAngle <= 135 && fAngle >= 45)
    {
        CCLOG("右方向");
        return SWIPE_DIRECTION_RIGHT;
    }

    if(fAngle <= -45 && fAngle >= -135)
    {
        CCLOG("左方向");
        return SWIPE_DIRECTION_LEFT;
    }

    if((fAngle <= -135 && fAngle >= -180) || (fAngle <= 180 && fAngle >= 135))
    {
        CCLOG("下方向");
        return SWIPE_DIRECTION_DOWN;
    }

    return SWIPE_DIRECTION_NONE;
}
