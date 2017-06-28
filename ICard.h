#pragma once
//////////////////////////////////////////////////////////////////////////
//牌的基类，代表一张牌
//////////////////////////////////////////////////////////////////////////
#include "IDrawable.h"
#include "ISortable.h"
#include "IClickable.h"

const int g_nCardWidth = 80;
const int g_nCardHeight = 100;

class ICard
	: public IDrawable
	, public ISortable
	, public IClickable
{
protected:
	BOOL m_bShowValue;
	UINT m_nValue;
	BOOL m_bSelect;
public:
	ICard(void);
	virtual ~ICard(void);
	void SetValue(UINT nValue){m_nValue = nValue;}//设置牌值
	UINT GetValue(){return m_nValue;}//获取牌值
	void ShowValue(BOOL bShowValue = TRUE){m_bShowValue = bShowValue;}//是否显示牌面
	virtual int GetSortFactor() const = 0;//获取排序因子
	virtual void DrawTo(CDC * pDC) = 0;//绘制逻辑
	void SelectCard(BOOL bSet);//对卡牌取反色
};
