#ifndef TAB_INTERFERE_ANALYSIS_H
#define TAB_INTERFERE_ANALYSIS_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class tab_interfere_analysis;
}

class tab_interfere_analysis : public QWidget
{
    Q_OBJECT

public:
    explicit tab_interfere_analysis(QSqlDatabase db,QWidget *parent = 0);
    ~tab_interfere_analysis();

private:
    Ui::tab_interfere_analysis *ui;
    QSqlDatabase db;
};

#endif // TAB_INTERFERE_ANALYSIS_H
