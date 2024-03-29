// UpdateIt! application.
// Copyright (c) 2002-2011 by Elijah Zarezky,
// All rights reserved.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// MainWizard.h - interface of the CMainWizard class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__MainWizard_h)
#define __MainWizard_h

//////////////////////////////////////////////////////////////////////////////////////////////
// dependent includes

#include "CustomPropSheet.h"
#include "AboutPage.h"
#include "FirstLaunchPage.h"
#include "OptionsPage.h"
#include "FilesPage.h"
#include "ActionPage.h"
#include "ProgressPage.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CMainWizard

class CMainWizard: public CCustomPropSheet
{
	DECLARE_DYNAMIC(CMainWizard)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CMainWizard(CWnd* pOwnerWnd = NULL);
	virtual ~CMainWizard(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);

// message map functions
protected:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT uIndex, BOOL fSysMenu);
	afx_msg void OnSysCommand(UINT uID, LPARAM lParam);
	afx_msg void OnNcDestroy(void);

// attributes
public:
	enum
	{
		// page indices
		I_ABOUT,
		I_FIRST_LAUNCH,
		I_OPTIONS,
		I_FILES,
		I_ACTION,
		I_PROGRESS,
		NUM_PAGES   // should be the LAST enumerator!
	};

	HICON m_hIcon;
	HICON m_hSmIcon;

	CAboutPage m_pageAbout;
	CFirstLaunchPage m_pageFirstLaunch;
	COptionsPage m_pageOptions;
	CFilesPage m_pageFiles;
	CActionPage m_pageAction;
	CProgressPage m_pageProgress;

	CArray<CString> m_arrLangNames;

// implementation helpers
private:
	void OnScExportSettings(void);
	void OnScImportSettings(void);
	void OnLanguageChange(UINT uMenuID);
	void CheckCurLangMenuItem(void);
	void CheckLangMenuItem(UINT iLangName);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif   // __MainWizard_h

// end of file
