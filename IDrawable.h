#pragma once
//////////////////////////////////////////////////////////////////////////
//绘制接口
//默认已实现了背底及边界的绘制
//////////////////////////////////////////////////////////////////////////

//颜色定义
enum EMColorDefine
{
	EM_Color_Whole_Bound = RGB(0, 90, 0),					//整体显示区边框色
	EM_Color_Whole_Back = RGB(0, 90, 0),					//整体显示区背景色
	EM_Color_Player_Bound = RGB(0, 125, 20),					//玩家显示区边框色
	EM_Color_Player_Back = RGB(0, 50, 50),						//玩家显示区背景色
	EM_Color_Player_Head_Bound = RGB(125, 125, 125),			//玩家头像边框色
	EM_Color_Player_Head_Back = RGB(50, 50, 50),				//玩家头像背景色
	EM_Color_Player_Card_Bound = RGB(200, 200, 200),			//玩家纸牌区边框色
	EM_Color_Player_Card_Back = RGB(150, 150, 150),				//玩家纸牌区背景色
	EM_Color_PokerCard_Bound = RGB(0, 50, 50),					//纸牌背面边框颜色
	EM_Color_PokerCard_Back = RGB(0, 200, 50),					//纸牌背面背景颜色
	EM_Color_PokerCard_Value_Bound = RGB(125, 125, 150),		//纸牌正面边框颜色
	EM_Color_PokerCard_Value_Back = RGB(255, 255, 255),			//纸牌正面背景颜色
	EM_Color_Button_Bound = RGB(200, 200, 200),					//按钮边框颜色
	EM_Color_Button_Back = RGB(200, 200, 200),					//按钮背景颜色
};

class IDrawable
{
protected:
	int m_nWidth;//绘制区宽
	int m_nHeight;//绘制区高
	CPoint m_ptPosition;//绘制区位置
	//背底
	BOOL m_bDrawBackground;
	COLORREF m_nBackColor;
	//边界
	BOOL m_bDrawBound;
	COLORREF m_nBoundColor;
	int m_nBoundWidth;
public:
	IDrawable(void);
	virtual ~IDrawable(void) = 0;
	virtual void DrawTo(CDC * pDC) = 0;//绘制逻辑
	void SetBound(CRect rtBound);
	void SetWidth(int nWidth);
	void SetHeight(int nHeight);
	void SetPosition(CPoint ptPos);
	int GetWidth();
	int GetHeight();
	CPoint GetPosition();
	CRect GetRegion();
	void ShowBackground(BOOL bEnabled = TRUE){m_bDrawBackground = bEnabled;}
	void SetBackColor(COLORREF nColor){m_nBackColor = nColor;}
	void ShowBound(BOOL bEnabled = TRUE){m_bDrawBound = bEnabled;}
	void SetBoundColor(COLORREF nColor){m_nBoundColor = nColor;}
	void SetBoundWidth(int nWidth){m_nBoundWidth = nWidth;}
};
