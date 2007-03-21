// UpdateIt! application.
// Copyright (c) 2002-2006 by Elijah Zarezky,
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

#if !defined(__UpdateItApp_h)
#define __UpdateItApp_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

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
#endif	// _MFC_VER

// overridables
public:
	virtual BOOL InitInstance(void);
	virtual int ExitInstance(void);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __UpdateItApp_h

// end of file
