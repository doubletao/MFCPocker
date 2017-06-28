#include "StdAfx.h"
#include "CardPile.h"
#include "GlobalFunction.h"
#include <algorithm>
#include "CardGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CardPile::CardPile(void)
	: m_bShowValue(FALSE)
{
	//�����ƶ���˵����������Ʊ���
	ShowBound(FALSE);
	ShowBackground(FALSE);
}

CardPile::CardPile(const vector<ICard *> & vecValue)
{
	m_vecCard = vecValue;
}

CardPile::~CardPile(void)
{
}

void CardPile::SortCard(BOOL bDec /*= TRUE*/)
{
	//�����������ӽ�������
	GlobalFunction::SortVecPointorByValue(m_vecCard, bDec);
}

void CardPile::RandonSortCard()
{
	//�������
	GlobalFunction::RandomSortVector(m_vecCard);
}

void CardPile::InsertCard(ICard * pCard, int nPos /*= 0*/)
{
	m_vecCard.insert(m_vecCard.begin() + nPos, pCard);
}

void CardPile::PushBackCard(ICard * pCard)
{
	m_vecCard.push_back(pCard);
}

ICard * CardPile::GetCard(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < (int)m_vecCard.size());
	return m_vecCard[nIndex];
}

vector<ICard *> & CardPile::GetCardPile()
{
	return m_vecCard;
}

void CardPile::DrawTo(CDC * pDC)
{
	CRect rtRegion = GetRegion();

	CSize sizeCards = GetCardSize();
	//������ָ�������ڴ����һ���ϵ��°���
	BOOL bIsVer = rtRegion.Width() < rtRegion.Height();//�ɻ�������Ŀ���������Ƿ�����ֱ����
	if (bIsVer)
	{
		for (int i = 0; i < (int)m_vecCard.size(); i++)
		{
			m_vecCard[i]->SelectCard(CheckOnHoldCard(i));
			m_vecCard[i]->ShowValue(m_bShowValue);
			m_vecCard[i]->SetPosition(CPoint(m_ptPosition.x, m_ptPosition.y + (rtRegion.Height() - sizeCards.cy) / 2 + g_nDefaultCardGap * i));
			m_vecCard[i]->DrawTo(pDC);
		}
	}
	else
	{
		for (int i = (int)m_vecCard.size() - 1; i > -1; i--)
		{
			m_vecCard[i]->SelectCard(CheckOnHoldCard(i));
			m_vecCard[i]->ShowValue(m_bShowValue);
			m_vecCard[i]->SetPosition(CPoint(m_ptPosition.x + (rtRegion.Width() - sizeCards.cx) / 2 + g_nDefaultCardGap * i, m_ptPosition.y));
			m_vecCard[i]->DrawTo(pDC);
		}
	}
}

ICard * CardPile::PopCard()
{
	ICard * pCard = NULL;
	if (m_vecCard.size() > 0)
	{
		pCard = *m_vecCard.rbegin();
		m_vecCard.pop_back();
	}
	return pCard;
}

CSize CardPile::GetCardSize()
{
	CSize sizeRet(0, 0);

	if (m_vecCard.size() < 1)
	{
		return sizeRet;
	}

	CRect rtRegion = GetRegion();
	//������ָ�������ڴ����һ���ϵ��°���
	BOOL bIsVer = rtRegion.Width() < rtRegion.Height();//�ɻ�������Ŀ���������Ƿ�����ֱ����
	if (bIsVer)
	{
		sizeRet.cx = g_nCardWidth;
		sizeRet.cy = g_nCardHeight + (m_vecCard.size() - 1) * g_nDefaultCardGap;
	}
	else
	{
		sizeRet.cx = g_nCardWidth + (m_vecCard.size() - 1) * g_nDefaultCardGap;
		sizeRet.cy = g_nCardHeight;
	}
	return sizeRet;
}

CardGroup CardPile::GetOnHoldCardPile()
{
	CardGroup cardGroup;
	for (UINT i = 0; i < m_vecCard.size(); i++)
	{
		if (CheckOnHoldCard(i))
		{
			cardGroup.PushBackCard(m_vecCard[i]);
		}
	}
	return cardGroup;
}

CardGroup CardPile::PlayOutCard()
{
	CardGroup cardGroup;
	int i = 0;
	for(vector<ICard *>::iterator it = m_vecCard.begin(); it != m_vecCard.end();)
	{
		if (CheckOnHoldCard(i++))
		{
			cardGroup.PushBackCard(*it);
			it = m_vecCard.erase(it);
		}
		else
		{
			it++;
		}
	}
	//�ƴ���������ѡ����
	m_vecOnHoldCard.clear();
	return cardGroup;
}

BOOL CardPile::CheckOnHoldCard(int nIndex)
{
	BOOL bRet = FALSE;
	if (std::find(m_vecOnHoldCard.begin(), m_vecOnHoldCard.end(), nIndex) != m_vecOnHoldCard.end())
	{
		bRet = TRUE;
	}
	return bRet;
}

void CardPile::HoldOneCard(DWORD dwID)
{
	for (UINT i = 0; i <m_vecCard.size(); i++)
	{
		//�ҵ�������
		if (m_vecCard[i]->GetID() == dwID)
		{
			vector<int>::iterator it = std::find(m_vecOnHoldCard.begin(), m_vecOnHoldCard.end(), i);
			//����������Ѿ���ѡ���Ƽ����У���Ӽ����иɵ���������뵽������
			if (it != m_vecOnHoldCard.end())
			{
				m_vecOnHoldCard.erase(it);
			}
			else
			{
				m_vecOnHoldCard.push_back(i);
			}
			break;
		}
	}
}
