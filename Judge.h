#pragma once
//////////////////////////////////////////////////////////////////////////
//法官，负责推动游戏进程，决定游戏结果
//////////////////////////////////////////////////////////////////////////
#include "PokerDlg.h"
#include "Player.h"

class Judge
{
private:
	Judge(void);
	HWND m_hMsgWnd;
	Player * m_pCurCardPlayer;//当前场牌所有者
	Player * m_pCurPlayer;//当前回合的玩家ID
	int m_nCurHighstScore;//记录当前叫地主最高分
	Player * m_pToBeLandOwer;//地主嫌疑人
private:
	void CallLandOwer();//触发下一玩家的叫地主动作
	void CallPlayCard();//触发下一玩家的出牌动作
public:
	~Judge(void);
	static Judge & Instance();
	void SetHwnd(HWND hWnd){m_hMsgWnd = hWnd;}
	void RefreshView();//通知视图刷新
	void ShowCallLandOwerBtn(BOOL bShow);//显示叫地主按钮
	void ShowCardPlayBtn(BOOL bShow);//显示出牌按钮
	void Restart();//重开
	void MainFlow();//主要流程
	Player * GetCurPlayer(){return m_pCurPlayer;}//获取当前玩家
	Player * SwitchToNextPlayer();//切换当前玩家为下一玩家
	Player * GetCurCardPlayer(){return m_pCurCardPlayer;}//获取当前场牌所属玩家
	int GetCurHighstScore(){return m_nCurHighstScore;}//获取当前叫地主的最高分
	void CurPlayerCallScore(int nScore);//处理当前玩家叫分
	void CurPlayerPlayCard();//处理当前玩家出牌
	BOOL CheckCurPlayerCard();//检测当前玩家出牌是否符合规则
};

