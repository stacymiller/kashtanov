// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__07501CB4_A4EC_4B69_8579_11949B3BD480__INCLUDED_)
#define AFX_SERVERDLG_H__07501CB4_A4EC_4B69_8579_11949B3BD480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct Data
{ double a;
  double b;
}; 


/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog
class CClientSocket;
class CListenSocket;

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor
    CListenSocket* m_pListenSocket;
    void CalcResult(CClientSocket*);

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__07501CB4_A4EC_4B69_8579_11949B3BD480__INCLUDED_)
