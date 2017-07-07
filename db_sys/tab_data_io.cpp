#include "tab_data_io.h"
#include "ui_tab_data_io.h"
#include <ActiveQt/QAxObject>
#include <QFileDialog>

tab_data_IO::tab_data_IO(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_data_IO)
{
    ui->setupUi(this);
    this->db = db;
}

tab_data_IO::~tab_data_IO()
{
    delete ui;
}

void tab_data_IO::inportExcel(QString filename){
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible",false);
    QAxObject *workbooks = excel.querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)",filename);
    QAxObject *workbook = excel.querySubObject("ActiveWorkBook");//获取活动工作簿
    QAxObject *worksheets = workbook->querySubObject("WorkSheets");//获取所有的工作表
    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)",1);//获取第一个工作表
    //QAxObject *range = worksheet->querySubObject("Cells(int,int)",1,1); //获取cell的值
    //QString strVal = range->dynamicCall("Value2()").toString();
    //ui->label->setText(strVal);

    QAxObject * usedrange = worksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject * columns = usedrange->querySubObject("Columns");
    int intRows = rows->property("Count").toInt();
    int intCols = columns->property("Count").toInt();
    qDebug() << "xls行数:"<<intRows;
    qDebug() << "xls列数:"<<intCols;

    // 批量载入数据，这里默认读取B13:C最后
    QString Range = "A1:C" +QString::number(intRows);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range);
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();

    for(int i=0; i<= intRows-1; i++)
    {
        QVariantList allEnvDataList_i =  allEnvDataList[i].toList() ;
        qDebug()<< allEnvDataList_i[0].toString()<< allEnvDataList_i[1].toString();
        //map.insert(allEnvDataList_i[0].toString(),allEnvDataList_i[1].toFloat());
    }
    workbooks->dynamicCall("Close()");
}

//**导入tbCell功能
void tab_data_IO::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*);;所有Excel文件(*.xls,*.xlsx)"));
    qDebug()<<fileName;
    this->inportExcel(fileName);
}

void tab_data_IO::on_pushButton_2_clicked()
{

}

void tab_data_IO::on_pushButton_3_clicked()
{

}
