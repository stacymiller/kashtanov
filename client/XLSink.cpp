// XLSink.cpp: implementation of the IXLSink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "excel9.h"
#include "XLSink.h"
#include "XLClient.h"
#include "XLClientDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IXLSink::IXLSink(CXLClientDlg *_pDlg) :
  m_RefCount(0),
  m_pDlg(_pDlg)
{
}

HRESULT IXLSink::QueryInterface(REFIID riid, void **ppvObject) 
{
  if (IsEqualGUID(riid, __uuidof(IUnknown)) || IsEqualGUID(riid, __uuidof(IDispatch))) 
  {
    AddRef();
    *ppvObject = this;
    return S_OK;
  }
  return E_NOINTERFACE;
}

ULONG IXLSink::AddRef() {
  return ++m_RefCount;
}

ULONG IXLSink::Release() {
  if (!(--m_RefCount)) {
    delete this;
    return 0;
  }
  return m_RefCount;
}

HRESULT IXLSink::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
    DISPPARAMS *pDispParams, VARIANT *pVarResult,
    EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
  switch (dispIdMember) 
  {
    case 0x617: //SheetBeforeDoubleClick
    {
      _Worksheet ws = V_DISPATCH(pDispParams->rgvarg);
      Range rg = V_DISPATCH(pDispParams->rgvarg+1);
      m_pDlg->SendQuery(rg);
      *((pDispParams->rgvarg+2)->pboolVal) = (short)0xffff;
      rg.DetachDispatch();
      ws.DetachDispatch();
      break;
    }
    case 0x622: //WorkbookBeforeClose
      PostQuitMessage(0);
  }
  return S_OK;
}
