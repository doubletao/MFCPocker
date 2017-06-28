#pragma once
//////////////////////////////////////////////////////////////////////////
//���٣������ƶ���Ϸ���̣�������Ϸ���
//////////////////////////////////////////////////////////////////////////
#include "PokerDlg.h"
#include "Player.h"

class Judge
{
private:
	Judge(void);
	HWND m_hMsgWnd;
	Player * m_pCurCardPlayer;//��ǰ����������
	Player * m_pCurPlayer;//��ǰ�غϵ����ID
	int m_nCurHighstScore;//��¼��ǰ�е�����߷�
	Player * m_pToBeLandOwer;//����������
private:
	void CallLandOwer();//������һ��ҵĽе�������
	void CallPlayCard();//������һ��ҵĳ��ƶ���
public:
	~Judge(void);
	static Judge & Instance();
	void SetHwnd(HWND hWnd){m_hMsgWnd = hWnd;}
	void RefreshView();//֪ͨ��ͼˢ��
	void ShowCallLandOwerBtn(BOOL bShow);//��ʾ�е�����ť
	void ShowCardPlayBtn(BOOL bShow);//��ʾ���ư�ť
	void Restart();//�ؿ�
	void MainFlow();//��Ҫ����
	Player * GetCurPlayer(){return m_pCurPlayer;}//��ȡ��ǰ���
	Player * SwitchToNextPlayer();//�л���ǰ���Ϊ��һ���
	Player * GetCurCardPlayer(){return m_pCurCardPlayer;}//��ȡ��ǰ�����������
	int GetCurHighstScore(){return m_nCurHighstScore;}//��ȡ��ǰ�е�������߷�
	void CurPlayerCallScore(int nScore);//����ǰ��ҽз�
	void CurPlayerPlayCard();//����ǰ��ҳ���
	BOOL CheckCurPlayerCard();//��⵱ǰ��ҳ����Ƿ���Ϲ���
};

