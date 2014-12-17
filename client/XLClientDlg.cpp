// XLClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include <comdef.h>
#include <direct.h>
#include <stdlib.h>


#include "excel9.h"
#include "socket.h"
#include "XLSink.h"
#include "XLClient.h"
#include "XLClientDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXLClientDlg dialog

IMPLEMENT_DYNAMIC(CXLClientDlg, CDialog);

CXLClientDlg::CXLClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXLClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXLClientDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSocket=new CMySocket(this);	
}

CXLClientDlg::~CXLClientDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	delete m_pSocket;
}

void CXLClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXLClientDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXLClientDlg, CDialog)
	//{{AFX_MSG_MAP(CXLClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXLClientDlg message handlers

char* XLArg(int row, int col)
{ static char StrArg[20],
ABC[26]= {'A','B','C','D','E','F','G','H','I','J','K','L','M',
'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
static const int n1=26, n2=n1*n1;

col--;
if (col<n1)
{ StrArg[0]=ABC[col];
StrArg[1]=0;
}
else if(col<n2)
{ StrArg[0]=ABC[col/n1-1];
StrArg[1]=ABC[col%n1];
StrArg[2]=0;
}
else
{ StrArg[0]=ABC[col/n2-1];
StrArg[1]=ABC[col%n2/n1];
StrArg[2]=ABC[col%n1];
StrArg[3]=0;
}

char buffer[10]="";
strcat (StrArg,_ltoa(row, buffer, 10));
return(StrArg);
}

/////////////////////////////////////////////////////////////
VARIANT noVar;

BOOL CXLClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	noVar.vt = VT_ERROR;
	noVar.scode = DISP_E_PARAMNOTFOUND;

	if (m_app) m_app.ReleaseDispatch();
	if (!m_app.CreateDispatch("Excel.Application"))
	{ AfxMessageBox("Can't open Excel!");
	return FALSE;
	}
	m_app.SetVisible(TRUE);
	ShowWindow(SW_MINIMIZE);
	m_wbs = m_app.GetWorkbooks();

	char dir[256];
	_getcwd( dir, 256);
	m_file=dir;
	m_file+="\\xlclient.xls";  

	try
	{  
		m_wbs.Open(m_file, noVar, noVar, noVar, noVar, noVar, noVar, noVar, noVar, noVar, noVar, noVar, noVar);

		IConnectionPointContainer *pConnPointCont;
		HRESULT hr = m_app.m_lpDispatch->QueryInterface(__uuidof(IConnectionPointContainer),
			reinterpret_cast<void **>(&pConnPointCont));
		if (FAILED(hr)) 
		{ AfxMessageBox("Can't query IConnectionPointContainer!");
		return FALSE;
		}

		static const GUID IID_IExcelAppEvents = {0x00024413,0x000,0x0000,{0xc0,0x00,0x0,0x00,0x00,0x00,0x00,0x46}};
		hr = pConnPointCont->FindConnectionPoint(IID_IExcelAppEvents, &m_pConnPoint);
		if (FAILED(hr))
		{ AfxMessageBox("Can't FindConnectionPoint IExcelAppEvents!");
		pConnPointCont->Release();
		return FALSE;
		}
		IXLSink *pSink = new IXLSink(this);
		if (FAILED(hr = m_pConnPoint->Advise(pSink, &m_dwCookie)))
		{ AfxMessageBox("Advise failed!");
		m_pConnPoint->Release();
		m_pConnPoint = 0;
		pConnPointCont->Release();
		return FALSE;
		}
		pConnPointCont->Release();

		m_wb = m_wbs.GetItem(_variant_t((short)1));

		Worksheets ws = m_wb.GetWorksheets();
		_Worksheet sh = ws.GetItem(_variant_t((short)1));
		COleVariant A1("A1");
		Range rgServer=sh.GetRange(A1,A1),
			rgPort=rgServer.GetOffset(_variant_t((short)0),_variant_t((short)1));

		m_server=(char*)_bstr_t(rgServer.GetValue());
		m_port=(long)variant_t(rgPort.GetValue());

	}
	catch (COleDispatchException* pException)
	{
		AfxMessageBox(pException->m_strDescription);
		pException->Delete();
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXLClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXLClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CXLClientDlg::SendQuery(Range rg)
{ 

	struct{
		int N;
		int columns;
		double cementationTime;
		double cementationTimedelta;
		double hardeningTime;
		double hardeningTimedelta;
	} data;

	m_rg=rg;
	
	if (rg.GetColumn() == 1 && rg.GetRow() == 8){
		data.N=((Range)(rg.GetOffset(COleVariant(-5l),COleVariant(1l)))).GetValue().dblVal;
		data.columns=((Range)(rg.GetOffset(COleVariant(-4l),COleVariant(1l)))).GetValue().dblVal;
		data.cementationTime = ((Range)(rg.GetOffset(COleVariant(-3l),COleVariant(1l)))).GetValue().dblVal;
		data.cementationTimedelta = ((Range)(rg.GetOffset(COleVariant(-3l),COleVariant(4l)))).GetValue().dblVal;
		data.hardeningTime = ((Range)(rg.GetOffset(COleVariant(-2l),COleVariant(1l)))).GetValue().dblVal;
		data.hardeningTimedelta = ((Range)(rg.GetOffset(COleVariant(-2l),COleVariant(4l)))).GetValue().dblVal;
		m_pSocket->Close();
		m_pSocket->Create();
		if(m_pSocket->Connect(m_server, m_port))
			m_pSocket->Send(&data, sizeof(data));
	}
}


void CXLClientDlg::SetResult()
{
	//m_rg.SetValue(COleVariant(m_pSocket->m_pStringResult));
	CString param = m_pSocket->m_pStringResult;
	CString curResult;
	int curPos = 0;
	int min, max, sectors=0;
	curResult = param.Tokenize(_T(":"),curPos);
	while (curResult != "histogram"){
		if (curResult == "min"){
			CString minString = param.Tokenize(_T("\n"),curPos);
			min = atoi(minString);
		} else if (curResult == "max"){
			CString maxString = param.Tokenize(_T("\n"),curPos);
			max = atoi(maxString);
		} else if (curResult == "sectors"){
			CString sectorsString = param.Tokenize(_T("\n"),curPos);
			sectors = atoi(sectorsString);
		}
		if (curPos >= 0){
			curResult = param.Tokenize(_T(":"),curPos);
		} else {
			return; //received data does not have appropriate format
		}
	}

	if (curResult == "histogram"){
		for (int i = 0; i < sectors; i++){
			Range place=(Range)(m_rg.GetOffset(COleVariant((long)(i + 1)),COleVariant(0l)));
			curResult = param.Tokenize(_T(",:"),curPos);
			long value = (long)atoi(curResult);
			place.SetValue(COleVariant(value));
		}
	}
	curResult = param.Tokenize(_T(",:"),curPos);
	curResult.Trim();

	if (curResult == "averages"){
		for (int i = 0; i < sectors; i++){
			Range place=(Range)(m_rg.GetOffset(COleVariant((long)(i+1)),COleVariant(1l)));
			curResult = param.Tokenize(_T(",:"),curPos);
			long value = (long)atoi(curResult);
			place.SetValue(COleVariant(value));
		}
	}

	long const xlLocationAsObject=2,
		xlColumnClustered=51,
		xlColumns=2;
	_Worksheet ws=m_app.GetActiveSheet(); 
	((Worksheets)m_app.GetCharts()).Add(noVar, noVar, noVar, noVar);
	CString dataStart = XLArg(m_rg.GetRow()+1,m_rg.GetColumn()),
		dataEnd = XLArg(m_rg.GetRow() + sectors,m_rg.GetColumn()),
		averagesStart = XLArg(m_rg.GetRow()+1,m_rg.GetColumn() + 1),
		averagesEnd = XLArg(m_rg.GetRow() + sectors,m_rg.GetColumn() + 1);
	Range data = ws.GetRange(COleVariant(dataStart),COleVariant(dataEnd)),
		averages = ws.GetRange(COleVariant(averagesStart),COleVariant(averagesEnd));
	_Chart ActiveChart=m_app.GetActiveChart();
	ActiveChart.SetChartType(xlColumnClustered);
	ActiveChart.SetSourceData(data, COleVariant(xlColumns));
	((Series)ActiveChart.SeriesCollection(COleVariant(1l))).SetXValues(averages.GetValue());
	ActiveChart.Location(xlLocationAsObject, COleVariant("Sheet1"));
}

void CXLClientDlg::XLClose()
{ 
	if (m_pConnPoint) 
	{ HRESULT hr = m_pConnPoint->Unadvise(m_dwCookie);
	m_pConnPoint->Release();
	}
	if (m_app) 
	{ //if (m_wb) m_wb.Save();
		m_app.Quit();
		m_app.ReleaseDispatch();
	}
}


void CXLClientDlg::OnCancel() 
{	XLClose();
CDialog::OnCancel();
}