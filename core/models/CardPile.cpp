#include "CardPile.h"
#include "core/utils/GlobalUtils.h"
#include <algorithm>
#include "CardGroup.h"

CardPile::CardPile(void)
    : m_bShowValue(false)
{
    ShowBound(false);
    ShowBackground(false);
}

CardPile::CardPile(const std::vector<ICard *> & vecValue)
{
    m_vecCard = vecValue;
}

CardPile::~CardPile(void)
{
}

void CardPile::SortCard(bool bDec)
{
    GlobalUtils::SortVecPointerByValue(m_vecCard, bDec);
}

void CardPile::RandonSortCard()
{
    GlobalUtils::RandomSortVector(m_vecCard);
}

void CardPile::InsertCard(ICard * pCard, int nPos)
{
    m_vecCard.insert(m_vecCard.begin() + nPos, pCard);
}

void CardPile::PushBackCard(ICard * pCard)
{
    m_vecCard.push_back(pCard);
}

ICard * CardPile::GetCard(int nIndex)
{
    Q_ASSERT(nIndex >= 0 && nIndex < (int)m_vecCard.size());
    return m_vecCard[nIndex];
}

std::vector<ICard *> & CardPile::GetCardPile()
{
    return m_vecCard;
}

void CardPile::DrawTo(QPainter *painter)
{
    QRect rtRegion = GetRegion();

    QSize sizeCards = GetCardSize();
    // 判断牌堆方向
    bool bIsVer = rtRegion.width() < rtRegion.height();
    if (bIsVer)
    {
        // 垂直方向（左右两侧AI玩家）：从下往上绘制，最上方一张完整显示
        int startY = m_ptPosition.y() + rtRegion.height() - sizeCards.height();
        for (int i = 0; i < (int)m_vecCard.size(); i++)
        {
            m_vecCard[i]->SelectCard(CheckOnHoldCard(i));
            m_vecCard[i]->ShowValue(m_bShowValue);
            m_vecCard[i]->SetPosition(QPoint(m_ptPosition.x(), startY + g_nDefaultCardGap * i));
            m_vecCard[i]->DrawTo(painter);
        }
    }
    else
    {
        // 水平方向（底部玩家手牌）：从左往右绘制，右侧牌压住左侧牌，左侧数字露出
        // 点击检测在GameWidget中已经是逆序，能正确命中上层的牌
        int centerStartX = m_ptPosition.x() + (rtRegion.width() - sizeCards.width()) / 2;
        for (int i = 0; i < (int)m_vecCard.size(); i++)
        {
            m_vecCard[i]->SelectCard(CheckOnHoldCard(i));
            m_vecCard[i]->ShowValue(m_bShowValue);
            m_vecCard[i]->SetPosition(QPoint(centerStartX + g_nDefaultCardGap * i, m_ptPosition.y()));
            m_vecCard[i]->DrawTo(painter);
        }
    }
}

ICard * CardPile::PopCard()
{
    ICard * pCard = nullptr;
    if (m_vecCard.size() > 0)
    {
        pCard = *m_vecCard.rbegin();
        m_vecCard.pop_back();
    }
    return pCard;
}

QSize CardPile::GetCardSize()
{
    QSize sizeRet(0, 0);

    if (m_vecCard.size() < 1)
    {
        return sizeRet;
    }

    QRect rtRegion = GetRegion();
    bool bIsVer = rtRegion.width() < rtRegion.height();
    if (bIsVer)
    {
        sizeRet.setWidth(g_nCardWidth);
        sizeRet.setHeight(g_nCardHeight + (m_vecCard.size() - 1) * g_nDefaultCardGap);
    }
    else
    {
        sizeRet.setWidth(g_nCardWidth + (m_vecCard.size() - 1) * g_nDefaultCardGap);
        sizeRet.setHeight(g_nCardHeight);
    }
    return sizeRet;
}

CardGroup CardPile::GetOnHoldCardPile()
{
    CardGroup cardGroup;
    for (unsigned int i = 0; i < m_vecCard.size(); i++)
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
    for(std::vector<ICard *>::iterator it = m_vecCard.begin(); it != m_vecCard.end();)
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
    m_vecOnHoldCard.clear();
    return cardGroup;
}

bool CardPile::CheckOnHoldCard(int nIndex)
{
    bool bRet = false;
    if (std::find(m_vecOnHoldCard.begin(), m_vecOnHoldCard.end(), nIndex) != m_vecOnHoldCard.end())
    {
        bRet = true;
    }
    return bRet;
}

void CardPile::HoldOneCard(unsigned int dwID)
{
    for (unsigned int i = 0; i < m_vecCard.size(); i++)
    {
        if (m_vecCard[i]->GetID() == dwID)
        {
            std::vector<int>::iterator it = std::find(m_vecOnHoldCard.begin(), m_vecOnHoldCard.end(), i);
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
