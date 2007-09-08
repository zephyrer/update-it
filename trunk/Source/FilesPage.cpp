// UpdateIt! application.
// Copyright (c) 2002-2007 by Elijah Zarezky,
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

// FilesPage.cpp - implementation of the CFilesPage class

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"
#include "../Languages/English_USA.1252/Source/Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "BetterPropPage.h"
#include "AboutPage.h"
#include "OptionsPage.h"
#include "FilesList.h"
#include "FilesPage.h"
#include "CustomDialog.h"
#include "AuthenticationDialog.h"
#include "ActionPage.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"
#include "UpdateItApp.h"
#include "Registry.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// unwanted ICL warnings

#if defined(__INTEL_COMPILER)
// remark #171: invalid type conversion
#pragma warning(disable: 171)
// remark #174: expression has no effect
#pragma warning(disable: 174)
// remark #177: variable was declared but never referenced
#pragma warning(disable: 177)
// remark #279: controlling expression is constant
#pragma warning(disable: 279)
// remark #797: a class-qualified name is not allowed
#pragma warning(disable: 797)
// remark #981: operands are evaluated in unspecified order
#pragma warning(disable: 981)
#endif	// __INTEL_COMPILER

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CFilesPage, CBetterPropPage)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CFilesPage, CBetterPropPage)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILES, OnItemChanged)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CFilesPage::CFilesPage(void):
CBetterPropPage(IDD_PAGE_FILES),
m_fShowGrid(FALSE),
m_cbFiles(0),
m_iDefIcon(-1)
{
	m_psp.dwFlags |= PSP_PREMATURE;

	int cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
	int cySmIcon = ::GetSystemMetrics(SM_CYSMICON);
	m_imageList.Create(cxSmIcon, cySmIcon, ILC_COLOR16 | ILC_MASK, 8, 8);

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	m_fShowGrid = pApp->GetProfileInt(SZ_REGK_FILES, SZ_REGV_FILES_SHOW_GRID, FALSE);

	HICON hIcon = pApp->LoadSmIcon(MAKEINTRESOURCE(IDI_APP_ICON));
	m_iDefIcon = m_imageList.Add(hIcon);
	::DestroyIcon(hIcon);
}

CFilesPage::~CFilesPage(void)
{
	m_imageList.DeleteImageList();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CFilesPage::OnInitDialog(void)
{
	CRect rectList;
	CString strHeading;

	// invoke inherited handler
	BOOL fResult = __super::OnInitDialog();

	// setup the file list...
	m_listFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT | (m_fShowGrid ? LVS_EX_GRIDLINES : 0));
	m_listFiles.SetImageList(&m_imageList, LVSIL_SMALL);
	// ...and then insert columns in the list
	m_listFiles.InsertColumns();

	// assign tool tips
	CToolTipCtrl& tipWnd = GetToolTipCtrl();
	tipWnd.AddTool(GetDlgItem(IDC_LIST_FILES));
	tipWnd.AddTool(GetDlgItem(IDC_BUTTON_REMOVE));
	tipWnd.Activate(TRUE);

	// initialized
	return (fResult);
}

BOOL CFilesPage::OnSetActive(void)
{
	BOOL fSuccess = __super::OnSetActive();
	if (fSuccess)
	{
		if (m_listFiles.GetItemCount() == 0)
		{
			m_listFiles.ShowWindow(SW_HIDE);
			m_buttonRemove.ShowWindow(SW_HIDE);
		}
	}
	return (fSuccess);
}

void CFilesPage::OnBecameActive(void)
{
	using CMainWizard::I_OPTIONS;
	using CFilesList::I_NAME;
	using CFilesList::NUM_COLUMNS;
	using CFilesList::I_DATE;
	using CSortingListCtrl::SORT_ASCENDING;

	CString strInfo;

	__super::OnBecameActive();
	if (m_listFiles.GetItemCount() == 0)
	{
		// prepare controls
		CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
		ASSERT(pWiz != NULL);
		pWiz->SendMessage(DM_SETDEFID, IDCANCEL, 0);
		pWiz->GetDlgItem(ID_WIZNEXT)->EnableWindow(FALSE);
		pWiz->GetDlgItem(IDCANCEL)->SetFocus();

		// search for files
		m_listExclude.RemoveAll();
		COptionsPage* pOptionsPage = DYNAMIC_DOWNCAST(COptionsPage, pWiz->GetPage(I_OPTIONS));
		ASSERT(pOptionsPage != NULL);
		BOOL fMemTrack = AfxEnableMemoryTracking(FALSE);
		LPTSTR pszExcludes = _tcsdup(pOptionsPage->m_strExclude);
		LPTSTR pszSpec = _tcstok(pszExcludes, _T(","));
		while (pszSpec != NULL)
		{
			m_listExclude.AddTail(pszSpec);
			pszSpec = _tcstok(NULL, _T(","));
		}
		free(pszExcludes);
		AfxEnableMemoryTracking(fMemTrack);
		m_imageList.SetImageCount(0);
		m_mapIcons.RemoveAll();
		BOOL fRecurse = pOptionsPage->m_nRecurse == BST_CHECKED;
		SearchForFiles(pOptionsPage->m_strSource, fRecurse, pOptionsPage->m_timeWrite, -1);

		// setup and restore controls
		int cItems = m_listFiles.GetItemCount();
		if (cItems > 0)
		{
			pWiz->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
			strInfo.Format(IDS_FOUND_FORMAT, cItems);
			for (int iCol = I_NAME; iCol < NUM_COLUMNS; ++iCol)
			{
				m_listFiles.SetColumnWidth(iCol, LVSCW_AUTOSIZE);
			}
			m_listFiles.SortItems(I_DATE, SORT_ASCENDING);
			m_listFiles.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, (UINT)-1);
			m_listFiles.EnableWindow();
			m_buttonRemove.EnableWindow();
			m_listFiles.SetFocus();
		}
		else {
			pWiz->SetWizardButtons(PSWIZB_BACK);
			strInfo.LoadString(IDS_NO_FILES);
			m_listFiles.EnableWindow(FALSE);
			m_buttonRemove.EnableWindow(FALSE);
		}
		m_textInfo.SetWindowText(strInfo);
		m_listFiles.ShowWindow(SW_SHOW);
		m_buttonRemove.ShowWindow(SW_SHOW);
	}
}

LRESULT CFilesPage::OnWizardBack(void)
{
	CString strInfo;

	strInfo.LoadString(IDS_DISCARDING_SEARCH);
	m_textInfo.SetWindowText(strInfo);
	CleanupFileList();
	return (__super::OnWizardBack());
}

void CFilesPage::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_textInfo);
	DDX_Control(pDX, IDC_LIST_FILES, m_listFiles);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonRemove);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

void CFilesPage::OnDestroy(void)
{
	CleanupFileList();
	__super::OnDestroy();
}

void CFilesPage::OnItemChanged(NMHDR* /*pHdr*/, LRESULT* pnResult)
{
	m_buttonRemove.EnableWindow(m_listFiles.GetSelectedCount() > 0);
	*pnResult = 0;
}

void CFilesPage::OnButtonRemove(void)
{
	int iFocus;
	int iItem;
	DWORD dwFocusData;
	LVFINDINFO lvfi;
	CString strInfo;

	// search for the item that will be focused after
	int cItems = m_listFiles.GetItemCount();
#pragma warning(disable: 4018)
	if (m_listFiles.GetSelectedCount() < cItems)
#pragma warning(default: 4018)
	{
		iFocus = m_listFiles.GetNextItem(-1, LVNI_FOCUSED);
		// search forward
		while (iFocus < cItems && (m_listFiles.GetItemState(iFocus, (UINT)-1) & LVIS_SELECTED) != 0)
		{
			++iFocus;
		}
		if (iFocus == cItems)
		{
			iFocus = m_listFiles.GetNextItem(-1, LVNI_FOCUSED);
			// search backward
			while (iFocus > 0 && (m_listFiles.GetItemState(iFocus, (UINT)-1) & LVIS_SELECTED) != 0)
			{
				--iFocus;
			}
		}
		dwFocusData = m_listFiles.GetItemData(iFocus);
	}
	else {
		// due to avoid C4701 warning
		dwFocusData = NULL;
	}

	// remove all selected items
	while ((iItem = m_listFiles.GetNextItem(-1, LVNI_SELECTED)) != -1)
	{
		FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(m_listFiles.GetItemData(iItem));
		ASSERT(pData != NULL);
		m_cbFiles -= pData->cbLength;
		delete pData;
		m_listFiles.DeleteItem(iItem);
		--cItems;
	}

	// adjust controls
	if (cItems > 0)
	{
		// there is still some items
		ASSERT(dwFocusData != NULL);
		memset(&lvfi, 0, sizeof(lvfi));
		lvfi.flags = LVFI_PARAM;
		lvfi.lParam = dwFocusData;
		iFocus = m_listFiles.FindItem(&lvfi);
		ASSERT(iFocus != -1);
		m_listFiles.SetItemState(iFocus, LVIS_FOCUSED | LVIS_SELECTED, (UINT)-1);
		m_listFiles.SetFocus();
		strInfo.Format(IDS_SELECTED_FORMAT, cItems);
	}
	else {
		// no more items
		CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
		ASSERT(pWiz != NULL);
		pWiz->SetWizardButtons(PSWIZB_BACK);
		m_listFiles.EnableWindow(FALSE);
		m_buttonRemove.EnableWindow(FALSE);
		strInfo.LoadString(IDS_NO_SELECTED);
	}
	m_textInfo.SetWindowText(strInfo);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

BOOL CFilesPage::IsFileMatchesExcludeList(LPCTSTR pszFilePath)
{
	BOOL fMatch;
	POSITION pos;

	ASSERT(AfxIsValidString(pszFilePath));

	for (fMatch = FALSE, pos = m_listExclude.GetHeadPosition(); !fMatch && pos != NULL;)
	{
		fMatch = ::PathMatchSpec(pszFilePath, m_listExclude.GetNext(pos));
	}
	return (fMatch);
}

void CFilesPage::SearchForFiles(LPCTSTR pszFolder, BOOL fRecurse, CTime timeMin, int iRelative)
{
	using CFilesList::I_EXTENSION;
	using CFilesList::NUM_COLUMNS;

	LVITEM lvi;
	CFileFind finder;
	CTime timeWrite;
	CString strFolder;
	SHFILEINFO shfi;

	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.pszText = LPSTR_TEXTCALLBACK;

	strFolder.Format(IDS_FOLDER_FORMAT, pszFolder);
	m_textInfo.SetWindowText(strFolder);
	CString strWildcard(pszFolder);
	if (strWildcard[strWildcard.GetLength() - 1] != _T('\\'))
	{
		strWildcard += _T('\\');
	}
	if (iRelative < 0)
	{
		iRelative = strWildcard.GetLength();
	}
	strWildcard += _T("*.*");

	BOOL fStop = !finder.FindFile(strWildcard);
	while (!fStop)
	{
		fStop = !finder.FindNextFile();
		if (!finder.IsDots())	// skip "." and ".."
		{
			if (finder.IsDirectory())
			{
				// search recursively (if needed)
				if (fRecurse)
				{
					SearchForFiles(finder.GetFilePath(), fRecurse, timeMin, iRelative);
				}
			}
			else if (!IsFileMatchesExcludeList(finder.GetFilePath()))
			{
				// obtain and compare last write time
				finder.GetLastWriteTime(timeWrite);
				if (timeWrite >= timeMin && !CompareContents(finder.GetFilePath().Mid(iRelative)))
				{
					// obtain system icon for that file
					CString strExt(::PathFindExtension(finder.GetFilePath()));
					if (!m_mapIcons.Lookup(strExt, lvi.iImage))
					{
						memset(&shfi, 0, sizeof(shfi));
						enum { fuFlags = SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON };
						::SHGetFileInfo(strExt, FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), fuFlags);
						if (shfi.hIcon != NULL)
						{
							lvi.iImage = m_imageList.Add(shfi.hIcon);
							::DestroyIcon(shfi.hIcon);
							m_mapIcons.SetAt(strExt, lvi.iImage);
						}
						else {
							lvi.iImage = m_iDefIcon;
						}
					}
					
					FILE_DATA* pData = new FILE_DATA;
					
					// name
					CString strNameExt = finder.GetFileName();
					int iLastDot = strNameExt.ReverseFind(_T('.'));
					_tcscpy(pData->szName, strNameExt.Left(iLastDot));
					
					// extension
					_tcscpy(pData->szExt, strNameExt.Mid(iLastDot + 1));
					
					// relative path
					CString strPath = finder.GetFilePath();
					int iLastSlash = strPath.ReverseFind(_T('\\'));
					_tcscpy(pData->szFolder, strPath.Mid(iRelative, iLastSlash - iRelative));
					
					// date/time
					pData->timeWrite = timeWrite;
					
					// size
					pData->cbLength = finder.GetLength();
#if (_MFC_VER < 0x0700)
					m_cbFiles += pData->cbLength;
					
					// insert an item
					lvi.lParam = reinterpret_cast<LPARAM>(pData);
					VERIFY(m_listFiles.InsertItem(&lvi) == lvi.iItem);
					for (int i = I_EXTENSION; i < NUM_COLUMNS; ++i)
					{
						m_listFiles.SetItemText(lvi.iItem, i, LPSTR_TEXTCALLBACK);
					}
					++lvi.iItem;
#else
// in case to handle possible total size overflow
					if (_UI64_MAX - pData->cbLength < m_cbFiles)
					{
						// overflow - stop the search and clean-up
						fStop = TRUE;
						delete pData;
						AfxMessageBox(IDS_TOTAL_SIZE_OVERFLOW, MB_ICONWARNING | MB_OK);
					}
					else {
						m_cbFiles += pData->cbLength;
						// insert an item
						lvi.lParam = reinterpret_cast<LPARAM>(pData);
						VERIFY(m_listFiles.InsertItem(&lvi) == lvi.iItem);
						for (int i = I_EXTENSION; i < NUM_COLUMNS; ++i) {
							m_listFiles.SetItemText(lvi.iItem, i, LPSTR_TEXTCALLBACK);
						}
						++lvi.iItem;
					}
#endif	// _MFC_VER
				}
			}
		}
		
		// pump waiting messages (if any)
		PumpWaitingMessages();
	}
	finder.Close();
}

void CFilesPage::CleanupFileList(void)
{
	int cItems = m_listFiles.GetItemCount();
	for (int i = 0; i < cItems; ++i)
	{
		FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(m_listFiles.GetItemData(i));
		ASSERT(pData != NULL);
		delete pData;
	}
	m_listFiles.DeleteAllItems();
	m_cbFiles = 0;
}

BOOL CFilesPage::CompareContents(LPCTSTR pszRelativeName)
{
	using CMainWizard::I_OPTIONS;

	BOOL fResult;
	CMemMapFile mmfPrev;
	CMemMapFile mmfNew;

	ASSERT(AfxIsValidString(pszRelativeName));

	static COptionsPage* pOptionsPage = NULL;
	if (pOptionsPage == NULL)
	{
		CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
		ASSERT_VALID(pWiz);
		pOptionsPage = DYNAMIC_DOWNCAST(COptionsPage, pWiz->GetPage(I_OPTIONS));
		ASSERT_VALID(pOptionsPage);
	}

	if (pOptionsPage->m_fCompare)
	{
		CString strPrevFile(pOptionsPage->m_strTarget);
		strPrevFile += _T('\\');
		strPrevFile += pszRelativeName;
		if (::PathFileExists(strPrevFile))
		{
			CString strNewFile(pOptionsPage->m_strSource);
			strNewFile += _T('\\');
			strNewFile += pszRelativeName;
			try
			{
				void* pvPrevData = mmfPrev.Create(strPrevFile, FALSE);
				void* pvNewData = mmfNew.Create(strNewFile, FALSE);
				if (pvPrevData == NULL || pvNewData == NULL)
				{
					// one or both files has zero length
					fResult = FALSE;
				}
				else if (mmfPrev.GetLength() != mmfNew.GetLength())
				{
					// don't need to really compare contents
					fResult = FALSE;
				}
				else {
					fResult = memcmp(pvPrevData, pvNewData, mmfPrev.GetLength()) == 0;
				}
				mmfNew.Close();
				mmfPrev.Close();
			}
			catch (CWin32Error* pXcpt)
			{
				// unable to compare
				delete pXcpt;
				fResult = FALSE;
			}
		}
		else {
			// no previous verison
			fResult = FALSE;
		}
	}
	else {
		// assume that files aren't equal
		fResult = FALSE;
	}
	return (fResult);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CFilesPage::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify own state as well
	ASSERT_VALID(&m_textInfo);
	ASSERT_VALID(&m_listFiles);
	ASSERT_VALID(&m_buttonRemove);
	ASSERT_VALID(&m_imageList);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CFilesPage::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_fShowGrid = " << m_fShowGrid;
		dumpCtx << "\nm_textInfo = " << m_textInfo;
		dumpCtx << "\nm_listFiles = " << m_listFiles;
		dumpCtx << "\nm_buttonRemove = " << m_buttonRemove;
		dumpCtx << "\nm_cbFiles = " << m_cbFiles;
		dumpCtx << "\nm_listExclude = " << m_listExclude;
		dumpCtx << "\nm_imageList = " << m_imageList;
		dumpCtx << "\nm_mapIcons = " << m_mapIcons;
		dumpCtx << "\nm_iDefIcon = " << m_iDefIcon;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
