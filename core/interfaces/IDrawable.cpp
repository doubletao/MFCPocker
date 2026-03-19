#include "IDrawable.h"

IDrawable::IDrawable(void)
    : m_bDrawBackground(false)
    , m_nBackColor(QColor())
    , m_bDrawBound(false)
    , m_nBoundColor(QColor())
    , m_nBoundWidth(0)
    , m_nWidth(0)
    , m_nHeight(0)
    , m_ptPosition(0, 0)
{
}

IDrawable::~IDrawable(void)
{
}

void IDrawable::DrawTo(QPainter *painter)
{
    QRect rtRegion = GetRegion();

    // 保存画笔状态
    painter->save();

    if (m_bDrawBound) {
        QPen pen(m_nBoundColor, m_nBoundWidth);
        painter->setPen(pen);
    } else {
        painter->setPen(Qt::NoPen);
    }

    if (m_bDrawBackground) {
        painter->fillRect(rtRegion, m_nBackColor);
    } else {
        painter->setBrush(Qt::NoBrush);
    }

    painter->drawRect(rtRegion);
    painter->restore();
}

void IDrawable::SetBound(QRect rtBound)
{
    m_nWidth = rtBound.width();
    m_nHeight = rtBound.height();
    m_ptPosition = rtBound.topLeft();
}

void IDrawable::SetWidth(int nWidth)
{
    m_nWidth = nWidth;
}

void IDrawable::SetHeight(int nHeight)
{
    m_nHeight = nHeight;
}

void IDrawable::SetPosition(QPoint ptPos)
{
    m_ptPosition = ptPos;
}

int IDrawable::GetWidth()
{
    return m_nWidth;
}

int IDrawable::GetHeight()
{
    return m_nHeight;
}

QPoint IDrawable::GetPosition()
{
    return m_ptPosition;
}

QRect IDrawable::GetRegion()
{
    QRect rtRegion(0, 0, m_nWidth, m_nHeight);
    rtRegion.translate(m_ptPosition.x(), m_ptPosition.y());
    return rtRegion;
}

