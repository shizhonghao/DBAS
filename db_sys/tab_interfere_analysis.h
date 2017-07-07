#ifndef TAB_INTERFERE_ANALYSIS_H
#define TAB_INTERFERE_ANALYSIS_H

#include <QWidget>

namespace Ui {
class tab_interfere_analysis;
}

class tab_interfere_analysis : public QWidget
{
    Q_OBJECT

public:
    explicit tab_interfere_analysis(QWidget *parent = 0);
    ~tab_interfere_analysis();

private:
    Ui::tab_interfere_analysis *ui;
};

#endif // TAB_INTERFERE_ANALYSIS_H
