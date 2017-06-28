#pragma once
//////////////////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////////////////
#include "CardPile.h"
#include "CardGroup.h"
#include "Player.h"
#include "IDrawable.h"
#include <vector>
using std::vector;

//��Ϸ����״̬
enum EMPlayState
{
	EM_WaitToStart_PlayState,		//�ȴ���ʼ
	EM_LandHolderBorn_PlayState,	//�ȴ�ѡ����
	EM_WaitPlayer_PlayState,		//�ȴ���ҳ���
};

class DataCenter : public IDrawable
{
private:
	EMPlayState m_emPlayState;
	DataCenter(void);
	vector<Player> m_vecPlayer;//����б������Ҫ��¼�������ƣ�
	CardPile m_stuMainCard;//���ƶѣ���δ�ַ����ƣ�
	CardPile m_stuAbordCard;//���ƶѣ��Ѿ�������ƣ�
	CardGroup m_stuCurCard;//���ƣ����ڴ����������Ҷ������ƣ���Ҫ��¼�������ID��
	PokerCard m_stuBottomCard[3];//���ŵ���
	PokerCard m_stuLandOwnerCard;//һ�ŵ�����
public:
	~DataCenter(void);
	static DataCenter & Instance();
	virtual void DrawTo(CDC * pDC);//�����߼�
	void Reset();//���ƣ�ϴ��
	void SendOutCard();//����,��ʱ�����Ⱥ�ͳһ���ϼҿ�ʼ��
	void SendOutBottomCard();//�����ƣ���������
	void SetPlayState(EMPlayState emPlayState){m_emPlayState = emPlayState;}
	EMPlayState GetPlayState(){return m_emPlayState;}
	BOOL IsOver(BOOL & bLandOwerWin);//�ж��Ƿ����������ָʾũ��ʤ���ǵ���ʤ
	vector<Player> & GetPlayerList(){return m_vecPlayer;}//��ȡ����б�
	Player * GetLandOwnerCardHolder();//��ȡ�����Ƶ������
	Player * GetLandOwner();//��ȡ�������
	void ShowAllPlayerCard();//չʾ�����������
	CardGroup & GetCurCardGroup(){return m_stuCurCard;}//��ȡ��ǰ����
	CardPile & GetAbortCardPile(){return m_stuAbordCard;}//��ȡ���ƶ�
};

