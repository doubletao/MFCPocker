#pragma once
//////////////////////////////////////////////////////////////////////////
//�ƶѣ�����һ����
//�����򣬿�����
//////////////////////////////////////////////////////////////////////////
#include "ICard.h"
#include "IDrawable.h"
#include <vector>
using std::vector;

const int g_nDefaultCardGap = 20;

class CardGroup;
class CardPile : public IDrawable
{
protected:
	vector<ICard *> m_vecCard;//���Ƽ���
	BOOL m_bShowValue;//�Ƿ�չʾ����ֵ
	vector<int> m_vecOnHoldCard;//��ѡ�е�����ż�
public:
	CardPile(void);
	CardPile(const vector<ICard *> & vecValue);
	~CardPile(void);
	void ShowValue(BOOL bShowValue = TRUE){m_bShowValue = bShowValue;}//�Ƿ���ʾ����
	void SortCard(BOOL bDec = TRUE);//�Կ�������Ĭ�Ͻ���
	void RandonSortCard();//�Կ�������ϴ�ƣ�
	void InsertCard(ICard * pCard, int nPos = 0);//��ָ��λ�ò���һ����
	void PushBackCard(ICard * pCard);//�ڶ�β����һ����
	ICard * GetCard(int nIndex);//���ָ��λ����
	vector<ICard *> & GetCardPile();//����ƶ�
	void DrawTo(CDC * pDC);
	ICard * PopCard();//����һ����
	CSize GetCardSize();//�������ǰ������ռ���
	void SetOnHoldCard(vector<int> vecOnHoldCard){m_vecOnHoldCard = vecOnHoldCard;}//���ñ�ѡ�е��Ƽ���
	vector<int> & GetOnHoldCard(){return m_vecOnHoldCard;}//��ȡ��ѡ�е�����ż���
	CardGroup GetOnHoldCardPile();//����ѡ������Ż�ȡѡ����
	CardGroup PlayOutCard();//���ѡ����
	BOOL CheckOnHoldCard(int nIndex);//�ж�һ�����Ƿ�ѡ��
	void HoldOneCard(DWORD dwID);//ѡ�л�ȡ��ѡ��һ����
};
