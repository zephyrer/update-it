// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// MainWizard.h - interface of the CMainWizard class

#if !defined(__MainWizard_h)
#define __MainWizard_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

class CMainWizard: public CCustomPropSheet
{
	DECLARE_DYNAMIC(CMainWizard)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CMainWizard(void);
	virtual ~CMainWizard(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);

// message map functions
protected:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT uIndex, BOOL fSysMenu);
	afx_msg void OnSysCommand(UINT uID, LPARAM lParam);

// attributes
private:
	HICON m_hIcon;
	HICON m_hSmIcon;
	enum {
		// page indices
		I_ABOUT,
		I_OPTIONS,
		I_FILES,
		I_ACTION,
		I_PROGRESS,
		NUM_PAGES		// should be the LAST enumerator!
	};
	CAboutPage m_pageAbout;
	COptionsPage m_pageOptions;
	CFilesPage m_pageFiles;
	CActionPage m_pageAction;
	CProgressPage m_pageProgress;

// implementation helpers
private:
	void OnScExportSettings(void);
	void OnScImportSettings(void);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __MainWizard_h

// end of file
