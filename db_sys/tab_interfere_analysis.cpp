#include "tab_interfere_analysis.h"
#include "ui_tab_interfere_analysis.h"

#include <QSqlQuery>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <ActiveQt/QAxObject>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <stdio.h>

using namespace std;

struct C2I
{
    C2I(QString _SCell,QString _NCell, double _C2I_mean, double _std)
    {
        SCell = _SCell;
        NCell = _NCell;
        C2I_mean = _C2I_mean;
        std = _std;
    }

    QString SCell;
    QString NCell;
    double C2I_mean;
    double std;
    double PrbC2I9;
    double PrbABS6;
};

struct edge
{
    int u,v;
    bool operator <(const edge e)const
    {
        if(u==e.u)
        {
            return v<e.v;
        }
        return u<e.u;
    }
};

struct triple
{
    int a[3];
    bool operator <(const triple x)const
    {
        if(this->a[0] == x.a[0])
        {
            if(this->a[1] == x.a[1])
            {
                return this->a[2] < x.a[2];
            }
            return this->a[1] < x.a[1];
        }
        return this->a[0] < x.a[0];
    }
};

tab_interfere_analysis::tab_interfere_analysis(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_interfere_analysis)
{
    ui->setupUi(this);
    this->db = db;

    /*
    QSqlQuery query(db);
    query.exec("select * from dbo.tbCell");
    while(query.next())
    {
        qDebug()<<query.value(1).toString();
    }
    */
}

tab_interfere_analysis::~tab_interfere_analysis()
{
    delete ui;
}

void tab_interfere_analysis::on_C2I_file_clicked() //设定tbC2Inew文件保存路径
{
    QFileDialog *file = new QFileDialog(this);
    file->setWindowTitle(tr("Open Image"));
    file->setDirectory(".");
    //file->setFilter(tr("Image Files(*.jpg *.png)"));
    if(file->exec() == QDialog::Accepted) {
            QString path = file->selectedFiles()[0];
            ui->C2I_path->setText(path);
            QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    } else {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

void tab_interfere_analysis::on_triple_file_clicked() //设定tbC2I3文件保存路径
{
    QFileDialog *file = new QFileDialog(this);
    file->setWindowTitle(tr("Open Image"));
    file->setDirectory(".");
    //file->setFilter(tr("Image Files(*.jpg *.png)"));
    if(file->exec() == QDialog::Accepted) {
            QString path = file->selectedFiles()[0];
            ui->triple_path->setText(path);
            QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    } else {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

double prb(double x,double mean,double std)
{
    if(std==0)
    {
        return mean<x ? 1.0 : 0.0;
    }
    return (1 + erf((x - mean)/std/sqrt(2)))/2.0;
}

void tab_interfere_analysis::on_C2I_clicked() //
{
    QString path = ui->C2I_path->text();
    if(path.isEmpty())
    {
        QMessageBox::information(NULL, tr("Path"), tr("You need to select a file first."));
        //return;
    }

    vector<C2I> v;
    QSqlQuery query(db);
    query.exec("delete from dbo.tbC2Inew");
    //query.exec("select distinct ServingSector, InterferingSector from dbo.tbMROData");
    query.exec("select distinct ServingSector, InterferingSector,avg(LteScRSRP-LteNcRSRP) as C2I_mean,stdev(LteScRSRP-LteNcRSRP) as std\
               from dbo.tbMROData\
               group by ServingSector, InterferingSector");
    while(query.next())
    {
        C2I c2i(query.value(0).toString(), query.value(1).toString(),query.value(2).toDouble(),query.value(3).toDouble());
        v.push_back(c2i);
    }
    for(int i=0;i<v.size();i++)
    {
        double mean = v[i].C2I_mean;
        double std = v[i].std;
        v[i].PrbC2I9 = prb(9.0,mean,std);
        v[i].PrbABS6 = prb(6.0,mean,std)-prb(-6.0000000000001,mean,std);
        qDebug() << v[i].SCell << v[i].NCell << v[i].C2I_mean << v[i].std << v[i].PrbC2I9 << v[i].PrbABS6;
        //QString insert = "insert into dbo.tbC2Inew values()";
        query.exec(QString("insert into dbo.tbC2Inew values('%1','%2',%3,%4,%5,%6)")
                   .arg(v[i].SCell)
                   .arg(v[i].NCell)
                   .arg(v[i].C2I_mean)
                   .arg(v[i].std)
                   .arg(v[i].PrbC2I9)
                   .arg(v[i].PrbABS6));
    }
}

void tab_interfere_analysis::on_triple_clicked()
{
    QString path = ui->triple_path->text();
    if(path.isEmpty())
    {
        QMessageBox::information(NULL, tr("Path"), tr("You need to select a file first."));
        //return;
    }

    vector<edge> e;
    vector<triple> res;
    vector<int> adj[1010];  //编号定义的邻接矩阵，共128个基站,154条边
    int sub = 0,ecnt = 0;
    map<int,QString> edge_name; //基站编号-基站名对应表
    map<QString,int> edge_id;  //基站名-基站编号对应表
    QSqlQuery query(db);
    query.exec("select SCELL, NCELL, PrbABS6 from dbo.tbC2Inew");
    while(query.next())
    {
        if(query.value(2).toDouble()>=0.7) //将所有PrbABS6大于0.7的边加入统计中
        {
            QString u = query.value(0).toString();
            QString v = query.value(1).toString();
            int u_sub,v_sub;
            if(edge_id.count(u)>0)
            {
                u_sub = edge_id[u];
            }
            else
            {
                u_sub = sub;
                edge_id[u] = sub;
                edge_name[sub] = u;
                sub++;
            }
            if(edge_id.count(v)>0)
            {
                v_sub = edge_id[v];
            }
            else
            {
                v_sub = sub;
                edge_id[v] = sub;
                edge_name[sub] = v;
                sub++;
            }
            adj[u_sub].push_back(v_sub);
            adj[v_sub].push_back(u_sub);
            //qDebug() << u_sub << v_sub;

            edge temp;
            temp.u = u_sub;
            temp.v = v_sub;
            ecnt++;
            e.push_back(temp);
        }
    }
    qDebug() << "point number is:" << sub;
    qDebug() << "edge number is:" << ecnt;

    for(int i=0;i<e.size();i++)
    {
        int u = e[i].u;
        int v = e[i].v;
        if(adj[u].size()>adj[v].size())
        {
            swap(u,v);
        }
        for(int j=0;j<adj[u].size();j++)
        {
            int w = adj[u][j];
            if(w==v)
            {
                continue;
            }
            //qDebug() << "check:" << u << v << w;
            vector<int>::iterator iter = std::find(adj[w].begin(),adj[w].end(),v);
            if(iter!=adj[w].end())
            {
                triple t;
                t.a[0] = u;
                t.a[1] = v;
                t.a[2] = w;
                //qDebug() << u << v << w;
                res.push_back(t);
            }
        }
    }

    for(int i=0;i<res.size();i++)
    {
        sort(res[i].a,res[i].a+3);
    }
    sort(res.begin(),res.end());

    qDebug() << "res:";
    query.exec("delete from dbo.tbC2I3");
    for(int i=0;i<res.size();i+=3)
    {
        query.exec(QString("insert into dbo.tbC2I3 values('%1','%2','%3')")
                   .arg(edge_name[res[i].a[0]])
                   .arg(edge_name[res[i].a[1]])
                   .arg(edge_name[res[i].a[2]]));
        qDebug() << res[i].a[0] << res[i].a[1] << res[i].a[2];
    }

}
