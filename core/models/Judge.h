#pragma once
//////////////////////////////////////////////////////////////////////////
// 裁判，管理牌堆和游戏流程，控制游戏进度
//////////////////////////////////////////////////////////////////////////
#include "Player.h"
#include <QObject>

class Judge : public QObject
{
    Q_OBJECT
private:
    Judge(void);
    Player * m_pCurCardPlayer;
    Player * m_pCurPlayer;
    int m_nCurHighstScore;
    Player * m_pToBeLandOwer;
private:
    void CallLandOwer();
    void CallPlayCard();
public:
    ~Judge(void);
    static Judge & Instance();
    void Restart();
    void MainFlow();
    Player * GetCurPlayer() { return m_pCurPlayer; }
    Player * SwitchToNextPlayer();
    Player * GetCurCardPlayer() { return m_pCurCardPlayer; }
    int GetCurHighstScore() { return m_nCurHighstScore; }

signals:
    void refreshView();
    void showCallLandOwerBtn(bool show, int currentScore);
    void showCardPlayBtn(bool show, bool hidePassBtn);
    void playerAction(QString playerName, QString action);

public:
    void ShowCallLandOwerBtn(bool bShow);
    void ShowCardPlayBtn(bool bShow);

public slots:
    void CurPlayerCallScore(int nScore);
    void CurPlayerPlayCard();
    bool CheckCurPlayerCard();
};
