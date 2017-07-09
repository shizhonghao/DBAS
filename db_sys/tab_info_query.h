#ifndef TAB_INFO_QUERY_H
#define TAB_INFO_QUERY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class tab_info_query;
}

class tab_info_query : public QWidget
{
    Q_OBJECT

public:
    explicit tab_info_query(QSqlDatabase db,QWidget *parent = 0);
    ~tab_info_query();

private:
    Ui::tab_info_query *ui;
    QSqlDatabase db;
    QComboBox *mission;
    QComboBox *ComSectorID;
    QLineEdit *LineSectorID;
    QLineEdit *LineSectorName;
    QPushButton *queryButton;
    void iniInfo();

private slots:
    void missionChange(int n);
    void QuerytbCell();
    void QueryeNodeB();
    void QueryKPI();
    void QueryPRB();
    void ButtonAction();
};

#endif // TAB_INFO_QUERY_H
