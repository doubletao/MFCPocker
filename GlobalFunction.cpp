#include "StdAfx.h"
#include "GlobalFunction.h"
#include "ICard.h"
#include <algorithm>
#pragma comment(lib, "Rpcrt4.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GlobalFunction::GlobalFunction(void)
{
}


GlobalFunction::~GlobalFunction(void)
{
}

CString GlobalFunction::GetNewGUID()
{
	GUID guid = GUID_NULL;
	::CoCreateGuid(&guid);
	CString strGUID;
	GuidToString( guid, strGUID );
	strGUID.MakeUpper();
	return strGUID;
}

HRESULT GlobalFunction::GuidToString(const GUID& guid, CString& str)
{
	GUID guidNo = guid;
#ifdef UNICODE
	RPC_WSTR pUuidString;
#else
	BYTE* pUuidString;
#endif
	UuidToString(&guidNo, &pUuidString);
	str =(LPCTSTR)pUuidString;
	str.MakeUpper();

	if (RPC_S_OK == RpcStringFree(&pUuidString))
		return S_OK;

	return S_FALSE;
}

void GlobalFunction::SplitString(const CString & strSource, LPCTSTR szToken ,CStringArray &strArr)
{
	strArr.RemoveAll();

	//开始解析
	int nStart = 0;
	int nLen = _tcslen(szToken);
	while(true)
	{
		int i = strSource.Find(szToken, nStart);
		if(i < 0)
			break;

		if(i >= nStart)
		{
			CString s = strSource.Mid(nStart, i - nStart);
			strArr.Add(s);
		}

		nStart = i + nLen;
	}

	//剩下的字符串
	if(nStart <= strSource.GetLength())
	{
		CString strLast(strSource.Mid(nStart));
		if (!strLast.IsEmpty())
		{
			strArr.Add(strSource.Mid(nStart));
		}
	}
}

int GlobalFunction::ConvertCString2Int(CString strValue)
{
	int nRet = _ttoi(strValue);
	return nRet;
}

BOOL GlobalFunction::CheckCardSequence( vector<int> vecValue, BOOL bNeedSort /*= FALSE*/ )
{
	if (bNeedSort)
	{
		std::sort(vecValue.begin(), vecValue.end());
	}
	//牌连续的概念是必须严格递减，且不能出现2及双毛(value<12)
	for (UINT i = 0; i < vecValue.size() - 1; i++)
	{
		//后一项加1不等于前一项，或者前一项大于12，立马否定
		if (vecValue[i] != vecValue[i + 1] + 1 || vecValue[i] > 11)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void GlobalFunction::SplitCard( vector<ICard *> & vecCard, vector<int> & vecSingle, vector<int> & vecCouple, vector<int> & vecTriple, vector<int> & vecFour, BOOL bNeedSort /*= FALSE*/ )
{
	if (bNeedSort)
	{
		//排序
		SortVecPointorByValue(vecCard, FALSE);
	}
	int nCount = 0;//统计当前值出现次数
	int nValue = 0;//当前值
	for (UINT i = 0; i < vecCard.size(); i++)
	{
		if (nValue != vecCard[i]->GetValue())
		{
			//如果不相等，判定前一个nCount，如果大于0，则将上一个Value加入到对应集合
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
	//循环结束时将最后一个元素加入进去
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

vector<int> GlobalFunction::GetIndexByValueAndCount( vector<ICard *> & vecCard, UINT nValue, UINT nCount, BOOL bNeedSort /*= FALSE*/ )
{
	vector<int> vecRet;
	if (bNeedSort)
	{
		SortVecPointorByValue(vecCard, FALSE);
	}
	for (UINT i = 0; i < vecCard.size(); i++)
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
	//如果没有找齐，则清空结果，只返回空集或正确结果
	if (vecRet.size() < nCount)
	{
		vecRet.clear();
	}
	return vecRet;
}
