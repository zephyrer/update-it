// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// UpdateItApp.h - interface of the CUpdateItApp class

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
