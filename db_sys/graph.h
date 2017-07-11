#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QSqlDatabase>
#include <QPaintEvent>
#include <QPainter>

namespace Ui {
class graph;
}

class graph : public QWidget
{
    Q_OBJECT

public:
    explicit graph(QSqlDatabase db,QWidget *parent = 0);
    ~graph();
    void KPIGraph(QString name);
    void PRBGraph(QString name,QString Sday,QString Eday);

private:
    Ui::graph *ui;
    QSqlDatabase db;
    QImage *image;
    void Paint();

protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.drawImage(0,0,*image);
    }
};

#endif // GRAPH_H
