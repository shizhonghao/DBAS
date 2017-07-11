#include "act_data_io.h"
#include<QVariantList>
#include<QDebug>
#include<QSqlError>

act_data_cell_io::act_data_cell_io(QSqlDatabase db)
{
    this->db = db;
    this->excell = new QAxObject("Excel.Application");
    this->excell->setProperty("Visible",false);
}

act_data_mro_io::act_data_mro_io(QSqlDatabase db)
{
    this->db = db;
    this->excell = new QAxObject("Excel.Application");
    this->excell->setProperty("Visible",false);
}

act_data_prb_io::act_data_prb_io(QSqlDatabase db)
{
    this->db = db;
    this->excell = new QAxObject("Excel.Application");
    this->excell->setProperty("Visible",false);
}

act_data_kpi_io::act_data_kpi_io(QSqlDatabase db)
{
    this->db = db;
    this->excell = new QAxObject("Excel.Application");
    this->excell->setProperty("Visible",false);
}

QString act_data_io::getEndCol(int cols){
    QString res;
    if(cols>26){
        char a = 'A'+(cols/26-1);
        char b = 'A'+(cols%26-1);
        res = QString(a) + QString(b);
        //qDebug()<<"end Colum"<<res<<a<<b;
    }
    else{
        char b = 'A'+(cols%26-1);
        res = QString(b);
        //qDebug()<<"end Colum"<<res<<b;
    }
    return res;
}

void act_data_cell_io::inporttb(tbUnit *buffer[100],int len){
    QSqlQuery query(db);
    //query.exec("bulk_insert_on");
    query.prepare("insert into tbCellNew values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? ,? ,? ,? ,?)");
    QVariantList List[19];
    for(int i=0;i<19;i++){
        for(int j=0;j<len;j++){
            List[i]<<buffer[j]->DataList[i];
        }
    }
    for(int i = 0 ;i<19;i++)
        query.addBindValue(List[i]);

    if (!query.execBatch())
        qDebug() << query.lastError();
}

void act_data_mro_io::inporttb(tbUnit *buffer[100],int len){
    QSqlQuery query(db);
    //query.exec("bulk_insert_on");
    query.prepare("insert into tbMRODataNew values (?, ?, ?, ?, ?, ?, ?)");
    QVariantList List[7];
    for(int i=0;i<7;i++){
        for(int j=0;j<len;j++){
            List[i]<<buffer[j]->DataList[i];
        }
    }
    for(int i = 0 ;i<7;i++)
        query.addBindValue(List[i]);

    //qDebug()<<List[0]<<List[1];
    if (!query.execBatch()){

        qDebug() << query.lastError();
        return;
             }
}

void act_data_prb_io::inporttb(tbUnit *buffer[100],int len){
    QSqlQuery query(db);
    //query.exec("bulk_insert_on");
    QString sql = "insert into tbPRB values (";
    QVariantList List[105];
    for(int i=0;i<105;i++){
        if(i==0)
            sql+="?";
        else
            sql+=",?";
        for(int j=0;j<len;j++){
            List[i]<<buffer[j]->DataList[i];
        }
    }
    sql +=")";
    query.prepare(sql);
    for(int i = 0 ;i<105;i++)
        query.addBindValue(List[i]);

    //qDebug()<<List[0]<<List[1];
    if (!query.execBatch()){

        qDebug() << query.lastError();
        return;
             }
}
void act_data_kpi_io::inporttb(tbUnit *buffer[100],int len){
     QSqlQuery query(db);
     //query.exec("bulk_insert_on");
     QString sql = "insert into tbKPI values (";
     QVariantList List[42];
     for(int i=0;i<42;i++){
         if(i==0)
             sql+="?";
         else
             sql+=",?";
         for(int j=0;j<len;j++){
             List[i]<<buffer[j]->DataList[i];
         }
     }
     sql +=")";
     query.prepare(sql);
     for(int i = 0 ;i<42;i++)
         query.addBindValue(List[i]);

     //qDebug()<<List[0]<<List[1];
     if (!query.execBatch()){

         qDebug() << query.lastError();
         return;
              }
 }
