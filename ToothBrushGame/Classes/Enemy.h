//
//  Enemy.h
//  ToothBrushGame
//
//  Created by 川原 岳大 on 2014/10/14.
//
//

#ifndef __ToothBrushGame__Enemy__
#define __ToothBrushGame__Enemy__

#include <stdio.h>
#include "cocos2d.h"
#include <random>

using namespace cocos2d;


class Enemy
{
private:
    enum ACTION_MODE
    {
        ACTION_NONE = 0,
        ACTION_MOVE,
        ACTION_ATTACK,
        ACTION_DELAY,
        ACTION_MAX
    };

public:
    Enemy();   //コンストラクタ
    ~Enemy();  //デストラクタ

    bool init();    //初期設定
    void uninit();  //
    void update();  //更新
    void disappear();   //消滅

    static Enemy* create(const Vec2& pos = Vec2(0.0f,0.0f));
    Sprite* getSprite(void){return m_pSprite;}
    
    //セッター　ゲッター
    void setPos(Vec2 pos){m_pos = pos;}
    void setPos(float x,float y){m_pos.x = x;m_pos.y = y;}
    void setPosX(float x){m_pos.x = x;}
    void setPosY(float y){m_pos.y = y;}
    void addPos(Vec2 pos){m_pos += pos;}
    void addPos(float x,float y){m_pos.x += x;m_pos.y += y;}
    void addPosX(float x){m_pos.x += x;}
    void addPosY(float y){m_pos.y += y;}
    Vec2 getPos(void){return m_pos;}
    bool getDisapper(void){return m_bDeath;}
    //行動選択関数
    void choiceAction(void);
    
    //移動
    void moveAction(void);
    //攻撃
    void attackAction(void);
    //待機
    void delayAction(void);
    
    //位置を画像に適用
    void refreshSpritePos(void){m_pSprite->setPosition(m_pos);}
    //出現
    void setSpawn(Vec2 pos);
    //ダメージ処理
    void addDamage(int nDamage = 1);

private:
    void (Enemy::*m_pFunc[ACTION_MAX])();	// 関数ポインタ
    Sprite* m_pSprite;
    Vec2 m_pos;
    Vec2 m_move;
    Vec3 m_rot;
    //時間
    int m_time;
    //行動状態
    int m_actionMode;
    //生きてるか死んでるかのフラグ
    bool m_bDeath;
    //HP
    int m_nLife;
//定数
public:
    //ライフの最大値
    static const int MAX_LIFE = 3;
    //時間の最大値
    static const int MAX_TIME = 100;
    //移動の最大値
    static const int MIN_X = 0;
    static const int MAX_X = 600;
    static const int MIN_Y = 200;
    static const int MAX_Y = 900;
    static const int MAX_MOVE = 20;
    static const int DAMEGE = 1;
    static const unsigned short OPACITY_SPEED = 255;

};



#endif /* defined(__ToothBrushGame__Enemy__) */
