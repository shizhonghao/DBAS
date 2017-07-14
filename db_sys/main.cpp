#include "db_sys.h"
#include <QApplication>
#include <QtGui>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
    QApplication a(argc, argv);
    db_sys w;
    w.show();

    return a.exec();
}
