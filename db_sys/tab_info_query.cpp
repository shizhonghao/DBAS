#include "tab_info_query.h"
#include "ui_tab_info_query.h"

tab_info_query::tab_info_query(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_info_query)
{
    ui->setupUi(this);
}

tab_info_query::~tab_info_query()
{
    delete ui;
}
