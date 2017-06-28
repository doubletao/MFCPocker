
// PokerDlg.cpp : 实现文件
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

// CPokerDlg 对话框

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


// CPokerDlg 消息处理程序

BOOL CPokerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	srand((UINT)time(0));

	//加载菜单
	m_Menu.LoadMenu(IDR_MAIN_MENU);
	SetMenu(&m_Menu);

	//初始化按钮管理器
	InitialBtnManager();

	//当前窗口句柄交给裁判
	Judge::Instance().SetHwnd(GetSafeHwnd());
	//通知裁判开始游戏
	Judge::Instance().Restart();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPokerDlg::InitialBtnManager()
{
	m_mapButton.clear();
	{
		GDIButton button;
		button.SetID(EM_PASS_SCORE_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("不叫"));
		m_mapButton.insert(make_pair(EM_PASS_SCORE_BUTTON, button));
	}
	{
		GDIButton button;
		button.SetID(EM_ONE_SCORE_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("1分"));
		m_mapButton.insert(make_pair(EM_ONE_SCORE_BUTTON, button));
	}
	{
		GDIButton button;
		button.SetID(EM_TWO_SCORE_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("2分"));
		m_mapButton.insert(make_pair(EM_TWO_SCORE_BUTTON, button));
	}
	{
		GDIButton button;
		button.SetID(EM_THREE_SCORE_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("3分"));
		m_mapButton.insert(make_pair(EM_THREE_SCORE_BUTTON, button));
	}

	{
		GDIButton button;
		button.SetID(EM_PASS_CARD_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("不要"));
		m_mapButton.insert(make_pair(EM_PASS_CARD_BUTTON, button));
	}
	{
		GDIButton button;
		button.SetID(EM_SEND_CARD_BUTTON);
		button.SetCallBackFunc(OnButtonClicked);
		button.SetButtonTxt(_T("出牌"));
		m_mapButton.insert(make_pair(EM_SEND_CARD_BUTTON, button));
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPokerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		CDialogEx::OnPaint();

		CRect rtClient;
		GetClientRect(rtClient);

		//绘制卡牌等
		DataCenter::Instance().SetBound(rtClient);
		DataCenter::Instance().DrawTo(&dc);

		//绘制按钮
		//按钮的纵坐标为mid玩家的上方
		int nY = rtClient.Height() - g_nCardHeight - g_nButtonHeight - g_nGap * 4;
		//计算四个叫分按钮的横坐标
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

		//计算两个出牌按钮的横坐标
		nX1 = (rtClient.Width() - g_nButtonWidth * 2 - g_nGap) / 2;
		nX2 = nX1 + g_nButtonWidth + g_nGap;
		m_mapButton[EM_PASS_CARD_BUTTON].SetPosition(CPoint(nX1, nY));
		m_mapButton[EM_PASS_CARD_BUTTON].DrawTo(&dc);

		m_mapButton[EM_SEND_CARD_BUTTON].SetPosition(CPoint(nX2, nY));
		m_mapButton[EM_SEND_CARD_BUTTON].DrawTo(&dc);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	AfxMessageBox(_T("邮箱：doubletao@yeah.net"));
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
		//玩家选了pass，则清空玩家的选择牌并告知裁判结束
		Judge::Instance().GetCurPlayer()->GetOnHoldCard().clear();
		Judge::Instance().CurPlayerPlayCard();
		break;
	case EM_SEND_CARD_BUTTON:
		//玩家选了出牌，则通知裁判检查玩家的牌是否符合规则
		if (Judge::Instance().CheckCurPlayerCard())
		{
			Judge::Instance().CurPlayerPlayCard();
		}
		else
		{
			AfxMessageBox(_T("您选择的牌不符合出牌规则！"));
		}
		break;
	default:
		break;
	}
	return 0;
}


LRESULT CPokerDlg::OnCardClicked(LPARAM lParam)
{
	Player & pPlayer = DataCenter::Instance().GetPlayerList()[1];//获取中间玩家
	ASSERT(&pPlayer == Judge::Instance().GetCurPlayer());
	if (&pPlayer == Judge::Instance().GetCurPlayer())
	{
		//将该牌选中或取消选中
		pPlayer.GetHandCard().HoldOneCard(lParam);
		Judge::Instance().RefreshView();
	}
	return 0;
}

void CPokerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//对按钮集合及卡牌进行hittest
	for (map<DWORD, GDIButton>::iterator it = m_mapButton.begin(); it != m_mapButton.end(); it++)
	{
		if(it->second.IsShow() && it->second.HitTest(point))
		{
			break;
		}
	}
	Player & pPlayer = DataCenter::Instance().GetPlayerList()[1];//获取中间玩家
	if (&pPlayer == Judge::Instance().GetCurPlayer())
	{
		//如果中间玩家是当前玩家，则检测是不是点中了他的手牌
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
