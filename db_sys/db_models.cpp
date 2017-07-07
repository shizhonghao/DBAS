#include <iostream>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QtSql>
#include <QSqlQuery>
#include <QString>
using namespace std;

class db_models
{
public:
    db_models(){}

    void init()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

        db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                       "SERVER=%1;"
                                       "DATABASE=%2;"
                                       "UID=%3;"
                                       "PWD=%4;").arg("127.0.0.1")
                               .arg("TD_LTE")
                               .arg("sa")
                               .arg("password"));

        if(!db.open())
        {
            qDebug()<<db.lastError().text();
            QMessageBox::critical(0, QObject::tr("Database error"), db.lastError().text());
            return;
        }
        else
        {
            qDebug()<<"database open success!";
        }
        QSqlQuery query(db);
        query.exec("select * from dbo.tbCell");
        while(query.next())
        {
            qDebug()<<query.value(1).toString();
        }
    }
};



