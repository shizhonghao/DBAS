#include "tab_info_query.h"
#include "ui_tab_info_query.h"
#include <QDebug>
#include <QSqlQuery>
#include <QPaintEvent>
#include <QPainter>
#include <ActiveQt/QAxObject>
#include <QDir>

tab_info_query::tab_info_query(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_info_query)
{
    ui->setupUi(this);
    this->db = db;
    Sday = "07/17/2016";
    Eday = "07/17/2016";

    //奇妙的小弹窗
    message = new QMessageBox(this);
    message->setWindowTitle("wait");
    message->hide();

    //添加用于选择查询类型的下拉菜单
    mission = new QComboBox(this);
    mission->setGeometry(100,30,87,22);
    mission->addItem(QWidget::tr("小区配置信息查询"));
    mission->addItem(QWidget::tr("基站eNodeB信息查询"));
    mission->addItem(QWidget::tr("KPI指标信息查询"));
    mission->addItem(QWidget::tr("PRB信息统计&查询"));

    //小区名称选择的下拉菜单
    ComSectorID = new QComboBox(this);
    ComSectorID->setGeometry(200,30,87,22);

    //小区编号选择的输入框
    LineSectorID = new QLineEdit(this);
    LineSectorID->setGeometry(300,30,87,22);

    //小区名称选择的输入框
    LineSectorName = new QLineEdit(this);
    LineSectorName->setGeometry(400,30,87,22);

    //利用用户输入信息开始查找的按钮
    queryButton = new QPushButton(this);
    queryButton->setGeometry(600,30,87,22);
    queryButton->setText(QWidget::tr("搜索"));

    //日期选择按钮
    chooseSDate = new QPushButton(this);
    chooseSDate->setGeometry(700,5,95,22);
    chooseSDate->setText(QWidget::tr("选择开始日期"));
    chooseSDate->hide();
    chooseEDate = new QPushButton(this);
    chooseEDate->setGeometry(700,30,95,22);
    chooseEDate->setText(QWidget::tr("选择结束日期"));
    chooseEDate->hide();

    //显示搜索结果的表格
    displayTable = new QTableWidget(this);
    displayTable->setGeometry(50,80,700,400);

    //绘制折线图和柱状图的画布/**/
    graphic = new graph(db,this);
    graphic->setGeometry(50,80,700,400);
    graphic->hide();

    //日期控件
    calendarS = new QCalendarWidget(this);
    calendarS->setGeometry(500,30,296,236);
    calendarS->hide();
    calendarE = new QCalendarWidget(this);
    calendarE->setGeometry(500,30,296,236);
    calendarE->hide();

    //初始页面信息
    iniInfo();

    //响应各项动作
    connect(mission,SIGNAL(currentIndexChanged(int)),this,SLOT(missionChange(int)));
    connect(queryButton,SIGNAL(clicked(bool)),this,SLOT(ButtonAction()));
    connect(ComSectorID,SIGNAL(currentIndexChanged(QString)),this,SLOT(comSecNameChange(QString)));
    connect(chooseSDate,SIGNAL(clicked(bool)),this,SLOT(ChoSDate()));
    connect(chooseEDate,SIGNAL(clicked(bool)),this,SLOT(ChoEDate()));
    connect(calendarS,SIGNAL(clicked(QDate)),this,SLOT(SDateChange(QDate)));
    connect(calendarE,SIGNAL(clicked(QDate)),this,SLOT(EDateChange(QDate)));
}

tab_info_query::~tab_info_query()
{
    delete ui;
}

void tab_info_query::missionChange(int n)
{
    switch(n)
    {
    case 0:
        QuerytbCell();
        break;
    case 1:
        QueryeNodeB();
        break;
    case 2:
        QueryKPI();
        break;
    case 3:
        QueryPRB();
        break;
    }
}

void tab_info_query::iniInfo()
{
    qDebug()<<"iniInfo";
    QSqlQuery query(db);
    query.exec("select distinct sector_name from dbo.tbCell");
    while(query.next())
    {
        ComSectorID->addItem(query.value(0).toString());
    }
    query.exec(QString("select count(*) from dbo.tbCell where sector_name = '%1'").arg(ComSectorID->currentText()));
    query.next();
    int row = query.value(0).toInt();
    QStringList header;
    header<<"CITY"<<"SECTOR_ID"<<"SECTOR_NAME"<<"ENODEBID"<<"ENODEB_NAME"<<"EARFCN"<<"PCI"<<"PSS"<<"SSS"<<"TAC"<<"VENDOR"<<"LONGITUDE"<<"LATITUDE"<<"STYLE"<<"AZIMUTH"<<"HEIGHT"<<"ELECTTILT"<<"MECHTILT"<<"TOTLETILT";
    displayTable->setColumnCount(19);
    displayTable->setHorizontalHeaderLabels(header);
    displayTable->setRowCount(row);
    query.exec(QString("select * from dbo.tbCell where sector_name = '%1'").arg(ComSectorID->currentText()));
    int i,j;
    query.next();
    for(i = 0;i < row;i ++)
    {
        for(j = 0;j < 19;j ++)
        {
            displayTable->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
        }
        query.next();
    }
}

void tab_info_query::QuerytbCell()
{
    qDebug()<<"tbCell";
    ComSectorID->show();
    displayTable->show();
    LineSectorID->show();
    graphic->hide();
    chooseSDate->hide();
    chooseEDate->hide();
    ComSectorID->clear();
    QSqlQuery query(db);
    query.exec("select distinct sector_name from dbo.tbcell");
    while(query.next())
    {
        ComSectorID->addItem(query.value(0).toString());
    }
}

void tab_info_query::QueryeNodeB()
{
    qDebug()<<"eNodeB";
    ComSectorID->show();
    displayTable->show();
    LineSectorID->show();
    chooseSDate->hide();
    chooseEDate->hide();
    graphic->hide();
    ComSectorID->clear();
    QSqlQuery query(db);
    query.exec("select distinct enodebid from dbo.tbcell");
    while(query.next())
    {
        ComSectorID->addItem(query.value(0).toString());
    }
}

void tab_info_query::QueryKPI()
{
    qDebug()<<"KPI";
    ComSectorID->hide();
    displayTable->hide();
    LineSectorID->hide();
    chooseSDate->hide();
    chooseEDate->hide();
    graphic->show();
    graphic->KPIGraph("");
}

void tab_info_query::QueryPRB()
{

    message->show();
    qDebug()<<"PRB";
    QString sql;
    chooseSDate->show();
    chooseEDate->show();
    ComSectorID->hide();
    displayTable->hide();
    LineSectorID->hide();
    graphic->show();
    graphic->PRBGraph("",Sday,Eday);

    QAxObject *excel = new QAxObject(this);
    excel->setControl("Excel.Application");//连接Excel控件
    excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
    excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
    QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
    workbooks->dynamicCall("Add");//新建一个工作簿
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
    QAxObject *worksheets = workbook->querySubObject("Sheets");//获取工作表集合
    QAxObject *worksheet = worksheets->querySubObject("Item(int)",1);//获取工作表集合的工作表1，即sheet1
    QVariant var;
    QVariant temp = QVariant(QVariantList());
    QVariantList line;
    QVariantList record;
    int lineCount = 1;
    line << "起始时间" << "网元名称" << "小区名" << "PRB60";
    temp = line;
    record << temp;
    QSqlQuery query(db);
    query.exec("delete from tbPRBnew");
    QSqlQuery queryI(db);
    int date,time;
    for(date = 17;date < 20;date++)
    {
        for(time = 0;time < 24;time++)
        {
            sql = QString("select 网元名称,小区名,sum(PRB60)/4 as PRB60 from tbPRB where 起始时间 between '07/%1/2016 %2:00:00' and '07/%1/2016 %2:59:00'group by 网元名称,小区名").arg(date).arg(time);
            //qDebug()<<sql;
            query.exec(sql);
            while(query.next())
            {
                line.clear();
                line << QString("07/%1/2016 %2:00:00").arg(date).arg(time);
                line << query.value(0).toString();
                line << query.value(1).toString();
                line << query.value(2).toString();
                temp = line;
                record << temp;
                sql = QString("insert into tbPRBnew(起始时间,网元名称,小区名,PRB60) values('07/%1/2016 %2:00:00','%3','%4',%5)").arg(date).arg(time).arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
                queryI.exec(sql);
                //qDebug()<<sql;
                lineCount ++;
            }
        }
    }
    temp = record;
    var = temp;

    QString rangStr = "A1:D";
    rangStr += QString::number(lineCount+1);
    QAxObject* range = worksheet->querySubObject("Range(const QString&)", rangStr);
    range->setProperty("Value", var);
    delete range;
    qDebug() << "wrote";
    workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(".\\tbPRBnew.xlsx"));//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
    workbook->dynamicCall("Close()");//关闭工作簿
    excel->dynamicCall("Quit()");//关闭excel
    delete excel;
    message->hide();
}

void tab_info_query::ButtonAction()
{
    QString id,name;
    id = LineSectorID->text();
    name = LineSectorName->text();
    qDebug()<<"ButtonAction"<<id<<name;
    QSqlQuery query(db);
    int row = 0;
    int i,j;
    if(mission->currentIndex() == 0 || mission->currentIndex() == 1)
    {
        displayTable->clearContents();
        if(mission->currentIndex() == 0)
        {
            if(id != NULL && name != NULL)
            {
                query.exec(QString("select count(*) from tbCell where sector_id = '%1' and sector_name = '%2'").arg(id).arg(name));
                query.next();
                row = query.value(0).toInt();
                query.exec(QString("select * from tbCell where sector_id = '%1' and sector_name = '%2'").arg(id).arg(name));
            }
            else if(id == NULL)
            {
                query.exec(QString("select count(*) from tbCell where sector_name = '%1'").arg(name));
                query.next();
                row = query.value(0).toInt();
                query.exec(QString("select * from tbCell where sector_name = '%1'").arg(name));
            }
            else
            {
                query.exec(QString("select count(*) from tbCell where sector_id = '%1'").arg(id));
                query.next();
                row = query.value(0).toInt();
                query.exec(QString("select * from tbCell where sector_id = '%1'").arg(id));
            }
        }
        else
        {
            if(id != NULL && name != NULL)
            {
                query.exec(QString("select count(*) from tbCell where enodebid = %1 and enodeb_name = '%2'").arg(id).arg(name));
                query.next();
                row = query.value(0).toInt();
                query.exec(QString("select * from tbCell where enodebid = %1 and enodeb_name = '%2'").arg(id).arg(name));
            }
            else if(id == NULL)
            {
                query.exec(QString("select count(*) from tbCell where enodeb_name = '%1'").arg(name));
                query.next();
                row = query.value(0).toInt();
                query.exec(QString("select * from tbCell where enodeb_name = '%1'").arg(name));
            }
            else
            {
                query.exec(QString("select count(*) from tbCell where enodebid = %1").arg(id));
                query.next();
                row = query.value(0).toInt();
                query.exec(QString("select * from tbCell where enodebid = %1").arg(id));
            }
        }
        displayTable->setRowCount(row);
        i = 0;
        while(query.next())
        {
            for(j = 0;j < 19;j ++)
            {
                displayTable->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
            }
            i++;
        }
    }
    else
    {
        if(mission->currentIndex() == 2)
        {
            graphic->KPIGraph(name);
        }
        else
        {
            graphic->PRBGraph(name,Sday,Eday);
        }
    }
}

void tab_info_query::comSecNameChange(QString name)
{
    qDebug()<<"comSecNameChange"<<name;
    QSqlQuery query(db);
    int row = 0;
    displayTable->clearContents();
    if(mission->currentIndex() == 0)
    {
        query.exec(QString("select count(*) from tbCell where sector_name = '%1'").arg(name));
        query.next();
        row = query.value(0).toInt();
        query.exec(QString("select * from tbCell where sector_name = '%1'").arg(name));
    }
    else
    {
        query.exec(QString("select count(*) from tbCell where enodebid = '%1'").arg(name));
        query.next();
        row = query.value(0).toInt();
        query.exec(QString("select * from tbCell where enodebid = '%1'").arg(name));
    }
    query.next();
    displayTable->setRowCount(row);
    int i,j;
    for(i = 0;i < row;i ++)
    {
        for(j = 0;j < 19;j ++)
        {
            displayTable->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
        }
        query.next();
    }
}

void tab_info_query::ChoSDate()
{
    qDebug()<<"ChoDate";
    calendarS->show();
}

void tab_info_query::ChoEDate()
{
    qDebug()<<"ChoDate";
    calendarE->show();
}

void tab_info_query::SDateChange(QDate Ndate)
{
    qDebug()<<"SDateChange"<<Ndate;
    Sday = Ndate.toString("MM/dd/yyyy");
    qDebug()<<Sday;
    calendarS->hide();
    ButtonAction();
}

void tab_info_query::EDateChange(QDate Ndate)
{
    qDebug()<<"EDateChange"<<Ndate;
    Eday = Ndate.toString("MM/dd/yyyy");
    qDebug()<<Eday;
    calendarE->hide();
    ButtonAction();
}
