// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.
// Portions copyright (c) 1996-1998 by Microsoft (KB Q142170).

// CustomPropSheet.h - interface of the CCustomPropSheet class

#if !defined(__CustomPropSheet_h)
#define __CustomPropSheet_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

#define PSM_RESIZE_PAGE (WM_APP + 2)

class CCustomPropSheet: public CPropertySheet
{
	DECLARE_DYNAMIC(CCustomPropSheet)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CCustomPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCustomPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CCustomPropSheet(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pnResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

// message map functions
protected:
	afx_msg LRESULT OnResizePage(WPARAM wParam, LPARAM lParam);

// attributes
protected:
	RECT m_rcPage;
	CFont m_fontPage;

// implementation helpers
protected:
	enum { CDF_CENTER, CDF_TOPLEFT, CDF_NONE };
	void ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag);
	virtual void BuildPropPageArray(void);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __CustomPropSheet_h

// end of file
