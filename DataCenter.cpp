#include "StdAfx.h"
#include "DataCenter.h"
#include "GlobalFunction.h"
#include "PokerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DataCenter::DataCenter(void)
	: m_emPlayState(EM_WaitToStart_PlayState)
{
	//设置色调
	ShowBound(TRUE);
	SetBoundColor(EM_Color_Whole_Bound);
	SetBoundWidth(1);
	ShowBackground(TRUE);
	SetBackColor(EM_Color_Whole_Back);

	//初始化牌堆
	//除了双王，每个Value各插4张
	for (int i = 0; i < g_nCardDispModel - 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			PokerCard * pCard = NULL;
			pCard = new PokerCard;
			pCard->SetValue(i);
			m_stuMainCard.PushBackCard(pCard);
			pCard->SetID(m_stuMainCard.GetCardPile().size());
			pCard->SetCallBackFunc(CPokerDlg::OnCardClicked);
		}
	}
	//然后插两个王
	{
		PokerCard * pCard = NULL;
		pCard = new PokerCard;
		pCard->SetValue(g_nCardDispModel - 2);
		m_stuMainCard.PushBackCard(pCard);
		pCard->SetID(m_stuMainCard.GetCardPile().size());
		pCard->SetCallBackFunc(CPokerDlg::OnCardClicked);
	}
	{
		PokerCard * pCard = NULL;
		pCard = new PokerCard;
		pCard->SetValue(g_nCardDispModel - 1);
		m_stuMainCard.PushBackCard(pCard);
		pCard->SetID(m_stuMainCard.GetCardPile().size());
		pCard->SetCallBackFunc(CPokerDlg::OnCardClicked);
	}
	//一共54张牌
	ASSERT(m_stuMainCard.GetCardPile().size() == 54);

	//三个玩家
	Player pL;
	Player pM;
	Player pR;

	pL.SetPlayerType(EM_Left_Player);
	pL.SetName(_T("张大妈"));
	pL.ShowValue(FALSE);

	pM.SetPlayerType(EM_Mid_Player);
	pM.SetName(_T("李大婶"));
	pM.ShowValue(TRUE);

	pR.SetPlayerType(EM_Right_Player);
	pR.SetName(_T("王大爷"));
	pR.ShowValue(FALSE);

	m_vecPlayer.push_back(pL);
	m_vecPlayer.push_back(pM);
	m_vecPlayer.push_back(pR);

	//场牌不绘制背景色及边框
	m_stuCurCard.ShowBound(FALSE);
	m_stuCurCard.ShowBackground(FALSE);
}


DataCenter::~DataCenter(void)
{
	//游戏结束的时候销毁牌
	Reset();//将牌归到主牌堆
	GlobalFunction::ReleasePointorContainer(m_stuMainCard.GetCardPile());
}

DataCenter & DataCenter::Instance()
{
	static DataCenter g_dataCenter;
	return g_dataCenter;
}

void DataCenter::DrawTo(CDC * pDC)
{
	//绘制全局背景
	IDrawable::DrawTo(pDC);

	//绘制玩家（斗地主是三人局）
	CRect rtClient(0, 0, m_nWidth, m_nHeight);
	for (UINT i = 0; i < m_vecPlayer.size(); i++)
	{
		switch (m_vecPlayer[i].GetPlayerType())
		{
		case EM_Left_Player:
			m_vecPlayer[i].SetWidth(g_nCardWidth + g_nGap * 2);
			m_vecPlayer[i].SetHeight(rtClient.Height() - g_nCardHeight - g_nGap * 5);
			m_vecPlayer[i].SetPosition(CPoint(g_nGap, g_nGap));
			break;
		case EM_Right_Player:
			m_vecPlayer[i].SetWidth(g_nCardWidth + g_nGap * 2);
			m_vecPlayer[i].SetHeight(rtClient.Height() - g_nCardHeight - g_nGap * 5);
			m_vecPlayer[i].SetPosition(CPoint(rtClient.right - g_nCardWidth - g_nGap * 3, g_nGap));
			break;
		case EM_Mid_Player:
			m_vecPlayer[i].SetWidth(rtClient.Width() - g_nGap * 2);
			m_vecPlayer[i].SetHeight(g_nCardHeight + g_nGap * 2);
			m_vecPlayer[i].SetPosition(CPoint(g_nGap, rtClient.Height() - g_nCardHeight - g_nGap * 3));
			break;
		default:
			break;
		}
		m_vecPlayer[i].DrawTo(pDC);
	}

	//绘制场牌(居中)
	CSize sizeCard = m_stuCurCard.GetCardSize();
	m_stuCurCard.SetPosition(CPoint((rtClient.Width() - sizeCard.cx) / 2, (rtClient.Height() - sizeCard.cy) / 2));
	m_stuCurCard.DrawTo(pDC);

	//绘制地主牌(放在玩家旁边)
	//当地主被确定出来以后，且地主牌值有效，则绘制地主牌
	if(m_stuLandOwnerCard.GetValue() < g_nCardDispModel)
	{
		for (UINT i = 0; i < m_vecPlayer.size(); i++)
		{
			if (m_vecPlayer[i].IsLandOwerCardHolder())
			{
				CRect rtPlayer = m_vecPlayer[i].GetRegion();
				switch(m_vecPlayer[i].GetPlayerType())
				{
				case EM_Left_Player:
					m_stuLandOwnerCard.SetPosition(CPoint(rtPlayer.right + g_nGap, rtPlayer.top + (rtPlayer.Height() - g_nCardHeight) / 2));
					break;
				case EM_Mid_Player:
					m_stuLandOwnerCard.SetPosition(CPoint(rtPlayer.left + (rtPlayer.Width() - g_nCardWidth) / 2, rtPlayer.top - g_nCardHeight - g_nGap));
					break;
				case EM_Right_Player:
					m_stuLandOwnerCard.SetPosition(CPoint(rtPlayer.left - g_nCardWidth - g_nGap, rtPlayer.top + (rtPlayer.Height() - g_nCardHeight) / 2));
					break;
				default:
					break;
				}
				m_stuLandOwnerCard.DrawTo(pDC);
				break;
			}
		}
	}

	//绘制底牌（居上）
	if (m_stuBottomCard[0].GetValue() < g_nCardDispModel)
	{
		m_stuBottomCard[0].SetPosition(CPoint(rtClient.left + (rtClient.Width() - g_nCardWidth * 3 - g_nGap * 2) / 2, g_nGap));
		m_stuBottomCard[0].DrawTo(pDC);
	}
	if (m_stuBottomCard[1].GetValue() < g_nCardDispModel)
	{
		m_stuBottomCard[1].SetPosition(CPoint(rtClient.left + (rtClient.Width() - g_nCardWidth * 3 - g_nGap * 2) / 2 + g_nCardWidth + g_nGap, g_nGap));
		m_stuBottomCard[1].DrawTo(pDC);
	}
	if (m_stuBottomCard[2].GetValue() < g_nCardDispModel)
	{
		m_stuBottomCard[2].SetPosition(CPoint(rtClient.left + (rtClient.Width() - g_nCardWidth * 3 - g_nGap * 2) / 2 + g_nCardWidth * 2 + g_nGap * 2, g_nGap));
		m_stuBottomCard[2].DrawTo(pDC);
	}
}

void DataCenter::Reset()
{
	//场牌时钟显示
	m_stuCurCard.ShowValue(TRUE);
	//清除地主牌及底牌
	m_stuLandOwnerCard.SetValue(-1);
	m_stuLandOwnerCard.ShowValue(FALSE);
	m_stuLandOwnerCard.SelectCard(FALSE);
	m_stuBottomCard[0].SetValue(-1);
	m_stuBottomCard[0].ShowValue(FALSE);
	m_stuBottomCard[0].SelectCard(FALSE);
	m_stuBottomCard[1].SetValue(-1);
	m_stuBottomCard[1].ShowValue(FALSE);
	m_stuBottomCard[1].SelectCard(FALSE);
	m_stuBottomCard[2].SetValue(-1);
	m_stuBottomCard[2].ShowValue(FALSE);
	m_stuBottomCard[3].SelectCard(FALSE);

	ICard * pCard = NULL;
	//回收玩家手中的牌(手牌及待选牌)，清除玩家身份标记
	for (UINT i = 0; i < m_vecPlayer.size(); i++)
	{
		m_vecPlayer[i].SetLandOwer(FALSE);
		m_vecPlayer[i].SetLandOwerCardHolder(FALSE);
		m_vecPlayer[i].SetLandOwerScore(-1);
		m_vecPlayer[i].Say(_T(""));
		m_vecPlayer[i].GetOnHoldCard().clear();
		CardPile & handCard = m_vecPlayer[i].GetHandCard();
		while(pCard = handCard.PopCard())
		{
			pCard->SelectCard(FALSE);
			m_stuMainCard.PushBackCard(pCard);
		}
	}
	m_vecPlayer[0].ShowValue(FALSE);
	m_vecPlayer[1].ShowValue(TRUE);
	m_vecPlayer[2].ShowValue(FALSE);
	//回收弃牌堆里的牌
	while(pCard = m_stuAbordCard.PopCard())
	{
		pCard->SelectCard(FALSE);
		m_stuMainCard.PushBackCard(pCard);
	}
	//回收场牌
	while(pCard = m_stuCurCard.PopCard())
	{
		pCard->SelectCard(FALSE);
		m_stuMainCard.PushBackCard(pCard);
	}
	ASSERT(m_stuMainCard.GetCardPile().size() == 54);
	//洗牌
	m_stuMainCard.RandonSortCard();
}

void DataCenter::SendOutCard()
{
	ASSERT(m_stuMainCard.GetCardPile().size() == 54);
	int nLandOwerCard = rand() % 54;
	m_stuLandOwnerCard.SetValue(m_stuMainCard.GetCard(nLandOwerCard)->GetValue());
	m_stuLandOwnerCard.ShowValue(TRUE);
	
	m_vecPlayer[nLandOwerCard % 3].SetLandOwerCardHolder(TRUE);

	m_stuBottomCard[0].SetValue(m_stuMainCard.GetCard(0)->GetValue());
	m_stuBottomCard[0].ShowValue(FALSE);
	m_stuBottomCard[1].SetValue(m_stuMainCard.GetCard(1)->GetValue());
	m_stuBottomCard[1].ShowValue(FALSE);
	m_stuBottomCard[2].SetValue(m_stuMainCard.GetCard(2)->GetValue());
	m_stuBottomCard[2].ShowValue(FALSE);

	ICard * pCard = NULL;
	for (UINT i = 0; i < 51; i++)
	{
		m_vecPlayer[i % 3].PushBackCard(m_stuMainCard.PopCard());
	}

	//给玩家牌排序
	for(UINT i = 0; i < m_vecPlayer.size(); i++)
	{
		m_vecPlayer[i].GetHandCard().SortCard();
	}
}

void DataCenter::SendOutBottomCard()
{
	for (UINT i = 0; i < m_vecPlayer.size(); i++)
	{
		if (m_vecPlayer[i].IsLandOwer())
		{
			m_vecPlayer[i].PushBackCard(m_stuMainCard.PopCard());
			m_vecPlayer[i].PushBackCard(m_stuMainCard.PopCard());
			m_vecPlayer[i].PushBackCard(m_stuMainCard.PopCard());
			m_vecPlayer[i].GetHandCard().SortCard();
			break;
		}
	}
	ASSERT(m_stuMainCard.GetCardPile().size() == 0);

	m_stuBottomCard[0].ShowValue(TRUE);
	m_stuBottomCard[1].ShowValue(TRUE);
	m_stuBottomCard[2].ShowValue(TRUE);
	m_stuLandOwnerCard.SetValue(-1);
}

BOOL DataCenter::IsOver(BOOL & bLandOwerWin)
{
	BOOL bRet = FALSE;
	//结束的条件是，游戏状态为出牌阶段，某人的手牌数为0
	if (GetPlayState() == EM_WaitPlayer_PlayState)
	{
		for (UINT i = 0; i < m_vecPlayer.size(); i++)
		{
			if (m_vecPlayer[i].GetCardCount() == 0)
			{
				bRet = TRUE;
				if(m_vecPlayer[i].IsLandOwer())
				{
					bLandOwerWin = TRUE;
				}
				else
				{
					bLandOwerWin = FALSE;
				}
				break;
			}
		}
	}
	return bRet;
}

Player * DataCenter::GetLandOwnerCardHolder()
{
	Player * pPlayer = NULL;
	vector<Player> & vecPlayer = DataCenter::Instance().GetPlayerList();
	for (UINT i = 0; i < vecPlayer.size(); i++)
	{
		if (vecPlayer[i].IsLandOwerCardHolder())
		{
			pPlayer = &vecPlayer[i];
			break;
		}
	}
	return pPlayer;
}

Player * DataCenter::GetLandOwner()
{
	Player * pPlayer = NULL;
	for (UINT i = 0; i < m_vecPlayer.size(); i++)
	{
		if (m_vecPlayer[i].IsLandOwer())
		{
			pPlayer = &m_vecPlayer[i];
			break;
		}
	}
	return pPlayer;
}

void DataCenter::ShowAllPlayerCard()
{
	for (UINT i = 0; i < m_vecPlayer.size(); i++)
	{
		m_vecPlayer[i].ShowValue(TRUE);
	}
}

