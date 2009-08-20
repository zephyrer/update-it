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

// ActionPage.cpp - implementation of the CActionPage class

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"
#include "../Languages/English_USA.1252/Source/Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "BetterPropPage.h"
#include "AboutPage.h"
#include "FirstLaunchPage.h"
#include "OptionsPage.h"
#include "FilesList.h"
#include "FilesPage.h"
#include "CustomDialog.h"
#include "AuthenticationDialog.h"
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
// avoid unwanted ICL warnings

#if defined(__INTEL_COMPILER)
// remark #279: controlling expression is constant
#pragma warning(disable: 279)
#endif	// __INTEL_COMPILER

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CActionPage, CBetterPropPage)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CActionPage, CBetterPropPage)
	ON_BN_CLICKED(IDC_CHECK_UPLOAD, OnCheckUpload)
	ON_BN_CLICKED(IDC_CHECK_ZIP, OnCheckZip)
	ON_BN_CLICKED(IDC_CHECK_SEND, OnCheckSend)
	ON_BN_CLICKED(IDC_BUTTON_ZIP_OPTIONS, OnButtonZipOptions)
	ON_BN_CLICKED(IDC_BUTTON_AUTH, OnButtonAuthentication)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction

CActionPage::CActionPage(void):
CBetterPropPage(IDD_PAGE_ACTION),
m_nAction(COPY_FILES),
m_nUpload(BST_UNCHECKED),
m_nZip(BST_UNCHECKED),
m_nSend(BST_UNCHECKED),
m_nFtpPort(INTERNET_DEFAULT_FTP_PORT),
m_fPassive(FALSE),
m_nSmtpPort(IPPORT_SMTP)
{
	m_psp.dwFlags |= PSP_PREMATURE;

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	// initialize and validate initial input values

	InitActionSettings(pApp);

	// FTP settings
	if (m_nUpload == BST_CHECKED)
	{
		InitFtpSettings(pApp);
	}

	// SMTP Settings
	if (m_nSend == BST_CHECKED)
	{
		InitSmtpSettings(pApp);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CActionPage::OnInitDialog(void)
{
	// invoke inherited handler
	BOOL fResult = __super::OnInitDialog();

	// adjust page as needed
	BOOL fEnableZipOpts = IsDlgButtonChecked(IDC_CHECK_ZIP) == BST_CHECKED;
	GetDlgItem(IDC_BUTTON_ZIP_OPTIONS)->EnableWindow(fEnableZipOpts);
	ShowMailControls(m_fCanSend);

	// assign tool tips
	CToolTipCtrl& tipWnd = GetToolTipCtrl();
	tipWnd.AddTool(GetDlgItem(IDC_RADIO_COPY));
	tipWnd.AddTool(GetDlgItem(IDC_RADIO_MOVE));
	tipWnd.AddTool(GetDlgItem(IDC_CHECK_UPLOAD));
	tipWnd.AddTool(GetDlgItem(IDC_CHECK_ZIP));
	tipWnd.AddTool(GetDlgItem(IDC_CHECK_SEND));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_SERVER));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_FTP_PORT));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_LOGIN));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_PASSWORD));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_ROOT));
	tipWnd.AddTool(GetDlgItem(IDC_CHECK_PASSIVE));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_FROM));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_MAILTO));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_SUBJECT));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_HOST));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_PORT));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_BODY));
	tipWnd.Activate(TRUE);

	// initialized
	return (fResult);
}

BOOL CActionPage::OnSetActive(void)
{
	BOOL fSuccess = __super::OnSetActive();
	if (fSuccess)
	{
		EnableFtpControls(m_nUpload == BST_CHECKED);
		if (m_fCanSend)
		{
			GetDlgItem(IDC_CHECK_SEND)->EnableWindow(m_nZip == BST_CHECKED);
			EnableMailControls(IsDlgButtonChecked(IDC_CHECK_SEND) == BST_CHECKED);
		}
		CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
		ASSERT(pWiz != NULL);
		pWiz->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	}
	return (fSuccess);
}

BOOL CActionPage::OnKillActive(void)
{
	// validate mail settings (if needed)
	if (m_fCanSend && m_nSend == BST_CHECKED)
	{
	}

	// adjust and validate FTP settings (if needed)
	if (m_nUpload == BST_CHECKED)
	{
		if (m_strRoot.IsEmpty())
		{
			// assume root directory
			m_strRoot += _T('/');
		}
		else {
			m_strRoot.Replace(_T('\\'), _T('/'));
			if (m_strRoot[0] != _T('/'))
			{
				m_strRoot.Insert(0, _T('/'));
			}
			int cchRoot = m_strRoot.GetLength();
			if (cchRoot > 1 && m_strRoot[cchRoot - 1] != _T('/'))
			{
				m_strRoot += _T('/');
			}
		}
	}

	// invoke inherited handler
	BOOL fSuccess = __super::OnKillActive();

	if (fSuccess)
	{
		CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
		ASSERT_VALID(pApp);

		if (!pApp->m_argsParser.HasKey(SZ_ARGV_DONT_SAVE_INPUT))
		{
			pApp->WriteProfileInt(SZ_REGK_ACTION, SZ_REGV_ACTION_ACTION, m_nAction);
			pApp->WriteProfileInt(SZ_REGK_ACTION, SZ_REGV_ACTION_UPLOAD, m_nUpload);

			if (m_nUpload == BST_CHECKED)
			{
				pApp->WriteProfileString(SZ_REGK_FTP, SZ_REGV_FTP_SERVER, m_strServer);
				pApp->WriteProfileInt(SZ_REGK_FTP, SZ_REGV_FTP_PORT, m_nFtpPort);
				pApp->WriteProfileString(SZ_REGK_FTP, SZ_REGV_FTP_LOGIN, m_strLogin);
				pApp->WriteProfilePassword(SZ_REGK_FTP, SZ_REGV_FTP_PASSWORD, m_strPassword);
				pApp->WriteProfileString(SZ_REGK_FTP, SZ_REGV_FTP_ROOT, m_strRoot);
				pApp->WriteProfileInt(SZ_REGK_FTP, SZ_REGV_FTP_PASSIVE, m_fPassive);
			}

			pApp->WriteProfileInt(SZ_REGK_ACTION, SZ_REGV_ACTION_ZIP, m_nZip);

			if (m_fCanSend)
			{
				pApp->WriteProfileInt(SZ_REGK_ACTION, SZ_REGV_ACTION_SEND, m_nSend);
				if (m_nSend == BST_CHECKED)
				{
					pApp->WriteProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_FROM, m_strFrom);
					pApp->WriteProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_MAILTO, m_strTo);
					pApp->WriteProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_SUBJ, m_strSubject);
					pApp->WriteProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_HOST, m_strHost);
					pApp->WriteProfileInt(SZ_REGK_SMTP, SZ_REGV_SMTP_PORT, m_nSmtpPort);
					UINT cbBody = (m_strBody.GetLength() + 1) * sizeof(TCHAR);
					BYTE* pbTemp = reinterpret_cast<BYTE*>(m_strBody.GetBuffer(0));
					pApp->WriteProfileBinary(SZ_REGK_SMTP, SZ_REGV_SMTP_BODY, pbTemp, cbBody);
					m_strBody.ReleaseBuffer();
				}
			}
		}
	}

	return (fSuccess);
}

void CActionPage::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	// general actions
	DDX_Radio(pDX, IDC_RADIO_COPY, m_nAction);
	DDX_Check(pDX, IDC_CHECK_UPLOAD, m_nUpload);
	DDX_Check(pDX, IDC_CHECK_ZIP, m_nZip);
	if (m_fCanSend)
	{
		DDX_Check(pDX, IDC_CHECK_SEND, m_nSend);
	}

	// FTP settings
	if (m_nUpload == BST_CHECKED)
	{
		DDX_Text(pDX, IDC_EDIT_SERVER, m_strServer);
		DDV_MinMaxChars(pDX, m_strServer, MIN_FTP_SERVER_LENGTH, MAX_FTP_SERVER_LENGTH);
		DDXV_Word(pDX, IDC_EDIT_FTP_PORT, m_nFtpPort, 1, INTERNET_MAX_PORT_NUMBER_VALUE);
		DDX_Text(pDX, IDC_EDIT_LOGIN, m_strLogin);
		DDV_MaxChars(pDX, m_strLogin, MAX_FTP_LOGIN_LENGTH);
		DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
		DDV_MaxChars(pDX, m_strPassword, MAX_FTP_PASSWORD_LENGTH);
		DDX_Text(pDX, IDC_EDIT_ROOT, m_strRoot);
		DDV_MaxChars(pDX, m_strRoot, _MAX_PATH);
		DDX_Check(pDX, IDC_CHECK_PASSIVE, m_fPassive);
	}

	// SMTP settings
	if (m_nSend == BST_CHECKED)
	{
		DDX_Text(pDX, IDC_EDIT_FROM, m_strFrom);
		DDV_MinMaxChars(pDX, m_strFrom, MIN_EMAIL_ADDRESS_LENGTH, MAX_EMAIL_ADDRESS_LENGTH);
		DDX_Text(pDX, IDC_EDIT_MAILTO, m_strTo);
		DDV_MinMaxChars(pDX, m_strTo, MIN_EMAIL_ADDRESS_LENGTH, MAX_EMAIL_ADDRESS_LENGTH);
		DDX_Text(pDX, IDC_EDIT_SUBJECT, m_strSubject);
		DDV_MaxChars(pDX, m_strSubject, MAX_EMAIL_SUBJECT_LENGTH);
		DDX_Text(pDX, IDC_EDIT_HOST, m_strHost);
		DDV_MinMaxChars(pDX, m_strHost, MIN_SMTP_HOST_LENGTH, MAX_SMTP_HOST_LENGTH);
		DDXV_Word(pDX, IDC_EDIT_PORT, m_nSmtpPort, 1, INTERNET_MAX_PORT_NUMBER_VALUE);
		DDX_Text(pDX, IDC_EDIT_BODY, m_strBody);
		DDV_MaxChars(pDX, m_strBody, MAX_EMAIL_BODY_LENGTH);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

void CActionPage::OnCheckUpload(void)
{
	BOOL fEnable = IsDlgButtonChecked(IDC_CHECK_UPLOAD) == BST_CHECKED;
	if (fEnable)
	{
		CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
		ASSERT_VALID(pApp);

		// restore the most recently saved settings
		InitFtpSettings(pApp);
		SetDlgItemText(IDC_EDIT_SERVER, m_strServer);
		SetDlgItemInt(IDC_EDIT_FTP_PORT, m_nFtpPort);
		SetDlgItemText(IDC_EDIT_LOGIN, m_strLogin);
		SetDlgItemText(IDC_EDIT_PASSWORD, m_strPassword);
		SetDlgItemText(IDC_EDIT_ROOT, m_strRoot);
		CheckDlgButton(IDC_CHECK_PASSIVE, m_fPassive);
	}
	EnableFtpControls(fEnable);
}

void CActionPage::OnCheckZip(void)
{
	if (m_fCanSend)
	{
		CWnd* pCheckSend = GetDlgItem(IDC_CHECK_SEND);
		ASSERT_VALID(pCheckSend);
		CWnd* pButtonZipOptions = GetDlgItem(IDC_BUTTON_ZIP_OPTIONS);
		ASSERT_VALID(pButtonZipOptions);

		if (IsDlgButtonChecked(IDC_CHECK_ZIP) == BST_CHECKED)
		{
			pCheckSend->EnableWindow();
			pButtonZipOptions->EnableWindow();
		}
		else {
			if (IsDlgButtonChecked(IDC_CHECK_SEND) != BST_UNCHECKED)
			{
				CheckDlgButton(IDC_CHECK_SEND, BST_UNCHECKED);
				EnableMailControls(FALSE);
			}
			pCheckSend->EnableWindow(FALSE);
			pButtonZipOptions->EnableWindow(FALSE);
		}
	}
}

void CActionPage::OnCheckSend(void)
{
	BOOL fEnable = IsDlgButtonChecked(IDC_CHECK_SEND) == BST_CHECKED;
	if (fEnable)
	{
		CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
		ASSERT_VALID(pApp);

		// restore the most recently saved settings
		InitSmtpSettings(pApp);
		SetDlgItemText(IDC_EDIT_FROM, m_strFrom);
		SetDlgItemText(IDC_EDIT_MAILTO, m_strTo);
		SetDlgItemText(IDC_EDIT_SUBJECT, m_strSubject);
		SetDlgItemText(IDC_EDIT_HOST, m_strHost);
		SetDlgItemInt(IDC_EDIT_PORT, m_nSmtpPort);
		SetDlgItemText(IDC_EDIT_BODY, m_strBody);
	}
	EnableMailControls(fEnable);
}

void CActionPage::OnButtonZipOptions(void)
{
	if (m_dlgZipOpts.DoModal() == IDOK)
	{
		CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
		ASSERT_VALID(pApp);

		if (!pApp->m_argsParser.HasKey(SZ_ARGV_DONT_SAVE_INPUT))
		{
			pApp->WriteProfileInt(SZ_REGK_ZIP, SZ_REGV_ZIP_COMPR_METHOD, m_dlgZipOpts.m_iComprMethod);
			pApp->WriteProfileInt(SZ_REGK_ZIP, SZ_REGV_ZIP_COMPR_LEVEL, m_dlgZipOpts.m_iComprLevel);
			pApp->WriteProfileInt(SZ_REGK_ZIP, SZ_REGV_ZIP_ENCR_METHOD, m_dlgZipOpts.m_iEncrMethod);
			if (m_dlgZipOpts.m_iEncrMethod != CZipOptionsDialog::I_METHOD_NONE)
			{
				pApp->WriteProfilePassword(SZ_REGK_ZIP, SZ_REGV_ZIP_PASSWORD, m_dlgZipOpts.m_strPassword);
			}
		}
	}
}

void CActionPage::OnButtonAuthentication(void)
{
	if (m_dlgAuth.DoModal() == IDOK)
	{
		CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
		ASSERT_VALID(pApp);

		if (!pApp->m_argsParser.HasKey(SZ_ARGV_DONT_SAVE_INPUT))
		{
			pApp->WriteProfileInt(SZ_REGK_SMTP, SZ_REGV_SMTP_AUTHENTICATION, m_dlgAuth.m_eAuthMethod);
			if (m_dlgAuth.m_eAuthMethod != CSmtpConnection::AUTH_NONE)
			{
				pApp->WriteProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_USERNAME, m_dlgAuth.m_strUserName);
				pApp->WriteProfilePassword(SZ_REGK_SMTP, SZ_REGV_SMTP_PASSWORD, m_dlgAuth.m_strPassword);
				pApp->WriteProfileInt(SZ_REGK_SMTP, SZ_REGV_SMTP_USE_SSL, m_dlgAuth.m_fUseSSL);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

void CActionPage::InitActionSettings(class CUpdateItApp* pApp)
{
	m_nAction = pApp->GetConfigInt(SZ_ARG_ACTION_MOVE, SZ_REGK_ACTION, SZ_REGV_ACTION_ACTION, COPY_FILES);
	if (m_nAction != COPY_FILES && m_nAction != MOVE_FILES)
	{
		m_nAction = COPY_FILES;
	}

	m_nUpload = pApp->GetConfigCheck(SZ_ARG_ACTION_UPLOAD, SZ_REGK_ACTION, SZ_REGV_ACTION_UPLOAD, BST_UNCHECKED);
	m_nZip = pApp->GetConfigCheck(SZ_ARG_ACTION_ZIP, SZ_REGK_ACTION, SZ_REGV_ACTION_ZIP, BST_UNCHECKED);
	m_fCanSend = pApp->GetConfigBool(SZ_ARG_SMTP_ENABLE, SZ_REGK_SMTP, SZ_REGV_SMTP_ENABLE, FALSE);

	if (m_nZip == BST_CHECKED && m_fCanSend)
	{
		m_nSend = pApp->GetConfigCheck(SZ_ARG_ACTION_SEND, SZ_REGK_ACTION, SZ_REGV_ACTION_SEND, BST_UNCHECKED);
	}
	else {
		m_nSend = BST_UNCHECKED;
	}
}

void CActionPage::InitFtpSettings(class CUpdateItApp* pApp)
{
	m_strServer = pApp->GetConfigString(SZ_ARG_FTP_SERVER, SZ_REGK_FTP, SZ_REGV_FTP_SERVER);

	m_nFtpPort = LOWORD(pApp->GetConfigInt(SZ_ARG_FTP_PORT, SZ_REGK_FTP, SZ_REGV_FTP_PORT, INTERNET_DEFAULT_FTP_PORT));
	if (m_nFtpPort == INTERNET_INVALID_PORT_NUMBER || m_nFtpPort > INTERNET_MAX_PORT_NUMBER_VALUE)
	{
		m_nFtpPort = INTERNET_DEFAULT_FTP_PORT;
	}

	m_strLogin = pApp->GetConfigString(SZ_ARG_FTP_LOGIN, SZ_REGK_FTP, SZ_REGV_FTP_LOGIN);
	m_strPassword = pApp->GetConfigPassword(SZ_ARG_FTP_PASSWORD, SZ_REGK_FTP, SZ_REGV_FTP_PASSWORD);
	m_strRoot = pApp->GetConfigString(SZ_ARG_FTP_ROOT, SZ_REGK_FTP, SZ_REGV_FTP_ROOT);
	m_fPassive = pApp->GetConfigCheck(SZ_ARG_FTP_PASSIVE, SZ_REGK_FTP, SZ_REGV_FTP_PASSIVE, BST_UNCHECKED);
}

void CActionPage::InitSmtpSettings(class CUpdateItApp* pApp)
{
	m_strFrom = pApp->GetConfigString(SZ_ARG_SMTP_FROM, SZ_REGK_SMTP, SZ_REGV_SMTP_FROM);
	m_strTo = pApp->GetConfigString(SZ_ARG_SMTP_MAILTO, SZ_REGK_SMTP, SZ_REGV_SMTP_MAILTO);
	m_strSubject = pApp->GetConfigString(SZ_ARG_SMTP_SUBJ, SZ_REGK_SMTP, SZ_REGV_SMTP_SUBJ);
	m_strHost = pApp->GetConfigString(SZ_ARG_SMTP_HOST, SZ_REGK_SMTP, SZ_REGV_SMTP_HOST);

	m_nSmtpPort = LOWORD(pApp->GetConfigInt(SZ_ARG_SMTP_PORT, SZ_REGK_SMTP, SZ_REGV_SMTP_PORT, IPPORT_SMTP));
	if (m_nSmtpPort == INTERNET_INVALID_PORT_NUMBER || m_nSmtpPort > INTERNET_MAX_PORT_NUMBER_VALUE)
	{
		m_nSmtpPort = IPPORT_SMTP;
	}

	BYTE* pbTemp = NULL;
	UINT cbBody = 0;
	pApp->GetProfileBinary(SZ_REGK_SMTP, SZ_REGV_SMTP_BODY, &pbTemp, &cbBody);
	if (pbTemp != NULL && cbBody > 0)
	{
		memmove(m_strBody.GetBuffer(cbBody - 1), pbTemp, cbBody);
		m_strBody.ReleaseBuffer();
		delete[] pbTemp;
	}
}

void CActionPage::EnableFtpControls(BOOL fEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_GROUP_FTP);
	do
	{
		pWnd->EnableWindow(fEnable);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	while (pWnd->GetDlgCtrlID() != IDC_GROUP_MAIL);
}

void CActionPage::EnableMailControls(BOOL fEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_GROUP_MAIL);
	ASSERT(pWnd != NULL);
	do
	{
		pWnd->EnableWindow(fEnable);
	}
	while ((pWnd = pWnd->GetWindow(GW_HWNDNEXT)) != NULL);
}

void CActionPage::ShowMailControls(BOOL fShow)
{
	GetDlgItem(IDC_CHECK_SEND)->ShowWindow(fShow ? SW_SHOW : SW_HIDE);
	CWnd* pWnd = GetDlgItem(IDC_GROUP_MAIL);
	ASSERT(pWnd != NULL);
	do
	{
		pWnd->ShowWindow(fShow ? SW_SHOW : SW_HIDE);
	}
	while ((pWnd = pWnd->GetWindow(GW_HWNDNEXT)) != NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CActionPage::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify own state as well
	ASSERT_VALID(&m_dlgAuth);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CActionPage::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_nAction = " << m_nAction;
		dumpCtx << "\nm_nUpload = " << m_nUpload;
		dumpCtx << "\nm_nZip = " << m_nZip;
		dumpCtx << "\nm_fCanSend = " << m_fCanSend;
		dumpCtx << "\nm_nSend = " << m_nSend;
		dumpCtx << "\nm_strServer = " << m_strServer;
		dumpCtx << "\nm_nFtpPort = " << m_nFtpPort;
		dumpCtx << "\nm_strLogin = " << m_strLogin;
		dumpCtx << "\nm_strPassword = " << m_strPassword;
		dumpCtx << "\nm_strRoot = " << m_strRoot;
		dumpCtx << "\nm_fPassive = " << m_fPassive;
		dumpCtx << "\nm_strFrom = " << m_strFrom;
		dumpCtx << "\nm_strTo = " << m_strTo;
		dumpCtx << "\nm_strSubject = " << m_strSubject;
		dumpCtx << "\nm_strHost = " << m_strHost;
		dumpCtx << "\nm_nSmtpPort = " << m_nSmtpPort;
		dumpCtx << "\nm_strBody = " << m_strBody;
		dumpCtx << "\nm_dlgAuth = " << m_dlgAuth;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
