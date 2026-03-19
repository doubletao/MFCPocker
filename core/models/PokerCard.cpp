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

// 获取花色符号
static const char* getSuitSymbol(CardSuit suit)
{
    switch (suit)
    {
    case CardSuit::Spade: return "♠";
    case CardSuit::Heart: return "♥";
    case CardSuit::Club: return "♣";
    case CardSuit::Diamond: return "♦";
    default: return "";
    }
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

    if (!m_bShowValue)
    {
        // 牌背面：绘制重复花纹
        int tileSize = 12;
        QColor backPatternColor(100, 150, 100);
        painter->setPen(QPen(backPatternColor, 1));
        // 绘制网格状花纹
        for (int x = rtRegion.left() + 8; x < rtRegion.right(); x += tileSize)
        {
            for (int y = rtRegion.top() + 8; y < rtRegion.bottom(); y += tileSize)
            {
                if ((x / tileSize + y / tileSize) % 2 == 0)
                {
                    painter->drawEllipse(x, y, 6, 6);
                }
            }
        }
        // 中心绘制一个大菱形
        int cx = rtRegion.center().x();
        int cy = rtRegion.center().y();
        QPolygon diamond;
        diamond << QPoint(cx, cy - 20) << QPoint(cx + 20, cy) << QPoint(cx, cy + 20) << QPoint(cx - 20, cy);
        painter->setBrush(QColor(80, 130, 80));
        painter->drawPolygon(diamond);
    }
    else
    {
        QString strTxt = GetDisplayTxt();
        int nGap = 4;
        QColor textColor = IsRedSuit() ? Qt::red : Qt::black;

        if (m_nValue >= 13) // Joker
        {
            QFont font = painter->font();
            font.setPointSize(12);
            font.setBold(true);
            painter->setFont(font);
            painter->setPen(textColor);
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

            // Joker中间绘制大花色符号
            const char* bigSymbol = (m_suit == CardSuit::JokerSmall) ? "小王" : "大王";
            QFont bigFont = painter->font();
            bigFont.setPointSize(28);
            bigFont.setBold(true);
            painter->setFont(bigFont);
            QFontMetrics bigFm(bigFont);
            int centerX = rtRegion.center().x() - bigFm.horizontalAdvance(bigSymbol) / 2;
            int centerY = rtRegion.center().y() + bigFm.ascent() / 2;
            painter->drawText(centerX, centerY, QString(bigSymbol));
        }
        else
        {
            QFont font = painter->font();
            font.setPointSize(16);
            font.setBold(true);
            painter->setFont(font);
            painter->setPen(textColor);
            QFontMetrics fm(font);
            // 普通牌正常显示数字
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

            // 在牌面中心绘制大的花色符号
            const char* suitSymbol = getSuitSymbol(m_suit);
            if (suitSymbol[0] != '\0')
            {
                QFont suitFont = painter->font();
                suitFont.setPointSize(32);
                painter->setFont(suitFont);
                QFontMetrics suitFm(suitFont);
                int centerX = rtRegion.center().x() - suitFm.horizontalAdvance(suitSymbol) / 2;
                int centerY = rtRegion.center().y() + suitFm.ascent() / 2;
                painter->drawText(centerX, centerY, QString(suitSymbol));
            }
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
