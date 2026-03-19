#include <QApplication>
#include <QTranslator>
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

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
