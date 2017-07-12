#include "tab_data_io.h"
#include "ui_tab_data_io.h"
#include <QFileDialog>
#include <algorithm>

tab_data_IO::tab_data_IO(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_data_IO)
{
    ui->setupUi(this);
    this->db = db;
    this->excell= new QAxObject(this);
    this->excell->setControl("Excel.Application");//连接Excel控件
    this->excell->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
    this->excell->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
    QSqlQuery query(db);
    ui->progressBar->setValue(0);
    query.exec("SELECT name FROM [DBSA].[dbo].sysobjects WHERE (xtype = 'U')");
    while(query.next())
    {
        ui->tableSelect->addItem(query.value(0).toString());
    }
    this->cntEX = new QTimer(this);
    connect(cntEX,SIGNAL(timeout()),this,SLOT(changeExText()));
    this->cntIN = new QTimer(this);
    connect(cntIN,SIGNAL(timeout()),this,SLOT(changeProsBar()));
}

tab_data_IO::~tab_data_IO()
{
    delete ui;
}

//**导入tbCell功能
void tab_data_IO::on_pushButton_clicked()
{
    this->io_actor = new act_data_cell_io(db);
    this->buttonDisable();
    this->cntAll=0;
    this->cntInsert = 0;
    this->excelRows=0;
    this->setRange = false;
    ui->progressBar->setRange(2,5504);
    this->cntIN->start(1500);
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*);;所有Excel文件(*.xls;*.xlsx);;CSV文件(*.csv);"));
    qDebug()<<fileName;
    IOThread *readThread = new IOThread(this,1,fileName);
    connect(readThread,SIGNAL(stopTimer()),this,SLOT(stopTimerIn()));
    readThread->start();
}

void tab_data_IO::on_pushButton_2_clicked()
{
    ui->progressBar->setRange(2,875603);
    this->io_actor = new act_data_mro_io(db);
    this->buttonDisable();
    this->cntAll=0;
    this->cntInsert=0;
    this->excelRows=0;
    this->setRange = false;
    this->cntIN->start(1500);
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*);;所有Excel文件(*.xls;*.xlsx);;CSV文件(*.csv);"));
    qDebug()<<fileName;
    IOThread *readThread = new IOThread(this,2,fileName);
    connect(readThread,SIGNAL(stopTimer()),this,SLOT(stopTimerIn()));
    readThread->start();
}

void tab_data_IO::on_pushButton_3_clicked()
{
    ui->progressBar->setRange(2,93025);
    this->io_actor = new act_data_prb_io(db);
    this->buttonDisable();
    this->cntAll=0;
    this->cntInsert=0;
    this->excelRows=0;
    this->setRange = false;
    this->cntIN->start(1500);
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*);;所有Excel文件(*.xls;*.xlsx);;CSV文件(*.csv);"));
    qDebug()<<fileName;
    IOThread *readThread = new IOThread(this,3,fileName);
    connect(readThread,SIGNAL(stopTimer()),this,SLOT(stopTimerIn()));
    readThread->start();
}

void tab_data_IO::on_pushButton_4_clicked()
{
    ui->progressBar->setRange(2,971);
    this->io_actor = new act_data_kpi_io(db);
    this->buttonDisable();
    this->cntAll=0;
    this->cntInsert=0;
    this->excelRows=0;
    this->setRange = false;
    this->cntIN->start(1500);
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*);;所有Excel文件(*.xls;*.xlsx);;CSV文件(*.csv);"));
    qDebug()<<fileName;
    IOThread *readThread = new IOThread(this,4,fileName);
    connect(readThread,SIGNAL(stopTimer()),this,SLOT(stopTimerIn()));
    readThread->start();
}

QString tab_data_IO::getEndCol(int cols){
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

QVariantList tab_data_IO::readRows(QAxObject *worksheet,int startRow,int &endRow,int Rows,int Colums){
    // 批量载入数据，这里默认读取B13:C最后
    endRow = std::min(startRow+9999,Rows);
    qDebug()<<startRow<<" "<<endRow;
    QString Range = "A"+QString::number(startRow)+":"+getEndCol(Colums)+QString::number(endRow);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range);
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();
    return allEnvDataList;
}

void tab_data_IO::io_perform(int type,QString fileName){
    this->cntAll = 0;
    if(fileName.length()==0)
        return;
    ui->label->setText(QObject::tr("Opening Excel,please wait...") );
    int intRows,intCols;
    QVariantList allEnvDataList;
    //获取对应excel的sheet1
    qDebug()<<"toOpenExcel";
    QAxObject *excel = new QAxObject(this);
    excel->setControl("Excel.Application");//连接Excel控件
    excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
    excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
    QAxObject *workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)",fileName);
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取活动工作簿
    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)",1);//获取第一个工作表

    QAxObject * usedrange = worksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject * columns = usedrange->querySubObject("Columns");
    intRows = rows->property("Count").toInt();
    intCols = columns->property("Count").toInt();
    qDebug() << "xls行数:"<<intRows;
    qDebug() << "xls列数:"<<intCols;
    ui->label->setText("the file has rows: "+QString::number(intRows)+"\tcols : "+QString::number(intCols));
    int startRow = 2,endRow;
    this->excelRows = intRows;

    this->cntInsert = 0;
    tbUnit *DataBuffer[100];

    while(startRow<intRows){
        qDebug()<<"to read";
        allEnvDataList = this->readRows(worksheet,startRow,endRow,intRows,intCols);
        qDebug()<<"read end";
        int cnt = 0;
        int len = allEnvDataList.length();
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
            this->cntAll++;
            if(ok==true){
                //数据检查后没有问题
                this->cntInsert++;
                cnt ++;
            }
            if(cnt==100){
                this->io_actor->inporttb(DataBuffer,cnt);
                cnt = 0;
            }
        }
        qDebug()<<"write end";
        this->io_actor->inporttb(DataBuffer,cnt);
        startRow+=10000;
    }
    qDebug()<<"to close";
    workbooks->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete excel;
    excel=NULL;
    qDebug()<<"closed";
}

void tab_data_IO::changeProsBar(){
    ui->progressBar->setValue(this->cntAll);
    ui->inCntLabel->setText("All:"+QString::number(this->cntAll)+"/ Suc:"+QString::number(this->cntInsert));
}

void  tab_data_IO::buttonDisable(){
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->confirmButton->setEnabled(false);
}

void  tab_data_IO::buttonEnable(){
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->confirmButton->setEnabled(true);
}

void tab_data_IO::on_file_path_clicked()
{
    QFileDialog *file = new QFileDialog(this);
    file->setFileMode(QFileDialog::Directory);
    file->setWindowTitle(tr("Open Image"));
    file->setDirectory(".");
    if(file->exec() == QDialog::Accepted) {
            this->exPortPath = file->selectedFiles()[0];
            ui->targetPath->setText(this->exPortPath+ ui->tableSelect->currentText()+".xlsx");
            QMessageBox::information(NULL, tr("Path"), tr("You selected ") +this->exPortPath);
    } else {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

void tab_data_IO::on_tableSelect_currentIndexChanged(const QString &arg1)
{
    QString filePath = ui->targetPath->text();
    if(filePath.length()==0)
        return;
    ui->targetPath->setText(this->exPortPath+ arg1+".xlsx");
}

void tab_data_IO::runEXport(){
    QAxObject *excel = new QAxObject(this);
    excel->setControl("Excel.Application");//连接Excel控件
    excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
    excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
    qDebug() << "settings";
    QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
    workbooks->dynamicCall("Add");//新建一个工作簿
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
    QAxObject *worksheets = workbook->querySubObject("Sheets");//获取工作表集合
    QAxObject *worksheet = worksheets->querySubObject("Item(int)",1);//获取工作表集合的工作表1，即sheet1

    QVariant var;
    QVariant temp = QVariant(QVariantList());
    QVariantList line;
    QVariantList record;
    QSqlQuery query(db);
    query.exec("SELECT name FROM syscolumns where id=object_id('"+ ui->tableSelect->currentText()+"') order by colid");
    int cols = 0;
    while(query.next())
    {
        line<<query.value(0).toString();
        cols ++;
    }
    qDebug()<<line;
    temp = line;
    record << temp;
    temp = record;
    var = temp;
    QString rangStr = "A"+QString::number(1)+":"+this->io_actor->getEndCol(cols);
    rangStr += QString::number(1);
    QAxObject* range = worksheet->querySubObject("Range(const QString&)", rangStr);
    range->setProperty("Value", var);
    delete range;
    qDebug() << "wrote colums";


    this->EXendrows = 2;
    int startrows = 2;
    query.exec("SELECT * FROM "+ ui->tableSelect->currentText());
    temp.clear();
    record.clear();
    while(query.next())
    {
        line.clear();
        for(int i=0;i<cols;i++)
            line<<query.value(i).toString();
        temp = line;
        record << temp;
        this->EXendrows++;
        if(this->EXendrows-startrows==10000){
            temp = record;
            var = temp;
            rangStr = "A"+QString::number(startrows)+":"+this->io_actor->getEndCol(cols);
            rangStr += QString::number(this->EXendrows-1);
            range = worksheet->querySubObject("Range(const QString&)", rangStr);
            range->setProperty("Value", var);
            delete range;
            qDebug()<<this->EXendrows;
            startrows = this->EXendrows;
            temp.clear();
            record.clear();
        }
    }
    if(startrows!=this->EXendrows){
        temp = record;
        var = temp;
        rangStr = "A"+QString::number(startrows)+":"+this->io_actor->getEndCol(cols);
        rangStr += QString::number(this->EXendrows-1);
        range = worksheet->querySubObject("Range(const QString&)", rangStr);
        range->setProperty("Value", var);
        delete range;
    }
    ui->completedCnt->setText("current export:"+QString::number(this->EXendrows)+"Finish");
    qDebug() << "wrote";
    workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(ui->targetPath->text()));//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
    workbook->dynamicCall("Close()");//关闭工作簿
    excel->dynamicCall("Quit()");//关闭excel
    delete excel;
    excel=NULL;
}

void tab_data_IO::on_confirmButton_clicked()
{
    QString filePath = ui->targetPath->text();
    if(filePath.length()==0){
        QMessageBox::information(NULL, tr("Error"), tr("Path shouldn't be empty! "));
        return;
    }

    this->cntEX->start(1500);
    this->buttonDisable();
    EXThread *newEx = new EXThread(this);
    connect(newEx,SIGNAL(stopTimer()),this,SLOT(stopTimerEx()));
    newEx->start();
}

void tab_data_IO::stopTimerEx(){
    this->cntEX->stop();
    this->buttonEnable();
    QMessageBox::information(NULL, tr("Finish"), tr("Export Finished!"));
    ui->progressBar->setValue(0);
    ui->completedCnt->setText("Export Finish! "+QString::number(this->EXendrows)+" rows");
}
void tab_data_IO::stopTimerIn(){
    this->cntIN->stop();
    this->changeProsBar();
    this->buttonEnable();
    QMessageBox::information(NULL, tr("Finish"), tr("Inport Finished!"));
    ui->progressBar->setValue(0);
}
void tab_data_IO::changeExText(){
    qDebug()<<"in";
    ui->completedCnt->setText("current export: "+QString::number(this->EXendrows)+" rows");
}
