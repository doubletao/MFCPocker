#pragma once
//////////////////////////////////////////////////////////////////////////
//排序接口
//////////////////////////////////////////////////////////////////////////
class ISortable
{
public:
	ISortable(void);
	virtual ~ISortable(void) = 0;
	bool operator < (const ISortable & value) const;
	virtual int GetSortFactor() const = 0;//获取排序因子
};
