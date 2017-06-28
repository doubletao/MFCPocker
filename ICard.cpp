#include "StdAfx.h"
#include "ICard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ICard::ICard(void)
	: m_bShowValue(FALSE)
	, m_nValue(-1)
	, m_bSelect(FALSE)
{
}


ICard::~ICard(void)
{
}

int ICard::GetSortFactor() const
{
	return ISortable::GetSortFactor();
}

void ICard::DrawTo(CDC * pDC)
{
	return IDrawable::DrawTo(pDC);
}

void ICard::SelectCard(BOOL bSet)
{
	m_bSelect = bSet && m_bShowValue;
}
