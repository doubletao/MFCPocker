#pragma once
//////////////////////////////////////////////////////////////////////////
// 卡片基类，包含一张牌
//////////////////////////////////////////////////////////////////////////
#include "IDrawable.h"
#include "ISortable.h"
#include "IClickable.h"

const int g_nCardWidth = 90;
const int g_nCardHeight = 120;

class ICard
    : public IDrawable
    , public ISortable
    , public IClickable
{
protected:
    bool m_bShowValue;
    unsigned int m_nValue;
    bool m_bSelect;
public:
    ICard(void);
    virtual ~ICard(void);
    void SetValue(unsigned int nValue) { m_nValue = nValue; }  // 设置牌值
    unsigned int GetValue() { return m_nValue; }  // 获取牌值
    void ShowValue(bool bShowValue = true) { m_bShowValue = bShowValue; }  // 是否显示牌值
    virtual int GetSortFactor() const override = 0;  // 获取排序因子
    virtual void DrawTo(QPainter *painter) override = 0;  // 绘制接口
    void SelectCard(bool bSet);  // 设置选择状态
};
