// sim.cpp : Defines the entry point for the console application.
//

#include <QMutex>
#include <QDateTime>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <queue>
#include <deque>
#include <limits>
#include <stdio.h> 
#include <math.h>
using namespace std;
#include "rand.h"
#include "sim.h"
#include "model.h"

ofstream out;
QString outstr="";

const int unit=60;
const int n=400;

CGenerator::CGenerator(CMyModel* pModel, double time, double timedelta): CSimThread((CSimTimer*)pModel){
    m_pModel = pModel;
    if (timedelta < 0) {
        timedelta = -timedelta;
    }
    m_pLTime = time - timedelta;
    m_pUTime = time + timedelta;
}

void CGenerator::Main(){
    CPart Part;
    CDevice* pCementation=m_pModel->m_pCementation;
    int exp = m_pModel->expNumber;
    for (int i=0; i<exp; i++)
    {
        Wait (rnunif()*unit*(m_pUTime-m_pLTime) + m_pLTime*unit);
        Part.t0=CurTime();
        Part.id=i+1;
        Send(Part, pCementation);
        out << (CurTime()) << " new " << i+1 << "\n";
    }
}

CDevice::CDevice(CMyModel* pModel): CSimThread((CSimTimer*)pModel){
    m_pModel=pModel;
    m_pLTime = 1;
    m_pUTime = 1;
}

CDevice::CDevice(CMyModel* pModel, double time, double timedelta): CSimThread((CSimTimer*)pModel){
    m_pModel = pModel;
    if (timedelta < 0) {
        timedelta = -timedelta;
    }
    m_pLTime = time - timedelta;
    m_pUTime = time + timedelta;
}

void CDevice::Main(){
    CPart Part;
    for(;;){
        Receive(Part);
        Wait(rnunif()*unit);
        m_pModel->dt+=CurTime()-Part.t0;
        m_pModel->N++;
        out << CurTime() << " done\n";
  }
}

void CCementationDevice::Main(){
    CPart part;
    while(1) {
        Receive(part);
        Wait(rnunif()*unit*(m_pUTime-m_pLTime) + m_pLTime*unit);
        Send(part, m_pModel->m_pHardening);
        out << CurTime() << " Cementation for " << part.id << " done\n";
    }
}

void CHardeningDevice::Main(){
    CPart part;
    while(1) {
        Receive(part);
        Wait(rnunif()*unit*(m_pUTime-m_pLTime) + m_pLTime*unit);
        out << CurTime() << " Hardening for " << part.id << " done\n";
        double t = CurTime() - part.t0;
        if (t > 25*unit) {
            part.type = false;
            sendDetailOff(t, part.id);
        } else if (t > 20*unit) {
            Send(part, m_pModel->m_pHardening);
        } else {
            part.type = true;
            sendDetailOff(t, part.id);
        }
    }
}

void CHardeningDevice::sendDetailOff(double t, int detailId){
    m_pModel->dt+=t;
    m_pModel->N++;
    m_pModel->experiments.append(t);
    if (t < m_pModel->minTime) {
        m_pModel->minTime = t;
    }
    if (t > m_pModel->maxTime) {
        m_pModel->maxTime = t;
    }
    out << CurTime() << " done "  << detailId << ((t<20*unit)?" first sort\n":" second sort\n");
}
 
CMyModel::CMyModel(){
    CMyModel(400, 8, 10., 7., 10., 6.);
}

CMyModel::CMyModel(int exp, int col){
    CMyModel(exp, col, 10., 7., 10., 6.);
}

CMyModel::CMyModel(int exp, int col, double cementationTime, double cementationTimedelta, double hardeningTime, double hardeningTimedelta){
    m_pGenerator=new CGenerator(this, 10., 5.);
    m_pCementation=new CCementationDevice(this, cementationTime, cementationTimedelta);
    m_pHardening=new CHardeningDevice(this, hardeningTime, hardeningTimedelta);
    maxTime=numeric_limits<double>::min();
    minTime=numeric_limits<double>::max();
    this->columns=col;
    this->expNumber = exp;
    dt=0;
    N=0;
    printf("columns=%d expNumber=%d\n", columns, expNumber);
}


CMyModel::~CMyModel()
{ delete m_pGenerator;
  delete m_pCementation;
  delete m_pHardening;
}

QString CMyModel::analyze(){
    if (columns < 0 || columns > 10000) {
        columns = 1;
    }
    int histogram[columns];
    double averages[columns];
    for(int i = 0; i < columns; i++){
        histogram[i] = 0;
        averages[i] = 0;
    }
    QString ans = "";
    ans += "min:" + QString::number(minTime) + "\n";
    ans += "max:" + QString::number(maxTime) + "\n";
    ans += "sectors:" + QString::number(columns) + "\n";
    double sector = (maxTime - minTime) / columns;
    for(int i = 0; i < N; i++){
        double exp = experiments[i];
        out << exp << ", ";
        float current = minTime + sector;
//        int j = (int) floor((exp - minTime) / sector);
//        out << j << ", ";
//        histogram[j]++;
//        averages[j] = ((histogram[j] - 1)*averages[j] + exp) / histogram[j];
        for(int j = 0; j < columns; j++){
            if (exp <= current){
                histogram[j]++;
                averages[j] = ((histogram[j] - 1)*averages[j] + exp) / histogram[j];
                break;
            } else {
                current += sector;
            }
        }
    }

    ans += "histogram:";
    for(int i = 0; i < columns; i++){
        ans += QString::number(histogram[i]) + ",";
    }
    ans += "\n";

    ans += "averages:";
    for(int i = 0; i < columns; i++){
        ans += QString::number(averages[i]) + ",";
    }
    ans += "\n";
    return ans;
}

CCementationDevice::CCementationDevice(CMyModel* pModel, double time, double timedelta) : CDevice(pModel, time, timedelta) {}
CHardeningDevice::CHardeningDevice(CMyModel* pModel, double time, double timedelta) : CDevice(pModel, time, timedelta) {}

QString simulation(modelData data){
    out.open("sim.out");
    rninit(QDateTime::currentDateTime().toTime_t());
    CMyModel model(data.N, data.columns, data.cementationTime, data.cementationTimedelta, data.hardeningTime, data.hardeningTimedelta);
    model.Run();
    QString ans = model.analyze();

    out.close();
    return ans;
}
