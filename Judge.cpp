#include "StdAfx.h"
#include "Judge.h"
#include "DataCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Judge::Judge(void)
	: m_hMsgWnd(NULL)
	, m_pCurCardPlayer(NULL)
	, m_pCurPlayer(NULL)
	, m_pToBeLandOwer(NULL)
	, m_nCurHighstScore(0)
{
}


Judge::~Judge(void)
{
}

Judge & Judge::Instance()
{
	static Judge g_Instance;
	return g_Instance;
}

void Judge::Restart()
{
	BOOL bRestart = FALSE;
	if (DataCenter::Instance().GetPlayState() != EM_WaitToStart_PlayState)
	{
		if(IDYES==AfxMessageBox(_T("是否重新开始？"),MB_ICONQUESTION | MB_YESNO))
		{
			bRestart = TRUE;
		}
	}
	else
	{
		bRestart = TRUE;
	}
	if (bRestart)
	{
		m_pCurPlayer = NULL;
		m_pCurCardPlayer = NULL;
		m_pToBeLandOwer = NULL;
		m_nCurHighstScore = 0;
		DataCenter::Instance().Reset();
		DataCenter::Instance().SendOutCard();
		DataCenter::Instance().SetPlayState(EM_LandHolderBorn_PlayState);
		RefreshView();
		//开始主要流程
		MainFlow();
	}
}

void Judge::MainFlow()
{
	switch(DataCenter::Instance().GetPlayState())
	{
	case EM_LandHolderBorn_PlayState:
		{
			//先检查是否已经问完了
			//遍历玩家检查是否已经询问过了，如果已经都问过了，则设置叫分最高的为地主
			BOOL bAllAsked = TRUE;//是否已经询问完了
			vector<Player> & vecPlayer = DataCenter::Instance().GetPlayerList();
			for (UINT i = 0; i < vecPlayer.size(); i++)
			{
				//统计当前最高分
				if (vecPlayer[i].GetLandOwerScore() > m_nCurHighstScore)
				{
					m_nCurHighstScore = vecPlayer[i].GetLandOwerScore();
					m_pToBeLandOwer = &vecPlayer[i];
				}
				if (vecPlayer[i].GetLandOwerScore() < 0)
				{
					bAllAsked = FALSE;
				}
			}
			//如果已经全部问过了，则设置当前最高分的为地主
			if (bAllAsked)
			{
				m_pToBeLandOwer->SetLandOwer(TRUE);
			}
			//然后根据情况执行询问流程
			//如果地主已经产生，则跳入下一阶段
			if (NULL != DataCenter::Instance().GetLandOwner())
			{
				m_pCurPlayer = NULL;
				DataCenter::Instance().SetPlayState(EM_WaitPlayer_PlayState);
				MainFlow();
				return;
			}
			//如果当前player为空，设置当前player为地主牌得主
			if (m_pCurPlayer == NULL)
			{
				m_pCurPlayer = DataCenter::Instance().GetLandOwnerCardHolder();
			}
			//对当前玩家执行地主问询
			ASSERT(m_pCurPlayer);
			m_pCurPlayer->ExcuteCallLandOwer();
		}
		break;
	case EM_WaitPlayer_PlayState:
		{
			//如果游戏已经结束，则执行结束逻辑
			BOOL bLandOwerWin = FALSE;
			if (DataCenter::Instance().IsOver(bLandOwerWin))
			{
				if (bLandOwerWin)
				{
					AfxMessageBox(_T("地主赢了!"));
				}
				else
				{
					AfxMessageBox(_T("佃户赢了!"));
				}
				DataCenter::Instance().SetPlayState(EM_WaitToStart_PlayState);
				//将所有玩家明牌
				DataCenter::Instance().ShowAllPlayerCard();
				RefreshView();
				return;
			}
			//如果是出牌阶段而当前player为空，设置当前player为地主，并发予底牌
			if (m_pCurPlayer == NULL)
			{
				m_pCurPlayer = DataCenter::Instance().GetLandOwner();
				DataCenter::Instance().SendOutBottomCard();
				RefreshView();
			}
			ASSERT(m_pCurPlayer);
			m_pCurPlayer->ExcuteCallCardPlay();
		}
		break;
	}
}

void Judge::RefreshView()
{
	if (::IsWindow(m_hMsgWnd))
	{
		SendMessage(m_hMsgWnd, MSG_REFRESH_VIEW, 0, 0);
	}
}

Player * Judge::SwitchToNextPlayer()
{
	Player * pPlayer = NULL;
	CString strCurPlayerID = m_pCurPlayer->GetID();
	vector<Player> & vecPlayer = DataCenter::Instance().GetPlayerList();
	for (UINT i = 0; i < vecPlayer.size(); i++)
	{
		if (strCurPlayerID == vecPlayer[i].GetID())
		{
			pPlayer = &vecPlayer[(i + 1) % vecPlayer.size()];
			m_pCurPlayer = pPlayer;
			break;
		}
	}
	return pPlayer;
}

void Judge::CurPlayerCallScore(int nScore)
{
	if (m_pCurPlayer == NULL)
	{
		ASSERT(FALSE);
		return;
	}
	//将玩家选择的分数设置给玩家
	m_pCurPlayer->SetLandOwerScore(nScore);
	//如果当前玩家为空，直接返回
	if(m_pCurPlayer == NULL)
	{
		return;
	}
	if (nScore == 3)
	{
		//如果玩家叫了三分，直接设为地主
		m_pCurPlayer->SetLandOwer(TRUE);
	}
	else
	{
		//玩家叫的不是三分，则记下玩家叫的分数
		m_pCurPlayer->SetLandOwerScore(nScore);
	}
	if (nScore == 0)
	{
		CString strWord;
		strWord.Format(_T("不叫"));
		m_pCurPlayer->Say(strWord);
	}
	else
	{
		CString strWord;
		strWord.Format(_T("%d分"), nScore);
		m_pCurPlayer->Say(strWord);
	}
	//玩家叫分后隐藏叫地主按钮
	Judge::Instance().ShowCallLandOwerBtn(FALSE);
	//切换到下一个玩家，流程继续
	SwitchToNextPlayer();
	MainFlow();
}

void Judge::CurPlayerPlayCard()
{
	if (m_pCurPlayer == NULL)
	{
		ASSERT(FALSE);
		return;
	}
	vector<int> vecCardIndex = m_pCurPlayer->GetOnHoldCard();
	if (vecCardIndex.size() > 0)
	{
		//提取当前场牌堆
		CardGroup & curGroup = DataCenter::Instance().GetCurCardGroup();
		//提取当前弃牌堆
		CardPile & stuAbordCard = DataCenter::Instance().GetAbortCardPile();
		//取得并打出当前玩家的OnHold牌
		CardGroup onHoldCard = m_pCurPlayer->PlayOutCard();
		//查一下玩家打出的牌类型
		EMCardGroupType emType = onHoldCard.CheckPile();
		ASSERT(emType != EM_CardGroup_Illegal);//在玩家出牌时已经做过一次判定了，这里仅断言一下
		//玩家打牌的同时说出自己打出的牌
		{
			vector<ICard *> cardPile = onHoldCard.GetCardPile();
			CString strWord = g_strType[emType];
			strWord.Append(_T(":"));
			for (UINT i = 0; i < cardPile.size(); i++)
			{
				//这样写不合适，应该提供接口来获取牌面值，暂时这样写
				strWord.AppendFormat(_T("%s "), g_strCardDisp[cardPile[i]->GetValue()]);
			}
			m_pCurPlayer->Say(strWord);
		}
		//将当前场牌加入到弃牌堆
		ICard * pCard = NULL;
		while(pCard = curGroup.PopCard())
		{
			pCard->SelectCard(FALSE);
			stuAbordCard.PushBackCard(pCard);
		}
		//将玩家OnHold牌加入到场牌
		while(pCard = onHoldCard.PopCard())
		{
			pCard->SelectCard(FALSE);
			curGroup.PushBackCard(pCard);
		}
		curGroup.SortCard();//对场牌排一次序
		//替换场牌持有者为当前玩家
		m_pCurCardPlayer = m_pCurPlayer;
	}
	else
	{
		m_pCurPlayer->Say(_T("PASS"));
	}
	//玩家出牌后隐藏出牌按钮
	Judge::Instance().ShowCardPlayBtn(FALSE);
	//切换到下一个玩家，流程继续
	SwitchToNextPlayer();
	MainFlow();
}

BOOL Judge::CheckCurPlayerCard()
{
	//提取当前场牌堆
	CardGroup & curGroup = DataCenter::Instance().GetCurCardGroup();
	//提取当前玩家的OnHold牌
	CardGroup onHoldCard = m_pCurPlayer->GetOnHoldCardGroup();
	//如果当前场牌为空或者持有者就是玩家本人，则不需要与场牌比点，否则需要比点
	if (DataCenter::Instance().GetCurCardGroup().GetCardPile().size() == 0 || m_pCurPlayer->GetID() == m_pCurCardPlayer->GetID())
	{
		return EM_CardGroup_Illegal != onHoldCard.CheckPile();
	}
	else
	{
		return EM_CardGroup_Illegal != onHoldCard.CheckPile(&curGroup);
	}
	return FALSE;
}

void Judge::ShowCallLandOwerBtn( BOOL bShow )
{
	if (::IsWindow(m_hMsgWnd) && m_pCurPlayer != NULL)
	{
		BOOL bHidePassBtn = FALSE;
		if (!m_pCurPlayer->IsLandOwerCardHolder())
		{
			bHidePassBtn = TRUE;
		}
		LPARAM lParam = MAKELPARAM(bHidePassBtn, GetCurHighstScore());
		SendMessage(m_hMsgWnd, MSG_SHOW_CLO_BTN, bShow, lParam);
	}
}

void Judge::ShowCardPlayBtn( BOOL bShow )
{
	if (::IsWindow(m_hMsgWnd) && m_pCurPlayer != NULL)
	{
		BOOL bHidePassBtn = FALSE;
		//场牌为空或者场牌所有者就是当前玩家，则不显示pass按钮
		if (DataCenter::Instance().GetCurCardGroup().GetCardPile().size() == 0 || m_pCurPlayer->GetID() == m_pCurCardPlayer->GetID())
		{
			bHidePassBtn = TRUE;
		}
		SendMessage(m_hMsgWnd, MSG_SHOW_CP_BTN, bShow, bHidePassBtn);
	}
}
