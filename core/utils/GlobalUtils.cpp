#include "GlobalUtils.h"
#include "core/interfaces/ICard.h"
#include <algorithm>
#include <cstdlib>

GlobalUtils::GlobalUtils(void)
{
}

GlobalUtils::~GlobalUtils(void)
{
}

void GlobalUtils::SplitString(const QString & strSource, const QString &token, QStringList &strArr)
{
    strArr.clear();

    int nStart = 0;
    int nLen = token.length();
    while(true)
    {
        int i = strSource.indexOf(token, nStart);
        if(i < 0)
            break;

        if(i >= nStart)
        {
            QString s = strSource.mid(nStart, i - nStart);
            strArr.append(s);
        }

        nStart = i + nLen;
    }

    if(nStart <= strSource.length())
    {
        QString strLast = strSource.mid(nStart);
        if (!strLast.isEmpty())
        {
            strArr.append(strLast);
        }
    }
}

int GlobalUtils::ConvertQString2Int(const QString &strValue)
{
    return strValue.toInt();
}

bool GlobalUtils::CheckCardSequence(std::vector<int> vecValue, bool bNeedSort)
{
    if (bNeedSort)
    {
        std::sort(vecValue.begin(), vecValue.end());
    }
    // 检查每个元素是否连续递增，允许2-K（value<12）
    for (unsigned int i = 0; i < vecValue.size() - 1; i++)
    {
        if (vecValue[i] != vecValue[i + 1] + 1 || vecValue[i] > 11)
        {
            return false;
        }
    }
    return true;
}

void GlobalUtils::SplitCard(std::vector<ICard *> & vecCard, std::vector<int> & vecSingle, std::vector<int> & vecCouple, std::vector<int> & vecTriple, std::vector<int> & vecFour, bool bNeedSort)
{
    if (bNeedSort)
    {
        SortVecPointerByValue(vecCard, false);
    }
    int nCount = 0;
    int nValue = 0;
    for (unsigned int i = 0; i < vecCard.size(); i++)
    {
        if (nValue != vecCard[i]->GetValue())
        {
            switch(nCount)
            {
            case 1:
                vecSingle.push_back(nValue);
                break;
            case 2:
                vecCouple.push_back(nValue);
                break;
            case 3:
                vecTriple.push_back(nValue);
                break;
            case 4:
                vecFour.push_back(nValue);
                break;
            default:
                break;
            }
            nValue = vecCard[i]->GetValue();
            nCount = 1;
        }
        else
        {
            nCount++;
        }
    }
    switch(nCount)
    {
    case 1:
        vecSingle.push_back(nValue);
        break;
    case 2:
        vecCouple.push_back(nValue);
        break;
    case 3:
        vecTriple.push_back(nValue);
        break;
    case 4:
        vecFour.push_back(nValue);
        break;
    default:
        break;
    }
}

std::vector<int> GlobalUtils::GetIndexByValueAndCount(std::vector<ICard *> & vecCard, unsigned int nValue, unsigned int nCount, bool bNeedSort)
{
    std::vector<int> vecRet;
    if (bNeedSort)
    {
        SortVecPointerByValue(vecCard, false);
    }
    for (unsigned int i = 0; i < vecCard.size(); i++)
    {
        if (vecCard[i]->GetValue() == nValue)
        {
            vecRet.push_back(i);
        }
        if (vecRet.size() == nCount)
        {
            break;
        }
    }
    if (vecRet.size() < nCount)
    {
        vecRet.clear();
    }
    return vecRet;
}
