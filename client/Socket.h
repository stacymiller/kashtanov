const int BUF_SIZE=1024; 

class CXLClientDlg;
class CMySocket : public CSocket
{    int m_beg;
     char m_buf[BUF_SIZE];
     void OnReceive(int);
     CXLClientDlg* m_pDlg;
   public:
     double m_result;
	 CString m_pStringResult;
     CMySocket(CXLClientDlg* pDlg);
};