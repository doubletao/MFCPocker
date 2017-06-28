#pragma once
//////////////////////////////////////////////////////////////////////////
//���
//////////////////////////////////////////////////////////////////////////
#include "CardPile.h"
#include "IDrawable.h"
#include "PokerCard.h"
#include "CardGroup.h"

enum EMPlayerType
{
	EM_Left_Player, //�ϼ�
	EM_Mid_Player,	//���
	EM_Right_Player,	//�¼�
};

const int g_nHeadWidth = 50;
const int g_nHeadHeight = 50;
const int g_nGap = 5;

class Player : public IDrawable
{
private:
	EMPlayerType m_emPlayerType;//������ͣ��������Ʒ�λ��
	CString m_strID;//��ҵ�Ψһ��ʶ��
	CString m_strName;//��ҵ�����
	BOOL m_bShowValue;//����ʱչʾ����
	CardPile m_stuHandCard;//�������
	BOOL m_bLandOwerCardHolder;//�����Ƶ�����ֻ���õ��˵����ƣ��з�ѯ��ʱ��һ������������һ�����ǵ�����
	BOOL m_bLandOwer;//������������ǵ�����ݱ�ǣ�
	int m_nLandOwerScore;//�е����ķ���
	CString m_strWord;
protected:
	void DrawPlayerHead(CDC * pDC, CRect rtBound);//����ͷ��
	void DrawCard(CDC * pDC, CRect rtBound);//����ֽ��
public:
	Player(void);
	~Player(void);
	void ShowValue(BOOL bShowValue = TRUE){m_bShowValue = bShowValue;}//�Ƿ���ʾ����
	void SetName(CString str){m_strName = str;}
	void SetPlayerType(EMPlayerType emPlayerType){m_emPlayerType = emPlayerType;}
	EMPlayerType GetPlayerType(){return m_emPlayerType;}
	virtual void DrawTo(CDC * pDC);
	void PushBackCard(ICard * pCard){m_stuHandCard.PushBackCard(pCard);}//�ڶ�β����һ����
	CardPile & GetHandCard(){return m_stuHandCard;}//��ȡ�����ƶ�
	vector<int> & GetOnHoldCard(){return m_stuHandCard.GetOnHoldCard();}//��ȡѡ�������
	void SetOnHoldCard(vector<int> vecOnHoldCard){m_stuHandCard.SetOnHoldCard(vecOnHoldCard);}//����ѡ�������
	CardGroup GetOnHoldCardGroup(){return m_stuHandCard.GetOnHoldCardPile();}//����ѡ������Ż�ȡѡ����
	CardGroup PlayOutCard(){return m_stuHandCard.PlayOutCard();}//���ѡ����
	void SetLandOwer(BOOL bLandOwer){m_bLandOwer = bLandOwer;}
	BOOL IsLandOwer(){return m_bLandOwer;}
	void SetLandOwerCardHolder(BOOL bLandOwerCardHolder){m_bLandOwerCardHolder = bLandOwerCardHolder;}
	BOOL IsLandOwerCardHolder(){return m_bLandOwerCardHolder;}
	CString GetID(){return m_strID;}
	UINT GetCardCount(){return m_stuHandCard.GetCardPile().size();}//���س����Ƶ�����
	void ExcuteCallLandOwer();//ִ�не����߼�
	void ExcuteCallCardPlay();//ִ�г����߼�
	BOOL IsAIPlayer(){return m_emPlayerType != EM_Mid_Player;}
	void Say(CString strWord){m_strWord = strWord;}//���˵��
	void SetLandOwerScore(int nScore){m_nLandOwerScore = nScore;}
	int GetLandOwerScore(){return m_nLandOwerScore;}
};
