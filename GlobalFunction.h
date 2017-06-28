#pragma once
//////////////////////////////////////////////////////////////////////////
//���ߺ���
//////////////////////////////////////////////////////////////////////////
#include <vector>
using std::vector;
#include <map>
using std::multimap;
using std::make_pair;

class ICard;
class GlobalFunction
{
private:
	GlobalFunction(void);
	~GlobalFunction(void);
public:
	static CString GetNewGUID();//�����µ�GUID
	static HRESULT GuidToString(const GUID& guid, CString& str);//ת��GUID���ַ���
	static void SplitString(const CString & strSource, LPCTSTR szToken ,CStringArray &strArr );//����ַ�������szTokenΪ�ָ������szStr���ŵ�strArr��
	static int ConvertCString2Int(CString strValue);
	static BOOL CheckCardSequence(vector<int> vecValue, BOOL bNeedSort = FALSE);//�ж�������Ԫ���Ƿ������������������A�ⶥ��2�ʹ�Сë�����룩
	static void SplitCard(vector<ICard *> & vecCard, vector<int> & vecSingle, vector<int> & vecCouple, vector<int> & vecTriple, vector<int> & vecFour, BOOL bNeedSort = FALSE);//�ָ����飬�����鰴��Value�ָ�ɵ��ż��ϣ����Ӽ��ϣ����ż��Ϻ����ż��ϣ�������һ�����ж�
	static vector<int> GetIndexByValueAndCount(vector<ICard *> & vecCard, UINT nValue, UINT nCount, BOOL bNeedSort = FALSE);//��ȡָ���ƶ���nCount��ֵΪnValue�������
	template <typename T> static inline void Swap(T & a, T & b);//��������Ԫ�ص�ֵ������=�������
	template <typename T> static inline void SortVecPointorByValue(vector<T> & vecContainer, BOOL bDec);//��vec��ָ��Ԫ�ظ�����ָ��ֵ��������
	template <typename T> static inline void RandomSortVector(vector<T> & vecContainer);//��vec��ָ��Ԫ�ظ�����ָ��ֵ������������
	template <typename T> static inline void ReleasePointorContainer(T & container);//�ͷ�ָ�������ڵ�Ԫ��
};

template <typename T>
void GlobalFunction::Swap(T & a, T & b)
{
	T temp = a;
	a = b;
	b = temp;
}

template <typename T>
void GlobalFunction::SortVecPointorByValue(vector<T> & vecContainer, BOOL bDec)
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
void GlobalFunction::RandomSortVector(vector<T> & vecContainer)
{
	multimap<int, T> mapSortor;
	for (UINT i = 0; i < vecContainer.size(); i++)
	{
		mapSortor.insert(make_pair(rand(), vecContainer[i]));
	}
	vecContainer.clear();
	for (multimap<int, T>::iterator it = mapSortor.begin(); it != mapSortor.end(); it++)
	{
		vecContainer.push_back(it->second);
	}
}

template <typename T>
void GlobalFunction::ReleasePointorContainer(T & container)
{
	for (T::iterator it = container.begin(); it != container.end(); it++)
	{
		if (*it != NULL)
		{
			SAFE_DELETE(*it);
		}
	}
	container.clear();
}

