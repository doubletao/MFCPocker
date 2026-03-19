#include "RobotPilot.h"
#include "core/models/Judge.h"
#include "core/models/DataCenter.h"
#include "core/utils/GlobalUtils.h"
#include <QCoreApplication>
#include <QThread>

RobotPilot::RobotPilot(void)
{
}

void RobotPilot::Wait(Player * pPlayer, int nTimeLen)
{
    if (pPlayer == nullptr)
    {
        return;
    }
    // AI思考一会儿
    pPlayer->Say(QCoreApplication::translate("RobotPilot", "思考中..."));
    QThread::msleep(500);
    pPlayer->Say("");
}

RobotPilot::~RobotPilot(void)
{
}

RobotPilot & RobotPilot::Instance()
{
    static RobotPilot g_nInstance;
    return g_nInstance;
}

int RobotPilot::DecideCallLandOwer(QString strPlayerID)
{
    Player * pPlayer = Judge::Instance().GetCurPlayer();
    Q_ASSERT(pPlayer->GetID() == strPlayerID);
    Wait(pPlayer);
    int nCur = Judge::Instance().GetCurHighstScore();
    if(nCur < 3)
    {
        return nCur + 1;
    }
    return 0;
}

std::vector<int> RobotPilot::DecideCardToPlay(QString strPlayerID)
{
    std::vector<int> vecRet;
    Player * pPlayer = Judge::Instance().GetCurPlayer();
    Q_ASSERT(pPlayer->GetID() == strPlayerID);
    Wait(pPlayer);

    CardGroup & curGroup = DataCenter::Instance().GetCurCardGroup();

    if ((curGroup.GetCardPile().size() == 0) || (pPlayer->GetID() == Judge::Instance().GetCurCardPlayer()->GetID()))
    {
        std::vector<int> vecCurSingle;
        std::vector<int> vecCurCouple;
        std::vector<int> vecCurTriple;
        std::vector<int> vecCurFour;
        GlobalUtils::SplitCard(pPlayer->GetHandCard().GetCardPile(), vecCurSingle, vecCurCouple, vecCurTriple, vecCurFour, true);
        if (vecCurFour.size() > 0)
        {
            std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurFour[0], 4);
            vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
            if (vecCurSingle.size() > 1)
            {
                std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[0], 1);
                vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
                vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[1], 1);
                vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
            }
            else if (vecCurCouple.size() > 0)
            {
                std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[0], 2);
                vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
            }
        }
        else if(vecCurTriple.size() > 0)
        {
            std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurTriple[0], 3);
            vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
            if (vecCurSingle.size() > 0)
            {
                std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[0], 1);
                vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
            }
            else if (vecCurCouple.size() > 0)
            {
                std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[0], 2);
                vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
            }
        }
        else if (vecCurCouple.size() > 0)
        {
            std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[0], 2);
            vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
        }
        else if (vecCurSingle.size() > 0)
        {
            std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[0], 1);
            vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
        }
        else
        {
            vecRet.push_back((int)(pPlayer->GetHandCard().GetCardPile().size() - 1));
        }
    }
    else
    {
        std::vector<int> vecCurSingle;
        std::vector<int> vecCurCouple;
        std::vector<int> vecCurTriple;
        std::vector<int> vecCurFour;
        GlobalUtils::SplitCard(pPlayer->GetHandCard().GetCardPile(), vecCurSingle, vecCurCouple, vecCurTriple, vecCurFour, true);
        EMCardGroupType emCurType = curGroup.CheckPile();
        switch(emCurType)
        {
        case EM_Single_CardGroup:
            for (unsigned int i = 0; i < vecCurSingle.size(); i++)
            {
                if((unsigned int)vecCurSingle[i] > curGroup.GetCardPile()[0]->GetValue())
                {
                    std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[i], 1);
                    vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
                    break;
                }
            }
            break;
        case EM_Couple_CardGroup:
            for (unsigned int i = 0; i < vecCurCouple.size(); i++)
            {
                if((unsigned int)vecCurCouple[i] > curGroup.GetCardPile()[0]->GetValue())
                {
                    std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[i], 2);
                    vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
                    break;
                }
            }
            break;
        case EM_TripleWithout_CardGroup:
            for (unsigned int i = 0; i < vecCurTriple.size(); i++)
            {
                if((unsigned int)vecCurTriple[i] > curGroup.GetCardPile()[0]->GetValue())
                {
                    std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurTriple[i], 3);
                    vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
                    break;
                }
            }
            break;
        case EM_TripleWithOne_CardGroup:
            {
                std::vector<int> vecSingle;
                std::vector<int> vecCouple;
                std::vector<int> vecTriple;
                std::vector<int> vecFour;
                GlobalUtils::SplitCard(curGroup.GetCardPile(), vecSingle, vecCouple, vecTriple, vecFour, true);
                for (unsigned int i = 0; i < vecCurTriple.size(); i++)
                {
                    if(vecCurTriple[i] > vecTriple[0])
                    {
                        std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurTriple[i], 3);
                        vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
                        break;
                    }
                }
                if (vecCurSingle.size() > 0)
                {
                    std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurSingle[vecCurSingle.size() - 1], 1);
                    vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
                }
            }
            break;
        case EM_TripleWithCouple_CardGroup:
            {
                std::vector<int> vecSingle;
                std::vector<int> vecCouple;
                std::vector<int> vecTriple;
                std::vector<int> vecFour;
                GlobalUtils::SplitCard(curGroup.GetCardPile(), vecSingle, vecCouple, vecTriple, vecFour, true);
                for (unsigned int i = 0; i < vecCurTriple.size(); i++)
                {
                    if(vecCurTriple[i] > vecTriple[0])
                    {
                        std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurTriple[i], 3);
                        vecRet.insert(vecRet.begin(), vecTmp.begin(), vecTmp.end());
                        break;
                    }
                }
                if (vecCurCouple.size() > 0)
                {
                    std::vector<int> vecTmp = GlobalUtils::GetIndexByValueAndCount(pPlayer->GetHandCard().GetCardPile(), vecCurCouple[vecCurCouple.size() - 1], 2);
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
