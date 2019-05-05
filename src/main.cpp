#include "mywidget.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTextCodec *codec=QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    myWidget w;

    w.showMaximized();

    return a.exec();
}
