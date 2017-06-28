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
	//У��type�㷨
	/*
	1.����Ϊ0��ֱ�ӷǷ�
	2.����Ϊ1����Ȼ�ǵ���
	3.����Ϊ2���ж��Ƕ��ӻ�����ը
	4.����Ϊ3���ж��Ƿ���3����ͬ��
	5.��������3����ȡԪ�طֽⷨ
	*/

	//������������
	SortCard(TRUE);

	//���ƶѵķֽ���
	vector<int> vecSingle;
	vector<int> vecCouple;
	vector<int> vecTriple;
	vector<int> vecFour;

	EMCardGroupType emType = EM_CardGroup_Illegal;
	int nCount = m_vecCard.size();
	if(nCount == 0)
	{
		//ֱ�ӷ��طǷ�
		return EM_CardGroup_Illegal;
	}
	else if (nCount == 1)
	{
		//����������߼����ж�
		emType = EM_Single_CardGroup;
	}
	else if (nCount == 2)
	{
		if (m_vecCard[0]->GetValue() == m_vecCard[1]->GetValue())
		{
			//����������߼����ж�
			emType = EM_Couple_CardGroup;
		}
		else if(m_vecCard[0]->GetValue() == 14 && m_vecCard[1]->GetValue() == 13)
		{
			//��ըֱ�ӷ���
			return EM_JokingBomb_CardGroup;
		}
		else
		{
			//���Ų�ͬ���ƻ�������ը��ֱ�ӷ���
			return EM_CardGroup_Illegal;
		}
	}
	else if (nCount == 3)
	{
		if (m_vecCard[0]->GetValue() == m_vecCard[1]->GetValue() && m_vecCard[1]->GetValue() == m_vecCard[2]->GetValue())
		{
			//������������߼����ж�
			emType = EM_TripleWithout_CardGroup;
		}
		else
		{
			return EM_CardGroup_Illegal;
		}
	}
	else
	{
		//��������3��ִ�зֽ�
		GlobalFunction::SplitCard(m_vecCard, vecSingle, vecCouple, vecTriple, vecFour);
		//�Ӵ���С�����ж�
		if (vecFour.size() > 0)
		{
			//��������ֻ��һ���źϷ�
			if(vecFour.size() == 1)
			{
				//��ʱʣ�µ�����ֻ����0����2
				int nRemainCard = vecSingle.size() + vecCouple.size() * 2 + vecTriple.size() * 3;
				if (nRemainCard == 0)
				{
					return EM_Bomb_CardGroup;
				}
				else if (nRemainCard == 2)
				{
					return EM_FourWithCouple_CardGroup;
				}
				//Ӧ��3334445556669999,3334449999�ɻ���ߴ��˸�ը�������
				else if (nRemainCard > 5)
				{
					//��ʱ�ɻ�������������ڵ���1
					if (vecTriple.size() > 1)
					{
						//���ɷɻ���������������������
						if (GlobalFunction::CheckCardSequence(vecTriple))
						{
							//�ɻ����Դ����Ż��߶��ӣ�ը��������Ϊ�ĸ����Ż�����������
							//�Ȱ��������ж�����
							//���ڵ��ŵ�ʱ���ж��������Ƿ����
							int nRemainSingle = vecSingle.size() + vecCouple.size() * 2 + vecFour.size() * 4;
							if (nRemainSingle == vecTriple.size())
							{
								emType = EM_PlaneWithone_CardGroup;
							}
							else
							{
								emType = EM_CardGroup_Illegal;
							}
							//�������ûͨ�������Զ����Ƿ����
							if (emType == EM_CardGroup_Illegal)
							{
								//�����ڵ��ŵ�ʱ���ж��������Ƿ����
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
			//����һ�����ŵģ�����һ��������������Ϊ������Ȼ����3�����Բ���������������
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
			//���ɷɻ���������������������
			else if (GlobalFunction::CheckCardSequence(vecTriple))
			{
				//�ɻ����Դ����Ż��߶��ӣ������źͶ��ӵ�������Ҫ�ͷɻ���ƥ��
				if (vecSingle.size() > 0)
				{
					//���ڵ��ŵ�ʱ���ж��������Ƿ����
					int nRemainSingle = vecSingle.size() + vecCouple.size() * 2 + vecFour.size() * 4;
					if (nRemainSingle == 0)
					{
						//��˷ɻ�
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
					//�����ڵ��ŵ�ʱ���ж��������Ƿ����
					int nRemainCouple = vecCouple.size() + vecFour.size() * 2;
					if (nRemainCouple == 0)
					{
						//��˷ɻ�
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
			//��ʱֻ��������ö�
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
			//��ʱֻ�������˶�
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

	//���ߵ���һ����˵��Ҫ���й�ס�ж�
	if (pCurPile == NULL)
	{
		return emType;
	}
	else
	{
		//���й�ס�ж�ǰ���ж������Ƿ���ͬ
		if(emType == pCurPile->CheckPile(NULL))
		{
			switch(emType)
			{
				//�ȶԱȽϼ򵥣�Ԫ�ص�һ������������ж�(���ţ�����, ˳�ӣ�ը��)
			case EM_Single_CardGroup:
			case EM_Couple_CardGroup:
			case EM_TripleWithout_CardGroup:
			case EM_Bomb_CardGroup:
				//��һԪ�����ͽ��Ƚϵ�һ�Ŵ�С����
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
				//������, ���ԣ���˷ɻ�������������Ҫ�ȱȽ������Ƿ�һ��Ȼ���ٱȽϵ�һ�Ŵ�С�͹���
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
					//���ڱȽϸ��ӵ����
					//������ƶѵķֽ���
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
							//����һ���������ӵ���������ж�triple�Ĵ�С��ϵ����ѹ�㣩
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
							//�Ĵ���ֻ�ж��ĸ��Ĵ�С��ϵ
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
							//�ɻ����ж�triple�е��׸�Ԫ�ؼ���
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
