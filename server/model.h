#include "sim.h"

class CMyModel;

class CPart : public CSimMsg
{ public:
	long t0;
    bool sort; // true - 1st sort, false - 2nd sort
	CPart(int t=-1) : CSimMsg(sizeof(CPart), t) {}
};

class CGenerator : public CSimThread{
  protected:
    CMyModel* m_pModel;
    double m_pLTime;
    double m_pUTime;
  public :
    void Main();
	CGenerator (CMyModel* pModel); 
    CGenerator (CMyModel* pModel, double time, double timedelta);
};

class CDevice : public CSimThread{
protected:
    CMyModel* m_pModel;
    double m_pLTime;
    double m_pUTime;
public :
    void Main();
    CDevice (CMyModel* pModel);
    CDevice (CMyModel* pModel, double time, double timedelta);
};

class CCementationDevice : public CDevice {
public :
    void Main();
    CCementationDevice(CMyModel* pModel, double time, double timedelta);

};

class CHardeningDevice : public CDevice {
public:
    void Main();
    void sendDetailOff(double t);
    CHardeningDevice(CMyModel* pModel, double time, double timedelta);
};

class CMyModel : public CSimTimer
{ public:
   CDevice *m_pCementation;
   CDevice *m_pHardening;
   CGenerator *m_pGenerator;
   QList<double> experiments;
   double maxTime;
   double minTime;
   double dt;
   int N;
   int columns;

   CMyModel();
   ~CMyModel();
   QString analyze();
};

QString simulation();
