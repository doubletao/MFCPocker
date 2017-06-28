#include "StdAfx.h"
#include "RobotPilot.h"
#include "Judge.h"
#include "DataCenter.h"
#include "GlobalFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RobotPilot::RobotPilot(void)
{
}


void RobotPilot::Wait(Player * pPlayer, int nTimeLen /*= 1000*/)
{
	if (pPlayer == NULL)
	{
		return;
	}
	//AI增加一个延时
	pPlayer->Say(_T("别急，让我想想"));
	Sleep(0);
	pPlayer->Say(_T(""));
}

RobotPilot::~RobotPilot(void)
{
}

RobotPilot & RobotPilot::Instance()
{
	static RobotPilot g_nInstance;
	return g_nInstance;
}

int RobotPilot::DecideCallLandOwer( CString strPlayerID )
{
	//获取当前玩家
	Player * pPlayer = Judge::Instance().GetCurPlayer();
	ASSERT(pPlayer->GetID() == strPlayerID);
	Wait(pPlayer);
	//AI第一阶段叫地主原则为比当前场上最高分再高一分
	int nCur = Judge::Instance().GetCurHighstScore();
	if(nCur < 3)
	{
		return nCur + 1;
	}
	return 0;
}

vector<int> RobotPilot::DecideCardToPlay( CString strPlayerID )
{
	vector<int> vecRet;
	//获取当前玩家
	Player * pPlayer = Judge::Instance().GetCurPlayer();
	ASSERT(pPlayer->GetID() == strPlayerID);
	Wait(pPlayer);

	//提取当前场牌堆
	CardGroup & curGroup = DataCenter::Instance().GetCurCardGroup();

	//场牌为空或者场牌所有者就是当前玩家，则出当前最小单张
	if ((curGroup.GetCardPile().size() == 0) || (pPlayer->GetID() == Judge::Instance().GetCurCardPlayer()->GetID()))
	{
		vector<int> vecCurSingle;
		vector<int> vecCurCouple;
		vector<int> vecCurTriple;
		vector<int> vecCurFour;
		GlobalFunction::SplitCard(pPlayer->GetHandCard().GetCardPile(), vecCurSingle, vecCurCouple, vecCurTriple, vecCurFour, TRUE);
		//场牌为空的时候，先出四带二，三带一，三带对子，对子，单张
		if (vecCurFour.size() > 0)
		{
			vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurFour[0], 4);
			vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
			if (vecCurSingle.size() > 1)
			{
				vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[0], 1);
				vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
				vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[1], 1);
				vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
			}
			else if (vecCurCouple.size() > 0)
			{
				vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[0], 2);
				vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
			}
		}
		else if(vecCurTriple.size() > 0)
		{
			vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurTriple[0], 3);
			vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
			if (vecCurSingle.size() > 0)
			{
				vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[0], 1);
				vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
			}
			else if (vecCurCouple.size() > 0)
			{
				vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[0], 2);
				vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
			}
		}
		else if (vecCurCouple.size() > 0)
		{
			vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[0], 2);
			vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
		}
		else if (vecCurSingle.size() > 0)
		{
			vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[0], 1);
			vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
		}
		else
		{
			vecRet.push_back(pPlayer->GetHandCard().GetCardPile().size() - 1);
		}
	}
	//否则尽可能的出牌
	else
	{
		vector<int> vecCurSingle;
		vector<int> vecCurCouple;
		vector<int> vecCurTriple;
		vector<int> vecCurFour;
		GlobalFunction::SplitCard(pPlayer->GetHandCard().GetCardPile(), vecCurSingle, vecCurCouple, vecCurTriple, vecCurFour, TRUE);
		EMCardGroupType emCurType = curGroup.CheckPile();
		switch(emCurType)
		{
		case EM_Single_CardGroup:
			//从单张中找一张比它大的打出去
			for (UINT i = 0; i < vecCurSingle.size(); i++)
			{
				if((UINT)vecCurSingle[i] > curGroup.GetCardPile()[0]->GetValue())
				{
					vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[i], 1);
					vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
					break;
				}
			}
			break;
		case EM_Couple_CardGroup:
			//从对子中找一张比它大的打出去
			for (UINT i = 0; i < vecCurCouple.size(); i++)
			{
				if((UINT)vecCurCouple[i] > curGroup.GetCardPile()[0]->GetValue())
				{
					vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[i], 2);
					vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
					break;
				}
			}
			break;
		case EM_TripleWithout_CardGroup:
			//从三张中找一张比它大的打出去
			for (UINT i = 0; i < vecCurTriple.size(); i++)
			{
				if((UINT)vecCurTriple[i] > curGroup.GetCardPile()[0]->GetValue())
				{
					vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurTriple[i], 3);
					vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
					break;
				}
			}
			break;
		case EM_TripleWithOne_CardGroup:
			{
				vector<int> vecSingle;
				vector<int> vecCouple;
				vector<int> vecTriple;
				vector<int> vecFour;
				GlobalFunction::SplitCard(curGroup.GetCardPile(), vecSingle, vecCouple, vecTriple, vecFour, TRUE);
				//从三张中找一张比它大的加进去
				for (UINT i = 0; i < vecCurTriple.size(); i++)
				{
					if(vecCurTriple[i] > vecTriple[0])
					{
						vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurTriple[i], 3);
						vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
						break;
					}
				}
				//从单张中抽一张最小的加进去
				if (vecCurSingle.size() > 0)
				{
					vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[vecCurSingle.size() - 1], 1);
					vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
				}
			}
			break;
		case EM_TripleWithCouple_CardGroup:
			{
				vector<int> vecSingle;
				vector<int> vecCouple;
				vector<int> vecTriple;
				vector<int> vecFour;
				GlobalFunction::SplitCard(curGroup.GetCardPile(), vecSingle, vecCouple, vecTriple, vecFour, TRUE);
				//从三张中找一张比它大的加进去
				for (UINT i = 0; i < vecCurTriple.size(); i++)
				{
					if(vecCurTriple[i] > vecTriple[0])
					{
						vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurTriple[i], 3);
						vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
						break;
					}
				}
				//从对子中抽一张最小的加进去
				if (vecCurCouple.size() > 0)
				{
					vector<int> vecTmp = GlobalFunction::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[vecCurCouple.size() - 1], 2);
					vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
				}
			}
			break;
		default:
			break;
		}
		if (vecRet.size() != curGroup.GetCardPile().size())
		{
			vecRet.clear();
		}
	}

	return vecRet;
}
