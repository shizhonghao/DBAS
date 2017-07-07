#include "tab_info_query.h"
#include "ui_tab_info_query.h"

tab_info_query::tab_info_query(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_info_query)
{
    ui->setupUi(this);
    this->db = db;
}

tab_info_query::~tab_info_query()
{
    delete ui;
}
