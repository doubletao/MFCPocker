#pragma once
//////////////////////////////////////////////////////////////////////////
// 数据中心
//////////////////////////////////////////////////////////////////////////
#include "CardPile.h"
#include "CardGroup.h"
#include "Player.h"
#include "core/interfaces/IDrawable.h"
#include <vector>

// 游戏当前状态
enum EMPlayState
{
    EM_WaitToStart_PlayState,      // 等待开始
    EM_LandHolderBorn_PlayState,   // 等待选地主
    EM_WaitPlayer_PlayState,       // 等待玩家出牌
};

class DataCenter : public IDrawable
{
private:
    EMPlayState m_emPlayState;
    DataCenter(void);
    std::vector<Player> m_vecPlayer;
    CardPile m_stuMainCard;
    CardPile m_stuAbordCard;
    CardGroup m_stuCurCard;
    PokerCard m_stuBottomCard[3];
    PokerCard m_stuLandOwnerCard;
public:
    ~DataCenter(void);
    static DataCenter & Instance();
    virtual void DrawTo(QPainter *painter);
    void Reset();
    void SendOutCard();
    void SendOutBottomCard();
    void SetPlayState(EMPlayState emPlayState) { m_emPlayState = emPlayState; }
    EMPlayState GetPlayState() { return m_emPlayState; }
    bool IsOver(bool & bLandOwerWin);
    std::vector<Player> & GetPlayerList() { return m_vecPlayer; }
    Player * GetLandOwnerCardHolder();
    Player * GetLandOwner();
    void ShowAllPlayerCard();
    CardGroup & GetCurCardGroup() { return m_stuCurCard; }
    CardPile & GetAbortCardPile() { return m_stuAbordCard; }
};
