#include "tab_info_query.h"
#include "ui_tab_info_query.h"
#include <QDebug>

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

    //小区名称选择
    ComSectorID = new QComboBox(this);
    ComSectorID->setGeometry(200,30,87,22);
    ComSectorID->addItem(QWidget::tr("names"));

    //响应各项动作
    connect(mission,SIGNAL(currentIndexChanged(int)),this,SLOT(missionChange(int)));
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

void tab_info_query::QuerytbCell()
{
    qDebug()<<"tbCell";
}

void tab_info_query::QueryeNodeB()
{
    qDebug()<<"eNodeB";
}

void tab_info_query::QueryKPI()
{
    qDebug()<<"KPI";
}

void tab_info_query::QueryPRB()
{
    qDebug()<<"PRB";
}
