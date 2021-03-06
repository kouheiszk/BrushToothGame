//
//  ToothManager.h
//  test
//
//  Created by 丸山 潤 on 2014/10/08.
//
//

#ifndef __test__ToothManager__
#define __test__ToothManager__

//********************************************************************************
// インクルード
//********************************************************************************
#include "cocos2d.h"

//********************************************************************************
// 名前空間の使用
//********************************************************************************
using namespace cocos2d;

//********************************************************************************
// 前方クラス宣言
//********************************************************************************
class Tooth;
class Gum;

//********************************************************************************
// クラス宣言
//********************************************************************************
class ToothManager
{
public:
    ToothManager();
    ~ToothManager();
    
    bool init(void);
    void uninit(void);
    void update(void);
    
    static ToothManager* create(const Vec2& startLeftTopPos,Layer* layer);
    
    Tooth* getTopTooth(void){return m_pTopTooth;}
    Tooth* getBootomTooth(void){return m_pBottomTooth;}
    
    void addTopGumPosAndTopToothPos(const Vec2& vec);
    void addBottomGumPosAndBottomToothPos(const Vec2& vec);
    void moveToothAndGum(float time, Vec2 pos);
    
private:
    Vec2 m_startLeftTopPos;

    Tooth* m_pTopTooth;
    Tooth* m_pBottomTooth;
    
    Gum* m_pTopGum;
    Gum* m_pBottomGum;
    
    Layer* m_pLayer;
};
#endif /* defined(__test__ToothManager__) */
