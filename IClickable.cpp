#include "StdAfx.h"
#include "IClickable.h"


IClickable::IClickable(void)
	: m_pCallBackFun(NULL)
	, m_dwID(0)
{
}


IClickable::~IClickable(void)
{
}
