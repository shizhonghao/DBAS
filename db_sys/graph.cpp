#include "graph.h"
#include "ui_graph.h"
#include <QDebug>
#include <QSqlQuery>

#define pointx 60//原点x坐标
#define pointy 370//原点y坐标
#define height 360//图表高度
#define width 620//图表宽度

graph::graph(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::graph)
{
    ui->setupUi(this);
    this->db = db;
    image = new QImage(700,400,QImage::Format_RGB32);
    Paint();
}

graph::~graph()
{
    delete ui;
}

void graph::Paint()
{
    QPainter painter(image);
    QColor backColor = qRgb(255,255,255);
    image->fill(backColor);
    painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式，好看一点
    //绘制坐标轴 坐标轴原点(35，380)
    painter.drawRect(5,5,700-10,400-10);//外围的矩形，从(5,5)起，到(690,390)结束，周围留了5的间隙。
    painter.drawLine(pointx,pointy,width+pointx,pointy);//坐标轴x宽度为width
    painter.drawLine(pointx,pointy-height,pointx,pointy);//坐标轴y高度为height
}

void graph::KPIGraph(QString name)
{
    qDebug()<<"KPIGraph";
    Paint();
    QPainter painter(image);
    QPen penDegree;
    penDegree.setColor(Qt::black);
    penDegree.setWidth(2);
    painter.setPen(penDegree);
    //x轴刻度
    for(int i = 0;i < 3;i ++)
    {
        painter.drawLine(pointx + 5 + 300 * i,pointy,pointx + 5 + 300 * i,pointy + 4);
        painter.drawText(pointx - 50 + 300*i,pointy + 15,QString("07/%1/2016").arg(17+i));
    }
    //y轴刻度
    for(int i = 1;i <= 10; i++)
    {
        painter.drawLine(pointx,pointy - i*height/10,pointx - 4,pointy - i*height/10);
        painter.drawText(pointx-50,pointy-(i-0.15)*height/10,QString::number((float)height*i*0.234/3600));
    }
    QPen pen,penPoint;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    penPoint.setColor(Qt::blue);
    penPoint.setWidth(5);
    if(name != NULL)
    {
        qDebug()<<name;
        QSqlQuery queryN(db);
        QSqlQuery queryR(db);
        queryN.exec(QString("select distinct 小区名 from tbKPI where 网元名称 = '%1'").arg(name));
        QString EnodeName;
        int dx = 300;
        float x,y,fx,fy;
        while(queryN.next())
        {
            EnodeName = queryN.value(0).toString();
            queryR.exec(QString("select 起始时间,RRC连接重建比率 from tbKPI where 小区名 = '%1' order by 起始时间 asc").arg(EnodeName));
            fx = -1;
            x = 65;
            while(queryR.next())
            {
                qDebug()<<queryR.value(0).toString()<<queryR.value(1).toFloat();
                y = 370 - queryR.value(1).toFloat() * 360 / 0.234;
                if(fx != -1)
                {
                    x = fx + dx;
                    painter.setPen(pen);
                    painter.drawLine(fx,fy,x,y);
                }
                painter.setPen(penPoint);
                painter.drawPoint(x,y);
                fx = x;
                fy = y;
            }
        }
    }
    update();
}

void graph::PRBGraph(QString name, QString Sday, QString Eday)
{
    qDebug()<<"PRBGraph";
    int days = Eday.mid(3,2).toInt() - Sday.mid(3,2).toInt() + 1;
    qDebug()<<days;
    Paint();
    QPainter painter(image);
    QPen penDegree;
    penDegree.setColor(Qt::black);
    penDegree.setWidth(2);
    painter.setPen(penDegree);
    //x轴刻度
    for(int i = 0;i < 24;i ++)
    {
        painter.drawLine(pointx+5+620/23*i,pointy,pointx+5+620/23*i,pointy+4);
        painter.drawText(pointx+5+620/23*i,pointy+15,QString("%1").arg(i));
    }
    //y轴刻度
    for(int i = 1;i <= 10; i++)
    {
        painter.drawLine(pointx,pointy - i*height/10,pointx - 4,pointy - i*height/10);
        painter.drawText(pointx-50,pointy-(i-0.15)*height/10,QString::number((float)height*i*28/3600-124));
    }
    QPen pen,penPoint;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    penPoint.setColor(Qt::blue);
    penPoint.setWidth(5);
    if(name != NULL)
    {
        qDebug()<<name;
        QSqlQuery queryN(db);
        QSqlQuery queryR(db);
        float x,y,fx,fy;
        float dx = 620 / (24 * days - 1);
        QString sql;
        QString EnodeName;
        sql = QString("select distinct 小区名 from tbPRBnew where 网元名称 = '%1'").arg(name);
        queryN.exec(sql);
        //qDebug()<<sql;
        while(queryN.next())
        {
            EnodeName = queryN.value(0).toString();
            sql = QString("select 起始时间,PRB60 from tbPRBnew where 小区名 = '%1' and 起始时间 between '%2 00:00:00' and '%3 23:50:40' order by 起始时间 asc").arg(EnodeName).arg(Sday).arg(Eday);
            queryR.exec(sql);
            //qDebug()<<sql;
            x = 65;
            fx = -1;
            while(queryR.next())
            {
                qDebug()<<queryR.value(0).toString()<<queryR.value(1).toInt();
                y = 370 - (queryR.value(1).toInt() + 124)*360/28;
                if(fx != -1)
                {
                    x = fx + dx;
                    painter.setPen(pen);
                    painter.drawLine(fx,fy,x,y);
                }
                painter.setPen(penPoint);
                painter.drawPoint(x,y);
                fx = x;
                fy = y;
            }
        }
    }
    update();
}
