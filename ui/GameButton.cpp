#include "GameButton.h"
#include <QPainter>

GameButton::GameButton(void)
    : m_bShow(false)
{
    SetWidth(g_nButtonWidth);
    SetHeight(g_nButtonHeight);
    ShowBound(true);
    SetBoundColor(ColorDefine::ButtonBound);
    SetBoundWidth(2);
    ShowBackground(true);
    SetBackColor(ColorDefine::ButtonBack);
}

GameButton::~GameButton(void)
{
}

bool GameButton::HitTest(QPoint pt)
{
    if (m_bShow)
    {
        QRect rtRegion = GetRegion();
        if (rtRegion.contains(pt))
        {
            emit clicked(GetID());
            return true;
        }
    }
    return false;
}

void GameButton::DrawTo(QPainter *painter)
{
    if (m_bShow)
    {
        IDrawable::DrawTo(painter);
        painter->drawText(GetRegion(), m_strBtnTxt, QTextOption(Qt::AlignCenter));
    }
}
