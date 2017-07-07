#ifndef TAB_INTERFERE_ANALYSIS_H
#define TAB_INTERFERE_ANALYSIS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QFileDialog>

namespace Ui {
class tab_interfere_analysis;
}

class tab_interfere_analysis : public QWidget
{
    Q_OBJECT

public:
    explicit tab_interfere_analysis(QSqlDatabase db,QWidget *parent = 0);
    ~tab_interfere_analysis();

private slots:
    void on_C2I_file_clicked();

    void on_triple_file_clicked();

    void on_C2I_clicked();

    void on_triple_clicked();

private:
    Ui::tab_interfere_analysis *ui;
    QSqlDatabase db;
};

#endif // TAB_INTERFERE_ANALYSIS_H
