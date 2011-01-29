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

// ActionPage.h - interface of the CActionPage class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

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
	afx_msg void OnButtonZipOptions(void);
	afx_msg void OnButtonFtpRoot(void);
	afx_msg void OnButtonAuthentication(void);

// attributes
public:
	enum
	{
		// action to perform
		COPY_FILES = 0,
		MOVE_FILES = 1,

		// length limits
		MIN_FTP_SERVER_LENGTH = sizeof("aa.aa") - 1,
		MAX_FTP_SERVER_LENGTH = INTERNET_MAX_URL_LENGTH,
		MAX_FTP_LOGIN_LENGTH = 64,
		MAX_FTP_PASSWORD_LENGTH = 64,
		MIN_EMAIL_ADDRESS_LENGTH = sizeof("a@a.aa") - 1,
		MAX_EMAIL_ADDRESS_LENGTH = _MAX_PATH,
		MAX_EMAIL_SUBJECT_LENGTH = 80,
		MAX_EMAIL_BODY_LENGTH = 1024,
		MIN_SMTP_HOST_LENGTH = sizeof("aa.aa") - 1,
		MAX_SMTP_HOST_LENGTH = INTERNET_MAX_HOST_NAME_LENGTH
	};

	// action settings
	int m_nAction;
	int m_nUpload;
	int m_nZip;
	BOOL m_fCanSend;
	int m_nSend;
	// FTP settings
	CString m_strFtpServer;
	INTERNET_PORT m_nFtpPort;
	CString m_strFtpLogin;
	CString m_strFtpPassword;
	CString m_strFtpRoot;
	BOOL m_fPassive;
	// E-Mail settings
	CString m_strMailFrom;
	CString m_strMailTo;
	CString m_strMailSubject;
	CString m_strSmtpHost;
	INTERNET_PORT m_nSmtpPort;
	CString m_strMailBody;
	// helper dialogs
	CZipOptionsDialog m_dlgZipOpts;
	CBrowseFtpDialog m_dlgBrowseFtp;
	CAuthenticationDialog m_dlgAuth;

// implementation helpers
private:
	void InitActionSettings(class CUpdateItApp* pApp);
	void InitFtpSettings(class CUpdateItApp* pApp);
	void InitSmtpSettings(class CUpdateItApp* pApp);
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

#endif   // __ActionPage_h

// end of file
