#include "stdafx.h"
#include "resource.h"
#include "ServerDlg.h"
#include "Socket.h"


void CListenSocket::OnAccept(int nErrorCode) 
{      CSocket::OnAccept(nErrorCode);

       CClientSocket* pSocket=new CClientSocket(this);
       if (Accept(*pSocket))
       { CSocketRef *pSocketRef=new CSocketRef(pSocket, m_Clients);
         m_Clients=pSocketRef;
       }            
   	   else delete pSocket;
}

CListenSocket::~CListenSocket()
{ while(m_Clients)
  { delete m_Clients->pSocket;
    CSocketRef* tmp=m_Clients;
    m_Clients=m_Clients->next;
    delete tmp;
  }
}


CClientSocket::CClientSocket(CListenSocket* pListenSocket)
{ m_pListenSocket=pListenSocket;
  m_beg=0;
}

void CClientSocket::OnReceive(int nErrorCode)
{ CSocket::OnReceive(nErrorCode);

  int n=Receive(&m_buf[m_beg], BUF_SIZE-m_beg);
  if (n!=0 && n!=SOCKET_ERROR)
  { if (m_beg+n>=sizeof(m_data))
    { memcpy(&m_data,m_buf,sizeof(m_data));
      m_beg=0;
      m_pListenSocket->m_pDlg->CalcResult(this);
    } 
    else m_beg+=n;
  }
}
