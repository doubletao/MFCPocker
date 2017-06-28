#pragma once
//////////////////////////////////////////////////////////////////////////
//牌组，代表一组牌
//牌组的概念为一组可以一起使用的牌，例如斗地主中的对子，三带，运动包括炸弹等都可以视为牌组，单张也可以认为是一种牌组
//牌组根据具体的牌组类型不同也存在可比性
//////////////////////////////////////////////////////////////////////////
#include "CardPile.h"
//单张，对子，三不带，三带一，三带二，四带二，顺子，连对，飞机，炸弹，王炸
enum EMCardGroupType
{
	EM_Single_CardGroup, //单张
	EM_Couple_CardGroup, //对子
	EM_TripleWithout_CardGroup, //三不带
	EM_TripleWithOne_CardGroup, //三带一
	EM_TripleWithCouple_CardGroup, //三带二
	EM_FourWithCouple_CardGroup, //四带二
	EM_MoveLine_CardGroup, //顺子
	EM_CoupleMoveLine_CardGroup, //连对
	EM_PlaneWithout_CardGroup, //光杆飞机
	EM_PlaneWithone_CardGroup, //单张飞机
	EM_PlaneWithCouple_CardGroup, //对子飞机
	EM_Bomb_CardGroup, //炸弹
	EM_JokingBomb_CardGroup, //王炸
	EM_CardGroup_Illegal,//非法类型
};

const CString g_strType[] = {_T("单张"), _T("对子"), _T("三不带"), _T("三带一"), _T("三带二"),
	_T("四带二"), _T("顺子"), _T("连对"), _T("光杆飞机"), _T("单张飞机"),
	_T("对子飞机"), _T("炸弹"), _T("王炸"), _T("裁判都被我忽悠了，屌不屌？"),};

//运动及姊妹张数限制（默认5运3姊妹）
const int g_nMoveLineCardCountLimit = 5;
const int g_nMoveCoupleCardCountLimit = 3;

class CardGroup : public CardPile
{
public:
	CardGroup(void);
	virtual ~CardGroup(void);
	//核心校验函数，校验牌组type以及是否能管住参数(内部会对当前及场牌进行排序)
	EMCardGroupType CheckPile(CardGroup * pCurPile = NULL);//对牌组进行校验，参数为空时如果合法则返回类型，否则返回非法，参数不为空时，如果当前能压住参数牌组，则返回类型，否则返回非法
};
