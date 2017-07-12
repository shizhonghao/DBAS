#ifndef ACT_DATA_IO_H
#define ACT_DATA_IO_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include<inputbuffer.h>
#include<QThread>

class act_data_io
{
public:
    virtual void  inporttb(tbUnit *[100],int){};
    QString getEndCol(int);
    QSqlDatabase db;
    QAxObject *excell;
};

class act_data_cell_io:public act_data_io
{
public:
    act_data_cell_io(QSqlDatabase);
    void inporttb(tbUnit *[100],int);
};

class act_data_mro_io:public act_data_io
{
public:
    act_data_mro_io(QSqlDatabase);
    void inporttb(tbUnit *[100],int);
};

class act_data_prb_io:public act_data_io
{
public:
    act_data_prb_io(QSqlDatabase);
    void inporttb(tbUnit *[100],int);
};

class act_data_kpi_io:public act_data_io
{
public:
    act_data_kpi_io(QSqlDatabase);
    void inporttb(tbUnit *[100],int);
};

#endif // ACT_DATA_IO_H
