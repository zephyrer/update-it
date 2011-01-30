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

// BrowseFtpDialog.cpp - implementation of the CBrowseFtpDialog class

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"
#include "../Languages/English_USA.1252/Source/Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "CustomDialog.h"
#include "BrowseFtpDialog.h"
#include "../Common/Registry.h"
#include "Arguments.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// misc defines

// dialog window properties to transfer data between main and finder threads
#define SZ_PROP_SERVER _T("{C9815243-12AA-4ed1-BC55-AAA38D8E6269}")
#define SZ_PROP_LOGIN _T("{9EDC1F0D-F7B1-45c1-94DC-E516401C5635}")
#define SZ_PROP_PASSWORD _T("{EC671C5E-BB35-4a0a-9AA8-DC2B1798B876}")
#define SZ_PROP_PORT _T("{E0A5F0B9-8A7D-4094-BF8C-ABE78D844544}")
#define SZ_PROP_PASSIVE _T("{D7BE9CB6-952F-4d1f-845F-7D2223CBC7A4}")

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CBrowseFtpDialog, CCustomDialog)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CBrowseFtpDialog, CCustomDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CBrowseFtpDialog::CBrowseFtpDialog(CWnd* pParentWnd):
CCustomDialog(IDD_BROWSE_FTP, pParentWnd),
m_nPort(INTERNET_DEFAULT_FTP_PORT),
m_strRoot(_T("/")),
m_fPassive(FALSE)
{
}

CBrowseFtpDialog::~CBrowseFtpDialog(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CBrowseFtpDialog::OnInitDialog(void)
{
	__super::OnInitDialog();

	SetWindowProp(SZ_PROP_SERVER, m_strServer);
	SetWindowProp(SZ_PROP_LOGIN, m_strLogin);
	SetWindowProp(SZ_PROP_PASSWORD, m_strPassword);
	SetWindowProp(SZ_PROP_PORT, MAKEINTRESOURCE(m_nPort));
	SetWindowProp(SZ_PROP_PASSIVE, MAKEINTRESOURCE(m_fPassive));

	AfxBeginThread(&FoldersFinder, GetSafeHwnd());

	return (TRUE);
}

void CBrowseFtpDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

void CBrowseFtpDialog::OnDestroy(void)
{
	RemoveWindowProp(SZ_PROP_SERVER);
	RemoveWindowProp(SZ_PROP_LOGIN);
	RemoveWindowProp(SZ_PROP_PASSWORD);
	RemoveWindowProp(SZ_PROP_PORT);
	RemoveWindowProp(SZ_PROP_PASSIVE);

	__super::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

BOOL CBrowseFtpDialog::SetWindowProp(LPCTSTR pszPropName, LPCTSTR pszPropData)
{
	ASSERT(AfxIsValidString(pszPropName));

	if (IS_INTRESOURCE(pszPropData))
	{
		return (::SetProp(GetSafeHwnd(), pszPropName, const_cast<LPTSTR>(pszPropData)));
	}
	else
	{
		ASSERT(AfxIsValidString(pszPropData));

		HGLOBAL hMemBlock = ::GlobalAlloc(GPTR, (_tcslen(pszPropData) + 1) * sizeof(TCHAR));
		LPTSTR pszDataPtr = reinterpret_cast<LPTSTR>(::GlobalLock(hMemBlock));
		if (pszDataPtr != NULL)
		{
			_tcscpy(pszDataPtr, pszPropData);
			::GlobalUnlock(hMemBlock);
			return (::SetProp(GetSafeHwnd(), pszPropName, hMemBlock));
		}
		else
		{
			::GlobalFree(hMemBlock);
			return (FALSE);
		}
	}
}

void CBrowseFtpDialog::RemoveWindowProp(LPCTSTR pszPropName)
{
	ASSERT(AfxIsValidString(pszPropName));

	HGLOBAL hMemBlock = ::RemoveProp(GetSafeHwnd(), pszPropName);
	if (!IS_INTRESOURCE(hMemBlock))
	{
		::GlobalFree(hMemBlock);
	}
}

BOOL CBrowseFtpDialog::GetWindowProp(HWND hWnd, LPCTSTR pszPropName, CString& strDest)
{
	ASSERT(::IsWindow(hWnd));
	ASSERT(AfxIsValidString(pszPropName));

	HGLOBAL hMemBlock = ::GetProp(hWnd, pszPropName);
	if (hMemBlock == NULL)
	{
		strDest.Empty();
		return (TRUE);
	}
	else if (IS_INTRESOURCE(hMemBlock))
	{
		return (FALSE);
	}
	else
	{
		LPTSTR pszDataPtr = reinterpret_cast<LPTSTR>(::GlobalLock(hMemBlock));
		ASSERT(AfxIsValidString(pszDataPtr));
		strDest = pszDataPtr;
		::GlobalUnlock(hMemBlock);
		return (TRUE);
	}
}

DWORD CBrowseFtpDialog::GetWindowProp(HWND hWnd, LPCTSTR pszPropName)
{
	ASSERT(::IsWindow(hWnd));
	ASSERT(AfxIsValidString(pszPropName));

	return (reinterpret_cast<DWORD>(::GetProp(hWnd, pszPropName)));
}

void CBrowseFtpDialog::SearchForFolders(CFtpConnection* pFtpConn, LPCTSTR pszRoot)
{
	ASSERT(pFtpConn != NULL);
	ASSERT(pszRoot != NULL);

	CFtpFileFind ftpFinder(pFtpConn);
	CString strMask(pszRoot);
	strMask += _T("/*");
	BOOL fContinue = ftpFinder.FindFile(strMask);
	
	while (fContinue)
	{
		fContinue = ftpFinder.FindNextFile();
		if (ftpFinder.IsDirectory())
		{
			TRACE(_T("FTP: %s"), static_cast<LPCTSTR>(ftpFinder.GetFilePath()));
			SearchForFolders(pFtpConn, ftpFinder.GetFilePath());
		}
	}
}

UINT AFX_CDECL CBrowseFtpDialog::FoldersFinder(void* pvParam)
{
	HWND hThisDlg = reinterpret_cast<HWND>(pvParam);
	ASSERT(::IsWindow(hThisDlg));

	CInternetSession ftpSession(_T("UpdateIt/1.0"));
	CFtpConnection* pFtpConn = NULL;

	try
	{
		CString strServer, strLogin, strPassword;
		GetWindowProp(hThisDlg, SZ_PROP_SERVER, strServer);
		GetWindowProp(hThisDlg, SZ_PROP_LOGIN, strLogin);
		GetWindowProp(hThisDlg, SZ_PROP_PASSWORD, strPassword);
		INTERNET_PORT nPort = LOWORD(GetWindowProp(hThisDlg, SZ_PROP_PORT));
		BOOL fPassive = GetWindowProp(hThisDlg, SZ_PROP_PASSIVE);
		
		/*pFtpConn = ftpSession.GetFtpConnection(strServer, strLogin, strPassword, nPort, fPassive);*/
		pFtpConn = ftpSession.GetFtpConnection(_T("ftp.freebsd.org"), NULL, NULL, nPort, fPassive);
		ASSERT(pFtpConn != NULL);
		
		SearchForFolders(pFtpConn, _T(""));
	}
	catch (CInternetException* pErr)
	{
		pErr->ReportError(MB_ICONSTOP | MB_OK);
		pErr->Delete();
	}

	if (pFtpConn != NULL)
	{
		pFtpConn->Close();
		delete pFtpConn;
	}

	return (0);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CBrowseFtpDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
}

void CBrowseFtpDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_strServer = " << m_strServer;
		dumpCtx << "\nm_nPort = " << m_nPort;
		dumpCtx << "\nm_strLogin = " << m_strLogin;
		dumpCtx << "\nm_strPassword = " << m_strPassword;
		dumpCtx << "\nm_strRoot = " << m_strRoot;
		dumpCtx << "\nm_fPassive = " << m_fPassive;
	}
	catch (CFileException* pErr)
	{
		pErr->ReportError();
		pErr->Delete();
	}
}

#endif   // _DEBUG

// end of file
