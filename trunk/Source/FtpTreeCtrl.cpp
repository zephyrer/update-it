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

// FtpTreeCtrl.cpp - implementation of the CFtpTreeCtrl class

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"
#include "../Languages/English_USA.1252/Source/Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "FtpTreeCtrl.h"
#include "UpdateItApp.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CFtpTreeCtrl, CTreeCtrl)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CFtpTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CFtpTreeCtrl::CFtpTreeCtrl(UINT idcStatusText):
m_ftpSession(_T("UpdateIt/1.0")),
m_idcStatusText(idcStatusText),
m_iFolderImg(-1),
m_iFolderOpenImg(-1),
m_iFtpRootImg(-1)
{
	HICON hFolderIcon = NULL;

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	int cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
	int cySmIcon = ::GetSystemMetrics(SM_CYSMICON);

	HDC hdcScreen = ::GetDC(NULL);
	int nBitsPixel = ::GetDeviceCaps(hdcScreen, BITSPIXEL);
	::ReleaseDC(NULL, hdcScreen);

	m_imageList.Create(cxSmIcon, cySmIcon, nBitsPixel | ILC_MASK, 8, 8);

	hFolderIcon = pApp->LoadSmIcon(MAKEINTRESOURCE(m_idrFolder));
	m_iFolderImg = m_imageList.Add(hFolderIcon);
	hFolderIcon = pApp->LoadSmIcon(MAKEINTRESOURCE(m_idrFolderOpen));
	m_iFolderOpenImg = m_imageList.Add(hFolderIcon);
	hFolderIcon = pApp->LoadSmIcon(MAKEINTRESOURCE(m_idrFtpRoot));
	m_iFtpRootImg = m_imageList.Add(hFolderIcon);
}

CFtpTreeCtrl::~CFtpTreeCtrl(void)
{
	m_imageList.DeleteImageList();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// operations

BOOL CFtpTreeCtrl::Connect(LPCTSTR pszServer, LPCTSTR pszLogin, LPCTSTR pszPassword, INTERNET_PORT nPort, BOOL fPassive)
{
	CFtpConnection* pFtpConn = NULL;

	try
	{
		pFtpConn = m_ftpSession.GetFtpConnection(pszServer, pszLogin, pszPassword, nPort, fPassive);
		ASSERT(pFtpConn != NULL);
	}
	catch (CInternetException* pErr)
	{
		pErr->ReportError(MB_ICONSTOP | MB_OK);
		pErr->Delete();
		return (FALSE);
	}

	m_ptrFtpConn.Attach(pFtpConn);
	return (pFtpConn != NULL);
}

void CFtpTreeCtrl::Disconnect(void)
{
	if (m_ptrFtpConn != NULL)
	{
		m_ptrFtpConn->Close();
		delete m_ptrFtpConn.Detach();
	}
}

HTREEITEM CFtpTreeCtrl::InsertRootItem(LPCTSTR pszRootText)
{
	ASSERT(m_ptrFtpConn != NULL);

	SetImageList(&m_imageList, TVSIL_NORMAL);

	HTREEITEM hRootItem = InsertItem(pszRootText, m_iFtpRootImg, m_iFtpRootImg, TVI_ROOT, TVI_LAST);
	if (hRootItem != NULL)
	{
		SearchForFolders(hRootItem);
	}

	return (hRootItem);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

void CFtpTreeCtrl::OnItemExpanding(NMHDR* pHdr, LRESULT* pnResult)
{
	TVITEM& tviNew = reinterpret_cast<NMTREEVIEW*>(pHdr)->itemNew;

	if (reinterpret_cast<NMTREEVIEW*>(pHdr)->action == TVE_EXPAND)
	{
		GetParent()->GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetParent()->GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		BeginWaitCursor();
		
		HTREEITEM hNextItem = GetChildItem(tviNew.hItem);
		while (hNextItem != NULL)
		{
			DeleteChildItems(hNextItem);
			SearchForFolders(hNextItem);
			hNextItem = GetNextItem(hNextItem, TVGN_NEXT);
		}

		EndWaitCursor();
		GetParent()->GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		GetParent()->GetDlgItem(IDOK)->EnableWindow(TRUE);
	}

	SetStatusText(GetCurrentPath());

	pnResult = FALSE;   // TRUE prevents the list from expanding or collapsing
}

void CFtpTreeCtrl::OnSelChanged(NMHDR* pHdr, LRESULT* /*pnResult*/)
{
	SetStatusText(GetItemPath(reinterpret_cast<NMTREEVIEW*>(pHdr)->itemNew.hItem));
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

void CFtpTreeCtrl::SetStatusText(LPCTSTR pszText)
{
	ASSERT(AfxIsValidString(pszText));

	CWnd* pStatusWnd = GetParent()->GetDlgItem(m_idcStatusText);
	if (pStatusWnd != NULL)
	{
		pStatusWnd->SetWindowText(pszText);
	}
}

void CFtpTreeCtrl::SearchForFolders(HTREEITEM hParentItem)
{
	CString strMask;

	HTREEITEM hItem = hParentItem;
	while (hItem != GetRootItem())
	{
		strMask.Insert(0, GetItemText(hItem));
		strMask.Insert(0, _T('/'));
		hItem = GetParentItem(hItem);
	}

	CString strStatusText;
	strStatusText.Format(IDS_SEARCHING_FTP_FORMAT, static_cast<LPCTSTR>(strMask));
	SetStatusText(strStatusText);

	strMask += _T("/*");

	CFtpFileFind ftpFinder(m_ptrFtpConn);
	BOOL fContinue = ftpFinder.FindFile(strMask);

	while (fContinue)
	{
		fContinue = ftpFinder.FindNextFile();
		if (ftpFinder.IsDirectory() && !ftpFinder.IsDots())
		{
			InsertItem(ftpFinder.GetFileName(), m_iFolderImg, m_iFolderOpenImg, hParentItem, TVI_LAST);
		}
		PumpWaitingMessages();
	}

	ftpFinder.Close();

	SortChildren(hParentItem);
}

CString CFtpTreeCtrl::GetItemPath(HTREEITEM hItem)
{
	CString strItemPath;

	if (hItem == GetRootItem())
	{
		strItemPath = _T("/");
	}
	else if (hItem != NULL)
	{
		while (hItem != GetRootItem())
		{
			strItemPath.Insert(0, GetItemText(hItem));
			strItemPath.Insert(0, _T('/'));
			hItem = GetParentItem(hItem);
		}
	}

	return (strItemPath);
}

void CFtpTreeCtrl::DeleteChildItems(HTREEITEM hParentItem)
{
	HTREEITEM hCurItem = GetChildItem(hParentItem);
	while (hCurItem != NULL)
	{
		HTREEITEM hNextItem = GetNextSiblingItem(hCurItem);
		DeleteItem(hCurItem);
		hCurItem = hNextItem;
	}
}

void CFtpTreeCtrl::PumpWaitingMessages(void)
{
	MSG msg = { 0 };

	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
#if (_MFC_VER < 0x0700)
		AfxGetThread()->PumpMessage();
#else
		AfxPumpMessage();
#endif   // _MFC_VER
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CFtpTreeCtrl::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
	ASSERT_VALID(&m_ftpSession);
	ASSERT_VALID(&m_imageList);
}

void CFtpTreeCtrl::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_ftpSession = " << m_ftpSession;
		dumpCtx << "\nm_idcStatusText = " << m_idcStatusText;
		dumpCtx << "\nm_imageList = " << m_imageList;
		dumpCtx << "\nm_iFolderImg = " << m_iFolderImg;
		dumpCtx << "\nm_iFolderOpenImg = " << m_iFolderOpenImg;
		dumpCtx << "\nm_iFtpRootImg = " << m_iFtpRootImg;
	}
	catch (CFileException* pErr)
	{
		pErr->ReportError();
		pErr->Delete();
	}
}

#endif   // _DEBUG

// end of file
