#pragma once
//////////////////////////////////////////////////////////////////////////
// 卡片基类，包含一张牌
//////////////////////////////////////////////////////////////////////////
#include "IDrawable.h"
#include "ISortable.h"
#include "IClickable.h"
#include <QtGlobal>

const int g_nCardWidth = 90;
const int g_nCardHeight = 120;

// 花色枚举
enum class CardSuit : quint8
{
    Spade,      // 黑桃 ♠ - 黑色
    Heart,      // 红桃 ♥ - 红色
    Club,       // 梅花 ♣ - 黑色
    Diamond,    // 方块 ♦ - 红色
    JokerSmall, // 小王 - 红色
    JokerBig    // 大王 - 黑色
};

class ICard
    : public IDrawable
    , public ISortable
    , public IClickable
{
protected:
    bool m_bShowValue;
    unsigned int m_nValue;
    CardSuit m_suit;
    bool m_bSelect;
public:
    ICard(void);
    virtual ~ICard(void);
    void SetValue(unsigned int nValue) { m_nValue = nValue; }  // 设置牌值
    unsigned int GetValue() { return m_nValue; }  // 获取牌值
    void SetSuit(CardSuit suit) { m_suit = suit; }  // 设置花色
    CardSuit GetSuit() const { return m_suit; }  // 获取花色
    bool IsRedSuit() const {
        // 红桃、方块、小王 是红色
        return m_suit == CardSuit::Heart || m_suit == CardSuit::Diamond || m_suit == CardSuit::JokerSmall;
    }
    void ShowValue(bool bShowValue = true) { m_bShowValue = bShowValue; }  // 是否显示牌值
    virtual int GetSortFactor() const override = 0;  // 获取排序因子
    virtual void DrawTo(QPainter *painter) override = 0;  // 绘制接口
    void SelectCard(bool bSet);  // 设置选择状态
};
