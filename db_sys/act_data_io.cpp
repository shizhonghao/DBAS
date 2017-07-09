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

QAxObject * act_data_io::inportExcel(QString filename,QAxObject* &workbooks,QAxObject* &workbook,int &intRows,int &intCols){
    workbooks = excell->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)",filename);
    workbook = excell->querySubObject("ActiveWorkBook");//获取活动工作簿
    QAxObject *worksheets = workbook->querySubObject("WorkSheets");//获取所有工作表
    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)",1);//获取第一个工作表
    //QAxObject *range = worksheet->querySubObject("Cells(int,int)",1,1); //获取cell的值
    //QString strVal = range->dynamicCall("Value2()").toString();
    //ui->label->setText(strVal);

    QAxObject * usedrange = worksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject * columns = usedrange->querySubObject("Columns");
    intRows = rows->property("Count").toInt();
    intCols = columns->property("Count").toInt();

    return worksheet;
}

QString act_data_io::getEndCol(int cols){
    QString res;
    if(cols>26){
        char a = 'A'+(cols/26-1);
        char b = 'A'+(cols%26-1);
        res = QString(a) + QString(b);
        qDebug()<<"end Colum"<<res<<a<<b;
    }
    else{
        char b = 'A'+(cols%26-1);
        res = QString(b);
        qDebug()<<"end Colum"<<res<<b;
    }
    return res;
}

QVariantList act_data_io::readRows(QAxObject *worksheet,int startRow,int &endRow,int Rows,int Colums){
    // 批量载入数据，这里默认读取B13:C最后
    endRow = std::min(startRow+49,Rows);
    qDebug()<<startRow<<" "<<endRow;
    QString Range = "A"+QString::number(startRow)+":"+getEndCol(Colums)+QString::number(endRow);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range);
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();
    return allEnvDataList;
}

void act_data_cell_io::inporttb(tbUnit *buffer[50],int len){
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
    else
        qDebug()<<"leave insert";
}

void act_data_mro_io::inporttb(tbUnit *buffer[50],int len){
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
    else
        qDebug()<<"leave insert";
}
