#include "db_sys.h"
#include <QApplication>

#include "db_models.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    db_sys w;
    w.show();

    return a.exec();
}
