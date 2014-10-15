//
//  Number.cpp
//  ToothBrushGame
//
//  Created by 鈴木 愛忠 on 2014/10/15.
//
//

//********************************************************************************
// インクルード
//********************************************************************************
#include "Number.h"

//================================================================================
// コンストラクタ
//================================================================================
Number::Number(void)
{
    // メンバ変数の初期化
    m_pSprite = nullptr;
}

//================================================================================
// デストラクタ
//================================================================================
Number::~Number()
{
    
}

//================================================================================
// 初期化処理
//================================================================================
bool Number::init(void)
{
    // スプライトの作成
    m_pSprite = Sprite::create("tooth_01.png");
    
    // エラーチェック
    if(m_pSprite == nullptr)
    {
        // スプライト生成エラー
        return false;
    }
    
    // スプライトの座標設定
    m_pSprite->setPosition(m_pos);
    
    // 正常終了
    return true;
}

//================================================================================
// 終了処理
//================================================================================
void Number::uninit(void)
{
    
}

//================================================================================
// 更新処理
//================================================================================
void Number::update(void)
{
    //m_pNumberSprite->setPosition(m_pos);
    
    
}
//================================================================================
// 数字代入処理
//================================================================================
void Number::setNumber(int number)
{
    
}
//================================================================================
// 生成処理
//================================================================================
Number* Number::create(const Vec2& pos)
{
    // インスタンスの生成
    Number* pNumber = new Number();
    
    // メンバ変数の代入
    pNumber->m_pos = pos;
    
    // 初期化
    pNumber->init();
    
    return pNumber;
}