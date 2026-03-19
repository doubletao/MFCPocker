#pragma once
#include <QWidget>
#include <QString>
#include "core/models/DataCenter.h"
#include "ui/GameButton.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onRefreshView();
    void onShowCallLandOwerBtn(bool show, int currentScore);
    void onShowCardPlayBtn(bool show, bool hidePassBtn);
    void onButtonClicked(int buttonID);

private:
    void layoutButtons();
    void handleCardClick(QPoint pos);

private:
    std::map<int, GameButton*> m_buttons;
};
