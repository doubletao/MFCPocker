#include "DataCenter.h"
#include "core/utils/GlobalUtils.h"
#include <QCoreApplication>
#include <QPainter>

DataCenter::DataCenter(void)
    : m_emPlayState(EM_WaitToStart_PlayState)
{
    ShowBound(true);
    SetBoundColor(ColorDefine::WholeBound);
    SetBoundWidth(1);
    ShowBackground(true);
    SetBackColor(ColorDefine::WholeBack);

    // 初始化牌堆
    // 每个value 4张
    for (int i = 0; i < g_nCardDispModel - 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            PokerCard * pCard = nullptr;
            pCard = new PokerCard;
            pCard->SetValue(i);
            // 设置花色 j: 0黑桃 1红桃 2梅花 3方块
            switch (j)
            {
            case 0: pCard->SetSuit(CardSuit::Spade); break;
            case 1: pCard->SetSuit(CardSuit::Heart); break;
            case 2: pCard->SetSuit(CardSuit::Club); break;
            case 3: pCard->SetSuit(CardSuit::Diamond); break;
            }
            m_stuMainCard.PushBackCard(pCard);
            pCard->SetID((quint32)m_stuMainCard.GetCardPile().size());
        }
    }
    // 两个王
    {
        PokerCard * pCard = nullptr;
        pCard = new PokerCard;
        pCard->SetValue(g_nCardDispModel - 2);
        pCard->SetSuit(CardSuit::JokerSmall); // 小王 红色
        m_stuMainCard.PushBackCard(pCard);
        pCard->SetID((quint32)m_stuMainCard.GetCardPile().size());
    }
    {
        PokerCard * pCard = nullptr;
        pCard = new PokerCard;
        pCard->SetValue(g_nCardDispModel - 1);
        pCard->SetSuit(CardSuit::JokerBig); // 大王 黑色
        m_stuMainCard.PushBackCard(pCard);
        pCard->SetID((quint32)m_stuMainCard.GetCardPile().size());
    }
    Q_ASSERT(m_stuMainCard.GetCardPile().size() == 54);

    // 三个玩家
    Player pL;
    Player pM;
    Player pR;

    pL.SetPlayerType(EM_Left_Player);
    pL.SetName(QCoreApplication::translate("DataCenter", "电脑"));
    pL.ShowValue(false);

    pM.SetPlayerType(EM_Mid_Player);
    pM.SetName(QCoreApplication::translate("DataCenter", "玩家"));
    pM.ShowValue(true);

    pR.SetPlayerType(EM_Right_Player);
    pR.SetName(QCoreApplication::translate("DataCenter", "电脑"));
    pR.ShowValue(false);

    m_vecPlayer.push_back(pL);
    m_vecPlayer.push_back(pM);
    m_vecPlayer.push_back(pR);

    m_stuCurCard.ShowBound(false);
    m_stuCurCard.ShowBackground(false);
}

DataCenter::~DataCenter(void)
{
    Reset();
    GlobalUtils::ReleasePointerContainer(m_stuMainCard.GetCardPile());
}

DataCenter & DataCenter::Instance()
{
    static DataCenter g_dataCenter;
    return g_dataCenter;
}

void DataCenter::DrawTo(QPainter *painter)
{
    IDrawable::DrawTo(painter);

    QRect rtClient(0, 0, m_nWidth, m_nHeight);
    for (unsigned int i = 0; i < m_vecPlayer.size(); i++)
    {
        switch (m_vecPlayer[i].GetPlayerType())
        {
        case EM_Left_Player:
            m_vecPlayer[i].SetWidth(g_nCardWidth + g_nGap * 2);
            m_vecPlayer[i].SetHeight(rtClient.height() - g_nCardHeight - g_nGap * 5);
            m_vecPlayer[i].SetPosition(QPoint(g_nGap, g_nGap));
            break;
        case EM_Right_Player:
            m_vecPlayer[i].SetWidth(g_nCardWidth + g_nGap * 2);
            m_vecPlayer[i].SetHeight(rtClient.height() - g_nCardHeight - g_nGap * 5);
            m_vecPlayer[i].SetPosition(QPoint(rtClient.right() - g_nCardWidth - g_nGap * 3, g_nGap));
            break;
        case EM_Mid_Player:
            m_vecPlayer[i].SetWidth(rtClient.width() - g_nGap * 2);
            m_vecPlayer[i].SetHeight(g_nCardHeight + g_nGap * 2);
            m_vecPlayer[i].SetPosition(QPoint(g_nGap, rtClient.height() - g_nCardHeight - g_nGap * 3));
            break;
        default:
            break;
        }
        m_vecPlayer[i].DrawTo(painter);
    }

    QSize sizeCard = m_stuCurCard.GetCardSize();
    m_stuCurCard.SetPosition(QPoint((rtClient.width() - sizeCard.width()) / 2, (rtClient.height() - sizeCard.height()) / 2));
    m_stuCurCard.DrawTo(painter);

    if(m_stuLandOwnerCard.GetValue() < g_nCardDispModel)
    {
        for (unsigned int i = 0; i < m_vecPlayer.size(); i++)
        {
            if (m_vecPlayer[i].IsLandOwerCardHolder())
            {
                QRect rtPlayer = m_vecPlayer[i].GetRegion();
                switch(m_vecPlayer[i].GetPlayerType())
                {
                case EM_Left_Player:
                    m_stuLandOwnerCard.SetPosition(QPoint(rtPlayer.right() + g_nGap, rtPlayer.top() + (rtPlayer.height() - g_nCardHeight) / 2));
                    break;
                case EM_Mid_Player:
                    m_stuLandOwnerCard.SetPosition(QPoint(rtPlayer.left() + (rtPlayer.width() - g_nCardWidth) / 2, rtPlayer.top() - g_nCardHeight - g_nGap));
                    break;
                case EM_Right_Player:
                    m_stuLandOwnerCard.SetPosition(QPoint(rtPlayer.left() - g_nCardWidth - g_nGap, rtPlayer.top() + (rtPlayer.height() - g_nCardHeight) / 2));
                    break;
                default:
                    break;
                }
                m_stuLandOwnerCard.DrawTo(painter);
                break;
            }
        }
    }

    if (m_stuBottomCard[0].GetValue() < g_nCardDispModel)
    {
        m_stuBottomCard[0].SetPosition(QPoint(rtClient.left() + (rtClient.width() - g_nCardWidth * 3 - g_nGap * 2) / 2, g_nGap));
        m_stuBottomCard[0].DrawTo(painter);
    }
    if (m_stuBottomCard[1].GetValue() < g_nCardDispModel)
    {
        m_stuBottomCard[1].SetPosition(QPoint(rtClient.left() + (rtClient.width() - g_nCardWidth * 3 - g_nGap * 2) / 2 + g_nCardWidth + g_nGap, g_nGap));
        m_stuBottomCard[1].DrawTo(painter);
    }
    if (m_stuBottomCard[2].GetValue() < g_nCardDispModel)
    {
        m_stuBottomCard[2].SetPosition(QPoint(rtClient.left() + (rtClient.width() - g_nCardWidth * 3 - g_nGap * 2) / 2 + g_nCardWidth * 2 + g_nGap * 2, g_nGap));
        m_stuBottomCard[2].DrawTo(painter);
    }
}

void DataCenter::Reset()
{
    m_stuCurCard.ShowValue(true);
    m_stuLandOwnerCard.SetValue(-1);
    m_stuLandOwnerCard.ShowValue(false);
    m_stuLandOwnerCard.SelectCard(false);
    m_stuBottomCard[0].SetValue(-1);
    m_stuBottomCard[0].ShowValue(false);
    m_stuBottomCard[0].SelectCard(false);
    m_stuBottomCard[1].SetValue(-1);
    m_stuBottomCard[1].ShowValue(false);
    m_stuBottomCard[1].SelectCard(false);
    m_stuBottomCard[2].SetValue(-1);
    m_stuBottomCard[2].ShowValue(false);
    m_stuBottomCard[2].SelectCard(false);

    ICard * pCard = nullptr;
    for (unsigned int i = 0; i < m_vecPlayer.size(); i++)
    {
        m_vecPlayer[i].SetLandOwer(false);
        m_vecPlayer[i].SetLandOwerCardHolder(false);
        m_vecPlayer[i].SetLandOwerScore(-1);
        m_vecPlayer[i].Say("");
        m_vecPlayer[i].GetOnHoldCard().clear();
        CardPile & handCard = m_vecPlayer[i].GetHandCard();
        while(pCard = handCard.PopCard())
        {
            pCard->SelectCard(false);
            m_stuMainCard.PushBackCard(pCard);
        }
    }
    m_vecPlayer[0].ShowValue(false);
    m_vecPlayer[1].ShowValue(true);
    m_vecPlayer[2].ShowValue(false);

    while(pCard = m_stuAbordCard.PopCard())
    {
        pCard->SelectCard(false);
        m_stuMainCard.PushBackCard(pCard);
    }
    while(pCard = m_stuCurCard.PopCard())
    {
        pCard->SelectCard(false);
        m_stuMainCard.PushBackCard(pCard);
    }
    Q_ASSERT(m_stuMainCard.GetCardPile().size() == 54);
    m_stuMainCard.RandonSortCard();
}

void DataCenter::SendOutCard()
{
    Q_ASSERT(m_stuMainCard.GetCardPile().size() == 54);
    int nLandOwerCard = rand() % 54;
    m_stuLandOwnerCard.SetValue(m_stuMainCard.GetCard(nLandOwerCard)->GetValue());
    m_stuLandOwnerCard.ShowValue(true);

    m_vecPlayer[nLandOwerCard % 3].SetLandOwerCardHolder(true);

    m_stuBottomCard[0].SetValue(m_stuMainCard.GetCard(0)->GetValue());
    m_stuBottomCard[0].ShowValue(false);
    m_stuBottomCard[1].SetValue(m_stuMainCard.GetCard(1)->GetValue());
    m_stuBottomCard[1].ShowValue(false);
    m_stuBottomCard[2].SetValue(m_stuMainCard.GetCard(2)->GetValue());
    m_stuBottomCard[2].ShowValue(false);

    ICard * pCard = nullptr;
    for (unsigned int i = 0; i < 51; i++)
    {
        m_vecPlayer[i % 3].PushBackCard(m_stuMainCard.PopCard());
    }

    for(unsigned int i = 0; i < m_vecPlayer.size(); i++)
    {
        m_vecPlayer[i].GetHandCard().SortCard();
    }
}

void DataCenter::SendOutBottomCard()
{
    for (unsigned int i = 0; i < m_vecPlayer.size(); i++)
    {
        if (m_vecPlayer[i].IsLandOwer())
        {
            m_vecPlayer[i].PushBackCard(m_stuMainCard.PopCard());
            m_vecPlayer[i].PushBackCard(m_stuMainCard.PopCard());
            m_vecPlayer[i].PushBackCard(m_stuMainCard.PopCard());
            m_vecPlayer[i].GetHandCard().SortCard();
            break;
        }
    }
    Q_ASSERT(m_stuMainCard.GetCardPile().size() == 0);

    m_stuBottomCard[0].ShowValue(true);
    m_stuBottomCard[1].ShowValue(true);
    m_stuBottomCard[2].ShowValue(true);
    m_stuLandOwnerCard.SetValue(-1);
}

bool DataCenter::IsOver(bool & bLandOwerWin)
{
    bool bRet = false;
    if (GetPlayState() == EM_WaitPlayer_PlayState)
    {
        for (unsigned int i = 0; i < m_vecPlayer.size(); i++)
        {
            if (m_vecPlayer[i].GetCardCount() == 0)
            {
                bRet = true;
                if(m_vecPlayer[i].IsLandOwer())
                {
                    bLandOwerWin = true;
                }
                else
                {
                    bLandOwerWin = false;
                }
                break;
            }
        }
    }
    return bRet;
}

Player * DataCenter::GetLandOwnerCardHolder()
{
    Player * pPlayer = nullptr;
    std::vector<Player> & vecPlayer = DataCenter::Instance().GetPlayerList();
    for (unsigned int i = 0; i < vecPlayer.size(); i++)
    {
        if (vecPlayer[i].IsLandOwerCardHolder())
        {
            pPlayer = &vecPlayer[i];
            break;
        }
    }
    return pPlayer;
}

Player * DataCenter::GetLandOwner()
{
    Player * pPlayer = nullptr;
    for (unsigned int i = 0; i < m_vecPlayer.size(); i++)
    {
        if (m_vecPlayer[i].IsLandOwer())
        {
            pPlayer = &m_vecPlayer[i];
            break;
        }
    }
    return pPlayer;
}

void DataCenter::ShowAllPlayerCard()
{
    for (unsigned int i = 0; i < m_vecPlayer.size(); i++)
    {
        m_vecPlayer[i].ShowValue(true);
    }
}
