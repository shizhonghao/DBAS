#ifndef TAB_DATA_IO_H
#define TAB_DATA_IO_H

#include <QWidget>

namespace Ui {
class tab_data_IO;
}

class tab_data_IO : public QWidget
{
    Q_OBJECT

public:
    explicit tab_data_IO(QWidget *parent = 0);
    ~tab_data_IO();

private slots:
    void on_pushButton_clicked();

private:
    Ui::tab_data_IO *ui;
};

#endif // TAB_DATA_IO_H

