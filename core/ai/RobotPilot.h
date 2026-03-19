#pragma once
//////////////////////////////////////////////////////////////////////////
// 自动出牌算法 AI
//
// AI出牌原则
// 大牌：能压就压，能走就走，能够打赢就一定出牌，不出的时候永远不出，最后一张大牌一定能赢就出，
// 顺牌：顺牌能出就出顺子，飞机飞机，对子对子，单张单张，炸弹炸弹王炸
// 小牌：把小牌都压到大牌，先走小牌（先出小，后出大）
// 三张：三张带牌，如果剩下三张，当只剩三张的时候，三张带一个，三张带一对，不能准确判断，AI不能理解的时候，
// 算法：正确找到大牌后，从大到小尝试出牌，能出就出，决定出什么
//
// 策略：
// 能出就出，普通牌能出就出，不能出就pass
// 普通牌，比对方牌大，就能出，大过对方就能压，只要能压就压，最后走完
// 三牌，三张带什么对应走什么，当剩下三张的时候顺子，如果剩下三张，飞机就飞机
// 小牌，比对方大就出，小牌比对方大也出，不能大就pass，最小的能大就出最小的大，让大牌留下
// 如果已经有三张了，就出，打完就赢
//////////////////////////////////////////////////////////////////////////

#include "core/models/Player.h"
#include <vector>
#include <QString>

class RobotPilot
{
private:
    std::vector<int> m_vecSingle;
    std::vector<int> m_vecCouple;
    std::vector<int> m_vecTriple;
    std::vector<int> m_vecFour;
private:
    RobotPilot(void);
    void Wait(Player * pPlayer, int nTimeLen = 1000);
public:
    ~RobotPilot(void);
    static RobotPilot & Instance();
    int DecideCallLandOwer(QString strPlayerID);
    std::vector<int> DecideCardToPlay(QString strPlayerID);
};
