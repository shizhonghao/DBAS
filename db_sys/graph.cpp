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
    if(Eday.mid(6,4).toInt() < Sday.mid(6,4).toInt())
    {
        days = 1;
    }
    else if(Eday.mid(6,4).toInt() > Sday.mid(6,4).toInt())
    {
        days = 3;
    }
    else
    {
        if(Eday.mid(0,2).toInt() < Sday.mid(0,2).toInt())
        {
            days = 1;
        }
        else if(Eday.mid(0,2).toInt() > Sday.mid(0,2).toInt())
        {
            days = 3;
        }
    }
    if(days > 3)
    {
        days = 3;
    }
    if(days <= 0)
    {
        days = 1;
    }
    int i;
    qDebug()<<days;
    Paint();
    QPainter painter(image);
    QPen penDegree;
    penDegree.setColor(Qt::black);
    penDegree.setWidth(2);
    painter.setPen(penDegree);
    float dx = 615.0 / (24 * days - 1);
    //x轴刻度
    for(i = 0;i < days*4;i ++)
    {
        painter.drawLine(pointx+5+dx*i*6,pointy,pointx+5+dx*i*6,pointy+4);
        painter.drawText(pointx+dx*i*6,pointy+20,QString("%1").arg(i*6));
    }
    painter.drawLine(680,pointy,680,pointy+4);
    painter.drawText(675,pointy+20,QString("%1").arg(days*24-1));
    //y轴刻度
    for(i = 1;i <= 10; i++)
    {
        painter.drawLine(pointx,pointy - i*height/10,pointx - 4,pointy - i*height/10);
        painter.drawText(pointx-50,pointy-(i-0.15)*height/10,QString::number((float)height*i*28/3600-124));
    }
    QPen pen[6];
    QPen penPoint;
    pen[0].setColor(Qt::blue);
    pen[0].setWidth(2);
    pen[1].setColor(Qt::red);
    pen[1].setWidth(2);
    pen[2].setColor(Qt::green);
    pen[2].setWidth(2);
    pen[3].setColor(Qt::gray);
    pen[3].setWidth(2);
    pen[4].setColor(Qt::cyan);
    pen[4].setWidth(2);
    pen[5].setColor(Qt::black);
    pen[5].setWidth(2);
    penPoint.setColor(Qt::black);
    penPoint.setWidth(5);
    if(name != NULL)
    {
        qDebug()<<name;
        QSqlQuery queryN(db);
        QSqlQuery queryR(db);
        float x,y,fx,fy;
        QString sql;
        QString EnodeName;
        sql = QString("select distinct 小区名 from tbPRBnew where 网元名称 = '%1'").arg(name);
        queryN.exec(sql);
        //qDebug()<<sql;
        i = 0;
        int pointcount = 0;
        while(queryN.next())
        {
            EnodeName = queryN.value(0).toString();
            sql = QString("select 起始时间,PRB60 from tbPRBnew where 小区名 = '%1' and 起始时间 between '%2 00:00:00' and '%3 23:50:40' order by 起始时间 asc").arg(EnodeName).arg(Sday).arg(Eday);
            queryR.exec(sql);
            //qDebug()<<sql;
            x = 65;
            fx = -1;
            pointcount= 0;
            while(queryR.next())
            {
                qDebug()<<queryR.value(0).toString()<<queryR.value(1).toInt();
                y = 370 - (queryR.value(1).toInt() + 124)*360/28;
                if(fx != -1)
                {
                    x = fx + dx;
                    painter.setPen(pen[i]);
                    painter.drawLine(fx,fy,x,y);
                }
                painter.setPen(penPoint);
                painter.drawPoint(x,y);
                fx = x;
                fy = y;
                pointcount++;
            }
            i++;
            qDebug()<<"x"<<x;
            qDebug()<<pointx+5+dx*(days*24-1);
        }
        qDebug()<<pointcount;
    }
    update();
}
