#if !defined(PROCS_H)
#define PROCS_H

//#define MFCTHREAD

#define receive_time_end  -1
#define indefinite_type   -2
#define MAXTIME 0x7fffffff
#define ZEROTIME 0l

#include <QThread>
#include <QMutex>
#include <queue>
#include <deque>
#include <list>

using namespace std ;

typedef long SimTime;
class CSimThread;

class CSimMsg
{ public:
     CSimMsg(int s=2*sizeof(int), int t=indefinite_type){size=s; type=t;}
     int type;
     void Copy(CSimMsg&); 
  private:
	 int size;

  friend class CSimTimer;
  friend class CSimThread;
};

class CSimEvent
{ CSimThread* Thread;
  CSimMsg*    Msg;    
  int         TypeReceive;

 public:
  SimTime     Time;

  CSimEvent(CSimThread* thread, SimTime time, CSimMsg* msg=0, int type=indefinite_type)
  {Thread=thread; Time=time; Msg=msg; TypeReceive=type;}
  ~CSimEvent();    
  friend class CSimTimer;
  friend class CSimThread;
  friend bool operator < (CSimEvent e1, CSimEvent e2);
};

bool operator < (CSimEvent e1, CSimEvent e2);

class CSimTimer
{ QThread* thread;
  priority_queue<CSimEvent> QWait;
  priority_queue<CSimEvent> QRemind;
  deque<CSimEvent>          QReceive;
  deque<CSimThread*>        QNew;
  CSimThread* CurThread; 
  CSimThread*  CheckReceive();
  void TerminateThreads();
  int exit_code;
 public:
  SimTime CurTime;

  CSimTimer();
  ~CSimTimer();
  void Run();

  friend class CSimThread;
};

struct CQMsg : public deque<CSimMsg*>
{ void clear();
};

class CSimMutex : public QMutex
{    QThread* m_Host;
  public :
     void Lock();
     void Switch();
};

class CSimThread : public QThread
{ Q_OBJECT

  CQMsg QMsg;
  void Resume();
  void Suspend();

 public:
  CSimTimer* m_pTimer;
  CSimMutex m_Mutex;

  CSimThread(){}
  CSimThread(CSimTimer*);
  ~CSimThread();
  void run();
  virtual void Main(){};
  void Init();
  SimTime CurTime();
  bool Active();
  void ExitSimThread();
  void DeleteSimThread();


  void Wait (SimTime);
  bool IsMessage();
  bool Send (CSimMsg&, CSimThread*);
  bool Peek(CSimMsg&);
  bool PeekType (CSimMsg&, int);
  void Receive  (CSimMsg&);
  void ReceiveType (CSimMsg&, int);
  void TimeReceive (CSimMsg&, SimTime);
  void TimeReceiveType (CSimMsg&, SimTime, int);
  void Remind (CSimMsg&, SimTime);

  friend class CSimTimer;
};

#endif
