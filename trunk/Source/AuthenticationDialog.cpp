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

// AuthenticationDialog.cpp - implementation of the CAuthenticationDialog class

#include "stdafx.h"

#include "Resource.h"
#include "CustomDialog.h"
#include "AuthenticationDialog.h"
#include "BetterPropPage.h"
#include "AboutPage.h"
#include "OptionsPage.h"
#include "FilesList.h"
#include "FilesPage.h"
#include "ActionPage.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"
#include "UpdateItApp.h"
#include "Registry.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CAuthenticationDialog, CCustomDialog)

// message map
BEGIN_MESSAGE_MAP(CAuthenticationDialog, CCustomDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTH_METHOD, OnComboAuthMethodSelChange)
END_MESSAGE_MAP()

// construction/destruction

CAuthenticationDialog::CAuthenticationDialog(CWnd* pParentWnd):
CCustomDialog(IDD_AUTHENTICATION, pParentWnd),
m_eAuthMethod(CSmtpConnection::AUTH_NONE),
m_fUseSSL(FALSE)
{
	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	m_eAuthMethod = static_cast<CSmtpConnection::AuthenticationMethod>(pApp->GetProfileInt(SZ_REGK_SMTP,
		SZ_REGV_SMTP_AUTHENTICATION, CSmtpConnection::AUTH_NONE));
	m_strUserName = pApp->GetProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_USERNAME);
	m_strPassword = pApp->GetProfilePassword(SZ_REGK_SMTP, SZ_REGV_SMTP_PASSWORD);
	m_fUseSSL = pApp->GetProfileInt(SZ_REGK_SMTP, SZ_REGV_SMTP_USE_SSL, FALSE);
}

CAuthenticationDialog::~CAuthenticationDialog(void)
{
}

// overridables

BOOL CAuthenticationDialog::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	OnComboAuthMethodSelChange();

	return (TRUE);
}

void CAuthenticationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_AUTH_METHOD, m_comboAuthMethod);
	DDX_Control(pDX, IDC_STATIC_USERNAME_PROMPT, m_textUserNamePrompt);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_STATIC_PASSWORD_PROMPT, m_textPasswordPrompt);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);

	DDX_CBIndex(pDX, IDC_COMBO_AUTH_METHOD, *reinterpret_cast<int*>(&m_eAuthMethod));
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_CHECK_USE_SSL, m_fUseSSL);
}

// message map functions

void CAuthenticationDialog::OnComboAuthMethodSelChange(void)
{
	using CSmtpConnection::AUTH_NONE;

	int iCurSel = m_comboAuthMethod.GetCurSel();
	m_textUserNamePrompt.EnableWindow(iCurSel != AUTH_NONE);
	m_editUserName.EnableWindow(iCurSel != AUTH_NONE);
	m_textPasswordPrompt.EnableWindow(iCurSel != AUTH_NONE);
	m_editPassword.EnableWindow(iCurSel != AUTH_NONE);
}

// diagnostic services

#if defined(_DEBUG)

void CAuthenticationDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	CDialog::AssertValid();

	// ...and then verify our own state as well
	ASSERT_VALID(&m_comboAuthMethod);
	ASSERT_VALID(&m_textUserNamePrompt);
	ASSERT_VALID(&m_editUserName);
	ASSERT_VALID(&m_textPasswordPrompt);
	ASSERT_VALID(&m_editPassword);
}

void CAuthenticationDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		CDialog::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_comboAuthMethod = " << m_comboAuthMethod;
		dumpCtx << "\nm_textUserNamePrompt = " << m_textUserNamePrompt;
		dumpCtx << "\nm_editUserName = " << m_editUserName;
		dumpCtx << "\nm_textPasswordPrompt = " << m_textPasswordPrompt;
		dumpCtx << "\nm_editPassword = " << m_editPassword;
		dumpCtx << "\nm_eAuthMethod = " << m_eAuthMethod;
		dumpCtx << "\nm_strUserName = " << m_strUserName;
		dumpCtx << "\nm_strPassword = " << m_strPassword;
		dumpCtx << "\nm_fUseSSL = " << m_fUseSSL;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
