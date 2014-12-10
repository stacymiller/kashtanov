#include "stdafx.h"
#include "resource.h"
#include "excel9.h"
#include "Socket.h"
#include "XLClientDlg.h"

CMySocket::CMySocket(CXLClientDlg* pDlg)
{ m_beg=0;
  m_pDlg=pDlg;
  Create();
  //Connect(host, port); 
}

void CMySocket::OnReceive(int nErrorCode)
{ CSocket::OnReceive(nErrorCode);

  int n=Receive(&m_buf[m_beg], BUF_SIZE-m_beg);
  if (n!=0 && n!=SOCKET_ERROR)
  { if (m_beg+n>=sizeof(double))
    { 
		m_pStringResult = CString(m_buf, n);
		memcpy(&m_result, m_buf, sizeof(double));
		m_pDlg->SetResult();
		m_beg=0;
    } 
    else m_beg+=n;
  }
}
