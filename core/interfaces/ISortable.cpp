#include "ISortable.h"

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
