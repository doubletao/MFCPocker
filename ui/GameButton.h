#pragma once
//////////////////////////////////////////////////////////////////////////
// GDI自定义绘制button
//////////////////////////////////////////////////////////////////////////
#include "core/interfaces/IDrawable.h"
#include "core/interfaces/IClickable.h"
#include <QObject>
#include <QString>

enum EMButtonID
{
    EM_PASS_SCORE_BUTTON,     // 不叫
    EM_ONE_SCORE_BUTTON,      // 1分
    EM_TWO_SCORE_BUTTON,      // 2分
    EM_THREE_SCORE_BUTTON,    // 3分

    EM_PASS_CARD_BUTTON,      // 不要
    EM_SEND_CARD_BUTTON,      // 出牌
};

const int g_nButtonWidth = 70;
const int g_nButtonHeight = 25;
const int g_nButtonGap = 5;

class GameButton : public QObject, public IDrawable, public IClickable
{
    Q_OBJECT
private:
    QString m_strBtnTxt;
    bool m_bShow;
public:
    GameButton(void);
    ~GameButton(void);
    virtual bool HitTest(QPoint pt);
    virtual void DrawTo(QPainter *painter);
    void SetButtonText(QString strBtnTxt) { m_strBtnTxt = strBtnTxt; }
    void ShowButton(bool bShow) { m_bShow = bShow; }
    bool IsShow() { return m_bShow; }

signals:
    void clicked(int buttonID);
};
