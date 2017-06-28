#include "StdAfx.h"
#include "CardGroup.h"
#include "GlobalFunction.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CardGroup::CardGroup(void)
{
}

CardGroup::~CardGroup(void)
{
}

EMCardGroupType CardGroup::CheckPile(CardGroup * pCurPile /*= NULL*/)
{
	//校验type算法
	/*
	1.张数为0，直接非法
	2.张数为1，必然是单张
	3.张数为2，判定是对子还是王炸
	4.张数为3，判定是否是3张相同牌
	5.张数大于3，采取元素分解法
	*/

	//进来就先排序
	SortCard(TRUE);

	//本牌堆的分解牌
	vector<int> vecSingle;
	vector<int> vecCouple;
	vector<int> vecTriple;
	vector<int> vecFour;

	EMCardGroupType emType = EM_CardGroup_Illegal;
	int nCount = m_vecCard.size();
	if(nCount == 0)
	{
		//直接返回非法
		return EM_CardGroup_Illegal;
	}
	else if (nCount == 1)
	{
		//单张留待后边继续判定
		emType = EM_Single_CardGroup;
	}
	else if (nCount == 2)
	{
		if (m_vecCard[0]->GetValue() == m_vecCard[1]->GetValue())
		{
			//对子留待后边继续判定
			emType = EM_Couple_CardGroup;
		}
		else if(m_vecCard[0]->GetValue() == 14 && m_vecCard[1]->GetValue() == 13)
		{
			//王炸直接返回
			return EM_JokingBomb_CardGroup;
		}
		else
		{
			//两张不同的牌还不是王炸，直接返回
			return EM_CardGroup_Illegal;
		}
	}
	else if (nCount == 3)
	{
		if (m_vecCard[0]->GetValue() == m_vecCard[1]->GetValue() && m_vecCard[1]->GetValue() == m_vecCard[2]->GetValue())
		{
			//三不带留待后边继续判定
			emType = EM_TripleWithout_CardGroup;
		}
		else
		{
			return EM_CardGroup_Illegal;
		}
	}
	else
	{
		//张数大于3，执行分解
		GlobalFunction::SplitCard(m_vecCard, vecSingle, vecCouple, vecTriple, vecFour);
		//从大向小进行判定
		if (vecFour.size() > 0)
		{
			//四张有且只有一个才合法
			if(vecFour.size() == 1)
			{
				//此时剩下的牌数只能是0或者2
				int nRemainCard = vecSingle.size() + vecCouple.size() * 2 + vecTriple.size() * 3;
				if (nRemainCard == 0)
				{
					return EM_Bomb_CardGroup;
				}
				else if (nRemainCard == 2)
				{
					return EM_FourWithCouple_CardGroup;
				}
				//应对3334445556669999,3334449999飞机里边带了个炸弹的情况
				else if (nRemainCard > 5)
				{
					//此时飞机的数量必须大于等于1
					if (vecTriple.size() > 1)
					{
						//构成飞机的条件是三张数组连续
						if (GlobalFunction::CheckCardSequence(vecTriple))
						{
							//飞机可以带单张或者对子，炸弹可以视为四个单张或者两个对子
							//先按单张来判定试试
							//存在单张的时候判定单张数是否符合
							int nRemainSingle = vecSingle.size() + vecCouple.size() * 2 + vecFour.size() * 4;
							if (nRemainSingle == vecTriple.size())
							{
								emType = EM_PlaneWithone_CardGroup;
							}
							else
							{
								emType = EM_CardGroup_Illegal;
							}
							//如果单张没通过，试试对子是否符合
							if (emType == EM_CardGroup_Illegal)
							{
								//不存在单张的时候判定对子数是否符合
								int nRemainCouple = vecCouple.size() + vecFour.size() * 2;
								if (nRemainCouple == vecTriple.size())
								{
									emType = EM_PlaneWithCouple_CardGroup;
								}
								else
								{
									return EM_CardGroup_Illegal;
								}
							}
						}
					}
				}
				else
				{
					return EM_CardGroup_Illegal;
				}
			}
			else
			{
				return EM_CardGroup_Illegal;
			}
		}
		else if (vecTriple.size() > 0)
		{
			//存在一组三张的，三带一或者三带二（因为牌数必然大于3，所以不可能是三不带）
			if (vecTriple.size() == 1)
			{
				if (vecSingle.size() == 1 && vecCouple.size() == 0)
				{
					emType = EM_TripleWithOne_CardGroup;
				}
				else if(vecSingle.size() == 0 && vecCouple.size() == 1)
				{
					emType = EM_TripleWithCouple_CardGroup;
				}
				else
				{
					return EM_CardGroup_Illegal;
				}
			}
			//构成飞机的条件是三张数组连续
			else if (GlobalFunction::CheckCardSequence(vecTriple))
			{
				//飞机可以带单张或者对子，但单张和对子的数量需要和飞机数匹配
				if (vecSingle.size() > 0)
				{
					//存在单张的时候判定单张数是否符合
					int nRemainSingle = vecSingle.size() + vecCouple.size() * 2 + vecFour.size() * 4;
					if (nRemainSingle == 0)
					{
						//光杆飞机
						emType = EM_PlaneWithout_CardGroup;
					}
					else if (nRemainSingle == vecTriple.size())
					{
						emType = EM_PlaneWithone_CardGroup;
					}
					else
					{
						return EM_CardGroup_Illegal;
					}
				}
				else
				{
					//不存在单张的时候判定对子数是否符合
					int nRemainCouple = vecCouple.size() + vecFour.size() * 2;
					if (nRemainCouple == 0)
					{
						//光杆飞机
						emType = EM_PlaneWithout_CardGroup;
					}
					else if (nRemainCouple == vecTriple.size())
					{
						emType = EM_PlaneWithCouple_CardGroup;
					}
					else
					{
						return EM_CardGroup_Illegal;
					}
				}
			}
		}
		else if (vecCouple.size() > 0)
		{
			//此时只可能是姊妹对
			if (vecCouple.size() > g_nMoveCoupleCardCountLimit - 1 && GlobalFunction::CheckCardSequence(vecCouple))
			{
				emType = EM_CoupleMoveLine_CardGroup;
			}
			else
			{
				return EM_CardGroup_Illegal;
			}
		}
		else
		{
			//此时只可能是运动
			if (vecSingle.size() > g_nMoveLineCardCountLimit - 1 && GlobalFunction::CheckCardSequence(vecSingle))
			{
				emType = EM_MoveLine_CardGroup;
			}
			else
			{
				return EM_CardGroup_Illegal;
			}
		}
	}

	if (emType == EM_CardGroup_Illegal)
	{
		return EM_CardGroup_Illegal;
	}

	//能走到这一步，说明要进行管住判定
	if (pCurPile == NULL)
	{
		return emType;
	}
	else
	{
		//进行管住判定前先判定类型是否相同
		if(emType == pCurPile->CheckPile(NULL))
		{
			switch(emType)
			{
				//先对比较简单（元素单一）的情况进行判定(单张，对子, 顺子，炸弹)
			case EM_Single_CardGroup:
			case EM_Couple_CardGroup:
			case EM_TripleWithout_CardGroup:
			case EM_Bomb_CardGroup:
				//单一元素类型仅比较第一张大小即可
				if(m_vecCard[0]->GetValue() > pCurPile->m_vecCard[0]->GetValue())
				{
					return emType;
				}
				else
				{
					return EM_CardGroup_Illegal;
				}
				break;
			case EM_MoveLine_CardGroup:
			case EM_CoupleMoveLine_CardGroup:
			case EM_PlaneWithout_CardGroup:
				//三不带, 连对，光杆飞机这三种类型需要先比较牌张是否一致然后再比较第一张大小就够了
				if (m_vecCard.size() == pCurPile->m_vecCard.size())
				{
					if(m_vecCard[0]->GetValue() > pCurPile->m_vecCard[0]->GetValue())
					{
						return emType;
					}
					else
					{
						return EM_CardGroup_Illegal;
					}
				}
				else
				{
					return EM_CardGroup_Illegal;
				}
				break;
			default:
				{
					//对于比较复杂的情况
					//求出场牌堆的分解牌
					vector<int> vecCurSingle;
					vector<int> vecCurCouple;
					vector<int> vecCurTriple;
					vector<int> vecCurFour;
					GlobalFunction::SplitCard(pCurPile->m_vecCard, vecCurSingle, vecCurCouple, vecCurTriple, vecCurFour);
					switch(emType)
					{
					case EM_TripleWithOne_CardGroup:
					case EM_TripleWithCouple_CardGroup:
						{
							//三带一及三带对子的情况，仅判定triple的大小关系（不压点）
							ASSERT(vecTriple.size() == 1);
							ASSERT(vecCurTriple.size() == 1);
							if(vecTriple[0] > vecCurTriple[0])
							{
								return emType;
							}
							else
							{
								return EM_CardGroup_Illegal;
							}
						}
						break;
					case EM_FourWithCouple_CardGroup:
						{
							//四带二只判定四个的大小关系
							ASSERT(vecTriple.size() == 1);
							ASSERT(vecCurTriple.size() == 1);
							if(vecFour[0] > vecCurFour[0])
							{
								return emType;
							}
							else
							{
								return EM_CardGroup_Illegal;
							}
						}
						break;
					case EM_PlaneWithone_CardGroup:
					case EM_PlaneWithCouple_CardGroup:
						{
							//飞机仅判定triple中的首个元素即可
							ASSERT(vecTriple.size() > 0);
							ASSERT(vecCurTriple.size() > 0);
							if(vecTriple[0] > vecCurTriple[0])
							{
								return emType;
							}
							else
							{
								return EM_CardGroup_Illegal;
							}
						}
						break;
					default:
						return EM_CardGroup_Illegal;
					}
				}
				break;
			}
		}
		else
		{
			return EM_CardGroup_Illegal;
		}
	}
	return EM_CardGroup_Illegal;
}
