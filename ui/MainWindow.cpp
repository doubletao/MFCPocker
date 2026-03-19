#include "MainWindow.h"
#include "ui\GameWidget.h"
#include "core/models\Judge.h"
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("斗地主"));
    resize(800, 600);

    GameWidget *gameWidget = new GameWidget(this);
    setCentralWidget(gameWidget);

    // 创建菜单
    QMenu *gameMenu = menuBar()->addMenu(tr("游戏"));
    QAction *restartAction = gameMenu->addAction(tr("重新开始"));
    connect(restartAction, &QAction::triggered, this, [this]() {
        Judge::Instance().Restart();
    });
    gameMenu->addSeparator();
    QAction *aboutAction = gameMenu->addAction(tr("关于"));
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("关于"), tr("斗地主\n基于Qt 5.15.2 移植"));
    });

    Judge::Instance().Restart();
}

MainWindow::~MainWindow()
{
}
