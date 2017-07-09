#ifndef ACT_DATA_IO_H
#define ACT_DATA_IO_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include<inputbuffer.h>
class act_data_io
{
public:
    virtual void  inporttb(tbUnit *[50],int){};
    QAxObject * inportExcel(QString,QAxObject* &,QAxObject* &,int&,int&);
    QVariantList readRows(QAxObject*,int,int&,int,int);
    QString getEndCol(int);

    QSqlDatabase db;
    QAxObject *excell;
};

class act_data_cell_io:public act_data_io
{
public:
    act_data_cell_io(QSqlDatabase);
    void inporttb(tbUnit *[50],int);
};

class act_data_mro_io:public act_data_io
{
public:
    act_data_mro_io(QSqlDatabase);
    void inporttb(tbUnit *[50],int);
};

#endif // ACT_DATA_IO_H
