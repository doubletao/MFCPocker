#pragma once
//////////////////////////////////////////////////////////////////////////
// 牌堆，包含一堆牌
// 可以作为要打出的牌，也可以作为手里的牌，出牌完成之后还能作为打出牌堆，所以也可以当作一个牌组
//////////////////////////////////////////////////////////////////////////
#include "core/interfaces/ICard.h"
#include "core/interfaces/IDrawable.h"
#include <vector>

const int g_nDefaultCardGap = 25;

class CardGroup;
class CardPile : public IDrawable
{
protected:
    std::vector<ICard *> m_vecCard;      // 牌集合
    bool m_bShowValue;                   // 是否显示牌值
    std::vector<int> m_vecOnHoldCard;    // 选中的牌索引
public:
    CardPile(void);
    CardPile(const std::vector<ICard *> & vecValue);
    ~CardPile(void);
    void ShowValue(bool bShowValue = true) { m_bShowValue = bShowValue; }
    void SortCard(bool bDec = true);
    void RandonSortCard();
    void InsertCard(ICard * pCard, int nPos = 0);
    void PushBackCard(ICard * pCard);
    ICard * GetCard(int nIndex);
    std::vector<ICard *> & GetCardPile();
    virtual void DrawTo(QPainter *painter);
    ICard * PopCard();
    QSize GetCardSize();
    void SetOnHoldCard(std::vector<int> vecOnHoldCard) { m_vecOnHoldCard = vecOnHoldCard; }
    std::vector<int> & GetOnHoldCard() { return m_vecOnHoldCard; }
    CardGroup GetOnHoldCardPile();
    CardGroup PlayOutCard();
    bool CheckOnHoldCard(int nIndex);
    void HoldOneCard(unsigned int dwID);
};
