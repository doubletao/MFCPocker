#pragma once
//////////////////////////////////////////////////////////////////////////
//用GDI画出来的button
//////////////////////////////////////////////////////////////////////////
#include "IDrawable.h"
#include "IClickable.h"

enum EMButtonID
{
	EM_PASS_SCORE_BUTTON,	//不叫
	EM_ONE_SCORE_BUTTON,	//一分
	EM_TWO_SCORE_BUTTON,	//两分
	EM_THREE_SCORE_BUTTON,	//三分

	EM_PASS_CARD_BUTTON,	//不要
	EM_SEND_CARD_BUTTON,	//出牌
};

const int g_nButtonWidth = 70;
const int g_nButtonHeight = 25;
const int g_nButtonGap = 5;
class GDIButton
	: public IDrawable
	, public IClickable
{
private:
	CString m_strBtnTxt;//按钮文本
	BOOL m_bShow;//是否显示
public:
	GDIButton(void);
	~GDIButton(void);
	virtual BOOL HitTest(CPoint pt);//点击接口
	virtual void DrawTo(CDC * pDC);//绘制逻辑
	void SetButtonTxt(CString strBtnTxt){m_strBtnTxt = strBtnTxt;}//设置按钮文本
	void ShowButton(BOOL bShow){m_bShow = bShow;}
	BOOL IsShow(){return m_bShow;}//返回是否处于显示状态
};

