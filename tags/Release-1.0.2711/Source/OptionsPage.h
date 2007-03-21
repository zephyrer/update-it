// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// OptionsPage.h - interface of the COptionsPage class

#if !defined(__OptionsPage_h)
#define __OptionsPage_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

//! Encapsulates the "Setting options" step of the UpdateIt! wizard.
class COptionsPage: public CBetterPropPage
{
	DECLARE_DYNAMIC(COptionsPage)
	DECLARE_MESSAGE_MAP()

// construction
public:
	COptionsPage(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
	virtual BOOL OnSetActive(void);
	virtual BOOL OnKillActive(void);
	virtual LRESULT OnWizardNext(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// message map functions
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT uCtlColor);
	afx_msg void OnButtonSource(void);
	afx_msg void OnButtonTarget(void);
	afx_msg void OnCheckCleanup(void);

// attributes
public:
	CString m_strSource;
	int m_nRecurse;
	CString m_strExclude;
	CString m_strTarget;
	int m_nCleanup;
	int m_nRecycle;
	CDateTimeCtrl m_dtpWrite;
	CTime m_timeWrite;
	BOOL m_fCompare;

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __OptionsPage_h

// end of file
