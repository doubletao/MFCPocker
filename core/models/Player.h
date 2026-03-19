#pragma once
//////////////////////////////////////////////////////////////////////////
// 玩家
//////////////////////////////////////////////////////////////////////////
#include "CardPile.h"
#include "core/interfaces/IDrawable.h"
#include "PokerCard.h"
#include "CardGroup.h"
#include <QString>

enum EMPlayerType
{
    EM_Left_Player,   // 左边
    EM_Mid_Player,    // 中间（玩家）
    EM_Right_Player,  // 右边
};

const int g_nHeadWidth = 50;
const int g_nHeadHeight = 50;
const int g_nGap = 5;

class Player : public IDrawable
{
private:
    EMPlayerType m_emPlayerType;
    QString m_strID;
    QString m_strName;
    bool m_bShowValue;
    CardPile m_stuHandCard;
    bool m_bLandOwerCardHolder;
    bool m_bLandOwer;
    int m_nLandOwerScore;
    QString m_strWord;
protected:
    void DrawPlayerHead(QPainter *painter, QRect rtBound);
    void DrawCard(QPainter *painter, QRect rtBound);
public:
    Player(void);
    ~Player(void);
    void ShowValue(bool bShowValue = true) { m_bShowValue = bShowValue; }
    void SetName(QString str) { m_strName = str; }
    void SetPlayerType(EMPlayerType emPlayerType) { m_emPlayerType = emPlayerType; }
    EMPlayerType GetPlayerType() { return m_emPlayerType; }
    virtual void DrawTo(QPainter *painter);
    void PushBackCard(ICard * pCard) { m_stuHandCard.PushBackCard(pCard); }
    CardPile & GetHandCard() { return m_stuHandCard; }
    std::vector<int> & GetOnHoldCard() { return m_stuHandCard.GetOnHoldCard(); }
    void SetOnHoldCard(std::vector<int> vecOnHoldCard) { m_stuHandCard.SetOnHoldCard(vecOnHoldCard); }
    CardGroup GetOnHoldCardGroup() { return m_stuHandCard.GetOnHoldCardPile(); }
    CardGroup PlayOutCard() { return m_stuHandCard.PlayOutCard(); }
    void SetLandOwer(bool bLandOwer) { m_bLandOwer = bLandOwer; }
    bool IsLandOwer() { return m_bLandOwer; }
    void SetLandOwerCardHolder(bool bLandOwerCardHolder) { m_bLandOwerCardHolder = bLandOwerCardHolder; }
    bool IsLandOwerCardHolder() { return m_bLandOwerCardHolder; }
    QString GetID() { return m_strID; }
    QString GetName() const { return m_strName; }
    unsigned int GetCardCount() { return m_stuHandCard.GetCardPile().size(); }
    void ExcuteCallLandOwer();
    void ExcuteCallCardPlay();
    bool IsAIPlayer() { return m_emPlayerType != EM_Mid_Player; }
    void Say(QString strWord) { m_strWord = strWord; }
    void SetLandOwerScore(int nScore) { m_nLandOwerScore = nScore; }
    int GetLandOwerScore() { return m_nLandOwerScore; }
};
