// CrashReporter application.
// Copyright (c) 2009 by Elijah Zarezky,
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

// CrashReporterApp.cpp - implementation of the CCrashReporterApp class

// initially generated by AfxScratch v1.1.4778 on 14.08.2009 at 18:13:40
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "CrashReporterApp.h"
#include "MainDialog.h"
#include "../../Source/Registry.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CCrashReporterApp, CWinApp)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CCrashReporterApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CCrashReporterApp::CCrashReporterApp(void):
m_hLangDLL(NULL)
{
}

CCrashReporterApp::~CCrashReporterApp(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// operations

HICON CCrashReporterApp::LoadSmIcon(LPCTSTR pszResName)
{
	HINSTANCE hInstRes = AfxGetResourceHandle();
	int cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
	int cySmIcon = ::GetSystemMetrics(SM_CYSMICON);
	HANDLE hSmIcon = ::LoadImage(hInstRes, pszResName, IMAGE_ICON, cxSmIcon, cySmIcon, 0);
	
	return (static_cast<HICON>(hSmIcon));
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CCrashReporterApp::InitInstance(void)
{
#if defined(_DEBUG)
	for (int i = 0; i < __argc; i++)
	{
		TRACE(_T("argv[%i] == %s\n"), i, __targv[i]);
	}
#endif   // _DEBUG

	if (__argc < 3)
	{
		return (FALSE);
	}
	else
	{
		m_strUpdateItExe = __targv[1];
		m_strAppDataPath = __targv[2];
	}

	CMainDialog dlgMain;
	m_pMainWnd = &dlgMain;
	dlgMain.DoModal();
	
	return (FALSE);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

void CCrashReporterApp::GetAbsolutePath(LPTSTR pszDest, LPCTSTR pszRelativePath)
{
	if (::PathIsRelative(pszRelativePath))
	{
		TCHAR szOurDir[_MAX_PATH] = { 0 };
		::GetModuleFileName(AfxGetInstanceHandle(), szOurDir, _countof(szOurDir));
		LPTSTR pszLastSlash = _tcsrchr(szOurDir, _T('\\'));
		ASSERT(pszLastSlash != NULL);
		*pszLastSlash = 0;
		TCHAR szTmpPath[_MAX_PATH] = { 0 };
		if (pszRelativePath[0] != _T('.'))
		{
			_tcscpy(szTmpPath, _T(".\\"));
			_tcscat(szTmpPath, pszRelativePath);
			pszRelativePath = szTmpPath;
		}
		::PathCombine(pszDest, szOurDir, pszRelativePath);
	}
	else {
		_tcscpy(pszDest, pszRelativePath);
	}
}

bool CCrashReporterApp::RegQueryLanguagePath(LPCTSTR pszValueName, LPTSTR pszDest, ULONG cchMaxLen)
{
	// precondition
	ASSERT(pszValueName == NULL || (AfxIsValidString(pszValueName) && *pszValueName != 0));
	ASSERT(AfxIsValidString(pszDest));

	*pszDest = 0;
	LONG nError = ERROR_SUCCESS;

	ATL::CRegKey regKeyLangs;
	regKeyLangs.Attach(GetSectionKey(SZ_REGK_LANGUAGES));

	if (static_cast<HKEY>(regKeyLangs) != NULL)
	{
		TCHAR szCurLang[4] = { 0 };
		ULONG cchLangMax = _countof(szCurLang);
		nError = regKeyLangs.QueryStringValue(SZ_REGV_LANGUAGES_CURRENT, szCurLang, &cchLangMax);
		if (nError == ERROR_SUCCESS)
		{
			ATL::CRegKey regKeyCurLang;
			nError = regKeyCurLang.Open(regKeyLangs, szCurLang);
			if (nError == ERROR_SUCCESS)
			{
				nError = regKeyCurLang.QueryStringValue(pszValueName, pszDest, &cchMaxLen);
				regKeyCurLang.Close();
			}
		}
		::RegCloseKey(regKeyLangs.Detach());
	}

	return (nError == ERROR_SUCCESS && _tcslen(pszDest) > 0 /*&& ::PathFileExists(pszDest)*/);
}

#if defined(_DEBUG)

#if (_MSC_VER == 1300)
#define SZ_DEVENV_VER "2002"
#elif (_MSC_VER == 1310)
#define SZ_DEVENV_VER "2003"
#elif (_MSC_VER == 1400)
#define SZ_DEVENV_VER "2005"
#elif (_MSC_VER == 1500)
#define SZ_DEVENV_VER "2008"
#else
#error Unrecognized Development Environment version!
#endif   // _MSC_VER

#endif   // _DEBUG

bool CCrashReporterApp::GetLanguagePath(LPTSTR pszDest)
{
	// precondition
	ASSERT(AfxIsValidString(pszDest));

	TCHAR szTempPath[_MAX_PATH] = { 0 };
	if (RegQueryLanguagePath(SZ_REGV_LANG_DLL, szTempPath))
	{
#if defined(_DEBUG)
		TCHAR szExeDir[_MAX_PATH] = { 0 };
		::GetModuleFileName(AfxGetInstanceHandle(), szExeDir, _countof(szExeDir));
		::PathRemoveFileSpec(szExeDir);
		CString strDebugPath(::PathFindFileName(szTempPath));
		TCHAR szLangDir[_MAX_DIR] = { 0 };
		_tcscpy(szLangDir, strDebugPath);
		::PathRemoveExtension(szLangDir);
		_tcscat(szLangDir, _T("\\Output." SZ_DEVENV_VER "\\x86\\Debug"));
#if defined(_MBCS)
		_tcscat(szLangDir, _T("\\MBCS\\"));
#else
		_tcscat(szLangDir, _T("\\Unicode\\"));
#endif   // _MBCS
		strDebugPath.Insert(0, szLangDir);
		strDebugPath.Insert(0, _T("..\\..\\..\\..\\..\\Languages\\"));
		::PathCombine(pszDest, szExeDir, strDebugPath);
#else
		GetAbsolutePath(pszDest, szTempPath);
#endif   // _DEBUG
		return (true);
	}
	else {
		return (false);
	}
}

bool CCrashReporterApp::GetAfxLanguagePath(LPTSTR pszDest)
{
	// precondition
	ASSERT(AfxIsValidString(pszDest));

	TCHAR szTempPath[_MAX_PATH] = { 0 };
	if (RegQueryLanguagePath(NULL, szTempPath))
	{
#if defined(_DEBUG)
		TCHAR szExeDir[_MAX_PATH] = { 0 };
		::GetModuleFileName(AfxGetInstanceHandle(), szExeDir, _countof(szExeDir));
		::PathRemoveFileSpec(szExeDir);
		CString strRedistPath(::PathFindFileName(szTempPath));
		strRedistPath.Insert(0, _T("..\\..\\..\\..\\..\\Redist\\Microsoft.VC90.MFCLOC"));
		::PathCombine(pszDest, szExeDir, strRedistPath);
#else
		GetAbsolutePath(pszDest, szTempPath);
#endif   // _DEBUG
		return (true);
	}
	else {
		return (false);
	}
}

bool CCrashReporterApp::SetCurrentLanguage(void)
{
	bool fSuccess = false;

	if (m_hLangDLL != NULL)
	{
		::FreeLibrary(m_hLangDLL);
		m_hLangDLL = NULL;
	}

	CString strLangPath;
	if (GetLanguagePath(strLangPath.GetBuffer(_MAX_PATH)))
	{
		strLangPath.ReleaseBuffer();
		m_hLangDLL = ::LoadLibrary(strLangPath);
		if (m_hLangDLL != NULL)
		{
			int iNameStart = strLangPath.ReverseFind(_T('\\'));
			ASSERT(iNameStart > 0);
			++iNameStart;
			int iExtStart = strLangPath.ReverseFind(_T('.'));
			ASSERT(iExtStart > 0);
			CString strLocName = strLangPath.Mid(iNameStart, iExtStart - iNameStart);
			WriteProfileString(SZ_REGK_LOCALE, SZ_REGV_LOCALE_LC_ALL, strLocName);
			fSuccess = true;
		}
	}
	else {
		strLangPath.ReleaseBuffer();
	}

	return (fSuccess);
}

bool CCrashReporterApp::SetCurrentAfxLanguage(void)
{
	bool fSuccess = false;

	TCHAR szAfxLangPath[_MAX_PATH] = { 0 };
	if (GetAfxLanguagePath(szAfxLangPath))
	{
		HINSTANCE hAfxLangDLL = ::LoadLibrary(szAfxLangPath);
		if (hAfxLangDLL != NULL)
		{
			AFX_MODULE_STATE* pState = AfxGetModuleState();
			ASSERT(pState != NULL);
			if (pState->m_appLangDLL != NULL)
			{
				::FreeLibrary(pState->m_appLangDLL);
			}
			pState->m_appLangDLL = hAfxLangDLL;
			fSuccess = true;
		}
	}

	return (fSuccess);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CCrashReporterApp::AssertValid(void) const
{
	// first perform inherited validity check...
	CWinApp::AssertValid();

	// ...and then verify our own state as well
}

void CCrashReporterApp::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		CWinApp::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_hLangDLL = " << m_hLangDLL;
		dumpCtx << "\nm_strUpdateItExe = " << m_strUpdateItExe;
		dumpCtx << "\nm_strAppDataPath = " << m_strAppDataPath;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// the one and only application object

static CCrashReporterApp g_appCrashReporter;

// end of file
