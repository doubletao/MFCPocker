#include "StdAfx.h"
#include "ISortable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ISortable::ISortable(void)
{
}


ISortable::~ISortable(void)
{
}

int ISortable::GetSortFactor() const
{
	return 0;
}

bool ISortable::operator<(const ISortable & value) const
{
	return GetSortFactor() < value.GetSortFactor();
}
