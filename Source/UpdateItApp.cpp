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

// UpdateItApp.cpp - implementation of the CUpdateItApp class

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
#include "FirstLaunchPage.h"
#include "OptionsPage.h"
#include "FilesList.h"
#include "FilesPage.h"
#include "CustomDialog.h"
#include "ZipOptionsDialog.h"
#include "BrowseFtpDialog.h"
#include "AuthenticationDialog.h"
#include "ActionPage.h"
#include "ProgressPageBase.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"
#include "UpdateItApp.h"
#include "../Common/Registry.h"
#include "Arguments.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// misc defines

#define SZ_MUTEX_APP_INST_NAME _T("UpdateIt.Instance.03E2CBEF-1504-45d8-8BD2-79719D32E29F")

//////////////////////////////////////////////////////////////////////////////////////////////
// avoid unwanted ICL warnings

#if defined(__INTEL_COMPILER)
// remark #279: controlling expression is constant
#pragma warning(disable: 279)
// remark #981: operands are evaluated in unspecified order
#pragma warning(disable: 981)
#endif   // __INTEL_COMPILER

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CUpdateItApp, CWinApp)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CUpdateItApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CUpdateItApp::CUpdateItApp(void):
m_hLangDLL(NULL),
m_argsParser(NULL),
m_hMutexAppInst(NULL)
{
	_tzset();

#if defined(UPDATEIT_DETOURED)
	if (RegQueryCatchpit() > 0)
	{
		Detoured();

		(PVOID&)m_pfnLoadLibrary = ::DetourFindFunction("kernel32.dll", STRINGIZE(LoadLibrary));
		(PVOID&)m_pfnLoadLibraryEx = ::DetourFindFunction("kernel32.dll", STRINGIZE(LoadLibraryEx));
		
		DetourTransactionBegin();
		DetourUpdateThread(::GetCurrentThread());
		DetourAttach(reinterpret_cast<PVOID*>(&m_pfnLoadLibrary), &CUpdateItApp::LoadLibrary);
		DetourAttach(reinterpret_cast<PVOID*>(&m_pfnLoadLibraryEx), &CUpdateItApp::LoadLibraryEx);
		DetourTransactionCommit();
	}
#endif   // UPDATEIT_DETOURED
}

CUpdateItApp::~CUpdateItApp(void)
{
#if defined(UPDATEIT_DETOURED)
	if (!IsCatchpitEmpty())
	{
		DetourTransactionBegin();
		DetourUpdateThread(::GetCurrentThread());
		DetourDetach(reinterpret_cast<PVOID*>(&m_pfnLoadLibrary),  &CUpdateItApp::LoadLibrary);
		DetourDetach(reinterpret_cast<PVOID*>(&m_pfnLoadLibraryEx),  &CUpdateItApp::LoadLibraryEx);
		DetourTransactionCommit();
	}
#endif   // UPDATEIT_DETOURED
}

//////////////////////////////////////////////////////////////////////////////////////////////
// operations

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

	try
	{
		BeginWaitCursor();
		if (GetProfileBinary(pszSection, pszEntry, &pbTemp, &cbPassword))
		{
			// password was previously saved
			arrEncrPwd.SetSize(cbPassword);
			memcpy(arrEncrPwd.GetData(), pbTemp, cbPassword);
			delete[] pbTemp;
			CWinCrypto winCrypto(AfxGetAppName());
			winCrypto.DecryptString(arrEncrPwd, &bstrDecrPwd);
			strResult = _W2T(bstrDecrPwd);
		}
		else
		{
			// wasn't saved - use default value
			strResult = pszDefault;
		}
		EndWaitCursor();
	}
	catch (CWin32Error* pErr)
	{
		EndWaitCursor();
		pErr->ReportError(MB_ICONSTOP | MB_OK);
		pErr->Delete();
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

	try
	{
		BeginWaitCursor();
		CWinCrypto winCrypto(AfxGetAppName());
		BSTR bstrTemp = ::SysAllocString(_T2W(pszValue));
		winCrypto.EncryptString(bstrTemp, arrEncPwd);
		WriteProfileBinary(pszSection, pszEntry, arrEncPwd.GetData(), arrEncPwd.GetSize());
		::SysFreeString(bstrTemp);
		EndWaitCursor();
		fSuccess = TRUE;
	}
	catch (CWin32Error* pErr)
	{
		EndWaitCursor();
		pErr->ReportError(MB_ICONSTOP | MB_OK);
		pErr->Delete();
		fSuccess = FALSE;
	}
	return (fSuccess);
}

#if (_MFC_VER >= 0x0700)

__time64_t CUpdateItApp::GetProfileTime(LPCTSTR pszSection, LPCTSTR pszEntry, __time64_t timeDefault)
{
	__time64_t* pTimeValue;
	UINT cbSize;

	if (GetProfileBinary(pszSection, pszEntry, reinterpret_cast<BYTE**>(&pTimeValue), &cbSize))
	{
		ASSERT(cbSize == sizeof(timeDefault));
		timeDefault = *pTimeValue;
		delete[] reinterpret_cast<BYTE*>(pTimeValue);
	}
	return (timeDefault);
}

BOOL CUpdateItApp::WriteProfileTime(LPCTSTR pszSection, LPCTSTR pszEntry, __time64_t timeValue)
{
	return (WriteProfileBinary(pszSection, pszEntry, reinterpret_cast<BYTE*>(&timeValue), sizeof(timeValue)));
}

#endif   // _MFC_VER

CString CUpdateItApp::GetConfigString(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszDefault)
{
	// precondition
	ASSERT(AfxIsValidString(pszArgName));
	ASSERT(AfxIsValidString(pszSection));
	ASSERT(AfxIsValidString(pszEntry));

	if (!m_argsParser.HasKey(pszArgName))
	{
		return (GetProfileString(pszSection, pszEntry, pszDefault));
	}
	else
	{
		return (m_argsParser.GetStringValue(pszArgName));
	}
}

int CUpdateItApp::GetConfigInt(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, int nDefault)
{
	// precondition
	ASSERT(AfxIsValidString(pszArgName));
	ASSERT(AfxIsValidString(pszSection));
	ASSERT(AfxIsValidString(pszEntry));

	int nReturn = 0;

	if (!m_argsParser.GetIntValue(pszArgName, nReturn, 10))
	{
		nReturn = static_cast<int>(GetProfileInt(pszSection, pszEntry, nDefault));
	}

	return (nReturn);
}

UINT CUpdateItApp::GetConfigUInt(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, UINT uDefault)
{
	// precondition
	ASSERT(AfxIsValidString(pszArgName));
	ASSERT(AfxIsValidString(pszSection));
	ASSERT(AfxIsValidString(pszEntry));

	UINT uReturn = 0;

	if (!m_argsParser.GetUIntValue(pszArgName, uReturn, 10))
	{
		uReturn = GetProfileInt(pszSection, pszEntry, uDefault);
	}

	return (uReturn);
}

int CUpdateItApp::GetConfigCheck(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, int nDefault)
{
	// precondition
	ASSERT(AfxIsValidString(pszArgName));
	ASSERT(AfxIsValidString(pszSection));
	ASSERT(AfxIsValidString(pszEntry));

	int nCheck = 0;

	if (!m_argsParser.HasKey(pszArgName))
	{
		nCheck = GetProfileInt(pszSection, pszEntry, nDefault);
	}
	else
	{
		nCheck = BST_CHECKED;
	}
	if (nCheck != BST_UNCHECKED && nCheck != BST_CHECKED)
	{
		nCheck = nDefault;
	}

	return (nCheck);
}

CString CUpdateItApp::GetConfigPassword(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszDefault)
{
	// precondition
	ASSERT(AfxIsValidString(pszArgName));
	ASSERT(AfxIsValidString(pszSection));
	ASSERT(AfxIsValidString(pszEntry));

	if (!m_argsParser.HasKey(pszArgName))
	{
		return (GetProfilePassword(pszSection, pszEntry, pszDefault));
	}
	else
	{
		return (m_argsParser.GetStringValue(pszArgName));
	}
}

BOOL CUpdateItApp::GetConfigBool(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, BOOL fDefault)
{
	// precondition
	ASSERT(AfxIsValidString(pszArgName));
	ASSERT(AfxIsValidString(pszSection));
	ASSERT(AfxIsValidString(pszEntry));

	if (!m_argsParser.HasKey(pszArgName))
	{
		return (GetProfileInt(pszSection, pszEntry, fDefault));
	}
	else
	{
		return (TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

#include <ntverp.h>

//-----------------------------------------------------------------------
// Windows SDK version     |  2003  |  v5.0  |  v6.0A |  v7.0  |  v7.1  |
//-------------------------+--------+--------+--------+--------+--------+
// VER_PRODUCTBUILD        |   3668 |   3790 |   6000 |   7600 |   7600 |
// VER_PRODUCTBUILD_QFE    |      0 |   2075 |  16384 |  16385 |  16385 |
// VER_PRODUCTMAJORVERSION |      5 |      5 |      6 |      6 |      6 |
// VER_PRODUCTMINORVERSION |      2 |      2 |      0 |      1 |      1 |
// VER_PRODUCTVERSION_W    | 0x0502 | 0x0502 | 0x0600 | 0x0601 | 0x0601 |
//-----------------------------------------------------------------------

#if !defined(VER_PRODUCTBUILD) || (VER_PRODUCTBUILD < 7600)
#error Windows SDK version 7.0 or greater required to compile this code
#endif   // VER_PRODUCTBUILD

#include <sdkddkver.h>

// Why are the generic version numbers called NTDDI?
//
// In my earlier discussion on the variety of symbols that describe the target Windows version, I pointed 
// out that the NTDDI symbols attempt to cut through the mess and consolidate everything into a single symbol. 
// But why the name NTDDI?
// One of my colleagues contacted me privately with the story. When setting out to change the operating system
// version number, my colleague was shocked to find so many different version number mechanisms were scattered
// throughout the various Windows header files. It so happened that the DDK people were already in the process
// of cleaning up the version number mess and were using NTDDI as their version number system. Seeing no reason
// to invent a new different system for user mode, my colleague proposed using the DDK system in the SDK and 
// asked if anybody had any better ideas.
// Nobody came up with any better ideas, no compelling reason why we should have two different version number
// systems, so the NTDDI name stuck. And it stands for NT Device Driver Interface.
//
// http://blogs.msdn.com/b/oldnewthing/archive/2008/12/05/9177689.aspx

class CWinVer
{
public:
	__declspec(noinline) CWinVer(void):
	m_wWinNT(0),
	m_dwNTDDI(0)
	{
		OSVERSIONINFO osVerInfo = { 0 };
		osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);
		::GetVersionEx(&osVerInfo);
		m_wWinNT = MAKEWORD(LOBYTE(LOWORD(osVerInfo.dwMinorVersion)), LOBYTE(LOWORD(osVerInfo.dwMajorVersion)));
		m_dwNTDDI = MAKELONG(0, m_wWinNT);

		ATL::CRegKey regKeyWindows;
		LONG nError = regKeyWindows.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Windows"));
		if (nError == ERROR_SUCCESS)
		{
			DWORD dwCSDVersion = 0;
			nError = regKeyWindows.QueryDWORDValue(_T("CSDVersion"), dwCSDVersion);
			if (nError == ERROR_SUCCESS)
			{
				m_dwNTDDI |= LOWORD(dwCSDVersion);
			}
			regKeyWindows.Close();
		}
	}

	WORD WinNT(void) const
	{
		// compare with _WIN32_WINNT version constants
		return (m_wWinNT);
	}

	DWORD NTDDI(void) const
	{
		// compare with NTDDI version constants
		return (m_dwNTDDI);
	}

	__declspec(noinline) DWORD TrueWinNT(void) const
	{
		OSVERSIONINFOEX osVerInfo = { 0 };
		osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);
		::GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osVerInfo));

		DWORD fdwProductType = PRODUCT_UNDEFINED;
		BOOL (WINAPI* pfnGetProductInfo)(DWORD, DWORD, DWORD, DWORD, DWORD*) = NULL;
		HMODULE hKernel32 = ::GetModuleHandle(_T("kernel32.dll"));
		(FARPROC&)pfnGetProductInfo = ::GetProcAddress(hKernel32, "GetProductInfo");
		if (pfnGetProductInfo != NULL)
		{
			pfnGetProductInfo(osVerInfo.dwMajorVersion, osVerInfo.dwMinorVersion, 0, 0, &fdwProductType);
		}

		DWORD fdwTrueVer = MAKELONG(0, m_wWinNT);
		
		if (osVerInfo.wProductType != VER_NT_WORKSTATION)
		{
			fdwTrueVer |= EDITION_SERVER;
		}
		else if (fdwProductType == PRODUCT_STARTER)
		{
			fdwTrueVer |= EDITION_STARTER;
		}
		else if ((osVerInfo.wSuiteMask & VER_SUITE_PERSONAL) != 0)
		{
			fdwTrueVer |= EDITION_HOME;
		}
		else
		{
			fdwTrueVer |= EDITION_WORKSTATION;
		}

		// just in case of Windows Server 2003 R2
		if (m_wWinNT == _WIN32_WINNT_WS03 && ::GetSystemMetrics(SM_SERVERR2) > 0)
		{
			fdwTrueVer |= 0x0100;
		}

		fdwTrueVer |= (LOBYTE(osVerInfo.wServicePackMajor));

		return (fdwTrueVer);
	}

public:
	enum
	{
		MASK_VERMAJOR = 0xFF000000,
		MASK_VERMINOR = 0x00FF0000,
		MASK_WIN32_WINNT = MASK_VERMAJOR | MASK_VERMINOR,
		MASK_EDITION = 0x0000FF00,
		MASK_OSVER = MASK_WIN32_WINNT | MASK_EDITION,
		MASK_SPVER = 0x000000FF,

		EDITION_STARTER = 0x1000,
		EDITION_HOME = 0x2000,
		EDITION_WORKSTATION = 0x4000,
		EDITION_SERVER = 0x8000,

		NT4_WORKSTATION = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_NT4),
		NT4_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_NT4),
		WIN2K_PROFESSIONAL = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_WIN2K),
		WIN2K_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_WIN2K),
		WINXP_HOME = MAKELONG(EDITION_HOME, _WIN32_WINNT_WINXP),
		WINXP_PROFESSIONAL = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_WINXP),
		WS03_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_WS03),
		WS03R2_SERVER = MAKELONG(EDITION_SERVER | 0x0100, _WIN32_WINNT_WS03),
		VISTA_STARTER = MAKELONG(EDITION_STARTER, _WIN32_WINNT_VISTA),
		VISTA_HOME = MAKELONG(EDITION_HOME, _WIN32_WINNT_VISTA),
		VISTA_BUSINESS = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_VISTA),
		WS08_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_VISTA),
		WIN7_STARTER = MAKELONG(EDITION_STARTER, _WIN32_WINNT_WIN7),
		WIN7_HOME = MAKELONG(EDITION_HOME, _WIN32_WINNT_WIN7),
		WIN7_PROFESSIONAL = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_WIN7),
		WS08R2_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_WIN7)
	};

private:
	WORD m_wWinNT;
	DWORD m_dwNTDDI;
};

// Sample usage:
//
// if (CWinVer().WinNT() < _WIN32_WINNT_VISTA)
// {
//   ...
// }
//
// if (CWinVer().NTDDI() == NTDDI_VISTASP2)
// {
//   ...
// }

BOOL CUpdateItApp::InitInstance(void)
{
	if (CWinVer().TrueWinNT() > CWinVer::WINXP_HOME)
	{
		::MessageBeep(MB_OK);
	}

	m_argsParser.Parse(m_lpCmdLine, true);

	if (m_argsParser.HasKey(SZ_ARG_RESPONSE_FILE))
	{
		ParseResponseFile();
	}

	SetRegistryKey(_T("Elijah Zarezky"));

	SetCurrentAfxLanguage();
	SetCurrentLanguage();

	m_hMutexAppInst = ::CreateMutex(NULL, TRUE, SZ_MUTEX_APP_INST_NAME);
	if (m_hMutexAppInst == NULL)
	{
		AfxMessageBox(IDS_APP_INIT_FAILED, MB_OK | MB_ICONSTOP);
		return (FALSE);
	}
	else if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if (AfxMessageBox(IDS_OTHER_APP_INSTANCE, MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			::CloseHandle(m_hMutexAppInst);
			return (FALSE);
		}
	}

	::InitCommonControls();
	VERIFY(SUCCEEDED(::CoInitialize(NULL)));
	VERIFY(AfxSocketInit());

	FreeImage_Initialise(TRUE);

	CWnd ownerWindow;
	CString strClassName(AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW));
	CString strCaption(MAKEINTRESOURCE(AFX_IDS_APP_TITLE));
	RECT rcOwner = { 0 };
	ownerWindow.CreateEx(0, strClassName, strCaption, WS_POPUP, rcOwner, NULL, 0);
	ownerWindow.ShowWindow(SW_SHOWNORMAL);

	HICON hIcon = LoadIcon(IDI_APP_ICON);
	HICON hSmIcon = LoadSmIcon(MAKEINTRESOURCE(IDI_APP_ICON));
	ownerWindow.SetIcon(hIcon, TRUE);
	ownerWindow.SetIcon(hSmIcon, FALSE);

	do
	{
		SetCurrentAfxLanguage();
		SetCurrentLanguage();

		CMainWizard wizMain(&ownerWindow);
		m_pMainWnd = &ownerWindow;
		wizMain.DoModal();
	}
	while (g_fRestartInterface);

	ownerWindow.DestroyWindow();

	::DestroyIcon(hIcon);
	::DestroyIcon(hSmIcon);

	FreeImage_DeInitialise();

	::CoUninitialize();

	::CloseHandle(m_hMutexAppInst);
	m_hMutexAppInst = NULL;

	return (FALSE);
}

int CUpdateItApp::ExitInstance(void)
{
	return (__super::ExitInstance());
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

void CUpdateItApp::GetAbsolutePath(LPTSTR pszDest, LPCTSTR pszRelativePath)
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
	else
	{
		_tcscpy(pszDest, pszRelativePath);
	}
}

bool CUpdateItApp::RegQueryLanguagePath(LPCTSTR pszValueName, LPTSTR pszDest, ULONG cchMaxLen)
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
#define SZ_DEVENV_VER _T("2002")
#elif (_MSC_VER == 1310)
#define SZ_DEVENV_VER _T("2003")
#elif (_MSC_VER == 1400)
#define SZ_DEVENV_VER _T("2005")
#elif (_MSC_VER == 1500)
#define SZ_DEVENV_VER _T("2008")
#else
#error Unrecognized Development Environment version!
#endif   // _MSC_VER

#endif   // _DEBUG

bool CUpdateItApp::GetLanguagePath(LPTSTR pszDest)
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
		_tcscat(szLangDir, _T("\\Output.") SZ_DEVENV_VER _T("\\x86\\Debug"));
#if defined(_MBCS)
		_tcscat(szLangDir, _T("\\MBCS\\"));
#else
		_tcscat(szLangDir, _T("\\Unicode\\"));
#endif   // _MBCS
		strDebugPath.Insert(0, szLangDir);
#if defined(UPDATE_IT_PRO)
		strDebugPath.Insert(0, _T("..\\..\\..\\..\\..\\..\\UpdateIt\\Languages\\"));
#else
		strDebugPath.Insert(0, _T("..\\..\\..\\..\\Languages\\"));
#endif   // UPDATE_IT_PRO
		::PathCombine(pszDest, szExeDir, strDebugPath);
#else
		GetAbsolutePath(pszDest, szTempPath);
#endif   // _DEBUG
		return (true);
	}
	else
	{
		return (false);
	}
}

bool CUpdateItApp::GetAfxLanguagePath(LPTSTR pszDest)
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
		strRedistPath.Insert(0, _T("..\\..\\..\\..\\Redist\\Microsoft.VC90.MFCLOC\\"));
		::PathCombine(pszDest, szExeDir, strRedistPath);
#else
		GetAbsolutePath(pszDest, szTempPath);
#endif   // _DEBUG
		return (true);
	}
	else
	{
		return (false);
	}
}

bool CUpdateItApp::SetCurrentLanguage(void)
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
	else
	{
		strLangPath.ReleaseBuffer();
	}

	return (fSuccess);
}

bool CUpdateItApp::SetCurrentAfxLanguage(void)
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

bool CUpdateItApp::ParseResponseFile(void)
{
	bool fSuccess = false;

	CString strArgsPath = m_argsParser.GetStringValue(SZ_ARG_RESPONSE_FILE);
	if (!strArgsPath.IsEmpty())
	{
		if (::PathFindFileName(strArgsPath) == static_cast<LPCTSTR>(strArgsPath))
		{
			// only name is specified, e.g. -ArgumentsFile:OtherArguments.txt
			// assume current directory in this case
			strArgsPath.Insert(0, _T(".\\"));
		}

		if (::PathIsRelative(strArgsPath))
		{
			TCHAR szCurDir[_MAX_PATH] = { 0 };
			::GetCurrentDirectory(_countof(szCurDir), szCurDir);
			TCHAR szAbsArgsPath[_MAX_PATH] = { 0 };
			::PathCombine(szAbsArgsPath, szCurDir, strArgsPath);
			strArgsPath = szAbsArgsPath;
		}

		if (::PathFileExists(strArgsPath))
		{
			CStdioFile fileArgs;
			CFileException err;

			if (fileArgs.Open(strArgsPath, CFile::modeRead | CFile::shareExclusive, &err))
			{
				try
				{
					CString strArguments, strCurLine;
					while (fileArgs.ReadString(strCurLine))
					{
						strCurLine.Trim();
						if (!strCurLine.IsEmpty() && strCurLine[0] != _T('#'))
						{
							strArguments += strCurLine;
							strArguments += _T('\x20');
						}
					}
					fileArgs.Close();
					strArguments.TrimRight();
					m_argsParser.Parse(strArguments, true);
					fSuccess = true;
				}
				catch (CFileException* pErr)
				{
					pErr->ReportError(MB_ICONSTOP | MB_OK);
					pErr->Delete();
				}
			}
			else
			{
				err.ReportError(MB_ICONSTOP | MB_OK);
			}
		}
	}

	return (fSuccess);
}

#if defined(UPDATEIT_DETOURED)

CUpdateItApp::PFN_LOAD_LIBRARY CUpdateItApp::m_pfnLoadLibrary(NULL);
CUpdateItApp::PFN_LOAD_LIBRARY_EX CUpdateItApp::m_pfnLoadLibraryEx(NULL);

HMODULE WINAPI CUpdateItApp::LoadLibrary(LPCTSTR pszFileName)
{
	TRACE(_T("*** CUpdateItApp::LoadLibrary(%s)\n"), pszFileName);

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT(pApp != NULL);

	CString strFileNameLower(::PathFindFileName(pszFileName));
	strFileNameLower.MakeLower();

	DWORD fCatch = FALSE;
	if (pApp->m_mapCatchpit.Lookup(strFileNameLower, fCatch) && fCatch != 0)
	{
		::SetLastError(ERROR_FILE_NOT_FOUND);
		return (NULL);
	}
	else
	{
		return (m_pfnLoadLibrary(pszFileName));
	}
}

HMODULE WINAPI CUpdateItApp::LoadLibraryEx(LPCTSTR pszFileName, HANDLE hFile, DWORD fdwFlags)
{
	TRACE(_T("*** CUpdateItApp::LoadLibraryEx(%s, 0x%08X, 0x%08X)\n"), pszFileName, hFile, fdwFlags);

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT(pApp != NULL);

	CString strFileNameLower(::PathFindFileName(pszFileName));
	strFileNameLower.MakeLower();

	DWORD fCatch = FALSE;
	if (pApp->m_mapCatchpit.Lookup(strFileNameLower, fCatch) && fCatch != 0)
	{
		::SetLastError(ERROR_FILE_NOT_FOUND);
		return (NULL);
	}
	else
	{
		return (m_pfnLoadLibraryEx(pszFileName, hFile, fdwFlags));
	}
}

INT_PTR CUpdateItApp::RegQueryCatchpit(void)
{
	m_mapCatchpit.RemoveAll();

	CRegKey regKey;
	regKey.Create(HKEY_CURRENT_USER, _T("Software\\Elijah Zarezky\\UpdateIt!\\Catchpit"));
	
	DWORD cNumValues = 0;
	if (::RegQueryInfoKey(regKey, 0, 0, 0, 0, 0, 0, &cNumValues, 0, 0, 0, 0) == ERROR_SUCCESS)
	{
		for (DWORD i = 0; i < cNumValues; ++i)
		{
			TCHAR szValueName[_MAX_PATH] = { 0 };
			DWORD cchNameLen = _countof(szValueName);
			DWORD fdwValueType = REG_NONE;
			if (::RegEnumValue(regKey, i, szValueName, &cchNameLen, 0, &fdwValueType, 0, 0) == ERROR_SUCCESS)
			{
				if (fdwValueType == REG_DWORD)
				{
					DWORD fCatch = FALSE;
					regKey.QueryDWORDValue(szValueName, fCatch);
					_tcslwr_s(szValueName, cchNameLen + 1);
					m_mapCatchpit.SetAt(szValueName, fCatch);
				}
			}
		}
	}

	return (m_mapCatchpit.GetCount());
}

#endif   // UPDATEIT_DETOURED

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CUpdateItApp::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify own state as well
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CUpdateItApp::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_hLangDLL = " << m_hLangDLL;
		dumpCtx << "\nm_argsParser = " << m_argsParser;
		dumpCtx << "\nm_hMutexAppInst = " << m_hMutexAppInst;
	}
	catch (CFileException* pErr)
	{
		pErr->ReportError();
		pErr->Delete();
	}
}

#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// the one and only application object

static CUpdateItApp g_appUpdateIt;

// end of file
