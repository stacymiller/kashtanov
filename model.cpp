// sim.cpp : Defines the entry point for the console application.
//

#include <QMutex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <deque>
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
        long t = CurTime() - part.t0;
        if (t > 25) {
            m_pModel->dt+=t;
            m_pModel->N++;
            outstr += QString::number(CurTime()) + " done " + QString::number(m_pModel->N) + " first sort\n";
        } else if (t > 20) {
            Send(part, m_pModel->m_pHardening);
        } else {
            m_pModel->dt+=t;
            m_pModel->N++;
            outstr += QString::number(CurTime()) + " done " + QString::number(m_pModel->N) + " second sort\n";
        }
    }
}
 
CMyModel::CMyModel(){
    m_pGenerator=new CGenerator(this);
    m_pCementation=new CCementationDevice(this, 10., 7.);
    m_pHardening=new CHardeningDevice(this, 10., 6.);
    dt=0;
    N=0;
}

CMyModel::~CMyModel()
{ delete m_pGenerator;
  delete m_pCementation;
  delete m_pHardening;
}

CCementationDevice::CCementationDevice(CMyModel* pModel, double time, double timedelta) : CDevice(pModel, time, timedelta) {}
CHardeningDevice::CHardeningDevice(CMyModel* pModel, double time, double timedelta) : CDevice(pModel, time, timedelta) {}

QString simulation(){
    outstr = "";
    out.open("sim.out");
    rninit(1);
    CMyModel model;
    model.Run();

    outstr += "average=" + QString::number(model.dt/model.N);

    out.close();
    return outstr;
}
