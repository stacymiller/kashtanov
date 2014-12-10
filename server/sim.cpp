#include "sim.h"


CSimEvent::~CSimEvent()
{ 
}

bool operator < (CSimEvent e1, CSimEvent e2)
{ return e1.Time>e2.Time;
}

void CQMsg::clear()
{ for (iterator p=begin(); p<end(); p++) delete *p;
  deque<CSimMsg*>::clear();
}

////////////////////////////////////////


CSimTimer::CSimTimer()
{ CurTime=0;
  CurThread=0;
}

CSimTimer::~CSimTimer()
{ 
}

void CSimTimer::TerminateThreads()
{ while (!QWait.empty())
  { CSimThread* thread=QWait.top().Thread;
    thread->terminate();
    QWait.pop();
  }
  while (!QRemind.empty())
  { CSimThread* thread=QRemind.top().Thread;
    thread->terminate();
    QRemind.pop();
  }
  while (!QReceive.empty())
  { CSimThread* thread=QReceive.front().Thread;
    thread->terminate();
    QReceive.pop_front();
  }
}

void CSimTimer::Run()
{ for(;;)
  {  while(!QNew.empty())
     { QNew.front()->Init();
       QNew.pop_front();
     }
     CurThread=CheckReceive();
     if (!CurThread)  
     {  SimTime t1 = !QReceive.empty() ? QReceive.front().Time  : MAXTIME,
                t2 = !QWait.empty()    ? QWait.top().Time       : MAXTIME,
                t3 = !QRemind.empty()  ? QRemind.top().Time     : MAXTIME;
        if( t1<=t2 && t1<=t3 && !QReceive.empty())
        { CurTime = t1;
          if( CurTime == MAXTIME ) break;
          else
          { QReceive.front().Msg->type = receive_time_end;
            CurThread=QReceive.front().Thread;
            QReceive.pop_front();
          }
        }
        else if( t2<=t1 && t2<=t3 && !QWait.empty())
        { CurTime=t2;
          CurThread=QWait.top().Thread;
          QWait.pop();
        }
        else if( t3<=t1 && t3<=t2 && !QRemind.empty())
        { CSimEvent e = QRemind.top();
          CSimThread *p = e.Thread;
          CurTime=t3;
          if( p->Active())
              p->QMsg.push_back(e.Msg);
          else delete e.Msg;
          QRemind.pop();
        }
        else break;
     }
     if (CurThread)
     {  CurThread->Resume();
        if (exit_code >1)
        { while (CurThread->Active());
        }
        if (exit_code==3) delete CurThread;
     }
  }
  TerminateThreads(); 
}

CSimThread*  CSimTimer::CheckReceive()
{ for (deque<CSimEvent>::iterator e=QReceive.begin(); e!=QReceive.end(); e++)
  {  for (deque<CSimMsg*>::iterator m = e->Thread->QMsg.begin(); m!=e->Thread->QMsg.end(); m++)
     { if( e->TypeReceive==indefinite_type || e->TypeReceive == (**m).type)
       {  e->Msg->Copy(**m);
          delete *m;
          e->Thread->QMsg.erase(m);
          CSimThread* p=e->Thread;
          QReceive.erase(e); 
          return p;
       }
     }
  }
  return 0;
}


/////////////////////////////////////////

void CSimMutex::Lock()
{ lock();
  m_Host=QThread::currentThread();
}

void CSimMutex::Switch()
{ QThread* thread=QThread::currentThread();
  unlock();
  while(m_Host==thread);
  Lock();
}

/////////////////////////////////////////

CSimThread::CSimThread(CSimTimer* timer)
{ m_pTimer=timer;
  m_pTimer->QNew.push_back(this);
}

CSimThread::~CSimThread()
{ QMsg.clear();
}

void CSimThread::Init()
{ m_pTimer->QWait.push(CSimEvent(this,m_pTimer->CurTime));
  m_Mutex.Lock();
  start();
}

void CSimThread::run()
{ m_Mutex.Lock();
  Main();
  ExitSimThread();
}

void CSimThread::Resume()
{ m_Mutex.Switch();
  m_pTimer->exit_code=0;
}

void CSimThread::Suspend()
{ m_Mutex.Switch();
}

void CSimThread::ExitSimThread()
{ m_pTimer->exit_code=2;
  m_Mutex.unlock();
  exit(2);
}

void CSimThread::DeleteSimThread()
{ m_pTimer->exit_code=3;
  m_Mutex.unlock();
  exit(3);
}

bool CSimThread::Active()
{ return !isFinished();
}

void CSimThread::Wait(SimTime t)
{ m_pTimer->QWait.push(CSimEvent(this,m_pTimer->CurTime+t));
  Suspend();
}

SimTime CSimThread::CurTime()
{ return m_pTimer->CurTime;
}

bool CSimThread::IsMessage()
{ return (!QMsg.empty());
}

bool CSimThread::Peek (CSimMsg& msg)
{ return PeekType (msg, indefinite_type);
}

bool CSimThread::PeekType (CSimMsg& msg, int type )
{ for (deque <CSimMsg*>::iterator m = QMsg.begin(); m!=QMsg.end(); m++)
  { if( type == indefinite_type  || (**m).type == type )
    { msg.Copy(**m);
      delete *m;
      QMsg.erase(m);
      return true;
    }
  }
  return false;
}

void CSimThread::Receive (CSimMsg& msg)
{ TimeReceiveType (msg, MAXTIME-CurTime(), indefinite_type);
}

void CSimThread::ReceiveType (CSimMsg& msg, int type)
{ TimeReceiveType (msg, MAXTIME-CurTime(), type);
}

void CSimThread::TimeReceive (CSimMsg& msg, SimTime t)
{ TimeReceiveType (msg, t, indefinite_type);
}

void CSimThread::TimeReceiveType (CSimMsg& msg, SimTime t, int type)
{ if (PeekType(msg, type)) return;
  m_pTimer->QReceive.push_back(CSimEvent(this, CurTime()+t, &msg, type));
  Suspend();
}

void CSimThread::Remind(CSimMsg& msg, SimTime t)
{ CSimMsg* m = (CSimMsg*) new char[msg.size];
  memcpy (m, &msg, msg.size);
  m_pTimer->QRemind.push(CSimEvent(this, CurTime()+t, m));
}

bool CSimThread::Send(CSimMsg& msg, CSimThread* p)
{ if( p->Active())
  {  CSimMsg* m=(CSimMsg*) new char[msg.size];
     memcpy(m, &msg, msg.size);
     p->QMsg.push_back(m);
	 return true;
  }
  else return false;
}

///////////////////////////////////////////

void CSimMsg::Copy(CSimMsg& src_msg)
{ if (size<src_msg.size) 
  { return;
  }
  memcpy(this, &src_msg, src_msg.size);
}
