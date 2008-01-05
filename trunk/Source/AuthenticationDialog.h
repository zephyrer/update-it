// UpdateIt! application.
// Copyright (c) 2002-2008 by Elijah Zarezky,
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

// AuthenticationDialog.h - interface of the CAuthenticationDialog class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__AuthenticationDialog_h)
#define __AuthenticationDialog_h

class CAuthenticationDialog: public CCustomDialog
{
	DECLARE_DYNAMIC(CAuthenticationDialog)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CAuthenticationDialog(CWnd* pParentWnd = NULL);
	virtual ~CAuthenticationDialog(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// message map functions
protected:
	afx_msg void OnComboAuthMethodSelChange(void);
	afx_msg void OnEditUserNameChange(void);
	afx_msg void OnEditPasswordChange(void);

// attributes
public:
	// controls
	CComboBox m_comboAuthMethod;
	CStatic m_textUserNamePrompt;
	CEdit m_editUserName;
	CStatic m_textPasswordPrompt;
	CEdit m_editPassword;
	CButton m_checkUseSSL;

	// user's input
	CSmtpConnection::AuthenticationMethod m_eAuthMethod;
	CString m_strUserName;
	CString m_strPassword;
	BOOL m_fUseSSL;

// implementation helpers
private:
	void UpdateControls(void);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif   // __AuthenticationDialog_h

// end of file
