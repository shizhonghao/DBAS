#include "tab_info_query.h"
#include "ui_tab_info_query.h"
#include <QDebug>
#include <QSqlQuery>

tab_info_query::tab_info_query(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_info_query)
{
    ui->setupUi(this);
    this->db = db;
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

    //显示搜索结果的表格
    displayTable = new QTableWidget(this);
    displayTable->setGeometry(50,80,700,400);

    //初始页面信息
    iniInfo();

    //响应各项动作
    connect(mission,SIGNAL(currentIndexChanged(int)),this,SLOT(missionChange(int)));
    connect(queryButton,SIGNAL(clicked(bool)),this,SLOT(ButtonAction()));
    connect(ComSectorID,SIGNAL(currentIndexChanged(QString)),this,SLOT(comSecNameChange(QString)));
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
}

void tab_info_query::QueryPRB()
{
    qDebug()<<"PRB";
    ComSectorID->hide();
    displayTable->hide();
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
