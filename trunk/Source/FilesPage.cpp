// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// FilesPage.cpp - implementation of the CFilesPage class

#include "stdafx.h"
#include "Resource.h"
#include "BetterPropPage.h"
#include "AboutPage.h"
#include "OptionsPage.h"
#include "FilesPage.h"
#include "ActionPage.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CFilesPage, CBetterPropPage)

// message map
BEGIN_MESSAGE_MAP(CFilesPage, CBetterPropPage)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILES, OnItemChanged)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_FILES, OnGetDispInfo)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FILES, OnColumnClick)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
END_MESSAGE_MAP()

CFilesPage::CFilesPage(void):
CBetterPropPage(IDD_PAGE_FILES),
m_cbFiles(0)
{
	m_psp.dwFlags |= PSP_PREMATURE;
}

BOOL CFilesPage::OnInitDialog(void)
{
	RECT rcList;
	CString strHeading;

	// invoke inherited handler
	BOOL fResult = CBetterPropPage::OnInitDialog();

	// setup the file list...
	m_listFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// ...and then insert columns in the list
	m_listFiles.GetClientRect(&rcList);
	int cxWidth = (rcList.right - rcList.left) / NUM_COLUMNS;
	strHeading.LoadString(IDS_NAME);
	m_listFiles.InsertColumn(I_NAME, strHeading, LVCFMT_LEFT, cxWidth, I_NAME);
	strHeading.LoadString(IDS_EXTENSION);
	m_listFiles.InsertColumn(I_EXTENSION, strHeading, LVCFMT_LEFT, cxWidth, I_EXTENSION);
	strHeading.LoadString(IDS_PATH);
	m_listFiles.InsertColumn(I_PATH, strHeading, LVCFMT_LEFT, cxWidth, I_PATH);
	strHeading.LoadString(IDS_DATE);
	m_listFiles.InsertColumn(I_DATE, strHeading, LVCFMT_LEFT, cxWidth, I_DATE);
	strHeading.LoadString(IDS_TIME);
	m_listFiles.InsertColumn(I_TIME, strHeading, LVCFMT_LEFT, cxWidth, I_TIME);

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
	BOOL fSuccess = CBetterPropPage::OnSetActive();
	if (fSuccess) {
		if (m_listFiles.GetItemCount() == 0) {
			m_listFiles.ShowWindow(SW_HIDE);
			m_buttonRemove.ShowWindow(SW_HIDE);
		}
	}
	return (fSuccess);
}

void CFilesPage::OnBecameActive(void)
{
	using CMainWizard::I_OPTIONS;

	CString strInfo;

	CBetterPropPage::OnBecameActive();
	if (m_listFiles.GetItemCount() == 0) {
		// prepare controls
		CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
		ASSERT(pWiz != NULL);
		pWiz->SendMessage(DM_SETDEFID, IDCANCEL, 0);
		pWiz->GetDlgItem(ID_WIZNEXT)->EnableWindow(FALSE);
		pWiz->GetDlgItem(IDCANCEL)->SetFocus();
		// search for files
		COptionsPage* pOptionsPage = DYNAMIC_DOWNCAST(COptionsPage, pWiz->GetPage(I_OPTIONS));
		ASSERT(pOptionsPage != NULL);
		BOOL fRecurse = pOptionsPage->m_nRecurse == BST_CHECKED;
		SearchForFiles(pOptionsPage->m_strSource, fRecurse, pOptionsPage->m_timeWrite, -1);
		// setup and restore controls
		int cItems = m_listFiles.GetItemCount();
		if (cItems > 0) {
			pWiz->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
			strInfo.Format(IDS_FOUND_FORMAT, cItems);
			for (int iCol = I_NAME; iCol < NUM_COLUMNS; ++iCol) {
				m_listFiles.SetColumnWidth(iCol, LVSCW_AUTOSIZE);
			}
			m_listFiles.SortItems(CompareProc, I_DATE);
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
	return (CBetterPropPage::OnWizardBack());
}

void CFilesPage::DoDataExchange(CDataExchange* pDX)
{
	CBetterPropPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_textInfo);
	DDX_Control(pDX, IDC_LIST_FILES, m_listFiles);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonRemove);
}

void CFilesPage::OnDestroy(void)
{
	CleanupFileList();
	CBetterPropPage::OnDestroy();
}

void CFilesPage::OnItemChanged(NMHDR* /*pHdr*/, LRESULT* pnResult)
{
	m_buttonRemove.EnableWindow(m_listFiles.GetSelectedCount() > 0);
	*pnResult = 0;
}

void CFilesPage::OnGetDispInfo(NMHDR* pHdr, LRESULT* /*pnResult*/)
{
	SYSTEMTIME st;
	CString strFormat;

	LVITEM& lvi = reinterpret_cast<NMLVDISPINFO*>(pHdr)->item;
	if ((lvi.mask & LVIF_TEXT) != 0) {
		FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(m_listFiles.GetItemData(lvi.iItem));
		switch (lvi.iSubItem)
		{
		case I_NAME:
			::lstrcpyn(lvi.pszText, pData->szName, lvi.cchTextMax);
			break;
		case I_EXTENSION:
			::lstrcpyn(lvi.pszText, pData->szExt, lvi.cchTextMax);
			break;
		case I_PATH:
			::lstrcpyn(lvi.pszText, pData->szFolder, lvi.cchTextMax);
			break;
		case I_DATE:
			pData->timeWrite.GetAsSystemTime(st);
			strFormat.LoadString(IDS_DATE_FORMAT);
			::GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, strFormat, lvi.pszText, lvi.cchTextMax);
			break;
		case I_TIME:
			pData->timeWrite.GetAsSystemTime(st);
			strFormat.LoadString(IDS_TIME_FORMAT);
			::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, strFormat, lvi.pszText, lvi.cchTextMax);
			break;
		default:
			*lvi.pszText = 0;
			break;
		}
	}
}

void CFilesPage::OnColumnClick(NMHDR* pHdr, LRESULT* /*pnResult*/)
{
	m_listFiles.SortItems(CompareProc, reinterpret_cast<NMLISTVIEW*>(pHdr)->iSubItem);
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
	if (m_listFiles.GetSelectedCount() < cItems) {
#pragma warning(default: 4018)
		iFocus = m_listFiles.GetNextItem(-1, LVNI_FOCUSED);
		// search forward
		while (iFocus < cItems && (m_listFiles.GetItemState(iFocus, (UINT)-1) & LVIS_SELECTED) != 0) {
			++iFocus;
		}
		if (iFocus == cItems) {
			iFocus = m_listFiles.GetNextItem(-1, LVNI_FOCUSED);
			// search backward
			while (iFocus > 0 && (m_listFiles.GetItemState(iFocus, (UINT)-1) & LVIS_SELECTED) != 0) {
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
	while ((iItem = m_listFiles.GetNextItem(-1, LVNI_SELECTED)) != -1) {
		FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(m_listFiles.GetItemData(iItem));
		ASSERT(pData != NULL);
		m_cbFiles -= pData->cbLength;
		delete pData;
		m_listFiles.DeleteItem(iItem);
		--cItems;
	}

	// adjust controls
	if (cItems > 0) {
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

void CFilesPage::SearchForFiles(LPCTSTR pszFolder, BOOL fRecurse, CTime timeMin, int iRelative)
{
	CFileFind finder;
	CTime timeWrite;
	CString strFolder;
	MSG msg;

	strFolder.Format(IDS_FOLDER_FORMAT, pszFolder);
	m_textInfo.SetWindowText(strFolder);
	CString strWildcard(pszFolder);
	if (strWildcard[strWildcard.GetLength() - 1] != _T('\\')) {
		strWildcard += _T('\\');
	}
	if (iRelative < 0) {
		iRelative = strWildcard.GetLength();
	}
	strWildcard += _T("*.*");
	BOOL fStop = !finder.FindFile(strWildcard);
	while (!fStop) {
		fStop = !finder.FindNextFile();
		if (!finder.IsDots()) {		// skip "." and ".."
			if (finder.IsDirectory()) {
				// search recursively (if needed)
				if (fRecurse) {
					SearchForFiles(finder.GetFilePath(), fRecurse, timeMin, iRelative);
				}
			}
			else {
				finder.GetLastWriteTime(timeWrite);
				if (timeWrite >= timeMin) {
					FILE_DATA* pData = new FILE_DATA;
					// name
					CString strNameExt = finder.GetFileName();
					int iLastDot = strNameExt.ReverseFind(_T('.'));
					::lstrcpy(pData->szName, strNameExt.Left(iLastDot));
					// extension
					::lstrcpy(pData->szExt, strNameExt.Mid(iLastDot + 1));
					// relative path
					CString strPath = finder.GetFilePath();
					int iLastSlash = strPath.ReverseFind(_T('\\'));
					::lstrcpy(pData->szFolder, strPath.Mid(iRelative, iLastSlash - iRelative));
					// date/time
					pData->timeWrite = timeWrite;
					// size
					pData->cbLength = finder.GetLength();
					m_cbFiles += pData->cbLength;
					// insert item
					int iItem = m_listFiles.InsertItem(0, LPSTR_TEXTCALLBACK);
					m_listFiles.SetItemData(iItem, reinterpret_cast<DWORD>(pData));
				}
			}
		}
		// pump waiting messages (if any)
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	finder.Close();
}

void CFilesPage::CleanupFileList(void)
{
	int cItems = m_listFiles.GetItemCount();
	for (int i = 0; i < cItems; ++i) {
		FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(m_listFiles.GetItemData(i));
		ASSERT(pData != NULL);
		delete pData;
	}
	m_listFiles.DeleteAllItems();
	m_cbFiles = 0;
}

int CALLBACK CFilesPage::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM nData)
{
	FILE_DATA* pData1 = reinterpret_cast<FILE_DATA*>(lParam1);
	FILE_DATA* pData2 = reinterpret_cast<FILE_DATA*>(lParam2);
	switch (nData)
	{
	case I_NAME:
		return (::lstrcmpi(pData1->szName, pData2->szName));
	case I_EXTENSION:
		return (::lstrcmpi(pData1->szExt, pData2->szExt));
	case I_PATH:
		return (::lstrcmpi(pData1->szFolder, pData2->szFolder));
	case I_DATE:
	case I_TIME:
		if (pData1->timeWrite < pData2->timeWrite) {
			return (1);
		}
		else if (pData1->timeWrite > pData2->timeWrite) {
			return (-1);
		}
		// fall through
	default:
		return (0);
	}
}

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CFilesPage::AssertValid(void) const
{
	// first perform inherited validity check...
	CBetterPropPage::AssertValid();
	// ...and then verify own state as well
	ASSERT_VALID(&m_textInfo);
	ASSERT_VALID(&m_listFiles);
	ASSERT_VALID(&m_buttonRemove);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CFilesPage::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CBetterPropPage::Dump(dumpCtx);
		// ...and then dump own unique members
		dumpCtx << "m_textInfo = " << m_textInfo;
		dumpCtx << "\nm_listFiles = " << m_listFiles;
		dumpCtx << "\nm_buttonRemove = " << m_buttonRemove;
		dumpCtx << "\nm_cbFiles = " << m_cbFiles;
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
