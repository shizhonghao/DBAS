#ifndef TAB_DATA_IO_H
#define TAB_DATA_IO_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtSql>
#include <QString>
#include <QSqlQuery>
namespace Ui {
class tab_data_IO;
}

class tab_data_IO : public QWidget
{
    Q_OBJECT

public:
    explicit tab_data_IO(QSqlDatabase db,QWidget *parent = 0);
    void inportExcel(QString);
    ~tab_data_IO();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::tab_data_IO *ui;
    QSqlDatabase db;
};

#endif // TAB_DATA_IO_H

