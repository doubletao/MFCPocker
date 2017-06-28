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
	//对于牌堆来说，不额外绘制背景
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
	//按照排序因子进行排序
	GlobalFunction::SortVecPointorByValue(m_vecCard, bDec);
}

void CardPile::RandonSortCard()
{
	//随机乱序
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
	//将牌在指定区域内从左到右或从上到下摆满
	BOOL bIsVer = rtRegion.Width() < rtRegion.Height();//由绘制区域的宽高来决定是否是竖直排列
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
	//将牌在指定区域内从左到右或从上到下摆满
	BOOL bIsVer = rtRegion.Width() < rtRegion.Height();//由绘制区域的宽高来决定是否是竖直排列
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
	//牌打出后清理掉选中牌
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
		//找到这张牌
		if (m_vecCard[i]->GetID() == dwID)
		{
			vector<int>::iterator it = std::find(m_vecOnHoldCard.begin(), m_vecOnHoldCard.end(), i);
			//如果这张牌已经在选中牌集合中，则从集合中干掉，否则加入到集合中
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
