#include "mainwindow.h"
#include "sbxperiment.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SBXperiment x;

    ScrollSyncer ss;
    ss.addScrollBar(x.scroll1());
    ss.addScrollBar(x.scroll2());
    ss.addScrollBar(x.scroll3());

    x.show();

    return a.exec();
}
