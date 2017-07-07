#include "tab_interfere_analysis.h"
#include "ui_tab_interfere_analysis.h"

#include <QSqlQuery>
#include <QDebug>

tab_interfere_analysis::tab_interfere_analysis(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_interfere_analysis)
{
    ui->setupUi(this);
    this->db = db;/*
    QSqlQuery query(db);
    query.exec("select * from dbo.tbCell");
    while(query.next())
    {
        qDebug()<<query.value(1).toString();
    }
    */
}

tab_interfere_analysis::~tab_interfere_analysis()
{
    delete ui;
}
