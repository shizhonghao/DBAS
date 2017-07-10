#ifndef TAB_DATA_IO_H
#define TAB_DATA_IO_H
#include "act_data_io.h"
#include <QWidget>
#include <QSqlDatabase>
#include <QtSql>
#include <QString>
#include <QSqlQuery>
#include <ActiveQt/QAxObject>
#include<QMessageBox>
namespace Ui {
class tab_data_IO;
}

class tab_data_IO : public QWidget
{
    Q_OBJECT

public:
    explicit tab_data_IO(QSqlDatabase db,QWidget *parent = 0);
    QAxObject * inportExcel(QString,QAxObject* &,QAxObject* &,int&,int&);
    QVariantList readRows(QAxObject*,int,int&,int,int);
    QString getEndCol(int);
    void io_perform(int,QString);
    void  buttonDisable();
    void  buttonEnable();
    void runEXport();
    ~tab_data_IO();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void changeProsBar();

    void on_file_path_clicked();

    void on_tableSelect_currentIndexChanged(const QString &arg1);

    void on_confirmButton_clicked();

    void changeExText();

    void stopTimerEx();

    void stopTimerIn();

private:
    act_data_io *io_actor;
    Ui::tab_data_IO *ui;
    QSqlDatabase db;
    QAxObject *excell;
    int cntAll;
    int excelRows;
    bool setRange;

    QString exPortPath;
    int EXendrows;
    QTimer *cntEX;
    QTimer *cntIN;
};

class EXThread:public QThread{
    Q_OBJECT
signals:
    void stopTimer();
public:
    tab_data_IO *mytable;
    EXThread(tab_data_IO *table){
        this->mytable = table;
    }

    void run(){
        this->mytable->runEXport();
        emit this->stopTimer();
    }
};

class IOThread:public QThread{
    Q_OBJECT
signals:
    void stopTimer();
public:
    tab_data_IO *mytable;
    int type;
    QString name;
    IOThread(tab_data_IO *table,int type,QString fileName){
        this->mytable = table;
        this->type = type;
        this->name = fileName;
    }

    void run(){
        this->mytable->io_perform(this->type,this->name);
        emit this->stopTimer();
    }
};
#endif // TAB_DATA_IO_H

