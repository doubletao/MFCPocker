#pragma once
//////////////////////////////////////////////////////////////////////////
//自动出牌算法（AI）

//AI出牌原则：
//甩牌：在自己回合或自己手里的王牌能够压住场牌，此时自己手里剩一个王牌及一次性打完的牌，打出王牌后即胜利
//普通出牌：飞机，连对，顺子，三带，对子，单张，炸弹，王炸
//跟牌：不拆牌的情况下，将手牌顺出去(不带倾向性，能顺则顺)
//小顶牌：不拆牌的前提下捡大的出
//救命顶：不拆就可能输了（对手剩4张以内牌时），此时即使强拆也要顶住
//无法确定原则：随机赋为跟牌或小顶牌（场上情形不明确，AI无法决定原则时）

//概念：
//王牌，通过分析场上剩余牌，认定无法被管住的牌
//普通出牌：轮到自己出牌（自己是地主或者自己刚打出王牌，自由出牌的场景）
//跟牌：自己的下手是队友，对手手牌不小于3，风险不大时，顺牌
//小顶牌：自己下手是对手，对手手牌不小于3，风险不大时，小顶一下下（小顶不必出王牌）
//救命顶：对手手牌已经小于3了，情况危急，无论如何，先顶住

//算法先确定出牌原则，然后根据出牌原则分析手牌，并决定出什么牌
//////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include <vector>
using std::vector;

enum EMAIPlayPrinciple
{
};

class RobotPilot
{
private:
	//将牌拆分成基础元素
	vector<int> m_vecSingle;
	vector<int> m_vecCouple;
	vector<int> m_vecTriple;
	vector<int> m_vecFour;
private:
	RobotPilot(void);
	void Wait(Player * pPlayer, int nTimeLen = 1000);
public:
	~RobotPilot(void);
	static RobotPilot & Instance();
	int DecideCallLandOwer(CString strPlayerID);
	vector<int> DecideCardToPlay(CString strPlayerID);
};

