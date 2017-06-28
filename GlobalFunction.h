#pragma once
//////////////////////////////////////////////////////////////////////////
//工具函数
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
	static CString GetNewGUID();//生成新的GUID
	static HRESULT GuidToString(const GUID& guid, CString& str);//转换GUID到字符串
	static void SplitString(const CString & strSource, LPCTSTR szToken ,CStringArray &strArr );//拆分字符串，以szToken为分隔符拆分szStr，放到strArr中
	static int ConvertCString2Int(CString strValue);
	static BOOL CheckCardSequence(vector<int> vecValue, BOOL bNeedSort = FALSE);//判定数组内元素是否符合连续规则（连续到A封顶，2和大小毛不计入）
	static void SplitCard(vector<ICard *> & vecCard, vector<int> & vecSingle, vector<int> & vecCouple, vector<int> & vecTriple, vector<int> & vecFour, BOOL bNeedSort = FALSE);//分割牌组，将牌组按照Value分割成单张集合，对子集合，三张集合和四张集合，便于下一步的判定
	static vector<int> GetIndexByValueAndCount(vector<ICard *> & vecCard, UINT nValue, UINT nCount, BOOL bNeedSort = FALSE);//获取指定牌堆中nCount张值为nValue的牌序号
	template <typename T> static inline void Swap(T & a, T & b);//交换两个元素的值（依赖=运算符）
	template <typename T> static inline void SortVecPointorByValue(vector<T> & vecContainer, BOOL bDec);//对vec内指针元素根据其指针值进行排序
	template <typename T> static inline void RandomSortVector(vector<T> & vecContainer);//对vec内指针元素根据其指针值进行乱序排列
	template <typename T> static inline void ReleasePointorContainer(T & container);//释放指针容器内的元素
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

