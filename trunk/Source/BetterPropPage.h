// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// BetterPropPage.h - interface of the CBetterPropPage class

#if !defined(__BetterPropPage_h)
#define __BetterPropPage_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

#define BPP_BECAME_ACTIVE (WM_APP + 1)

//! Encapsulates an extended property page with additional notification and tool tips support.
class CBetterPropPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBetterPropPage)
	DECLARE_MESSAGE_MAP()

// construction
public:
	CBetterPropPage(UINT idTemplate, UINT idCaption = 0);
	CBetterPropPage(LPCTSTR pszTemplate, UINT idCaption = 0);

// overridables
public:
	virtual BOOL OnInitDialog(void);
	virtual BOOL OnSetActive(void);
	virtual void OnBecameActive(void);
protected:
	virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// message map functions
protected:
#if (_MFC_VER < 0x0700)
	afx_msg void OnGetTipText(UINT uID, NMTTDISPINFO* pInfo, LRESULT* pnResult);
#else
	afx_msg void OnGetTipText(UINT uID, NMHDR* pHdr, LRESULT* pnResult);
#endif	// _MFC_VER

// accessibility
public:
	CToolTipCtrl& GetToolTipCtrl(void);

// attributes
protected:
	CToolTipCtrl m_tipWnd;
	TCHAR m_szTipText[256];

// implementation helpers
protected:
	static UINT AFX_CDECL ActivationWatcher(void* pvParam);
	void PumpWaitingMessages(void);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __BetterPropPage_h

// end of file
