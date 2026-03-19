#include "GameWidget.h"
#include "core/models/Judge.h"
#include <QPainter>
#include <QMouseEvent>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_PaintOnScreen, false);
    setAutoFillBackground(true);

    // 创建叫分按钮
    GameButton *passScoreBtn = new GameButton();
    passScoreBtn->SetButtonText(tr("不叫"));
    passScoreBtn->SetID(EM_PASS_SCORE_BUTTON);
    m_buttons[EM_PASS_SCORE_BUTTON] = passScoreBtn;
    connect(passScoreBtn, &GameButton::clicked, this, &GameWidget::onButtonClicked);

    GameButton *oneBtn = new GameButton();
    oneBtn->SetButtonText(tr("1分"));
    oneBtn->SetID(EM_ONE_SCORE_BUTTON);
    m_buttons[EM_ONE_SCORE_BUTTON] = oneBtn;
    connect(oneBtn, &GameButton::clicked, this, &GameWidget::onButtonClicked);

    GameButton *twoBtn = new GameButton();
    twoBtn->SetButtonText(tr("2分"));
    twoBtn->SetID(EM_TWO_SCORE_BUTTON);
    m_buttons[EM_TWO_SCORE_BUTTON] = twoBtn;
    connect(twoBtn, &GameButton::clicked, this, &GameWidget::onButtonClicked);

    GameButton *threeBtn = new GameButton();
    threeBtn->SetButtonText(tr("3分"));
    threeBtn->SetID(EM_THREE_SCORE_BUTTON);
    m_buttons[EM_THREE_SCORE_BUTTON] = threeBtn;
    connect(threeBtn, &GameButton::clicked, this, &GameWidget::onButtonClicked);

    // 创建出牌按钮
    GameButton *passCardBtn = new GameButton();
    passCardBtn->SetButtonText(tr("不要"));
    passCardBtn->SetID(EM_PASS_CARD_BUTTON);
    m_buttons[EM_PASS_CARD_BUTTON] = passCardBtn;
    connect(passCardBtn, &GameButton::clicked, this, &GameWidget::onButtonClicked);

    GameButton *sendBtn = new GameButton();
    sendBtn->SetButtonText(tr("出牌"));
    sendBtn->SetID(EM_SEND_CARD_BUTTON);
    m_buttons[EM_SEND_CARD_BUTTON] = sendBtn;
    connect(sendBtn, &GameButton::clicked, this, &GameWidget::onButtonClicked);

    // 连接Judge信号
    connect(&Judge::Instance(), &Judge::refreshView, this, &GameWidget::onRefreshView);
    connect(&Judge::Instance(), &Judge::showCallLandOwerBtn, this, &GameWidget::onShowCallLandOwerBtn);
    connect(&Judge::Instance(), &Judge::showCardPlayBtn, this, &GameWidget::onShowCardPlayBtn);
}

GameWidget::~GameWidget()
{
    for (auto &pair : m_buttons)
    {
        delete pair.second;
    }
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    DataCenter::Instance().SetBound(rect());
    DataCenter::Instance().DrawTo(&painter);

    // 绘制按钮
    for (auto &pair : m_buttons)
    {
        if (pair.second->IsShow())
        {
            pair.second->DrawTo(&painter);
        }
    }
}

void GameWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint pt = event->pos();

    // 先检查按钮点击
    for (auto &pair : m_buttons)
    {
        GameButton *btn = pair.second;
        if (btn->IsShow() && btn->HitTest(pt))
        {
            return; // 按钮已经处理了
        }
    }

    // 处理扑克牌点击
    handleCardClick(pt);

    update();
}

void GameWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    layoutButtons();
    update();
}

void GameWidget::layoutButtons()
{
    int bottom = height() - 10;
    int y = bottom - g_nButtonHeight;

    // 叫分按钮在底部居中
    int totalWidth = 4 * g_nButtonWidth + 3 * g_nButtonGap;
    int x = (width() - totalWidth) / 2;
    for (int id = EM_PASS_SCORE_BUTTON; id <= EM_THREE_SCORE_BUTTON; id++)
    {
        if (m_buttons.count(id))
        {
            m_buttons[id]->SetPosition(QPoint(x, y));
            x += g_nButtonWidth + g_nButtonGap;
        }
    }

    // 出牌按钮
    totalWidth = 2 * g_nButtonWidth + g_nButtonGap;
    x = (width() - totalWidth) / 2;
    if (m_buttons.count(EM_PASS_CARD_BUTTON))
    {
        m_buttons[EM_PASS_CARD_BUTTON]->SetPosition(QPoint(x, y));
        x += g_nButtonWidth + g_nButtonGap;
    }
    if (m_buttons.count(EM_SEND_CARD_BUTTON))
    {
        m_buttons[EM_SEND_CARD_BUTTON]->SetPosition(QPoint(x, y));
    }
}

void GameWidget::onRefreshView()
{
    update();
}

void GameWidget::onShowCallLandOwerBtn(bool show, int currentScore)
{
    m_buttons[EM_PASS_CARD_BUTTON]->ShowButton(false);
    m_buttons[EM_SEND_CARD_BUTTON]->ShowButton(false);

    for (int id = EM_PASS_SCORE_BUTTON; id <= EM_THREE_SCORE_BUTTON; id++)
    {
        if (id <= currentScore)
        {
            m_buttons[id]->ShowButton(false);
        }
        else
        {
            m_buttons[id]->ShowButton(show);
        }
    }
    layoutButtons();
    update();
}

void GameWidget::onShowCardPlayBtn(bool show, bool hidePassBtn)
{
    for (int id = EM_PASS_SCORE_BUTTON; id <= EM_THREE_SCORE_BUTTON; id++)
    {
        m_buttons[id]->ShowButton(false);
    }

    m_buttons[EM_PASS_CARD_BUTTON]->ShowButton(show && !hidePassBtn);
    m_buttons[EM_SEND_CARD_BUTTON]->ShowButton(show);
    layoutButtons();
    update();
}

void GameWidget::onButtonClicked(int buttonID)
{
    switch(buttonID)
    {
    case EM_PASS_SCORE_BUTTON:
        Judge::Instance().CurPlayerCallScore(0);
        break;
    case EM_ONE_SCORE_BUTTON:
        Judge::Instance().CurPlayerCallScore(1);
        break;
    case EM_TWO_SCORE_BUTTON:
        Judge::Instance().CurPlayerCallScore(2);
        break;
    case EM_THREE_SCORE_BUTTON:
        Judge::Instance().CurPlayerCallScore(3);
        break;
    case EM_PASS_CARD_BUTTON:
        if (Judge::Instance().CheckCurPlayerCard())
        {
            Judge::Instance().CurPlayerPlayCard();
        }
        break;
    case EM_SEND_CARD_BUTTON:
        if (Judge::Instance().CheckCurPlayerCard())
        {
            Judge::Instance().CurPlayerPlayCard();
        }
        else
        {
            update();
        }
        break;
    }
}

void GameWidget::handleCardClick(QPoint pos)
{
    Player *curPlayer = Judge::Instance().GetCurPlayer();
    if (!curPlayer || curPlayer->IsAIPlayer())
    {
        return;
    }

    CardPile &handCard = curPlayer->GetHandCard();
    std::vector<ICard *> &cards = handCard.GetCardPile();

    for (auto it = cards.rbegin(); it != cards.rend(); ++it)
    {
        ICard *card = *it;
        if (card->HitTest(pos))
        {
            curPlayer->GetHandCard().HoldOneCard(card->GetID());
            update();
            return;
        }
    }
}
