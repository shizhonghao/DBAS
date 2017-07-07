#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>//del

#include "db_sys.h"
#include "ui_db_sys.h"

db_sys::db_sys(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::db_sys)
{
    ui->setupUi(this);
    ui->pwd->setEchoMode(QLineEdit::Password);
}

db_sys::~db_sys()
{
    delete ui;
}

void db_sys::on_tabWidget_tabBarClicked(int index)
{
    switch (index) {
    case 0:
        tab_data->show();
        tab_query->hide();
        tab_analysis->hide();
        break;
    case 1:
        tab_data->hide();
        tab_query->show();
        tab_analysis->hide();
        break;
    case 2:
        tab_data->hide();
        tab_query->hide();
        tab_analysis->show();
        break;
    default:
        break;
    }

}

void db_sys::on_login_btn_clicked()
{
    QString id = ui->id->text();
    QString pwd = ui->pwd->text();
    qDebug() << id << pwd;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                   "SERVER=%1;"
                                   "DATABASE=%2;"
                                   "UID=%3;"
                                   "PWD=%4;").arg("127.0.0.1")
                           .arg("DBSA")
                           .arg(id)
                           .arg(pwd));
    if(!db.open())
    {
        qDebug()<<db.lastError().text();
        QMessageBox::critical(0, QObject::tr("Database error"), db.lastError().text());
        return;
    }
    else
    {
        ui->login->hide();
        tab_data = new tab_data_IO(this->db,this);
        tab_data->setGeometry(0,40,800,600);
        tab_query = new tab_info_query(this->db,this);
        tab_query->setGeometry(0,40,800,600);
        tab_analysis = new tab_interfere_analysis(this->db,this);
        tab_analysis->setGeometry(0,40,800,600);

        tab_query->hide();
        tab_analysis->hide();
        tab_data->show();
        ui->tabWidget->setCurrentIndex(0);
        qDebug()<<"database open success!";
    }

}
