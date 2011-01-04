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

// UpdateItApp.h - interface of the CUpdateItApp class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__UpdateItApp_h)
#define __UpdateItApp_h

class CUpdateItApp: public CWinApp
{
	DECLARE_DYNAMIC(CUpdateItApp)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CUpdateItApp(void);
	virtual ~CUpdateItApp(void);

// operations
public:
	HICON LoadSmIcon(LPCTSTR pszResName);

	CString GetProfilePassword(LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszDefault = NULL);
	BOOL WriteProfilePassword(LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszValue);
#if (_MFC_VER >= 0x0700)
	__time64_t GetProfileTime(LPCTSTR pszSection, LPCTSTR pszEntry, __time64_t timeDefault = -1);
	BOOL WriteProfileTime(LPCTSTR pszSection, LPCTSTR pszEntry, __time64_t timeValue);
#endif   // _MFC_VER
	template <typename _Return_t>
	_Return_t GetProfileData(LPCTSTR pszSection, LPCTSTR pszEntry, _Return_t retDefault);

	CString GetConfigString(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszDefault = NULL);
	int GetConfigInt(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, int nDefault = 0);
	UINT GetConfigUInt(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, UINT uDefault = 0);
	int GetConfigCheck(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, int nDefault);
	CString GetConfigPassword(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszDefault = NULL);
	BOOL GetConfigBool(LPCTSTR pszArgName, LPCTSTR pszSection, LPCTSTR pszEntry, BOOL fDefault);

// overridables
public:
	virtual BOOL InitInstance(void);
	virtual int ExitInstance(void);

// attributes
public:
	HINSTANCE m_hLangDLL;
	CArgsParser m_argsParser;
	HANDLE m_hMutexAppInst;

// implementation helpers
private:
	void GetAbsolutePath(LPTSTR pszDest, LPCTSTR pszRelativePath);
	bool RegQueryLanguagePath(LPCTSTR pszValueName, LPTSTR pszDest, ULONG cchMaxLen = _MAX_PATH);
	bool GetLanguagePath(LPTSTR pszDest);
	bool GetAfxLanguagePath(LPTSTR pszDest);
	bool SetCurrentLanguage(void);
	bool SetCurrentAfxLanguage(void);
	bool ParseResponseFile(void);

#if defined(UPDATEIT_DETOURED)
	// LoadLibrary[A/W] catcher
	typedef HMODULE(WINAPI* PFN_LOAD_LIBRARY)(LPCTSTR);
	static PFN_LOAD_LIBRARY m_pfnLoadLibrary;
	static HMODULE WINAPI LoadLibrary(LPCTSTR pszFileName);
	// LoadLibraryEx[A/W] catcher
	typedef HMODULE(WINAPI* PFN_LOAD_LIBRARY_EX)(LPCTSTR, HANDLE, DWORD);
	static PFN_LOAD_LIBRARY_EX m_pfnLoadLibraryEx;
	static HMODULE WINAPI LoadLibraryEx(LPCTSTR pszFileName, HANDLE hFile, DWORD fdwFlags);
	// catched DLLs
	CMap<CString, LPCTSTR, DWORD, DWORD> m_mapCatchpit;
	INT_PTR RegQueryCatchpit(void);
	bool IsCatchpitEmpty(void) const;
#endif   // UPDATEIT_DETOURED

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

//////////////////////////////////////////////////////////////////////////////////////////////
// operations

template <typename _Return_t>
_Return_t CUpdateItApp::GetProfileData(LPCTSTR pszSection, LPCTSTR pszEntry, _Return_t retDefault)
{
	return (static_cast<_Return_t>(GetProfileInt(pszSection, pszEntry, retDefault)));
}

//////////////////////////////////////////////////////////////////////////////////////////////
// inlines

inline bool CUpdateItApp::IsCatchpitEmpty(void) const
{
	return (m_mapCatchpit.GetCount() == 0);
}

#endif   // __UpdateItApp_h

// end of file
