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
    this->excell = new QAxObject("Excel.Application");
    this->excell->setProperty("Visible",false);
    ui->progressBar->setValue(0);
}

tab_data_IO::~tab_data_IO()
{
    delete ui;
}

//**导入tbCell功能
void tab_data_IO::on_pushButton_clicked()
{
    this->io_actor = new act_data_cell_io(db);
    this->io_perform(1);
}

void callExcel(QString fileName,int type,act_data_io* io_actor,QAxObject *worksheet,int startRow,int endRow,int intRows,int intCols){
    READThread *readEXCEL = new READThread(fileName,type,io_actor,worksheet,startRow,endRow,intRows,intCols);
    //connect(readEXCEL,SIGNAL(uiChanged(int,int)),this,SLOT(changeProsBar(int,int)));
    readEXCEL->start();
    readEXCEL->wait();
}

void tab_data_IO::io_perform(int type){
    this->cntAll = 0;
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*);;所有Excel文件(*.xls;*.xlsx);;CSV文件(*.csv);"));
    qDebug()<<fileName;
    if(fileName.length()==0)
        return;
    ui->label->setText("opening file:\n"+fileName+"\nplease wait...");
    ui->progressBar->setValue(0);
    int intRows,intCols;
    QVariantList allEnvDataList;
    QAxObject *workbook;
    QAxObject *workbooks;
    //获取对应excel的sheet1
    QAxObject *worksheet = this->io_actor->inportExcel(fileName,workbooks,workbook,intRows,intCols);
    qDebug() << "xls行数:"<<intRows;
    qDebug() << "xls列数:"<<intCols;
    ui->label->setText(ui->label->text()+"\nrow Num: "+QString::number(intRows)+"\tcol Num: "+QString::number(intCols)+"\nBegining");
    int startRow = 2,endRow;
    ui->progressBar->setRange(2,intRows);
    //FILE* fp = std::fopen("D:\\tbCell.txt","w");
    int cntAll = 0;
    int lenDiv10=intRows/10;
    int lenSeg=0;
    tbUnit *DataBuffer[100];

    while(startRow<intRows){
        //callExcel(fileName,type,this->io_actor,worksheet,startRow,endRow,intRows,intCols);
        //READThread *readEXCEL = new READThread(fileName,type,io_actor,worksheet,startRow,endRow,intRows,intCols);
        //connect(readEXCEL,SIGNAL(uiChanged(int,int)),this,SLOT(changeProsBar(int,int)));
        //readEXCEL->start();
        //readEXCEL->wait();
        allEnvDataList = this->io_actor->readRows(worksheet,startRow,endRow,intRows,intCols);
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

            if(ok==true){
                //数据检查后没有问题
                //std::fputs(DataBuffer[cnt]->toString().toStdString().data(),fp);
                cnt ++;
            }
            if(cnt==100){
                cntAll += cnt;
                this->io_actor->inporttb(DataBuffer,cnt);
                //IOThread *newThread = new IOThread(this->io_actor,DataBuffer,cnt);
                //newThread->start();
                //newThread->wait();
                cnt = 0;
            }
        }
        qDebug()<<"write end";
        cntAll += cnt;
        ui->progressBar->setValue(endRow);
        this->io_actor->inporttb(DataBuffer,cnt);
        //IOThread *newThread = new IOThread(this->io_actor,DataBuffer,cnt);
        //newThread->start();
        //newThread->wait();
        startRow+=900;
        //break;
    }
    //std::fclose(fp);
    ui->label->setText(ui->label->text()+"\ntotal: "+QString::number(cntAll)+" rows");
    qDebug()<<"to close";
    workbooks->dynamicCall("Close()");
    this->io_actor->excell->dynamicCall("Quit()");
    delete this->io_actor->excell;
    this->io_actor->excell=NULL;
    qDebug()<<"closed";
}

void tab_data_IO::changeProsBar(int value,int cnt){
    ui->progressBar->setValue(value);
    this->cntAll+=cnt;
    ui->label->setText(ui->label->text()+"\ntotal: "+QString::number(this->cntAll)+" rows");
}


void tab_data_IO::on_pushButton_2_clicked()
{
    this->io_actor = new act_data_mro_io(db);
    this->io_perform(2);
}

void tab_data_IO::on_pushButton_3_clicked()
{
    this->io_actor = new act_data_prb_io(db);
    this->io_perform(3);
}

void tab_data_IO::on_pushButton_4_clicked()
{
    this->io_actor = new act_data_kpi_io(db);
    this->io_perform(4);
}
