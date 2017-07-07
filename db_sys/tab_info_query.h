#ifndef TAB_INFO_QUERY_H
#define TAB_INFO_QUERY_H

#include <QWidget>
#include <QSqlDatabase>

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
};

#endif // TAB_INFO_QUERY_H
