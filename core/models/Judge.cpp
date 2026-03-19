#include "Judge.h"
#include "DataCenter.h"
#include <QMessageBox>
#include <QTimer>

Judge::Judge(void)
    : m_pCurCardPlayer(nullptr)
    , m_pCurPlayer(nullptr)
    , m_pToBeLandOwer(nullptr)
    , m_nCurHighstScore(0)
{
}

Judge::~Judge(void)
{
}

Judge & Judge::Instance()
{
    static Judge g_Instance;
    return g_Instance;
}

void Judge::Restart()
{
    bool bRestart = false;
    if (DataCenter::Instance().GetPlayState() != EM_WaitToStart_PlayState)
    {
        int result = QMessageBox::question(nullptr, tr(""), tr("是否重新开始游戏？"), QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes)
        {
            bRestart = true;
        }
    }
    else
    {
        bRestart = true;
    }
    if (bRestart)
    {
        m_pCurPlayer = nullptr;
        m_pCurCardPlayer = nullptr;
        m_pToBeLandOwer = nullptr;
        m_nCurHighstScore = 0;
        DataCenter::Instance().Reset();
        DataCenter::Instance().SendOutCard();
        DataCenter::Instance().SetPlayState(EM_LandHolderBorn_PlayState);
        emit refreshView();
        MainFlow();
    }
}

void Judge::MainFlow()
{
    switch(DataCenter::Instance().GetPlayState())
    {
    case EM_LandHolderBorn_PlayState:
        {
            bool bAllAsked = true;
            std::vector<Player> & vecPlayer = DataCenter::Instance().GetPlayerList();
            for (unsigned int i = 0; i < vecPlayer.size(); i++)
            {
                if (vecPlayer[i].GetLandOwerScore() > m_nCurHighstScore)
                {
                    m_nCurHighstScore = vecPlayer[i].GetLandOwerScore();
                    m_pToBeLandOwer = &vecPlayer[i];
                }
                if (vecPlayer[i].GetLandOwerScore() < 0)
                {
                    bAllAsked = false;
                }
            }
            if (bAllAsked)
            {
                m_pToBeLandOwer->SetLandOwer(true);
            }
            if (nullptr != DataCenter::Instance().GetLandOwner())
            {
                m_pCurPlayer = nullptr;
                DataCenter::Instance().SetPlayState(EM_WaitPlayer_PlayState);
                MainFlow();
                return;
            }
            if (m_pCurPlayer == nullptr)
            {
                m_pCurPlayer = DataCenter::Instance().GetLandOwnerCardHolder();
            }
            Q_ASSERT(m_pCurPlayer);
            m_pCurPlayer->ExcuteCallLandOwer();
        }
        break;
    case EM_WaitPlayer_PlayState:
        {
            bool bLandOwerWin = false;
            if (DataCenter::Instance().IsOver(bLandOwerWin))
            {
                if (bLandOwerWin)
                {
                    QMessageBox::information(nullptr, tr(""), tr("地主赢了!"));
                }
                else
                {
                    QMessageBox::information(nullptr, tr(""), tr("农民赢了!"));
                }
                DataCenter::Instance().SetPlayState(EM_WaitToStart_PlayState);
                DataCenter::Instance().ShowAllPlayerCard();
                emit refreshView();
                return;
            }
            if (m_pCurPlayer == nullptr)
            {
                m_pCurPlayer = DataCenter::Instance().GetLandOwner();
                DataCenter::Instance().SendOutBottomCard();
                emit refreshView();
            }
            Q_ASSERT(m_pCurPlayer);
            m_pCurPlayer->ExcuteCallCardPlay();
        }
        break;
    }
}

Player * Judge::SwitchToNextPlayer()
{
    Player * pPlayer = nullptr;
    QString strCurPlayerID = m_pCurPlayer->GetID();
    std::vector<Player> & vecPlayer = DataCenter::Instance().GetPlayerList();
    for (unsigned int i = 0; i < vecPlayer.size(); i++)
    {
        if (strCurPlayerID == vecPlayer[i].GetID())
        {
            pPlayer = &vecPlayer[(i + 1) % vecPlayer.size()];
            m_pCurPlayer = pPlayer;
            break;
        }
    }
    return pPlayer;
}

void Judge::CurPlayerCallScore(int nScore)
{
    if (m_pCurPlayer == nullptr)
    {
        Q_ASSERT(false);
        return;
    }
    m_pCurPlayer->SetLandOwerScore(nScore);
    if (nScore == 3)
    {
        m_pCurPlayer->SetLandOwer(true);
    }
    else
    {
        m_pCurPlayer->SetLandOwerScore(nScore);
    }
    if (nScore == 0)
    {
        QString strWord = tr("不叫");
        m_pCurPlayer->Say(strWord);
        emit playerAction(m_pCurPlayer->GetName(), strWord);
    }
    else
    {
        QString strWord = QString::number(nScore) + tr("分");
        m_pCurPlayer->Say(strWord);
        emit playerAction(m_pCurPlayer->GetName(), strWord);
    }
    emit showCallLandOwerBtn(false, m_nCurHighstScore);
    SwitchToNextPlayer();
    // 异步执行，让界面先刷新隐藏按钮，再让AI叫分，避免用户误点卡住
    QTimer::singleShot(100, this, &Judge::MainFlow);
}

void Judge::CurPlayerPlayCard()
{
    if (m_pCurPlayer == nullptr)
    {
        Q_ASSERT(false);
        return;
    }
    std::vector<int> vecCardIndex = m_pCurPlayer->GetOnHoldCard();
    if (vecCardIndex.size() > 0)
    {
        CardGroup & curGroup = DataCenter::Instance().GetCurCardGroup();
        CardPile & stuAbordCard = DataCenter::Instance().GetAbortCardPile();
        CardGroup onHoldCard = m_pCurPlayer->PlayOutCard();
        EMCardGroupType emType = onHoldCard.CheckPile();
        Q_ASSERT(emType != EM_CardGroup_Illegal);

        {
            std::vector<ICard *> cardPile = onHoldCard.GetCardPile();
            QString strWord = g_strType[emType];
            strWord += ":";
            for (unsigned int i = 0; i < cardPile.size(); i++)
            {
                strWord += g_strCardDisp[cardPile[i]->GetValue()] + " ";
            }
            m_pCurPlayer->Say(strWord);
            emit playerAction(m_pCurPlayer->GetName(), strWord);
        }

        ICard * pCard = nullptr;
        while(pCard = curGroup.PopCard())
        {
            pCard->SelectCard(false);
            stuAbordCard.PushBackCard(pCard);
        }
        while(pCard = onHoldCard.PopCard())
        {
            pCard->SelectCard(false);
            curGroup.PushBackCard(pCard);
        }
        curGroup.SortCard();
        m_pCurCardPlayer = m_pCurPlayer;
    }
    else
    {
        QString strWord = tr("不要");
        m_pCurPlayer->Say(strWord);
        emit playerAction(m_pCurPlayer->GetName(), strWord);
    }
    emit showCardPlayBtn(false, (DataCenter::Instance().GetCurCardGroup().GetCardPile().size() == 0 || m_pCurPlayer->GetID() == m_pCurCardPlayer->GetID()));
    SwitchToNextPlayer();
    // 异步执行，让界面先刷新隐藏按钮，再让AI出牌，避免用户误点卡住
    QTimer::singleShot(100, this, &Judge::MainFlow);
}

bool Judge::CheckCurPlayerCard()
{
    CardGroup & curGroup = DataCenter::Instance().GetCurCardGroup();
    CardGroup onHoldCard = m_pCurPlayer->GetOnHoldCardGroup();
    if (DataCenter::Instance().GetCurCardGroup().GetCardPile().size() == 0 || m_pCurPlayer->GetID() == m_pCurCardPlayer->GetID())
    {
        return EM_CardGroup_Illegal != onHoldCard.CheckPile();
    }
    else
    {
        return EM_CardGroup_Illegal != onHoldCard.CheckPile(&curGroup);
    }
    return false;
}

void Judge::ShowCallLandOwerBtn(bool bShow)
{
    bool bHidePassBtn = false;
    if (!m_pCurPlayer->IsLandOwerCardHolder())
    {
        bHidePassBtn = true;
    }
    emit showCallLandOwerBtn(bShow, m_nCurHighstScore);
}

void Judge::ShowCardPlayBtn(bool bShow)
{
    bool bHidePassBtn = false;
    if (DataCenter::Instance().GetCurCardGroup().GetCardPile().size() == 0 || m_pCurPlayer->GetID() == m_pCurCardPlayer->GetID())
    {
        bHidePassBtn = true;
    }
    emit showCardPlayBtn(bShow, bHidePassBtn);
}
