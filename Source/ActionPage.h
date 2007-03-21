// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// ActionPage.h - interface of the CActionPage class

#if !defined(__ActionPage_h)
#define __ActionPage_h

//! Encapsulates the "Choosing an action" step of the UpdateIt! wizard.
class CActionPage: public CBetterPropPage
{
	DECLARE_DYNAMIC(CActionPage)
	DECLARE_MESSAGE_MAP()

// construction
public:
	CActionPage(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
	virtual BOOL OnSetActive(void);
	virtual BOOL OnKillActive(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// message map functions
protected:
	afx_msg void OnCheckUpload(void);
	afx_msg void OnCheckZip(void);
	afx_msg void OnCheckSend(void);

// attributes
public:
	int m_nAction;
	int m_nUpload;
	int m_nZip;
	BOOL m_fCanSend;
	int m_nSend;
	CString m_strServer;
	short m_nFtpPort;
	CString m_strLogin;
	CString m_strPassword;
	CString m_strRoot;
	CString m_strFrom;
	CString m_strTo;
	CString m_strSubject;
	CString m_strHost;
	int m_nPort;
	CString m_strBody;

// implementation helpers
private:
	void EnableFtpControls(BOOL fEnable);
	void EnableMailControls(BOOL fEnable);
	void ShowMailControls(BOOL fShow);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __ActionPage_h

// end of file
