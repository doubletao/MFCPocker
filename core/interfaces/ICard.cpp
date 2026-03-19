#include "ICard.h"

ICard::ICard(void)
    : m_bShowValue(false)
    , m_nValue(-1)
    , m_bSelect(false)
{
}

ICard::~ICard(void)
{
}

int ICard::GetSortFactor() const
{
    return ISortable::GetSortFactor();
}

void ICard::DrawTo(QPainter *painter)
{
    return IDrawable::DrawTo(painter);
}

void ICard::SelectCard(bool bSet)
{
    m_bSelect = bSet && m_bShowValue;
}
