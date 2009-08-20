// UpdateIt! application.
// Copyright (c) 2002-2009 by Elijah Zarezky,
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

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"
#include "../Languages/English_USA.1252/Source/Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "CustomDialog.h"
#include "AuthenticationDialog.h"
#include "BetterPropPage.h"
#include "AboutPage.h"
#include "OptionsPage.h"
#include "FilesList.h"
#include "FilesPage.h"
#include "ZipOptionsDialog.h"
#include "ActionPage.h"
#include "ProgressPageBase.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"
#include "UpdateItApp.h"
#include "../Common/Registry.h"
#include "Arguments.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CAuthenticationDialog, CCustomDialog)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CAuthenticationDialog, CCustomDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTH_METHOD, OnComboAuthMethodSelChange)
	ON_EN_CHANGE(IDC_EDIT_USERNAME, OnEditUserNameChange)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, OnEditPasswordChange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CAuthenticationDialog::CAuthenticationDialog(CWnd* pParentWnd):
CCustomDialog(IDD_AUTHENTICATION, pParentWnd),
m_eAuthMethod(CSmtpConnection::AUTH_NONE),
m_fUseSSL(FALSE)
{
	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	// initialize and validate initial input values

	CArgsParser& argsParser = pApp->m_argsParser;

	if (!argsParser.GetUIntValue(SZ_ARG_SMTP_AUTHENTICATION, *reinterpret_cast<UINT*>(&m_eAuthMethod), 10))
	{
		m_eAuthMethod = pApp->GetProfileData(SZ_REGK_SMTP, SZ_REGV_SMTP_AUTHENTICATION, CSmtpConnection::AUTH_NONE);
	}
	if (m_eAuthMethod < CSmtpConnection::AUTH_NONE || m_eAuthMethod > CSmtpConnection::AUTH_NTLM)
	{
		m_eAuthMethod = CSmtpConnection::AUTH_NONE;
	}

	if (m_eAuthMethod != CSmtpConnection::AUTH_NONE)
	{
		m_strUserName = pApp->GetConfigString(SZ_ARG_SMTP_USERNAME, SZ_REGK_SMTP, SZ_REGV_SMTP_USERNAME);
		m_strPassword = pApp->GetConfigPassword(SZ_ARG_SMTP_PASSWORD, SZ_REGK_SMTP, SZ_REGV_SMTP_PASSWORD);
		m_fUseSSL = pApp->GetConfigCheck(SZ_ARG_SMTP_USE_SSL, SZ_REGK_SMTP, SZ_REGV_SMTP_USE_SSL, FALSE);
	}
}

CAuthenticationDialog::~CAuthenticationDialog(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CAuthenticationDialog::OnInitDialog(void)
{
	__super::OnInitDialog();

	UpdateControls();

	return (TRUE);
}

void CAuthenticationDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_AUTH_METHOD, m_comboAuthMethod);
	DDX_Control(pDX, IDC_STATIC_USERNAME_PROMPT, m_textUserNamePrompt);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_STATIC_PASSWORD_PROMPT, m_textPasswordPrompt);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_CHECK_USE_SSL, m_checkUseSSL);

	DDX_CBIndex(pDX, IDC_COMBO_AUTH_METHOD, *reinterpret_cast<int*>(&m_eAuthMethod));
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_CHECK_USE_SSL, m_fUseSSL);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

void CAuthenticationDialog::OnComboAuthMethodSelChange(void)
{
	UpdateControls();
}

void CAuthenticationDialog::OnEditUserNameChange(void)
{
	if (m_comboAuthMethod.GetCurSel() != CSmtpConnection::AUTH_NONE)
	{
		bool fHasUserName = m_editUserName.GetWindowTextLength() > 0;
		bool fHasPassword = m_editPassword.GetWindowTextLength() > 0;
		GetDlgItem(IDOK)->EnableWindow(fHasUserName && fHasPassword);
	}
}

void CAuthenticationDialog::OnEditPasswordChange(void)
{
	if (m_comboAuthMethod.GetCurSel() != CSmtpConnection::AUTH_NONE)
	{
		bool fHasUserName = m_editUserName.GetWindowTextLength() > 0;
		bool fHasPassword = m_editPassword.GetWindowTextLength() > 0;
		GetDlgItem(IDOK)->EnableWindow(fHasUserName && fHasPassword);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

void CAuthenticationDialog::UpdateControls(void)
{
	// precondition
	ASSERT(::IsWindow(m_hWnd));

	if (m_comboAuthMethod.GetCurSel() != CSmtpConnection::AUTH_NONE)
	{
		m_textUserNamePrompt.EnableWindow(TRUE);
		m_editUserName.EnableWindow(TRUE);
		m_textPasswordPrompt.EnableWindow(TRUE);
		m_editPassword.EnableWindow(TRUE);
		m_checkUseSSL.EnableWindow(TRUE);
		bool fHasUserName = m_editUserName.GetWindowTextLength() > 0;
		bool fHasPassword = m_editPassword.GetWindowTextLength() > 0;
		GetDlgItem(IDOK)->EnableWindow(fHasUserName && fHasPassword);
	}
	else {
		m_textUserNamePrompt.EnableWindow(FALSE);
		m_editUserName.SetWindowText(NULL);
		m_editUserName.EnableWindow(FALSE);
		m_textPasswordPrompt.EnableWindow(FALSE);
		m_editPassword.SetWindowText(NULL);
		m_editPassword.EnableWindow(FALSE);
		m_checkUseSSL.SetCheck(BST_UNCHECKED);
		m_checkUseSSL.EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CAuthenticationDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
	ASSERT_VALID(&m_comboAuthMethod);
	ASSERT_VALID(&m_textUserNamePrompt);
	ASSERT_VALID(&m_editUserName);
	ASSERT_VALID(&m_textPasswordPrompt);
	ASSERT_VALID(&m_editPassword);
	ASSERT_VALID(&m_checkUseSSL);
}

void CAuthenticationDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_comboAuthMethod = " << m_comboAuthMethod;
		dumpCtx << "\nm_textUserNamePrompt = " << m_textUserNamePrompt;
		dumpCtx << "\nm_editUserName = " << m_editUserName;
		dumpCtx << "\nm_textPasswordPrompt = " << m_textPasswordPrompt;
		dumpCtx << "\nm_editPassword = " << m_editPassword;
		dumpCtx << "\nm_checkUseSSL = " << m_checkUseSSL;
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
