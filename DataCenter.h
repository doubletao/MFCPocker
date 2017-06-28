#pragma once
//////////////////////////////////////////////////////////////////////////
//数据中心
//////////////////////////////////////////////////////////////////////////
#include "CardPile.h"
#include "CardGroup.h"
#include "Player.h"
#include "IDrawable.h"
#include <vector>
using std::vector;

//游戏进行状态
enum EMPlayState
{
	EM_WaitToStart_PlayState,		//等待开始
	EM_LandHolderBorn_PlayState,	//等待选地主
	EM_WaitPlayer_PlayState,		//等待玩家出牌
};

class DataCenter : public IDrawable
{
private:
	EMPlayState m_emPlayState;
	DataCenter(void);
	vector<Player> m_vecPlayer;//玩家列表（玩家类要记录各自手牌）
	CardPile m_stuMainCard;//主牌堆（尚未分发的牌）
	CardPile m_stuAbordCard;//弃牌堆（已经打出的牌）
	CardGroup m_stuCurCard;//场牌（正在打出，征求玩家动作的牌，需要记录所属玩家ID）
	PokerCard m_stuBottomCard[3];//三张底牌
	PokerCard m_stuLandOwnerCard;//一张地主牌
public:
	~DataCenter(void);
	static DataCenter & Instance();
	virtual void DrawTo(CDC * pDC);//绘制逻辑
	void Reset();//收牌，洗牌
	void SendOutCard();//发牌,暂时不分先后，统一从上家开始发
	void SendOutBottomCard();//发底牌，发给地主
	void SetPlayState(EMPlayState emPlayState){m_emPlayState = emPlayState;}
	EMPlayState GetPlayState(){return m_emPlayState;}
	BOOL IsOver(BOOL & bLandOwerWin);//判定是否结束，参数指示农民胜还是地主胜
	vector<Player> & GetPlayerList(){return m_vecPlayer;}//获取玩家列表
	Player * GetLandOwnerCardHolder();//获取地主牌得主玩家
	Player * GetLandOwner();//获取地主玩家
	void ShowAllPlayerCard();//展示所有玩家手牌
	CardGroup & GetCurCardGroup(){return m_stuCurCard;}//获取当前场牌
	CardPile & GetAbortCardPile(){return m_stuAbordCard;}//获取弃牌堆
};

