#pragma once
#include "ICard.h"
#include "IClickable.h"

const int g_nCardDispModel = 15;
const CString g_strCardDisp[] = {_T("3"), _T("4"), _T("5"), _T("6"), _T("7"), 
_T("8"), _T("9"), _T("10"), _T("J"), _T("Q"), _T("K"), _T("A"), _T("2"), _T("j\no\nk\ne\nr"), _T("J\nO\nK\nE\nR")};

class PokerCard
	: public ICard
{
public:
	PokerCard(void);
	virtual ~PokerCard(void);
	CString GetDisplayTxt();//获取牌面文字
	virtual int GetSortFactor() const{return m_nValue;}//获取排序因子
	virtual void DrawTo(CDC * pDC);//绘制逻辑
	static BOOL ValidValue(int nValue);
	virtual BOOL HitTest(CPoint pt);//点击接口
};

