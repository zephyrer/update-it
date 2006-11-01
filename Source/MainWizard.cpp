// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
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

// MainWizard.cpp - implementation of the CMainWizard class

#include "stdafx.h"

#include "Resource.h"
#include "BetterPropPage.h"
#include "AboutPage.h"
#include "OptionsPage.h"
#include "FilesList.h"
#include "FilesPage.h"
#include "AuthenticationDialog.h"
#include "ActionPage.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"
#include "UpdateItApp.h"

#if defined(__INTEL_COMPILER)
// remark #171: invalid type conversion
#pragma warning(disable: 171)
#endif	// __INTEL_COMPILER

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CMainWizard, CCustomPropSheet)

// message map
BEGIN_MESSAGE_MAP(CMainWizard, CCustomPropSheet)
	ON_WM_INITMENUPOPUP()
	ON_WM_SYSCOMMAND()
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

	static HYPERLINKCOLORS linkColors =
	{
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
	CString strRestore;
	CString strMinimize;
	CMenu menuTemp;
	CString strNewItem;
	int nInitialDelay, nAutoPopDelay;
	int cxMaxWidth;
	DWORD crTipBk, crTipText;

	// invoke inherited handler
	BOOL fResult = CCustomPropSheet::OnInitDialog();

	// set wizard's icons
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hSmIcon, FALSE);

	// try to obtain localized text for the "Minimize" system command
	HMODULE hUser32 = ::GetModuleHandle(_T("user32"));
	if (menuTemp.Attach(::LoadMenu(hUser32, MAKEINTRESOURCE(16))))
	{
		menuTemp.GetMenuString(SC_RESTORE, strRestore, MF_BYCOMMAND);
		menuTemp.GetMenuString(SC_MINIMIZE, strMinimize, MF_BYCOMMAND);
		::DestroyMenu(menuTemp.Detach());
	}
	if (strMinimize.IsEmpty())
	{
		// probably fuckin' Win9x
		strRestore.LoadString(IDS_SC_RESTORE);
		strMinimize.LoadString(IDS_SC_MINIMIZE);
	}

	// adjust system menu
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	ASSERT_VALID(pSysMenu);
	pSysMenu->InsertMenu(SC_MOVE, MF_BYCOMMAND, SC_RESTORE, strRestore);
	pSysMenu->InsertMenu(SC_CLOSE, MF_BYCOMMAND, SC_MINIMIZE, strMinimize);
	pSysMenu->InsertMenu(SC_CLOSE, MF_BYCOMMAND | MF_SEPARATOR);
	MENUITEMINFO miInfo = { sizeof(miInfo), MIIM_BITMAP };
	miInfo.hbmpItem = HBMMENU_POPUP_RESTORE;
	::SetMenuItemInfo(pSysMenu->GetSafeHmenu(), SC_RESTORE, FALSE, &miInfo);
	miInfo.hbmpItem = HBMMENU_POPUP_MINIMIZE;
	::SetMenuItemInfo(pSysMenu->GetSafeHmenu(), SC_MINIMIZE, FALSE, &miInfo);
	ModifyStyle(0, WS_MINIMIZEBOX);
	OSVERSIONINFO osVerInfo = { sizeof(osVerInfo) };
	GetVersionEx(&osVerInfo);
	if (osVerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && osVerInfo.dwMajorVersion >= 5)
	{
		// Windows 2000/XP
		strNewItem.LoadString(IDS_SC_EXPORT_SETTINGS);
		pSysMenu->InsertMenu(0, MF_BYPOSITION, IDM_SC_EXPORT_SETTINGS, strNewItem);
		strNewItem.LoadString(IDS_SC_IMPORT_SETTINGS);
		pSysMenu->InsertMenu(1, MF_BYPOSITION, IDM_SC_IMPORT_SETTINGS, strNewItem);
		pSysMenu->InsertMenu(2, MF_BYPOSITION | MF_SEPARATOR);
	}

	// customize tool tips
	CWinApp* pApp = AfxGetApp();
	nInitialDelay = pApp->GetProfileInt(_T("Tips"), _T("InitialDelay"), 900);
	nAutoPopDelay = pApp->GetProfileInt(_T("Tips"), _T("AutoPopDelay"), 5000);
	cxMaxWidth = pApp->GetProfileInt(_T("Tips"), _T("MaxWidth"), 300);
	crTipBk = pApp->GetProfileInt(_T("Tips"), _T("BkColor"), ::GetSysColor(COLOR_INFOBK));
	crTipText = pApp->GetProfileInt(_T("Tips"), _T("TextColor"), ::GetSysColor(COLOR_INFOTEXT));
	int cPages = m_pages.GetSize();
	for (int i = 0; i < cPages; ++i)
	{
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

void CMainWizard::OnInitMenuPopup(CMenu* pPopupMenu, UINT uIndex, BOOL fSysMenu)
{
	CCustomPropSheet::OnInitMenuPopup(pPopupMenu, uIndex, fSysMenu);
	if (fSysMenu)
	{
		ASSERT_VALID(pPopupMenu);
		UINT fuEnable = MF_BYCOMMAND | (GetActiveIndex() > 0 ? MF_GRAYED : MF_ENABLED);
		pPopupMenu->EnableMenuItem(IDM_SC_IMPORT_SETTINGS, fuEnable); 
	}
}

void CMainWizard::OnSysCommand(UINT uID, LPARAM lParam)
{
	switch (uID & 0xFFF0)
	{
	case IDM_SC_EXPORT_SETTINGS:
		OnScExportSettings();
		break;
	case IDM_SC_IMPORT_SETTINGS:
		OnScImportSettings();
		break;
	default:
		CCustomPropSheet::OnSysCommand(uID, lParam);
		break;
	}
}

void CMainWizard::OnScExportSettings(void)
{
	CString strFilter;
	CString strTitle;
	CProcessPrivileges processPrivileges;

	enum { fdwFlags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY };
	strFilter.LoadString(IDS_SETTINGS_FILTER);
#if (_MFC_VER < 0x0700)
	CFileDialogEx dlgSaveAs(FALSE, _T("hive"), AfxGetAppName(), fdwFlags, strFilter);
#else
	CFileDialog dlgSaveAs(FALSE, _T("hive"), AfxGetAppName(), fdwFlags, strFilter);
#endif	// _MFC_VER
	strTitle.LoadString(IDS_TITLE_EXPORT);
	dlgSaveAs.m_ofn.lpstrTitle = strTitle;
	if (dlgSaveAs.DoModal() == IDOK)
	{
		BeginWaitCursor();
		CWinApp* pApp = AfxGetApp();
		ASSERT_VALID(pApp);
		processPrivileges[SE_BACKUP_NAME] = SE_PRIVILEGE_ENABLED;
		HKEY hAppKey = pApp->GetAppRegistryKey();
		::RegSaveKey(hAppKey, dlgSaveAs.GetPathName(), NULL);
		::RegCloseKey(hAppKey);
		processPrivileges[SE_BACKUP_NAME] = 0;
		EndWaitCursor();
	}
}

void CMainWizard::OnScImportSettings(void)
{
	CString strFilter;
	CString strTitle;
	CProcessPrivileges processPrivileges;

	enum { fdwFlags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST };
	strFilter.LoadString(IDS_SETTINGS_FILTER);
#if (_MFC_VER < 0x0700)
	CFileDialogEx dlgOpen(TRUE, _T("hive"), AfxGetAppName(), fdwFlags, strFilter);
#else
	CFileDialog dlgOpen(TRUE, _T("hive"), AfxGetAppName(), fdwFlags, strFilter);
#endif	// _MFC_VER
	strTitle.LoadString(IDS_TITLE_IMPORT);
	dlgOpen.m_ofn.lpstrTitle = strTitle;
	if (dlgOpen.DoModal() == IDOK)
	{
		BeginWaitCursor();
		CWinApp* pApp = AfxGetApp();
		ASSERT_VALID(pApp);
		processPrivileges[SE_RESTORE_NAME] = SE_PRIVILEGE_ENABLED;
		HKEY hAppKey = pApp->GetAppRegistryKey();
		::RegRestoreKey(hAppKey, dlgOpen.GetPathName(), REG_FORCE_RESTORE);
		::RegCloseKey(hAppKey);
		processPrivileges[SE_RESTORE_NAME] = 0;
		EndWaitCursor();
	}
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
	try
	{
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
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
