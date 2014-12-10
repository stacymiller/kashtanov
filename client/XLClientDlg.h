// XLClientDlg.h : header file
//

#if !defined(AFX_XLCLIENTDLG_H__A1596E6E_FB34_450C_A58F_9506E0B20032__INCLUDED_)
#define AFX_XLCLIENTDLG_H__A1596E6E_FB34_450C_A58F_9506E0B20032__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLClientDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CXLClientDlg dialog

class CMySocket;

class CXLClientDlg : public CDialog
{
	DECLARE_DYNAMIC(CXLClientDlg);
	friend class CXLClientDlgAutoProxy;

// Construction
public:
	CXLClientDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CXLClientDlg();

  _Application m_app;
  Workbooks m_wbs;
  _Workbook m_wb;
  Range m_rg;
  IConnectionPoint *m_pConnPoint;
  DWORD m_dwCookie;
  CString m_file;

  CString m_server;
  int m_port;
  CMySocket* m_pSocket; 

  void SendQuery(Range rg);
  void SetResult();
  void XLClose(); 

// Dialog Data
	//{{AFX_DATA(CXLClientDlg)
	enum { IDD = IDD_XLCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXLClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;


	// Generated message map functions
	//{{AFX_MSG(CXLClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

char* XLArg(int row, int col);



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLCLIENTDLG_H__A1596E6E_FB34_450C_A58F_9506E0B20032__INCLUDED_)
