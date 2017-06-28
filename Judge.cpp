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
		if(IDYES==AfxMessageBox(_T("�Ƿ����¿�ʼ��"),MB_ICONQUESTION | MB_YESNO))
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
		//��ʼ��Ҫ����
		MainFlow();
	}
}

void Judge::MainFlow()
{
	switch(DataCenter::Instance().GetPlayState())
	{
	case EM_LandHolderBorn_PlayState:
		{
			//�ȼ���Ƿ��Ѿ�������
			//������Ҽ���Ƿ��Ѿ�ѯ�ʹ��ˣ�����Ѿ����ʹ��ˣ������ýз���ߵ�Ϊ����
			BOOL bAllAsked = TRUE;//�Ƿ��Ѿ�ѯ������
			vector<Player> & vecPlayer = DataCenter::Instance().GetPlayerList();
			for (UINT i = 0; i < vecPlayer.size(); i++)
			{
				//ͳ�Ƶ�ǰ��߷�
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
			//����Ѿ�ȫ���ʹ��ˣ������õ�ǰ��߷ֵ�Ϊ����
			if (bAllAsked)
			{
				m_pToBeLandOwer->SetLandOwer(TRUE);
			}
			//Ȼ��������ִ��ѯ������
			//��������Ѿ���������������һ�׶�
			if (NULL != DataCenter::Instance().GetLandOwner())
			{
				m_pCurPlayer = NULL;
				DataCenter::Instance().SetPlayState(EM_WaitPlayer_PlayState);
				MainFlow();
				return;
			}
			//�����ǰplayerΪ�գ����õ�ǰplayerΪ�����Ƶ���
			if (m_pCurPlayer == NULL)
			{
				m_pCurPlayer = DataCenter::Instance().GetLandOwnerCardHolder();
			}
			//�Ե�ǰ���ִ�е�����ѯ
			ASSERT(m_pCurPlayer);
			m_pCurPlayer->ExcuteCallLandOwer();
		}
		break;
	case EM_WaitPlayer_PlayState:
		{
			//�����Ϸ�Ѿ���������ִ�н����߼�
			BOOL bLandOwerWin = FALSE;
			if (DataCenter::Instance().IsOver(bLandOwerWin))
			{
				if (bLandOwerWin)
				{
					AfxMessageBox(_T("����Ӯ��!"));
				}
				else
				{
					AfxMessageBox(_T("�軧Ӯ��!"));
				}
				DataCenter::Instance().SetPlayState(EM_WaitToStart_PlayState);
				//�������������
				DataCenter::Instance().ShowAllPlayerCard();
				RefreshView();
				return;
			}
			//����ǳ��ƽ׶ζ���ǰplayerΪ�գ����õ�ǰplayerΪ���������������
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
	//�����ѡ��ķ������ø����
	m_pCurPlayer->SetLandOwerScore(nScore);
	//�����ǰ���Ϊ�գ�ֱ�ӷ���
	if(m_pCurPlayer == NULL)
	{
		return;
	}
	if (nScore == 3)
	{
		//�����ҽ������֣�ֱ����Ϊ����
		m_pCurPlayer->SetLandOwer(TRUE);
	}
	else
	{
		//��ҽеĲ������֣��������ҽеķ���
		m_pCurPlayer->SetLandOwerScore(nScore);
	}
	if (nScore == 0)
	{
		CString strWord;
		strWord.Format(_T("����"));
		m_pCurPlayer->Say(strWord);
	}
	else
	{
		CString strWord;
		strWord.Format(_T("%d��"), nScore);
		m_pCurPlayer->Say(strWord);
	}
	//��ҽзֺ����ؽе�����ť
	Judge::Instance().ShowCallLandOwerBtn(FALSE);
	//�л�����һ����ң����̼���
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
		//��ȡ��ǰ���ƶ�
		CardGroup & curGroup = DataCenter::Instance().GetCurCardGroup();
		//��ȡ��ǰ���ƶ�
		CardPile & stuAbordCard = DataCenter::Instance().GetAbortCardPile();
		//ȡ�ò������ǰ��ҵ�OnHold��
		CardGroup onHoldCard = m_pCurPlayer->PlayOutCard();
		//��һ����Ҵ����������
		EMCardGroupType emType = onHoldCard.CheckPile();
		ASSERT(emType != EM_CardGroup_Illegal);//����ҳ���ʱ�Ѿ�����һ���ж��ˣ����������һ��
		//��Ҵ��Ƶ�ͬʱ˵���Լ��������
		{
			vector<ICard *> cardPile = onHoldCard.GetCardPile();
			CString strWord = g_strType[emType];
			strWord.Append(_T(":"));
			for (UINT i = 0; i < cardPile.size(); i++)
			{
				//����д�����ʣ�Ӧ���ṩ�ӿ�����ȡ����ֵ����ʱ����д
				strWord.AppendFormat(_T("%s "), g_strCardDisp[cardPile[i]->GetValue()]);
			}
			m_pCurPlayer->Say(strWord);
		}
		//����ǰ���Ƽ��뵽���ƶ�
		ICard * pCard = NULL;
		while(pCard = curGroup.PopCard())
		{
			pCard->SelectCard(FALSE);
			stuAbordCard.PushBackCard(pCard);
		}
		//�����OnHold�Ƽ��뵽����
		while(pCard = onHoldCard.PopCard())
		{
			pCard->SelectCard(FALSE);
			curGroup.PushBackCard(pCard);
		}
		curGroup.SortCard();//�Գ�����һ����
		//�滻���Ƴ�����Ϊ��ǰ���
		m_pCurCardPlayer = m_pCurPlayer;
	}
	else
	{
		m_pCurPlayer->Say(_T("PASS"));
	}
	//��ҳ��ƺ����س��ư�ť
	Judge::Instance().ShowCardPlayBtn(FALSE);
	//�л�����һ����ң����̼���
	SwitchToNextPlayer();
	MainFlow();
}

BOOL Judge::CheckCurPlayerCard()
{
	//��ȡ��ǰ���ƶ�
	CardGroup & curGroup = DataCenter::Instance().GetCurCardGroup();
	//��ȡ��ǰ��ҵ�OnHold��
	CardGroup onHoldCard = m_pCurPlayer->GetOnHoldCardGroup();
	//�����ǰ����Ϊ�ջ��߳����߾�����ұ��ˣ�����Ҫ�볡�Ʊȵ㣬������Ҫ�ȵ�
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
		//����Ϊ�ջ��߳��������߾��ǵ�ǰ��ң�����ʾpass��ť
		if (DataCenter::Instance().GetCurCardGroup().GetCardPile().size() == 0 || m_pCurPlayer->GetID() == m_pCurCardPlayer->GetID())
		{
			bHidePassBtn = TRUE;
		}
		SendMessage(m_hMsgWnd, MSG_SHOW_CP_BTN, bShow, bHidePassBtn);
	}
}
