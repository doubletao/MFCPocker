#include "CardGroup.h"
#include "core/utils/GlobalUtils.h"
#include <algorithm>
#include <QString>

const QString g_strType[] = {
    QT_TRANSLATE_NOOP("CardGroup", "单张"),
    QT_TRANSLATE_NOOP("CardGroup", "对子"),
    QT_TRANSLATE_NOOP("CardGroup", "三不带"),
    QT_TRANSLATE_NOOP("CardGroup", "三带一"),
    QT_TRANSLATE_NOOP("CardGroup", "三带对"),
    QT_TRANSLATE_NOOP("CardGroup", "四带二"),
    QT_TRANSLATE_NOOP("CardGroup", "顺子"),
    QT_TRANSLATE_NOOP("CardGroup", "连对"),
    QT_TRANSLATE_NOOP("CardGroup", "飞机不带"),
    QT_TRANSLATE_NOOP("CardGroup", "飞机带单"),
    QT_TRANSLATE_NOOP("CardGroup", "飞机带对"),
    QT_TRANSLATE_NOOP("CardGroup", "炸弹"),
    QT_TRANSLATE_NOOP("CardGroup", "王炸"),
    QT_TRANSLATE_NOOP("CardGroup", "不是合法的牌型，不能出")
};

CardGroup::CardGroup(void)
{
}

CardGroup::~CardGroup(void)
{
}

EMCardGroupType CardGroup::CheckPile(CardGroup * pCurPile)
{
    // 判断type算法
    // 1. 如果为0直接非法
    // 2. 如果为1就是单张
    // 3. 如果为2判断是否双王炸弹
    // 4. 如果为3判断是否三个相同
    // 5. 超过3拆分成单对三炸

    SortCard(true);

    // 拆分牌堆
    std::vector<int> vecSingle;
    std::vector<int> vecCouple;
    std::vector<int> vecTriple;
    std::vector<int> vecFour;

    EMCardGroupType emType = EM_CardGroup_Illegal;
    int nCount = m_vecCard.size();
    if(nCount == 0)
    {
        return EM_CardGroup_Illegal;
    }
    else if (nCount == 1)
    {
        emType = EM_Single_CardGroup;
    }
    else if (nCount == 2)
    {
        if (m_vecCard[0]->GetValue() == m_vecCard[1]->GetValue())
        {
            emType = EM_Couple_CardGroup;
        }
        else if(m_vecCard[0]->GetValue() == 14 && m_vecCard[1]->GetValue() == 13)
        {
            return EM_JokingBomb_CardGroup;
        }
        else
        {
            return EM_CardGroup_Illegal;
        }
    }
    else if (nCount == 3)
    {
        if (m_vecCard[0]->GetValue() == m_vecCard[1]->GetValue() && m_vecCard[1]->GetValue() == m_vecCard[2]->GetValue())
        {
            emType = EM_TripleWithout_CardGroup;
        }
        else
        {
            return EM_CardGroup_Illegal;
        }
    }
    else
    {
        // 拆分出三个以上
        GlobalUtils::SplitCard(m_vecCard, vecSingle, vecCouple, vecTriple, vecFour);
        // 根据大小判断
        if (vecFour.size() > 0)
        {
            // 炸弹只能有一个四
            if(vecFour.size() == 1)
            {
                int nRemainCard = vecSingle.size() + vecCouple.size() * 2 + vecTriple.size() * 3;
                if (nRemainCard == 0)
                {
                    return EM_Bomb_CardGroup;
                }
                else if (nRemainCard == 2)
                {
                    return EM_FourWithCouple_CardGroup;
                }
                else if (vecTriple.size() > 1)
                {
                    if (GlobalUtils::CheckCardSequence(vecTriple))
                    {
                        int nRemainSingle = vecSingle.size() + vecCouple.size() * 2 + vecFour.size() * 4;
                        if (nRemainSingle == vecTriple.size())
                        {
                            emType = EM_PlaneWithone_CardGroup;
                        }
                        else
                        {
                            emType = EM_CardGroup_Illegal;
                        }
                        if (emType == EM_CardGroup_Illegal)
                        {
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
            else if (GlobalUtils::CheckCardSequence(vecTriple))
            {
                if (vecSingle.size() > 0)
                {
                    int nRemainSingle = vecSingle.size() + vecCouple.size() * 2 + vecFour.size() * 4;
                    if (nRemainSingle == 0)
                    {
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
                    int nRemainCouple = vecCouple.size() + vecFour.size() * 2;
                    if (nRemainCouple == 0)
                    {
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
            if (vecCouple.size() > g_nMoveCoupleCardCountLimit - 1 && GlobalUtils::CheckCardSequence(vecCouple))
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
            if (vecSingle.size() > g_nMoveLineCardCountLimit - 1 && GlobalUtils::CheckCardSequence(vecSingle))
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

    // 如果要压牌，需要比较能否压住当前牌
    if (pCurPile == nullptr)
    {
        return emType;
    }
    else
    {
        if(emType == pCurPile->CheckPile(nullptr))
        {
            switch(emType)
            {
                // 简单比较，单张对子炸弹直接比较第一张牌（排完序）
            case EM_Single_CardGroup:
            case EM_Couple_CardGroup:
            case EM_TripleWithout_CardGroup:
            case EM_Bomb_CardGroup:
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
                    // 需要比较带牌中的
                    std::vector<int> vecCurSingle;
                    std::vector<int> vecCurCouple;
                    std::vector<int> vecCurTriple;
                    std::vector<int> vecCurFour;
                    GlobalUtils::SplitCard(pCurPile->m_vecCard, vecCurSingle, vecCurCouple, vecCurTriple, vecCurFour);
                    switch(emType)
                    {
                    case EM_TripleWithOne_CardGroup:
                    case EM_TripleWithCouple_CardGroup:
                        {
                            Q_ASSERT(vecTriple.size() == 1);
                            Q_ASSERT(vecCurTriple.size() == 1);
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
                            Q_ASSERT(vecFour.size() == 1);
                            Q_ASSERT(vecCurTriple.size() == 1);
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
                            Q_ASSERT(vecTriple.size() > 0);
                            Q_ASSERT(vecCurTriple.size() > 0);
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
            // 王炸可以压所有牌，炸弹可以压除王炸外所有牌
            if (emType == EM_JokingBomb_CardGroup)
            {
                return EM_JokingBomb_CardGroup;
            }
            else if (emType == EM_Bomb_CardGroup && pCurPile->CheckPile(nullptr) != EM_JokingBomb_CardGroup)
            {
                return EM_Bomb_CardGroup;
            }
            return EM_CardGroup_Illegal;
        }
    }
    return EM_CardGroup_Illegal;
}
