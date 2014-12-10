// XLSink.h: interface for the CXLSink class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CXLClientDlg;

class IXLSink : public IDispatch
{
public:
  IXLSink(CXLClientDlg *_pDlg);
  // IUnknown methods.
  virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject);
  virtual ULONG _stdcall AddRef(void); 
  virtual ULONG _stdcall Release(void); 

  // IDispatch methods.
  virtual HRESULT _stdcall GetTypeInfoCount(UINT *pctinfo) { return E_NOTIMPL; };
  virtual HRESULT _stdcall GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) { return E_NOTIMPL; };
  virtual HRESULT _stdcall GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) { return E_NOTIMPL; };

  virtual HRESULT _stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
    DISPPARAMS *pDispParams, VARIANT *pVarResult,
    EXCEPINFO *pExcepInfo, UINT *puArgErr);
private:
  CXLClientDlg *m_pDlg;
  ULONG m_RefCount;
};
