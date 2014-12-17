#include "sim.h"

class CMyModel;

struct modelData {
    int N;
    int columns;
    double cementationTime;
    double cementationTimedelta;
    double hardeningTime;
    double hardeningTimedelta;
};


class CPart : public CSimMsg
{ public:
	long t0;
    bool sort; // true - 1st sort, false - 2nd sort
    int id;
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
    void sendDetailOff(double t, int detailId);
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
   int expNumber;

   CMyModel();
   CMyModel(int exp, int col);
   CMyModel(int exp, int col, double cementationTime, double cementationTimedelta, double hardeningTime, double hardeningTimedelta);

   ~CMyModel();
   QString analyze();
};

QString simulation(modelData data);
