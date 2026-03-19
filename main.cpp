#include <QApplication>
#include <QTranslator>
#include <cstdlib>
#include <ctime>
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // 设置随机种子，保证每次发牌都不一样
    srand(time(nullptr));

    // 加载中文翻译
    QTranslator translator;
    if (translator.load(":/i18n/MFCPoker_zh_CN.qm"))
    {
        app.installTranslator(&translator);
    }

    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
