// sim.cpp : Defines the entry point for the console application.
//

#include <QMutex>
#include <QDateTime>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <deque>
#include <limits>
using namespace std;
#include "rand.h"
#include "sim.h"
#include "model.h"

ofstream out;
QString outstr="";

const int unit=1000;
const int n=400;

CGenerator::CGenerator(CMyModel* pModel) : CSimThread((CSimTimer*)pModel){
    m_pModel=pModel;
    m_pLTime = 1;
    m_pUTime = 1;
}

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
    for (int i=0; i<n; i++)
    {
        Wait (rnunif()*(m_pUTime-m_pLTime) + m_pLTime);
        Part.t0=CurTime();
        Send(Part, pCementation);
        outstr += QString::number(CurTime()) + " new " + QString::number(i+1) + "\n";
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

void CDevice::Main()
{ CPart Part;
  for(;;)
  { Receive(Part);
    Wait(rnunif()*unit);
    m_pModel->dt+=CurTime()-Part.t0;
    m_pModel->N++;
    outstr += QString::number(CurTime()) + " done\n";;
  }
}

void CCementationDevice::Main(){
    CPart part;
    while(1) {
        Receive(part);
        Wait(rnunif()*(m_pUTime-m_pLTime) + m_pLTime);
        Send(part, m_pModel->m_pHardening);
    }
}

void CHardeningDevice::Main(){
    CPart part;
    while(1) {
        Receive(part);
        Wait(rnunif()*(m_pUTime-m_pLTime) + m_pLTime);
        double t = CurTime() - part.t0;
        if (t > 25) {
            sendDetailOff(t);
        } else if (t > 20) {
            Send(part, m_pModel->m_pHardening);
        } else {
           sendDetailOff(t);
        }
    }
}

void CHardeningDevice::sendDetailOff(double t){
    m_pModel->dt+=t;
    m_pModel->N++;
    m_pModel->experiments.append(t);
    if (t < m_pModel->minTime) {
        m_pModel->minTime = t;
    } else if (t > m_pModel->maxTime) {
        m_pModel->maxTime = t;
    }
    outstr += QString::number(CurTime()) + " done " + QString::number(m_pModel->N) + ((t>25)?" first sort\n":" second sort\n");
}
 
CMyModel::CMyModel(){
    m_pGenerator=new CGenerator(this);
    m_pCementation=new CCementationDevice(this, 10., 7.);
    m_pHardening=new CHardeningDevice(this, 10., 6.);
    maxTime=numeric_limits<double>::min();
    minTime=numeric_limits<double>::max();
    columns=8;
    dt=0;
    N=0;
}

CMyModel::~CMyModel()
{ delete m_pGenerator;
  delete m_pCementation;
  delete m_pHardening;
}

QString CMyModel::analyze(){
    int histogram[columns];
    for(int i = 0; i < columns; i++){
        histogram[i] = 0;
    }
    QString ans = "";
    ans += "min:" + QString::number(minTime) + "\n";
    ans += "max:" + QString::number(maxTime) + "\n";
    double sector = (maxTime - minTime) / columns;
    for(int i = 0; i < N; i++){
        double exp = experiments[i];
        float current = sector;
        for(int j = 0; j < columns; j++){
            if (exp < current){
                histogram[j]++;
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
    return ans;
}

CCementationDevice::CCementationDevice(CMyModel* pModel, double time, double timedelta) : CDevice(pModel, time, timedelta) {}
CHardeningDevice::CHardeningDevice(CMyModel* pModel, double time, double timedelta) : CDevice(pModel, time, timedelta) {}

QString simulation(){
    outstr = "";
    out.open("sim.out");
    rninit(QDateTime::currentDateTime().toTime_t());
    CMyModel model;
    model.Run();
    QString ans = model.analyze();

    outstr += "average=" + QString::number(model.dt/model.N);

    out.close();
    return ans;
}
