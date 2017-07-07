#ifndef DB_SYS_H
#define DB_SYS_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "tab_data_io.h"
#include "tab_interfere_analysis.h"
#include "tab_info_query.h"

namespace Ui {
class db_sys;
}

class db_sys : public QMainWindow
{
    Q_OBJECT

public:
    explicit db_sys(QWidget *parent = 0);
    ~db_sys();

private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_login_btn_clicked();

private:
    QSqlDatabase db;
    tab_data_IO *tab_data;
    tab_interfere_analysis *tab_analysis;
    tab_info_query *tab_query;
    Ui::db_sys *ui;
};

#endif // DB_SYS_H
