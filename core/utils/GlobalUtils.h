#pragma once
//////////////////////////////////////////////////////////////////////////
// 工具类
//////////////////////////////////////////////////////////////////////////
#include <vector>
#include <map>
#include <QString>
#include <QStringList>

class ICard;

#define SAFE_DELETE(p) do { if (p) { delete p; p = nullptr; } } while(0)

class GlobalUtils
{
private:
    GlobalUtils(void);
    ~GlobalUtils(void);
public:
    static void SplitString(const QString & strSource, const QString &token, QStringList &strArr);
    static int ConvertQString2Int(const QString &strValue);
    static bool CheckCardSequence(std::vector<int> vecValue, bool bNeedSort = false);
    static void SplitCard(std::vector<ICard *> & vecCard, std::vector<int> & vecSingle, std::vector<int> & vecCouple, std::vector<int> & vecTriple, std::vector<int> & vecFour, bool bNeedSort = false);
    static std::vector<int> GetIndexByValueAndCount(std::vector<ICard *> & vecCard, unsigned int nValue, unsigned int nCount, bool bNeedSort = false);
    template <typename T> static inline void Swap(T & a, T & b);
    template <typename T> static inline void SortVecPointerByValue(std::vector<T> & vecContainer, bool bDec);
    template <typename T> static inline void RandomSortVector(std::vector<T> & vecContainer);
    template <typename T> static inline void ReleasePointerContainer(T & container);
};

template <typename T>
void GlobalUtils::Swap(T & a, T & b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void GlobalUtils::SortVecPointerByValue(std::vector<T> & vecContainer, bool bDec)
{
    int n = (int)vecContainer.size();
    if (bDec)
    {
        for (int i = 0; i < n; i++ )
        {
            for (int j = i; j < n; j++)
            {
                if (*vecContainer[i] < *vecContainer[j])
                {
                    Swap(vecContainer[i], vecContainer[j]);
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++ )
        {
            for (int j = i; j < n; j++)
            {
                if (!(*vecContainer[i] < *vecContainer[j]))
                {
                    Swap(vecContainer[i], vecContainer[j]);
                }
            }
        }
    }
}

template <typename T>
void GlobalUtils::RandomSortVector(std::vector<T> & vecContainer)
{
    std::multimap<int, T> mapSorter;
    for (unsigned int i = 0; i < vecContainer.size(); i++)
    {
        mapSorter.insert(std::make_pair(rand(), vecContainer[i]));
    }
    vecContainer.clear();
    for (typename std::multimap<int, T>::iterator it = mapSorter.begin(); it != mapSorter.end(); it++)
    {
        vecContainer.push_back(it->second);
    }
}

template <typename T>
void GlobalUtils::ReleasePointerContainer(T & container)
{
    for (typename T::iterator it = container.begin(); it != container.end(); it++)
    {
        if (*it != nullptr)
        {
            SAFE_DELETE(*it);
        }
    }
    container.clear();
}
