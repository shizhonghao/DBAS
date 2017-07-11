#ifndef TAB_INFO_QUERY_H
#define TAB_INFO_QUERY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QCalendarWidget>
#include "graph.h"
#include <QMessageBox>
#include <QLabel>

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
    QTableWidget *displayTable;
    QImage *image;
    graph *graphic;
    QPushButton *chooseSDate;
    QPushButton *chooseEDate;
    QCalendarWidget *calendarS;
    QCalendarWidget *calendarE;
    QString Sday;
    QString Eday;
    QMessageBox *message;
    QLabel *labelMission;
    QLabel *labelComSec;
    QLabel *labelId;
    QLabel *labelName;
    void iniInfo();
    void Paint();

private slots:
    void missionChange(int n);
    void QuerytbCell();
    void QueryeNodeB();
    void QueryKPI();
    void QueryPRB();
    void ButtonAction();
    void comSecNameChange(QString name);
    void ChoSDate();
    void ChoEDate();
    void SDateChange(QDate Ndate);
    void EDateChange(QDate Ndate);
};

#endif // TAB_INFO_QUERY_H
