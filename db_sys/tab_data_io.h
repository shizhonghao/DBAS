#ifndef TAB_DATA_IO_H
#define TAB_DATA_IO_H
#include "act_data_io.h"
#include <QWidget>
#include <QSqlDatabase>
#include <QtSql>
#include <QString>
#include <QSqlQuery>
#include <ActiveQt/QAxObject>
namespace Ui {
class tab_data_IO;
}

class tab_data_IO : public QWidget
{
    Q_OBJECT

public:
    explicit tab_data_IO(QSqlDatabase db,QWidget *parent = 0);
    QAxObject * inportExcel(QString,QAxObject* &,QAxObject* &,int&,int&);
    QVariantList readRows(QAxObject*,int,int,int,int);
    void io_perform(int);
    ~tab_data_IO();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void changeProsBar(int,int);

private:
    act_data_io *io_actor;
    Ui::tab_data_IO *ui;
    QSqlDatabase db;
    QAxObject *excell;
    int cntAll;
};

#endif // TAB_DATA_IO_H

