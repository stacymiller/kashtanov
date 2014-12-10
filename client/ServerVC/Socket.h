

const int BUF_SIZE=1024;

struct CSocketRef
{ CSocket* pSocket;
  CSocketRef* next;
  CSocketRef(CSocket* ps, CSocketRef* psr){ pSocket=ps; next=psr;}
};


class CListenSocket : public CSocket
{   CSocketRef* m_Clients;
    void OnAccept(int nErrorCode);
  public:
    CServerDlg* m_pDlg;
    CListenSocket(CServerDlg* pDlg) : m_pDlg(pDlg), m_Clients(0){}
    ~CListenSocket();
};

class CClientSocket : public CSocket
{    int m_beg;
     char m_buf[BUF_SIZE];
     void OnReceive(int);
     CListenSocket* m_pListenSocket; 
  public:
     struct Data m_data;
     double m_result;
     CClientSocket(CListenSocket*);
};