#include "tab_interfere_analysis.h"
#include "ui_tab_interfere_analysis.h"

#include <QSqlQuery>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

tab_interfere_analysis::tab_interfere_analysis(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_interfere_analysis)
{
    ui->setupUi(this);
    this->db = db;

    /*
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

void tab_interfere_analysis::on_C2I_file_clicked() //设定tbC2Inew文件保存路径
{
    QFileDialog *file = new QFileDialog(this);
    file->setWindowTitle(tr("Open Image"));
    file->setDirectory(".");
    //file->setFilter(tr("Image Files(*.jpg *.png)"));
    if(file->exec() == QDialog::Accepted) {
            QString path = file->selectedFiles()[0];
            ui->C2I_path->setText(path);
            QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    } else {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

void tab_interfere_analysis::on_triple_file_clicked() //设定tbC2I3文件保存路径
{
    QFileDialog *file = new QFileDialog(this);
    file->setWindowTitle(tr("Open Image"));
    file->setDirectory(".");
    //file->setFilter(tr("Image Files(*.jpg *.png)"));
    if(file->exec() == QDialog::Accepted) {
            QString path = file->selectedFiles()[0];
            ui->triple_path->setText(path);
            QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    } else {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

void tab_interfere_analysis::on_C2I_clicked() //
{

}

void tab_interfere_analysis::on_triple_clicked()
{

}
