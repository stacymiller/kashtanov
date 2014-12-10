; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CXLClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "XLClient.h"
ODLFile=XLClient.odl

ClassCount=3
Class1=CXLClientApp
Class2=CXLClientDlg

ResourceCount=4
Resource2=IDR_MAINFRAME
Class3=CXLClientDlgAutoProxy
Resource3=IDD_XLCLIENT_DIALOG
Resource4=IDD_XLCLIENT_DIALOG (English (U.S.))

[CLS:CXLClientApp]
Type=0
HeaderFile=XLClient.h
ImplementationFile=XLClient.cpp
Filter=N

[CLS:CXLClientDlg]
Type=0
HeaderFile=XLClientDlg.h
ImplementationFile=XLClientDlg.cpp
Filter=D
LastObject=CXLClientDlg
BaseClass=CDialog
VirtualFilter=dWC


[CLS:CXLClientDlgAutoProxy]
Type=0
HeaderFile=DlgProxy.h
ImplementationFile=DlgProxy.cpp
BaseClass=CCmdTarget
Filter=N

[DLG:IDD_XLCLIENT_DIALOG]
Type=1
Class=CXLClientDlg
ControlCount=1
Control1=IDCANCEL,button,1342242816

[DLG:IDD_XLCLIENT_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDCANCEL,button,1342242816

