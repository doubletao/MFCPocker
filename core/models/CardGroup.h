#pragma once
//////////////////////////////////////////////////////////////////////////
// 牌组，验证牌型
// 按照斗地主规则，验证一组牌是否符合牌型，并且判断是否比上家牌更大，可以出牌
//////////////////////////////////////////////////////////////////////////
#include "CardPile.h"

// 牌型定义
enum EMCardGroupType
{
    EM_Single_CardGroup,        // 单张
    EM_Couple_CardGroup,        // 对子
    EM_TripleWithout_CardGroup, // 三不带
    EM_TripleWithOne_CardGroup, // 三带一
    EM_TripleWithCouple_CardGroup, // 三带对
    EM_FourWithCouple_CardGroup,  // 四带二
    EM_MoveLine_CardGroup,        // 顺子
    EM_CoupleMoveLine_CardGroup,  // 连对
    EM_PlaneWithout_CardGroup,    // 飞机不带
    EM_PlaneWithone_CardGroup,    // 飞机带单
    EM_PlaneWithCouple_CardGroup, // 飞机带对
    EM_Bomb_CardGroup,            // 炸弹
    EM_JokingBomb_CardGroup,      // 王炸
    EM_CardGroup_Illegal,         // 非法
};

extern const QString g_strType[];

// 顺子最少5张，连对最少3对
const int g_nMoveLineCardCountLimit = 5;
const int g_nMoveCoupleCardCountLimit = 3;

class CardGroup : public CardPile
{
public:
    CardGroup(void);
    virtual ~CardGroup(void);
    // 验证，检查牌型，如果pCurPile为NULL只检查牌型，返回牌型，否则检查是否可以压住当前牌
    EMCardGroupType CheckPile(CardGroup * pCurPile = nullptr);
};
