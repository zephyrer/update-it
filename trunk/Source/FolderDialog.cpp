// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// FolderDialog.cpp - implementation of the CFolderDialog class

#include "stdafx.h"
#include "FolderDialog.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CFolderDialog, CDialog)

CFolderDialog::CFolderDialog(LPCTSTR pszTitle, CWnd* pParentWnd):
CDialog(),
m_strTitle(pszTitle), m_pidl(NULL)
{
	m_pParentWnd = pParentWnd;
	memset(&m_bi, 0, sizeof(m_bi));
	// allocate a buffer
	m_bi.pszDisplayName = new TCHAR[_MAX_PATH];
}

CFolderDialog::CFolderDialog(UINT idTitle, CWnd* pParentWnd):
CDialog(),
m_pidl(NULL)
{
	m_pParentWnd = pParentWnd;
	memset(&m_bi, 0, sizeof(m_bi));
	// allocate a buffer
	m_bi.pszDisplayName = new TCHAR[_MAX_PATH];
	// set prompt
	m_strTitle.LoadString(idTitle);
}

CFolderDialog::~CFolderDialog(void)
{
	delete[] m_bi.pszDisplayName;
	m_hWnd = NULL;
}

int CFolderDialog::DoModal(void)
{
	// assign owner window
	m_bi.hwndOwner = m_pParentWnd != NULL ? *m_pParentWnd : *AfxGetMainWnd();
	// prepare options
	m_bi.lpszTitle = m_strTitle;
	m_bi.ulFlags |= BIF_RETURNONLYFSDIRS;
#if defined(_MSC_VER)
	if (_winmajor >= 5) {
#else
	if (LOWORD(::GetVersion()) >= 5) {
#endif
		m_bi.ulFlags |= 0x0040;		// BIF_NEWDIALOGSTYLE
	}
	m_bi.lpfn = BrowseProc;
	m_bi.lParam = reinterpret_cast<LPARAM>(this);
	// display the dialog
	m_pidl = ::SHBrowseForFolder(&m_bi);
	// return to the caller
	return (m_pidl != NULL ? IDOK : IDCANCEL);
}

void CFolderDialog::EnableOK(BOOL fEnable)
{
	SendMessage(BFFM_ENABLEOK, 0, fEnable);
}

void CFolderDialog::SetSelection(ITEMIDLIST* pidl)
{
	SendMessage(BFFM_SETSELECTION, FALSE, reinterpret_cast<LONG>(pidl));
}

void CFolderDialog::SetSelection(LPCTSTR pszPath)
{
	SendMessage(BFFM_SETSELECTION, TRUE, reinterpret_cast<LONG>(pszPath));
}

void CFolderDialog::SetStatusText(LPCTSTR pszText)
{
	SendMessage(BFFM_SETSTATUSTEXT, 0, reinterpret_cast<LONG>(pszText));
}

CString CFolderDialog::GetFolderPath(void) const
{
	CString strFolder;
	HRESULT hr;
	STRRET srName;
	IShellFolder* pshf;
	IMalloc* pMalloc;

	if (m_pidl != NULL) {		// user has made the choice
		memset(&srName, 0, sizeof(srName));
		// try to obtain pointer to the IShellFolder interface
		hr = ::SHGetDesktopFolder(&pshf);
		if (SUCCEEDED(hr)) {
			// try to retrieve the display name for the PIDL
			hr = pshf->GetDisplayNameOf(m_pidl, SHGDN_FORPARSING, &srName);
			if (SUCCEEDED(hr)) {
				LPTSTR pszBuf = strFolder.GetBuffer(_MAX_PATH);
				if (srName.uType == STRRET_CSTR) {		// returned ANSI string
#if defined(_UNICODE)
					int nSize = ::lstrlenA(srName.cStr) + 1;
					LPWSTR pwszTemp = new WCHAR[nSize];
					::MultiByteToWideChar(CP_ACP, 0, srName.cStr, -1, pwszTemp, nSize);
					::lstrcpy(pszBuf, pwszTemp);
					delete[] pwszTemp;
#else
					::lstrcpy(pszBuf, srName.cStr);
#endif	// _UNICODE
				}
				else if (srName.uType == STRRET_WSTR) {	// returned Unicode string
#if defined(_UNICODE)
					::lstrcpy(pszBuf, srName.pOleStr);
#else
					int nSize = ::lstrlenW(srName.pOleStr) + 1;
					char* pszTemp = new char[nSize];
					::WideCharToMultiByte(CP_ACP, 0, srName.pOleStr, -1, pszTemp, nSize, NULL, NULL);
					::lstrcpy(pszBuf, pszTemp);
					delete[] pszTemp;
#endif	// _UNICODE
					// try to free used system resources
					hr = ::SHGetMalloc(&pMalloc);
					if (SUCCEEDED(hr)) {
						pMalloc->Free(srName.pOleStr);
						pMalloc->Release();
					}
				}
				strFolder.ReleaseBuffer();
			}
			pshf->Release();
		}
	}
	return (strFolder);
}

void CFolderDialog::OnInitialized(void)
{
	CenterWindow();
}

void CFolderDialog::OnSelChanged(const ITEMIDLIST* /*pidl*/, const CString& /*strPath*/)
{
}

BOOL CFolderDialog::OnValidateFailed(LPCTSTR /*pszName*/)
{
	return (TRUE);
}

int CALLBACK CFolderDialog::BrowseProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	CString strPath;

	CFolderDialog* pDlg = reinterpret_cast<CFolderDialog*>(lpData);
	ASSERT(pDlg != NULL);
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		pDlg->m_hWnd = hWnd;
		pDlg->OnInitialized();
		return (0);
	case BFFM_SELCHANGED:
		if (::SHGetPathFromIDList(reinterpret_cast<ITEMIDLIST*>(lParam), strPath.GetBuffer(_MAX_PATH))) {
			strPath.ReleaseBuffer();
		}
		else {
			strPath.ReleaseBuffer(0);
		}
		pDlg->OnSelChanged(reinterpret_cast<ITEMIDLIST*>(lParam), strPath);
		return (0);
	case BFFM_VALIDATEFAILED:
		return (pDlg->OnValidateFailed(reinterpret_cast<LPCTSTR>(lParam)));
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
void CFolderDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	CDialog::AssertValid();
	// ...and then verify own state as well
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CFolderDialog::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CDialog::Dump(dumpCtx);
		// ...and then dump own unique members
		dumpCtx << "m_bi.hwndOwner = " << m_bi.hwndOwner;
		dumpCtx << "\nm_bi.pidlRoot = " << m_bi.pidlRoot;
		dumpCtx << "\nm_bi.pszDisplayName = " << m_bi.pszDisplayName;
		dumpCtx << "\nm_bi.lpszTitle = " << m_bi.lpszTitle;
		dumpCtx << "\nm_bi.ulFlags = " << m_bi.ulFlags;
		dumpCtx << "\nm_bi.lpfn = " << m_bi.lpfn;
		dumpCtx << "\nm_bi.lParam = " << m_bi.lParam;
		dumpCtx << "\nm_bi.iImage = " << m_bi.iImage;
		dumpCtx << "\nm_strTitle = " << m_strTitle;
		dumpCtx << "\nm_pidl = " << m_pidl;
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
