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

void tab_data_IO::io_perform(int type){
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
    tbUnit *DataBuffer[50];
    int startRow = 2,endRow;
    ui->progressBar->setRange(2,intRows);
    FILE* fp = std::fopen("D:\\tbCell.txt","w");
    int cntAll = 0;
    while(startRow<intRows){
        allEnvDataList = this->io_actor->readRows(worksheet,startRow,endRow,intRows,intCols);
        int cnt = 0;
        for(int i=0; i< allEnvDataList.length(); i++)
        {
            QVariantList allEnvDataList_i =  allEnvDataList[i].toList();
            bool ok = true;
            switch(type){
                case 1:DataBuffer[cnt] = new tbCellUnit(allEnvDataList_i,ok);break;
                case 2:DataBuffer[cnt] = new tbMRODataUnit(allEnvDataList_i,ok);break;
                //case 1:DataBuffer[cnt] = new tbCellUnit(allEnvDataList_i,ok);break;
            }

            if(ok==true){
                //数据检查后没有问题
                std::fputs(DataBuffer[cnt]->toString().toStdString().data(),fp);
                cnt ++;
            }
            //qDebug()<< allEnvDataList_i[0].toString()<< allEnvDataList_i[1].toString();
        }
        startRow+=49;
        ui->progressBar->setValue(endRow);
        cntAll += cnt;
        this->io_actor->inporttb(DataBuffer,cnt);
        //break;
    }
    std::fclose(fp);
    qDebug()<<"to close";
    workbooks->dynamicCall("Close()");
    ui->label->setText(ui->label->text()+"\n导入结束,total: "+QString::number(cntAll)+" rows");
    qDebug()<<"closed";
}

void tab_data_IO::on_pushButton_2_clicked()
{
    this->io_actor = new act_data_mro_io(db);
    this->io_perform(2);
}

void tab_data_IO::on_pushButton_3_clicked()
{

}
