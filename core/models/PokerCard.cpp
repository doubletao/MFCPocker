#include "PokerCard.h"
#include "core/interfaces/IDrawable.h"
#include <QPainter>
#include <QFontMetrics>

const QString g_strCardDisp[] = {
    QT_TRANSLATE_NOOP("PokerCard", "3"),
    QT_TRANSLATE_NOOP("PokerCard", "4"),
    QT_TRANSLATE_NOOP("PokerCard", "5"),
    QT_TRANSLATE_NOOP("PokerCard", "6"),
    QT_TRANSLATE_NOOP("PokerCard", "7"),
    QT_TRANSLATE_NOOP("PokerCard", "8"),
    QT_TRANSLATE_NOOP("PokerCard", "9"),
    QT_TRANSLATE_NOOP("PokerCard", "10"),
    QT_TRANSLATE_NOOP("PokerCard", "J"),
    QT_TRANSLATE_NOOP("PokerCard", "Q"),
    QT_TRANSLATE_NOOP("PokerCard", "K"),
    QT_TRANSLATE_NOOP("PokerCard", "A"),
    QT_TRANSLATE_NOOP("PokerCard", "2"),
    QT_TRANSLATE_NOOP("PokerCard", "Joker"),
    QT_TRANSLATE_NOOP("PokerCard", "JOKER")
};

PokerCard::PokerCard(void)
{
    SetWidth(g_nCardWidth);
    SetHeight(g_nCardHeight);
    ShowBound(true);
    SetBoundColor(QColor(180, 180, 180));
    SetBoundWidth(1);
    ShowBackground(true);
    SetBackColor(Qt::white);
}

PokerCard::~PokerCard(void)
{
}

QString PokerCard::GetDisplayTxt()
{
    Q_ASSERT(ValidValue(m_nValue));
    return g_strCardDisp[m_nValue];
}

void PokerCard::DrawTo(QPainter *painter)
{
    QRect rtRegion = GetRegion();
    // 如果选中，牌向上偏移10像素
    if (m_bSelect)
    {
        rtRegion.translate(0, -10);
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 绘制圆角矩形背景和边框
    painter->setPen(QPen(GetBoundColor(), GetBoundWidth()));
    painter->setBrush(GetBackColor());
    painter->drawRoundedRect(rtRegion, 4, 4);

    if (m_bShowValue)
    {
        QString strTxt = GetDisplayTxt();
        int nGap = 4;

        // Joker使用红色，其他黑色，字号更小
        if (m_nValue >= 13) // joker 和 JOKER
        {
            QFont font = painter->font();
            font.setPointSize(12);
            font.setBold(true);
            painter->setFont(font);
            painter->setPen(Qt::red);
            QFontMetrics fm(font);

            // Joker 逐字符竖排显示，不旋转，简单可靠
            int lineHeight = fm.height();
            // 左上角绘制，每个字符占一行
            int currentX = rtRegion.left() + nGap;
            int currentY = rtRegion.top() + nGap;
            for (QChar c : strTxt)
            {
                painter->drawText(currentX, currentY + fm.ascent(), QString(c));
                currentY += lineHeight;
            }
            // 右下角绘制
            currentX = rtRegion.right() - nGap - fm.maxWidth();
            currentY = rtRegion.bottom() - nGap - strTxt.length() * lineHeight;
            for (QChar c : strTxt)
            {
                painter->drawText(currentX, currentY + fm.ascent(), QString(c));
                currentY += lineHeight;
            }
        }
        else
        {
            QFont font = painter->font();
            font.setPointSize(16);
            font.setBold(true);
            painter->setFont(font);
            painter->setPen(Qt::black);
            QFontMetrics fm(font);
            // 普通牌正常显示
            int nWidth = 0;
            int nHeight = 0;
            int nMaxWidth = 0;

            for (int i = 0; i < strTxt.length(); i++)
            {
                if (strTxt[i] == '\n')
                {
                    nHeight += fm.height();
                    nMaxWidth = qMax(nMaxWidth, nWidth);
                    nWidth = 0;
                }
                else
                {
                    nWidth += fm.horizontalAdvance(strTxt[i]);
                }
            }
            nHeight += fm.height();
            nMaxWidth = qMax(nMaxWidth, nWidth);

            QRect rtLeftTop(
                rtRegion.left() + nGap,
                rtRegion.top() + nGap,
                nMaxWidth,
                nHeight + nGap
            );
            QRect rtRightBottom(
                rtRegion.right() - nMaxWidth - nGap,
                rtRegion.bottom() - nHeight - nGap - nGap,
                nMaxWidth,
                nHeight + nGap
            );

            painter->drawText(rtLeftTop, strTxt, QTextOption(Qt::AlignLeft | Qt::AlignTop));
            painter->drawText(rtRightBottom, strTxt, QTextOption(Qt::AlignRight | Qt::AlignBottom));
        }
    }

    painter->restore();
}

bool PokerCard::ValidValue(int nValue)
{
    if (nValue > -1 && nValue < g_nCardDispModel)
    {
        return true;
    }
    return false;
}

bool PokerCard::HitTest(QPoint pt)
{
    if (m_bShowValue)
    {
        QRect rtRegion = GetRegion();
        // 选中的卡牌绘制时向上偏移了10像素，点击检测也需要对应偏移
        if (m_bSelect)
        {
            rtRegion.translate(0, -10);
        }
        if (rtRegion.contains(pt))
        {
            return true;
        }
    }
    return false;
}
