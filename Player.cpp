#include "StdAfx.h"
#include "Player.h"
#include "GlobalFunction.h"
#include "RobotPilot.h"
#include "Judge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Player::Player(void)
	: m_strID(_T(""))
	, m_strName(_T(""))
	, m_emPlayerType(EM_Mid_Player)
	, m_bShowValue(FALSE)
	, m_bLandOwerCardHolder(FALSE)
	, m_bLandOwer(FALSE)
	, m_nLandOwerScore(-1)
	, m_strWord(_T(""))
{
	ShowBound(TRUE);
	SetBoundColor(EM_Color_Player_Bound);
	SetBoundWidth(2);
	ShowBackground(TRUE);
	SetBackColor(EM_Color_Player_Back);
	m_strID = GlobalFunction::GetNewGUID();
}


Player::~Player(void)
{
}

void Player::DrawPlayerHead( CDC * pDC, CRect rtBound )
{
	//将玩家名字绘制在bound中
	int nBackup = pDC->SaveDC();

	//设置画笔画刷
	HGDIOBJ hOldPen = 0;
	HGDIOBJ hOldBrush = 0;
	CPen pen;
	CBrush brush;
	pen.CreatePen(PS_SOLID, 2, EM_Color_Player_Head_Bound);
	hOldPen = pDC->SelectObject(pen);
	brush.CreateSolidBrush(EM_Color_Player_Head_Back);
	hOldBrush = pDC->SelectObject(brush);

	//绘制底色
	pDC->Rectangle(rtBound);
	//绘制名字
	pDC->DrawText(m_strName, rtBound, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//绘制身份
	if (m_bLandOwer)
	{
		pDC->DrawText(_T("地主"), rtBound, DT_CENTER);
	}
	else
	{
		pDC->DrawText(_T("佃户"), rtBound, DT_CENTER);
	}

	pDC->SelectObject(hOldPen);
	pDC->SelectObject(hOldBrush);

	pDC->RestoreDC(nBackup);
}


void Player::DrawCard( CDC * pDC, CRect rtBound)
{
	int nBackup = pDC->SaveDC();

	//设置画笔画刷
	HGDIOBJ hOldPen = 0;
	HGDIOBJ hOldBrush = 0;
	CPen pen;
	CBrush brush;
	pen.CreatePen(PS_SOLID, 2, EM_Color_Player_Card_Bound);
	hOldPen = pDC->SelectObject(pen);
	brush.CreateSolidBrush(EM_Color_Player_Card_Back);
	hOldBrush = pDC->SelectObject(brush);

	//绘制底色
	pDC->Rectangle(rtBound);
	//绘制手牌
	m_stuHandCard.ShowValue(m_bShowValue);
	m_stuHandCard.SetBound(rtBound);
	m_stuHandCard.DrawTo(pDC);

	pDC->SelectObject(hOldPen);
	pDC->SelectObject(hOldBrush);

	pDC->RestoreDC(nBackup);
}

void Player::DrawTo( CDC * pDC )
{
	IDrawable::DrawTo(pDC);

	CRect rtRegion = GetRegion();

	//玩家区域由两块构成，分别是头像和纸牌区，对于斗地主来说，纸牌区只有一个
	CRect rtHead;
	rtHead.SetRect(0, 0, g_nHeadWidth, g_nHeadHeight);

	BOOL bVertical = FALSE;
	if (m_emPlayerType != EM_Mid_Player)
	{
		bVertical = TRUE;
	}
	
	CRect rtCard;
	if (bVertical)
	{
		rtHead.OffsetRect(m_ptPosition.x + (rtRegion.Width() - rtHead.Width()) / 2, m_ptPosition.y + g_nGap);

		rtCard.SetRect(0, 0, g_nCardWidth, rtRegion.Height() - rtHead.Height() - g_nGap * 3);
		rtCard.OffsetRect(rtRegion.left + g_nGap, rtHead.bottom + g_nGap);
	}
	else
	{
		rtHead.OffsetRect(m_ptPosition.x + g_nGap, m_ptPosition.y + (rtRegion.Height() - rtHead.Height()) / 2);

		rtCard.SetRect(0, 0, rtRegion.Width() - rtHead.Width() - g_nGap * 3, g_nCardHeight);
		rtCard.OffsetRect(rtHead.right + g_nGap, rtRegion.top + g_nGap);
	}

	DrawPlayerHead(pDC, rtHead);
	DrawCard(pDC, rtCard);

	//绘制文字
	CSize szWord = pDC->GetTextExtent(m_strWord);
	CRect rtWord;
	switch(m_emPlayerType)
	{
	case EM_Left_Player:
		rtWord.left = rtCard.right + g_nGap;
		rtWord.top = rtHead.top + g_nGap;
		rtWord.right = rtWord.left + szWord.cx;
		rtWord.bottom = rtWord.top + szWord.cy;
		break;
	case EM_Mid_Player:
		rtWord.left = rtHead.left + g_nGap;
		rtWord.top = rtCard.top - g_nGap - szWord.cy;
		rtWord.right = rtWord.left + szWord.cx;
		rtWord.bottom = rtWord.top + szWord.cy;
		break;
	case EM_Right_Player:
		rtWord.left = rtCard.left - g_nGap - szWord.cx;
		rtWord.top = rtHead.top + g_nGap;
		rtWord.right = rtWord.left + szWord.cx;
		rtWord.bottom = rtWord.top + szWord.cy;
		break;
	default:
		break;
	}
	pDC->DrawText(m_strWord, rtWord, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void Player::ExcuteCallLandOwer()
{
	//如果是AI，则拿着ID问AI应该怎么办
	if (IsAIPlayer())
	{
		int nScore = RobotPilot::Instance().DecideCallLandOwer(m_strID);
		Judge::Instance().CurPlayerCallScore(nScore);
	}
	//否则绘制按钮，由玩家操作
	else
	{
		Judge::Instance().ShowCallLandOwerBtn(TRUE);
	}
}

void Player::ExcuteCallCardPlay()
{
	//如果是AI，则拿着ID问AI应该怎么办
	if (IsAIPlayer())
	{
		vector<int> vecCardToPlay = RobotPilot::Instance().DecideCardToPlay(m_strID);
		SetOnHoldCard(vecCardToPlay);
		Judge::Instance().CurPlayerPlayCard();
	}
	//否则绘制按钮，由玩家操作
	else
	{
		Judge::Instance().ShowCardPlayBtn(TRUE);
	}
}
