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


QAxObject * act_data_io::inportExcel(QString filename,QAxObject* &workbooks,QAxObject* &workbook,int &intRows,int &intCols){
    workbooks = excell->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)",filename);
    workbook = excell->querySubObject("ActiveWorkBook");//获取活动工作簿
    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)",1);//获取第一个工作表

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
    endRow = std::min(startRow+899,Rows);
    qDebug()<<startRow<<" "<<endRow;
    QString Range = "A"+QString::number(startRow)+":"+getEndCol(Colums)+QString::number(endRow);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range);
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();
    return allEnvDataList;
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

void READThread::run(){
    QAxObject* excell = new QAxObject("Excel.Application");
    excell->setProperty("Visible",false);
    QAxObject *workbooks = excell->querySubObject("WorkBooks");
    QString file = fileName;
    workbooks->dynamicCall("Open (const QString&)",file);
    QAxObject *workbook = excell->querySubObject("ActiveWorkBook");//获取活动工作簿
    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)",1);//获取第一个工作表

    allEnvDataList=this->myIO->readRows(worksheet,startRow,endRow,intRows,intCols);
    int cnt = 0;
    int len = allEnvDataList.length();
    tbUnit *DataBuffer[100];
    int cntAll = 0;
    qDebug()<<"start insert";
    for(int i=0; i< len; i++)
    {
        QVariantList allEnvDataList_i =  allEnvDataList[i].toList();
        bool ok = true;
        switch(type){
            case 1:DataBuffer[cnt] = new tbCellUnit(allEnvDataList_i,ok);break;
            case 2:DataBuffer[cnt] = new tbMRODataUnit(allEnvDataList_i,ok);break;
            case 3:DataBuffer[cnt] = new tbPRBUnit(allEnvDataList_i,ok);break;
            case 4:DataBuffer[cnt] = new tbKPIUnit(allEnvDataList_i,ok);break;
        }

        if(ok==true){
            //数据检查后没有问题
            //std::fputs(DataBuffer[cnt]->toString().toStdString().data(),fp);
            cnt ++;
        }
        if(cnt==100){
            cntAll += cnt;
            this->myIO->inporttb(DataBuffer,cnt);
            /*
            IOThread *newThread = new IOThread(this->myIO,DataBuffer,cnt);
            newThread->start();
            newThread->wait();*/
            cnt = 0;
        }
    }
    this->myIO->inporttb(DataBuffer,cnt);
    emit this->uiChanged(endRow,cntAll);
    qDebug()<<"to close in thread";
    workbooks->dynamicCall("Close()");
    excell->dynamicCall("Quit()");
    delete excell;
    excell=NULL;
    qDebug()<<"closed in thread";
}
