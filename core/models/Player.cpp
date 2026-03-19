#include "Player.h"
#include "core/utils/GlobalUtils.h"
#include "core/ai/RobotPilot.h"
#include "core/models/Judge.h"
#include <QCoreApplication>
#include <QPainter>

Player::Player(void)
    : m_strID("")
    , m_strName("")
    , m_emPlayerType(EM_Mid_Player)
    , m_bShowValue(false)
    , m_bLandOwerCardHolder(false)
    , m_bLandOwer(false)
    , m_nLandOwerScore(-1)
    , m_strWord("")
{
    ShowBound(true);
    SetBoundColor(ColorDefine::PlayerBound);
    SetBoundWidth(2);
    ShowBackground(true);
    SetBackColor(ColorDefine::PlayerBack);
}

Player::~Player(void)
{
}

void Player::DrawPlayerHead(QPainter *painter, QRect rtBound)
{
    painter->save();

    painter->setPen(QPen(ColorDefine::PlayerHeadBound, 2));
    painter->setBrush(ColorDefine::PlayerHeadBack);
    painter->drawRect(rtBound);

    // 使用白色文字，深色背景上更清晰
    painter->setPen(Qt::white);
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(font.pointSize() + 1);
    painter->setFont(font);

    if (m_bLandOwer)
    {
        painter->drawText(rtBound, QString("%1\n%2").arg(m_strName).arg(QCoreApplication::translate("Player", "地主")), QTextOption(Qt::AlignCenter));
    }
    else if (m_bLandOwerCardHolder)
    {
        painter->drawText(rtBound, QString("%1\n%2").arg(m_strName).arg(QCoreApplication::translate("Player", "农民")), QTextOption(Qt::AlignCenter));
    }
    else
    {
        painter->drawText(rtBound, m_strName, QTextOption(Qt::AlignCenter));
    }

    painter->restore();
}

void Player::DrawCard(QPainter *painter, QRect rtBound)
{
    painter->save();

    painter->setPen(QPen(ColorDefine::PlayerCardBound, 2));
    painter->setBrush(ColorDefine::PlayerCardBack);
    painter->drawRect(rtBound);

    m_stuHandCard.ShowValue(m_bShowValue);
    m_stuHandCard.SetBound(rtBound);
    m_stuHandCard.DrawTo(painter);

    painter->restore();
}

void Player::DrawTo(QPainter *painter)
{
    IDrawable::DrawTo(painter);

    QRect rtRegion = GetRegion();

    QRect rtHead(0, 0, g_nHeadWidth, g_nHeadHeight);

    bool bVertical = false;
    if (m_emPlayerType != EM_Mid_Player)
    {
        bVertical = true;
    }

    QRect rtCard;
    if (bVertical)
    {
        rtHead.translate(m_ptPosition.x() + (rtRegion.width() - rtHead.width()) / 2, m_ptPosition.y() + g_nGap);

        rtCard.setRect(0, 0, g_nCardWidth, rtRegion.height() - rtHead.height() - g_nGap * 3);
        rtCard.translate(rtRegion.left() + g_nGap, rtHead.bottom() + g_nGap);
    }
    else
    {
        rtHead.translate(m_ptPosition.x() + g_nGap, m_ptPosition.y() + (rtRegion.height() - rtHead.height()) / 2);

        rtCard.setRect(0, 0, rtRegion.width() - rtHead.width() - g_nGap * 3, g_nCardHeight);
        rtCard.translate(rtHead.right() + g_nGap, rtRegion.top() + g_nGap);
    }

    DrawPlayerHead(painter, rtHead);
    DrawCard(painter, rtCard);

    if (!m_strWord.isEmpty())
    {
        // 使用更大、更粗的白色文字，更醒目
        QFont font = painter->font();
        font.setBold(true);
        font.setPointSize(font.pointSize() + 2);
        painter->setFont(font);
        painter->setPen(Qt::white);

        QFontMetrics fm(font);
        QSize szWord = fm.size(Qt::TextSingleLine, m_strWord);
        QRect rtWord;
        switch(m_emPlayerType)
        {
        case EM_Left_Player:
            rtWord.setLeft(rtCard.right() + g_nGap);
            rtWord.setTop(rtHead.top() + g_nGap);
            rtWord.setRight(rtWord.left() + szWord.width());
            rtWord.setBottom(rtWord.top() + szWord.height());
            break;
        case EM_Mid_Player:
            rtWord.setLeft(rtHead.left() + g_nGap);
            rtWord.setTop(rtCard.top() - g_nGap - szWord.height());
            rtWord.setRight(rtWord.left() + szWord.width());
            rtWord.setBottom(rtWord.top() + szWord.height());
            break;
        case EM_Right_Player:
            rtWord.setLeft(rtCard.left() - g_nGap - szWord.width());
            rtWord.setTop(rtHead.top() + g_nGap);
            rtWord.setRight(rtWord.left() + szWord.width());
            rtWord.setBottom(rtWord.top() + szWord.height());
            break;
        default:
            break;
        }
        // 添加黑色描边背景让文字更清晰
        painter->fillRect(rtWord.adjusted(-2, -2, 2, 2), QColor(0, 0, 0, 160));
        painter->drawText(rtWord, m_strWord, QTextOption(Qt::AlignCenter));
    }
}

void Player::ExcuteCallLandOwer()
{
    if (IsAIPlayer())
    {
        int nScore = RobotPilot::Instance().DecideCallLandOwer(m_strID);
        Judge::Instance().CurPlayerCallScore(nScore);
    }
    else
    {
        Judge::Instance().ShowCallLandOwerBtn(true);
    }
}

void Player::ExcuteCallCardPlay()
{
    if (IsAIPlayer())
    {
        std::vector<int> vecCardToPlay = RobotPilot::Instance().DecideCardToPlay(m_strID);
        SetOnHoldCard(vecCardToPlay);
        Judge::Instance().CurPlayerPlayCard();
    }
    else
    {
        Judge::Instance().ShowCardPlayBtn(true);
    }
}
