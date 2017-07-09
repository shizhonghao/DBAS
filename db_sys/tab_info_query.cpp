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

    //初始页面信息
    iniInfo();

    //响应各项动作
    connect(mission,SIGNAL(currentIndexChanged(int)),this,SLOT(missionChange(int)));
    connect(queryButton,SIGNAL(clicked(bool)),this,SLOT(ButtonAction()));
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

void tab_info_query::ButtonAction()
{
    qDebug()<<"ButtonAction";
    QString id,name;
    id = LineSectorID->text();
    name = LineSectorName->text();
    qDebug()<<id<<name;
}
