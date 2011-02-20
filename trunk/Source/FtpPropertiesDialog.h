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

// FtpPropertiesDialog.h - interface of the CFtpPropertiesDialog class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__FtpPropertiesDialog_h)
#define __FtpPropertiesDialog_h

//////////////////////////////////////////////////////////////////////////////////////////////
// dependent includes

#include "CustomDialog.h"
#include "ActionPage.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CFtpPropertiesDialog

class CFtpPropertiesDialog: public CCustomDialog
{
	DECLARE_DYNAMIC(CFtpPropertiesDialog)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CFtpPropertiesDialog(BOOL fChangeName, UINT idsCaption = 0, CWnd* pParentWnd = NULL);
	virtual ~CFtpPropertiesDialog(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// message map functions
protected:
	afx_msg void OnButtonFtpRoot(void);

// attributes
public:
	enum
	{
		// length limits
		MIN_FTP_NAME_LENGTH = 1,
		MAX_FTP_NAME_LENGTH = 32,
		MAX_FTP_COMMENT_LENGTH = 128,
		MIN_FTP_SERVER_LENGTH = CActionPage::MIN_FTP_SERVER_LENGTH,
		MAX_FTP_SERVER_LENGTH = CActionPage::MAX_FTP_SERVER_LENGTH,
		MAX_FTP_LOGIN_LENGTH = CActionPage::MAX_FTP_LOGIN_LENGTH,
		MAX_FTP_PASSWORD_LENGTH = CActionPage::MAX_FTP_PASSWORD_LENGTH
	};

	BOOL m_fChangeName;
	UINT m_idsCaption;

	// user's input
	CString m_strName;
	CString m_strComment;
	CString m_strServer;
	INTERNET_PORT m_nPort;
	CString m_strLogin;
	CString m_strPassword;
	CString m_strRoot;
	BOOL m_fPassive;

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif   // __FtpPropertiesDialog_h

// end of file
