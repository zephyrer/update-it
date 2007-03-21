// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

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
