// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// UpdateItApp.cpp - implementation of the CUpdateItApp class

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
IMPLEMENT_DYNAMIC(CUpdateItApp, CWinApp)

// message map
BEGIN_MESSAGE_MAP(CUpdateItApp, CWinApp)
END_MESSAGE_MAP()

CUpdateItApp::CUpdateItApp(void)
{
	_tzset();
}

CUpdateItApp::~CUpdateItApp(void)
{
}

HICON CUpdateItApp::LoadSmIcon(LPCTSTR pszResName)
{
	HINSTANCE hInstRes = AfxGetResourceHandle();
	int cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
	int cySmIcon = ::GetSystemMetrics(SM_CYSMICON);
	HANDLE hSmIcon = ::LoadImage(hInstRes, pszResName, IMAGE_ICON, cxSmIcon, cySmIcon, 0);
	return (static_cast<HICON>(hSmIcon));
}

CString CUpdateItApp::GetProfilePassword(LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszDefault)
{
	BYTE* pbTemp;
	UINT cbPassword;
	CArray<BYTE, BYTE> arrEncrPwd;
	BSTR bstrDecrPwd;
	CString strResult;

	// precondition
	ASSERT(AfxIsValidString(pszSection));
	ASSERT(AfxIsValidString(pszEntry));

	try {
		BeginWaitCursor();
		if (GetProfileBinary(pszSection, pszEntry, &pbTemp, &cbPassword)) {
			// password was previously saved
			arrEncrPwd.SetSize(cbPassword);
			memcpy(arrEncrPwd.GetData(), pbTemp, cbPassword);
			delete[] pbTemp;
			CWinCrypto winCrypto(AfxGetAppName());
			winCrypto.DecryptString(arrEncrPwd, &bstrDecrPwd);
			strResult = _W2T(bstrDecrPwd);
		}
		else {
			// wasn't saved - use default value
			strResult = pszDefault;
		}
		EndWaitCursor();
	}
	catch (CWin32Error* pXcpt) {
		EndWaitCursor();
		pXcpt->ReportError(MB_ICONSTOP | MB_OK);
		pXcpt->Delete();
		strResult.Empty();
	}
	return (strResult);
}

BOOL CUpdateItApp::WriteProfilePassword(LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszValue)
{
	CArray<BYTE, BYTE> arrEncPwd;
	BOOL fSuccess;

	// precondition
	ASSERT(AfxIsValidString(pszSection));
	ASSERT(AfxIsValidString(pszEntry));
	ASSERT(AfxIsValidString(pszValue));

	try {
		BeginWaitCursor();
		CWinCrypto winCrypto(AfxGetAppName());
		BSTR bstrTemp = ::SysAllocString(_T2W(pszValue));
		winCrypto.EncryptString(bstrTemp, arrEncPwd);
		WriteProfileBinary(pszSection, pszEntry, arrEncPwd.GetData(), arrEncPwd.GetSize());
		::SysFreeString(bstrTemp);
		EndWaitCursor();
		fSuccess = TRUE;
	}
	catch (CWin32Error* pXcpt) {
		EndWaitCursor();
		pXcpt->ReportError(MB_ICONSTOP | MB_OK);
		pXcpt->Delete();
		fSuccess = FALSE;
	}
	return (fSuccess);
}

BOOL CUpdateItApp::InitInstance(void)
{
	::InitCommonControls();
	VERIFY(SUCCEEDED(::CoInitialize(NULL)));
	VERIFY(AfxSocketInit());
	SetRegistryKey(IDS_REGKEY);
	CMainWizard m_wizMain;
	m_pMainWnd = &m_wizMain;
	m_wizMain.DoModal();
	return (FALSE);
}

int CUpdateItApp::ExitInstance(void)
{
	::CoUninitialize();
	return (CWinApp::ExitInstance());
}

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CUpdateItApp::AssertValid(void) const
{
	// first perform inherited validity check...
	CWinApp::AssertValid();
	// ...and then verify own state as well
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CUpdateItApp::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CWinApp::Dump(dumpCtx);
		// ...and then dump own unique members
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// the one and only application object
static CUpdateItApp g_appUpdateIt;

// end of file