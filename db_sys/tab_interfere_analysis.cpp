#include "tab_interfere_analysis.h"
#include "ui_tab_interfere_analysis.h"

tab_interfere_analysis::tab_interfere_analysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_interfere_analysis)
{
    ui->setupUi(this);
}

tab_interfere_analysis::~tab_interfere_analysis()
{
    delete ui;
}
