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
    QT_TRANSLATE_NOOP("PokerCard", "joker"),
    QT_TRANSLATE_NOOP("PokerCard", "JOKER")
};

PokerCard::PokerCard(void)
{
    SetWidth(g_nCardWidth);
    SetHeight(g_nCardHeight);
    ShowBound(true);
    SetBoundColor(ColorDefine::PokerCardBound);
    SetBoundWidth(2);
    ShowBackground(true);
    SetBackColor(ColorDefine::PokerCardBack);
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
    if (m_bShowValue)
    {
        painter->save();

        QRect rtValueRect = rtRegion.adjusted(2, 2, -2, -2);
        painter->setPen(ColorDefine::PokerCardValueBound);
        painter->setBrush(ColorDefine::PokerCardValueBack);
        painter->drawRect(rtValueRect);

        QString strTxt = GetDisplayTxt();
        QFont font = painter->font();
        QFontMetrics fm(font);

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

        int nGap = 2;

        QRect rtRightTop(
            rtRegion.right() - nMaxWidth - nGap,
            rtRegion.top() + nGap,
            nMaxWidth,
            nHeight + nGap
        );
        QRect rtLeftBottom(
            rtRegion.left() + nGap,
            rtRegion.bottom() - nHeight - nGap - nGap,
            nMaxWidth,
            nHeight + nGap
        );

        painter->drawText(rtRightTop, strTxt, QTextOption(Qt::AlignLeft | Qt::AlignTop));
        painter->drawText(rtLeftBottom, strTxt, QTextOption(Qt::AlignLeft | Qt::AlignTop));

        painter->restore();
    }
    else
    {
        ICard::DrawTo(painter);
    }
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
