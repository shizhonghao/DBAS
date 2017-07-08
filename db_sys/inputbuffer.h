#ifndef INPUTBUFFER_H
#define INPUTBUFFER_H

#endif // INPUTBUFFER_H
#include<QString>
#include <ActiveQt/QAxObject>

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

