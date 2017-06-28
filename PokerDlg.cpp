
// PokerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Poker.h"
#include "PokerDlg.h"
#include "afxdialogex.h"
#include "DataCenter.h"
#include "Judge.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPokerDlg �Ի���

CPokerDlg::CPokerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPokerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPokerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPokerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_RESTART, &CPokerDlg::OnRestart)
	ON_COMMAND(ID_ABOUT, &CPokerDlg::OnAbout)
	ON_MESSAGE(MSG_REFRESH_VIEW, &CPokerDlg::OnRefreshView)
	ON_MESSAGE(MSG_SHOW_CLO_BTN, &CPokerDlg::OnShowCloBtn)
	ON_MESSAGE(MSG_SHOW_CP_BTN, &CPokerDlg::OnShowCpBtn)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CPokerDlg ��Ϣ�������

BOOL CPokerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	srand((UINT)time(0));

	//���ز˵�
	m_Menu.LoadMenu(IDR_MAIN_MENU);
	SetMenu(&m_Menu);

	//��ʼ����ť������
	InitialBtnManager();

	//��ǰ���ھ����������
	Judge::Instance().SetHwnd(GetSafeHwnd());
	//֪ͨ���п�ʼ��Ϸ
	Judge::Instance().Restart();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPokerDlg::InitialBtnManager()
{
	m_mapButton.clear();
	{
		GDIButton button;
		button.SetID(EM_PASS_SCORE_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("����"));
		m_mapButton.insert(make_pair(EM_PASS_SCORE_BUTTON, button));
	}
	{
		GDIButton button;
		button.SetID(EM_ONE_SCORE_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("1��"));
		m_mapButton.insert(make_pair(EM_ONE_SCORE_BUTTON, button));
	}
	{
		GDIButton button;
		button.SetID(EM_TWO_SCORE_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("2��"));
		m_mapButton.insert(make_pair(EM_TWO_SCORE_BUTTON, button));
	}
	{
		GDIButton button;
		button.SetID(EM_THREE_SCORE_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("3��"));
		m_mapButton.insert(make_pair(EM_THREE_SCORE_BUTTON, button));
	}

	{
		GDIButton button;
		button.SetID(EM_PASS_CARD_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("��Ҫ"));
		m_mapButton.insert(make_pair(EM_PASS_CARD_BUTTON, button));
	}
	{
		GDIButton button;
		button.SetID(EM_SEND_CARD_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("����"));
		m_mapButton.insert(make_pair(EM_SEND_CARD_BUTTON, button));
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPokerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		CDialogEx::OnPaint();

		CRect rtClient;
		GetClientRect(rtClient);

		//���ƿ��Ƶ�
		DataCenter::Instance().SetBound(rtClient);
		DataCenter::Instance().DrawTo(&dc);

		//���ư�ť
		//��ť��������Ϊmid��ҵ��Ϸ�
		int nY = rtClient.Height() - g_nCardHeight - g_nButtonHeight - g_nGap * 4;
		//�����ĸ��зְ�ť�ĺ�����
		int nX1 = (rtClient.Width() - g_nButtonWidth * 4 - g_nGap * 3) / 2;
		int nX2 = nX1 + g_nButtonWidth + g_nGap;
		int nX3 = nX2 + g_nButtonWidth + g_nGap;
		int nX4 = nX3 + g_nButtonWidth + g_nGap;
		m_mapButton[EM_PASS_SCORE_BUTTON].SetPosition(CPoint(nX1, nY));
		m_mapButton[EM_PASS_SCORE_BUTTON].DrawTo(&dc);

		m_mapButton[EM_ONE_SCORE_BUTTON].SetPosition(CPoint(nX1, nY));
		m_mapButton[EM_ONE_SCORE_BUTTON].DrawTo(&dc);

		m_mapButton[EM_TWO_SCORE_BUTTON].SetPosition(CPoint(nX2, nY));
		m_mapButton[EM_TWO_SCORE_BUTTON].DrawTo(&dc);

		m_mapButton[EM_THREE_SCORE_BUTTON].SetPosition(CPoint(nX3, nY));
		m_mapButton[EM_THREE_SCORE_BUTTON].DrawTo(&dc);

		//�����������ư�ť�ĺ�����
		nX1 = (rtClient.Width() - g_nButtonWidth * 2 - g_nGap) / 2;
		nX2 = nX1 + g_nButtonWidth + g_nGap;
		m_mapButton[EM_PASS_CARD_BUTTON].SetPosition(CPoint(nX1, nY));
		m_mapButton[EM_PASS_CARD_BUTTON].DrawTo(&dc);

		m_mapButton[EM_SEND_CARD_BUTTON].SetPosition(CPoint(nX2, nY));
		m_mapButton[EM_SEND_CARD_BUTTON].DrawTo(&dc);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPokerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPokerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	Invalidate(TRUE);
}

void CPokerDlg::OnRestart()
{
	Judge::Instance().Restart();
}

void CPokerDlg::OnAbout()
{
	AfxMessageBox(_T("���䣺doubletao@yeah.net"));
}

LRESULT CPokerDlg::OnRefreshView(WPARAM wParam, LPARAM lParam)
{
	Invalidate(TRUE);
	return 0;
}

LRESULT CPokerDlg::OnButtonClicked( LPARAM lParam )
{
	switch(lParam)
	{
	case EM_PASS_SCORE_BUTTON:
		Judge::Instance().CurPlayerCallScore(0);
		break;
	case EM_ONE_SCORE_BUTTON:
		Judge::Instance().CurPlayerCallScore(1);
		break;
	case EM_TWO_SCORE_BUTTON:
		Judge::Instance().CurPlayerCallScore(2);
		break;
	case EM_THREE_SCORE_BUTTON:
		Judge::Instance().CurPlayerCallScore(3);
		break;
	case EM_PASS_CARD_BUTTON:
		//���ѡ��pass���������ҵ�ѡ���Ʋ���֪���н���
		Judge::Instance().GetCurPlayer()->GetOnHoldCard().clear();
		Judge::Instance().CurPlayerPlayCard();
		break;
	case EM_SEND_CARD_BUTTON:
		//���ѡ�˳��ƣ���֪ͨ���м����ҵ����Ƿ���Ϲ���
		if (Judge::Instance().CheckCurPlayerCard())
		{
			Judge::Instance().CurPlayerPlayCard();
		}
		else
		{
			AfxMessageBox(_T("��ѡ����Ʋ����ϳ��ƹ���"));
		}
		break;
	default:
		break;
	}
	return 0;
}


LRESULT CPokerDlg::OnCardClicked(LPARAM lParam)
{
	Player & pPlayer = DataCenter::Instance().GetPlayerList()[1];//��ȡ�м����
	ASSERT(&pPlayer == Judge::Instance().GetCurPlayer());
	if (&pPlayer == Judge::Instance().GetCurPlayer())
	{
		//������ѡ�л�ȡ��ѡ��
		pPlayer.GetHandCard().HoldOneCard(lParam);
		Judge::Instance().RefreshView();
	}
	return 0;
}

void CPokerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//�԰�ť���ϼ����ƽ���hittest
	for (map<DWORD, GDIButton>::iterator it = m_mapButton.begin(); it != m_mapButton.end(); it++)
	{
		if(it->second.IsShow() && it->second.HitTest(point))
		{
			break;
		}
	}
	Player & pPlayer = DataCenter::Instance().GetPlayerList()[1];//��ȡ�м����
	if (&pPlayer == Judge::Instance().GetCurPlayer())
	{
		//����м�����ǵ�ǰ��ң������ǲ��ǵ�������������
		vector<ICard *> & vecCard = pPlayer.GetHandCard().GetCardPile();
		for (UINT i = 0; i < vecCard.size(); i++)
		{
			if(vecCard[i]->HitTest(point))
			{
				break;
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

LRESULT CPokerDlg::OnShowCloBtn( WPARAM wParam, LPARAM lParam )
{
	if (wParam)
	{
		BOOL bShowPass = LOWORD(lParam);
		int nHighestScore = HIWORD(lParam);
		if (bShowPass)
		{
			m_mapButton[EM_PASS_SCORE_BUTTON].ShowButton(TRUE);
		}
		else
		{
			m_mapButton[EM_PASS_SCORE_BUTTON].ShowButton(FALSE);
		}

		m_mapButton[EM_ONE_SCORE_BUTTON].ShowButton(FALSE);
		m_mapButton[EM_TWO_SCORE_BUTTON].ShowButton(FALSE);
		m_mapButton[EM_THREE_SCORE_BUTTON].ShowButton(FALSE);
		switch(nHighestScore)
		{
		case 0:
			m_mapButton[EM_ONE_SCORE_BUTTON].ShowButton(TRUE);
		case 1:
			m_mapButton[EM_TWO_SCORE_BUTTON].ShowButton(TRUE);
		case 2:
			m_mapButton[EM_THREE_SCORE_BUTTON].ShowButton(TRUE);
			break;
		default:
			break;
		}
	}
	else
	{
		m_mapButton[EM_PASS_SCORE_BUTTON].ShowButton(FALSE);
		m_mapButton[EM_ONE_SCORE_BUTTON].ShowButton(FALSE);
		m_mapButton[EM_TWO_SCORE_BUTTON].ShowButton(FALSE);
		m_mapButton[EM_THREE_SCORE_BUTTON].ShowButton(FALSE);
	}
	Invalidate(TRUE);
	return 0;
}

LRESULT CPokerDlg::OnShowCpBtn( WPARAM wParam, LPARAM lParam )
{
	if (wParam)
	{
		m_mapButton[EM_SEND_CARD_BUTTON].ShowButton(TRUE);
		if (lParam)
		{
			m_mapButton[EM_PASS_CARD_BUTTON].ShowButton(FALSE);
		}
		else
		{
			m_mapButton[EM_PASS_CARD_BUTTON].ShowButton(TRUE);
		}
	}
	else
	{
		m_mapButton[EM_PASS_CARD_BUTTON].ShowButton(FALSE);
		m_mapButton[EM_SEND_CARD_BUTTON].ShowButton(FALSE);
	}
	Invalidate(TRUE);
	return 0;
}
