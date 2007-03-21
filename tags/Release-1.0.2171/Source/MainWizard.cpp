// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// MainWizard.cpp - implementation of the CMainWizard class

#include "stdafx.h"
#include "Resource.h"
#include "BetterPropPage.h"
#include "AboutPage.h"
#include "OptionsPage.h"
#include "FilesList.h"
#include "FilesPage.h"
#include "ActionPage.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"
#include "UpdateItApp.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CMainWizard, CCustomPropSheet)

// message map
BEGIN_MESSAGE_MAP(CMainWizard, CCustomPropSheet)
END_MESSAGE_MAP()

CMainWizard::CMainWizard(void):
CCustomPropSheet(AFX_IDS_APP_TITLE)
{
	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	// assign CRT locale
	_tsetlocale(LC_ALL, pApp->GetProfileString(_T("Locale"), _T("LC_ALL"), _T("English_USA.1252")));

	// load dialog's icons
	m_hIcon = pApp->LoadIcon(IDI_APP_ICON);
	m_hSmIcon = pApp->LoadSmIcon(MAKEINTRESOURCE(IDI_APP_ICON));

	static HYPERLINKCOLORS linkColors = {
		RGB(0, 0, 255),	// default
		RGB(0, 0, 255),	// active
		RGB(0, 0, 255),	// visited
		RGB(255, 0, 0)		// hover
	};
	CHyperLink::SetColors(linkColors);

	AddPage(&m_pageAbout);
	AddPage(&m_pageOptions);
	AddPage(&m_pageFiles);
	AddPage(&m_pageAction);
	AddPage(&m_pageProgress);
	SetWizardMode();
}

CMainWizard::~CMainWizard(void)
{
	::DestroyIcon(m_hSmIcon);
	::DestroyIcon(m_hIcon);
}

BOOL CMainWizard::OnInitDialog(void)
{
	int nInitialDelay, nAutoPopDelay;
	int cxMaxWidth;
	DWORD crTipBk, crTipText;

	// invoke inherited handler
	BOOL fResult = CCustomPropSheet::OnInitDialog();

	// set wizard's icons
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hSmIcon, FALSE);

	// customize tool tips
	CWinApp* pApp = AfxGetApp();
	nInitialDelay = pApp->GetProfileInt(_T("Tips"), _T("InitialDelay"), 900);
	nAutoPopDelay = pApp->GetProfileInt(_T("Tips"), _T("AutoPopDelay"), 5000);
	cxMaxWidth = pApp->GetProfileInt(_T("Tips"), _T("MaxWidth"), 300);
	crTipBk = pApp->GetProfileInt(_T("Tips"), _T("BkColor"), ::GetSysColor(COLOR_INFOBK));
	crTipText = pApp->GetProfileInt(_T("Tips"), _T("TextColor"), ::GetSysColor(COLOR_INFOTEXT));
	int cPages = m_pages.GetSize();
	for (int i = 0; i < cPages; ++i) {
		CBetterPropPage* pPage = reinterpret_cast<CBetterPropPage*>(m_pages[i]);
		CToolTipCtrl& tipWnd = pPage->GetToolTipCtrl();
		tipWnd.SetDelayTime(TTDT_INITIAL, nInitialDelay);
		tipWnd.SetDelayTime(TTDT_AUTOPOP, nAutoPopDelay);
		tipWnd.SetMaxTipWidth(cxMaxWidth);
		tipWnd.SetTipBkColor(crTipBk);
		tipWnd.SetTipTextColor(crTipText);
	}

	// initialized
	return (fResult);
}

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CMainWizard::AssertValid(void) const
{
	// first perform inherited validity check...
	CCustomPropSheet::AssertValid();
	// ...and then verify own state as well
	ASSERT_VALID(&m_pageAbout);
	ASSERT_VALID(&m_pageOptions);
	ASSERT_VALID(&m_pageFiles);
	ASSERT_VALID(&m_pageAction);
	ASSERT_VALID(&m_pageProgress);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CMainWizard::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CCustomPropSheet::Dump(dumpCtx);
		// ...and then dump own unique members
		dumpCtx << "m_hIcon = " << m_hIcon;
		dumpCtx << "\nm_hSmIcon = " << m_hSmIcon;
		dumpCtx << "\nm_pageAbout = " << m_pageAbout;
		dumpCtx << "\nm_pageOptions = " << m_pageOptions;
		dumpCtx << "\nm_pageFiles = " << m_pageFiles;
		dumpCtx << "\nm_pageAction = " << m_pageAction;
		dumpCtx << "\nm_pageProgress = " << m_pageProgress;
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
