#include "GameButton.h"
#include <QPainter>

GameButton::GameButton(void)
    : m_bShow(false)
{
    SetWidth(g_nButtonWidth);
    SetHeight(g_nButtonHeight);
    ShowBound(true);
    SetBoundColor(QColor(100, 150, 200));
    SetBoundWidth(1);
    ShowBackground(true);
    SetBackColor(QColor(230, 240, 250));
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
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QRect region = GetRegion();
        // 绘制圆角按钮背景和边框
        painter->setPen(QPen(GetBoundColor(), GetBoundWidth()));
        painter->setBrush(GetBackColor());
        painter->drawRoundedRect(region, 6, 6);

        // 绘制居中文字
        QFont font = painter->font();
        font.setPointSize(12);
        font.setBold(true);
        painter->setFont(font);
        painter->setPen(Qt::black);
        painter->drawText(region, m_strBtnTxt, QTextOption(Qt::AlignCenter));

        painter->restore();
    }
}
