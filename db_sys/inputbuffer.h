#ifndef INPUTBUFFER_H
#define INPUTBUFFER_H

#endif // INPUTBUFFER_H
#include<QString>
#include <ActiveQt/QAxObject>
#include<QDebug>

class tbUnit{
public:
    QVariantList DataList;
    bool isNotNull(QString str){
        if(str=="")
            return false;
        else return true;
    }

    bool isLegalNum(int sum,int lowb, int Upb){
        if(lowb<=sum&&sum<=Upb)
            return true;//是数字且在范围内
        else
            return false;
    }

    QString toString(){
        QString REstr="";
        for(int i =0;i<this->DataList.length();i++){
            if(i>0)
                REstr += QString(',');
            //qDebug()<<DataList[i].typeName()<<(strcmp(DataList[i].typeName(),"QString")==0);
            if(strcmp(DataList[i].typeName(),"QString")==0){
                REstr += QString('\'');
                REstr += DataList[i].toString();
                REstr += QString('\'');
            }
            else
                REstr += DataList[i].toString();
        }
        REstr += QString('\n');
        //qDebug()<<REstr;
        return REstr;
    }
};

class tbCellUnit:public tbUnit{
public:

    tbCellUnit(QVariantList allEnvDataList_i,bool &ok){
        if(allEnvDataList_i.length()!=19)//列长于表不符，为错表
        {
            ok = false;
            return;
        }
        //qDebug()<<allEnvDataList_i;
        this->DataList = allEnvDataList_i;
        bool test;
        this->CITY = allEnvDataList_i[0].toString();
        this->SECTOR_ID = allEnvDataList_i[1].toString();
        ok &= isNotNull(this->SECTOR_ID);
        //qDebug()<<"test1 ="<<ok;
        this->SECTOR_NAME = allEnvDataList_i[2].toString();
        ok &= isNotNull(this->SECTOR_NAME);
        //qDebug()<<"test2.1 ="<<ok;

        this->ENODEBID = allEnvDataList_i[3].toString().toInt(&test,10);
        ok&= test;
        //qDebug()<<"test2.2 ="<<ok;
        ok&= isLegalNum(this->ENODEBID,0,0x3f3f3f3f);
        //qDebug()<<"test2.3 ="<<ok;

        this->ENODEB_NAME = allEnvDataList_i[4].toString();
        ok &= isNotNull(this->ENODEB_NAME);
        //qDebug()<<"test3.1 ="<<ok;
        this->EARFCN = allEnvDataList_i[5].toString().toInt(&test);
        ok&=test;
        //qDebug()<<"test3.2 ="<<ok;
        ok&= isLegalNum(this->EARFCN,0,0x3f3f3f3f);
        //qDebug()<<"test3.3 ="<<ok;

        this->PCI = allEnvDataList_i[6].toString().toInt(&test);
        ok&=test;
        //qDebug()<<"test4 ="<<ok;
        ok&= isLegalNum(this->PCI,0,0x3f3f3f3f);
        //qDebug()<<"test4.1 ="<<ok;

        this->PSS = allEnvDataList_i[7].toString().toInt(&test);
        ok&=test;
        //qDebug()<<"test4.3 ="<<ok;
        ok&= isLegalNum(this->PSS,0,0x3f3f3f3f);

        this->SSS = allEnvDataList_i[8].toString().toInt(&test);
        ok&=test;
        //qDebug()<<"test4.4 ="<<ok;
        ok&= isLegalNum(this->SSS,0,0x3f3f3f3f);

        this->TAC = allEnvDataList_i[9].toString().toInt(&test);
        ok&=test;
        //qDebug()<<"test4.5 ="<<ok;
        ok&= isLegalNum(this->TAC,0,0x3f3f3f3f);

        this->VENDOR = allEnvDataList_i[10].toString();
        ok &= isNotNull(this->VENDOR);
        //qDebug()<<"test5 ="<<ok;
        this->LONGITUDE = allEnvDataList_i[11].toString().toFloat(&test);
        ok&= test;
        //qDebug()<<"test6 ="<<ok;
        this->LATITUDE = allEnvDataList_i[12].toString().toFloat(&test);
        ok&= test;
        //qDebug()<<"test7 ="<<ok;
        this->STYLE = allEnvDataList_i[13].toString();
        ok &= isNotNull(this->VENDOR);
        //qDebug()<<"test8 ="<<ok;
        this->AZIMUTH = allEnvDataList_i[14].toString().toFloat(&test);
        ok&= test;
        //qDebug()<<"test9 ="<<ok;
        this->HEIGHT= allEnvDataList_i[15].toString().toFloat(&test);
        ok&= test;
        //qDebug()<<"test10 ="<<ok;
        this->ELECTTILT = allEnvDataList_i[16].toString().toFloat(&test);
        ok&= test;
        //qDebug()<<"test11 ="<<ok;
        this->MECHTILT = allEnvDataList_i[17].toString().toFloat(&test);
        ok&= test;
        //qDebug()<<"test12 ="<<ok;
        this->TOTLETILT = allEnvDataList_i[18].toString().toFloat(&test);
        ok&= test;
        //qDebug()<<ok;
    }


private:
    QString CITY;
    QString SECTOR_ID;
    QString SECTOR_NAME;
    int ENODEBID;
    QString ENODEB_NAME;
    int EARFCN;
    int PCI,PSS,SSS,TAC	;
    QString VENDOR;
    float LONGITUDE	,LATITUDE;
    QString STYLE;
    float AZIMUTH,	HEIGHT,	ELECTTILT,	MECHTILT,	TOTLETILT;

};

class tbMRODataUnit:public tbUnit{
public:

    tbMRODataUnit(QVariantList allEnvDataList_i,bool &ok){
        if(allEnvDataList_i.length()!=7)//列长于表不符，为错表
        {
            ok = false;
            return;
        }
        this->DataList = allEnvDataList_i;
        bool test;
        this->TimeStamp = allEnvDataList_i[0].toString();
        ok &= isNotNull(this->TimeStamp);
        this->ServingSector = allEnvDataList_i[1].toString();
        ok &= isNotNull(this->ServingSector);
        //qDebug()<<"test1 ="<<ok;
        this->InterferingSector = allEnvDataList_i[2].toString();
        ok &= isNotNull(this->InterferingSector);
        //qDebug()<<"test2.1 ="<<ok;

        this->LteScRSRP = allEnvDataList_i[3].toString().toFloat(&test);
        ok&= test;
        //qDebug()<<"test2.2 ="<<ok;
        this->LteNcRSRP = allEnvDataList_i[4].toString().toFloat(&test);
        ok&= test;
        //qDebug()<<"test2.3 ="<<ok;

        this->LteNcEarfcn = allEnvDataList_i[5].toString().toInt(&test);
        ok&=test;
        //qDebug()<<"test3.2 ="<<ok;
        this->LteNcPci = allEnvDataList_i[6].toString().toInt(&test);
        ok&=test;
    }

private:
    QString TimeStamp;
    QString ServingSector;
    QString InterferingSector;
    float LteNcRSRP	,LteScRSRP;
    int LteNcEarfcn,LteNcPci;
};

class tbPRBUnit:public tbUnit{
public:

    tbPRBUnit(QVariantList allEnvDataList_i,bool &ok){
        if(allEnvDataList_i.length()!=105)//列长于表不符，为错表
        {
            ok = false;
            return;
        }
        this->DataList = allEnvDataList_i;
        bool test;
        this->StartTime = allEnvDataList_i[0].toString();
        ok &= isNotNull(this->StartTime);
        this->cycle = allEnvDataList_i[1].toString().toInt(&test);
        ok&=test;
        //qDebug()<<"test3.2 ="<<ok;
        this->eNodeBName = allEnvDataList_i[2].toString();
        ok &= isNotNull(this->eNodeBName);
        //qDebug()<<"test1 ="<<ok;
        this->Sector = allEnvDataList_i[3].toString();
        ok &= isNotNull(this->Sector);
        //qDebug()<<"test2.1 ="<<ok;
        this->SECTOR_NAME = allEnvDataList_i[4].toString();
        ok &= isNotNull(this->SECTOR_NAME);
        //qDebug()<<"test2.3 ="<<ok;

        for(int i=0;i<100;i++){
            this->PRB[i] = allEnvDataList_i[i+5].toString().toInt(&test);
            ok&=test;
        }
    }

private:
    QString StartTime;
    int cycle;
    QString eNodeBName,Sector,SECTOR_NAME;
    int PRB[100];
};

class tbKPIUnit:public tbUnit{
public:

    tbKPIUnit(QVariantList allEnvDataList_i,bool &ok){
        if(allEnvDataList_i.length()!=42)//列长于表不符，为错表
        {
            ok = false;
            return;
        }
        this->DataList = allEnvDataList_i;
        bool test;
        this->StartTime = allEnvDataList_i[0].toString();
        ok &= isNotNull(this->StartTime);
        this->cycle = allEnvDataList_i[1].toString().toInt(&test);
        ok&=test;
        //qDebug()<<"testcycle ="<<ok;
        this->eNodeBName = allEnvDataList_i[2].toString();
        ok &= isNotNull(this->eNodeBName);
        //qDebug()<<"testNodeName ="<<ok;
        this->Sector = allEnvDataList_i[3].toString();
        ok &= isNotNull(this->Sector);
        //qDebug()<<"testSector ="<<ok;
        this->SECTOR_NAME = allEnvDataList_i[4].toString();
        ok &= isNotNull(this->SECTOR_NAME);
        //qDebug()<<"testSecName ="<<ok;

        int is_float[11]={7,10,13,14,18,27,28,29,30,31,35};
        int j=0;
        QString zero="0";
        for(int i=0;i<37;i++){
            if(QString::compare("NIL",allEnvDataList_i[i+5].toString())==0){
                this->DataList[i+5].setValue(zero);
                if((i+5)==is_float[j])//浮点数
                {
                    j++;
                }
                continue;
            }
            if((i+5)==is_float[j])//浮点数
            {
                allEnvDataList_i[i+5].toString().toFloat(&test);
                ok&= test;
                j++;
            }
            else{//整数
                allEnvDataList_i[i+5].toString().toLongLong(&test);
                ok&=test;
            }
            if(ok == false){
                qDebug()<<allEnvDataList_i[i+5].toString()<<(i+5);
                break;
            }
        }
    }

private:
    QString StartTime;
    int cycle;
    QString eNodeBName;
    QString Sector;
    QString SECTOR_NAME;
    int RPCreq;
    int RPCreqInc;
    float qf;
    int EARBSuc;
    int EARBTry;
    float EARBSucRate;
    int eNode_ERABRelease;
    int Sector_ERABRelease;
    float ERABMiss;
    float ay;
    int eNodeB_UEConRelease;
    int UEConRelease;
    int UEConSuc;
    float Miss;
    long eNodeBSuc1;
    long eNodeBTry1;
    long eNodeBSuc2;
    long eNodeBTry2;
    long eNodeBSuc3;
    long eNodeBTry3;
    long eNodeBSuc4;
    long eNodeBTry4;
    float eNBSucRate1;
    float eNBSucRate2;
    float syncZsp;
    float asyncZsp;
    float switchRate;
    long PDCPUpdT;
    long PDCPDownT;
    long RCreq;
    float RRCrate;
    long reBuildeNodeSuc1;
    long reBuildeNodeSuc2;
    long reBuildeNodeSuc3;
    long reBuildeNodeSuc4;
    long eNBSuc;
    long eNBTry;
};
