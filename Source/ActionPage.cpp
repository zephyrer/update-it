// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// ActionPage.cpp - implementation of the CActionPage class

#include "stdafx.h"
#include "Resource.h"
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

#if defined(__INTEL_COMPILER)
// remark #279: controlling expression is constant
#pragma warning(disable: 279)
#endif	// __INTEL_COMPILER

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CActionPage, CBetterPropPage)

// message map
BEGIN_MESSAGE_MAP(CActionPage, CBetterPropPage)
	ON_BN_CLICKED(IDC_CHECK_UPLOAD, OnCheckUpload)
	ON_BN_CLICKED(IDC_CHECK_ZIP, OnCheckZip)
	ON_BN_CLICKED(IDC_CHECK_SEND, OnCheckSend)
END_MESSAGE_MAP()

CActionPage::CActionPage(void):
CBetterPropPage(IDD_PAGE_ACTION),
m_nFtpPort(21), m_nPort(25)
{
	BYTE* pbTemp;
	UINT cbBody;

	m_psp.dwFlags |= PSP_PREMATURE;

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);
	m_nAction = pApp->GetProfileInt(_T("Action"), _T("Action"), 0);
	m_nUpload = pApp->GetProfileInt(_T("Action"), _T("Upload"), BST_UNCHECKED);
	if (m_nUpload == BST_CHECKED)
	{
		m_strServer = pApp->GetProfileString(_T("FTP"), _T("Server"));
		m_nFtpPort = LOWORD(pApp->GetProfileInt(_T("FTP"), _T("Port"), 21));
		m_strLogin = pApp->GetProfileString(_T("FTP"), _T("Login"));
		m_strPassword = pApp->GetProfilePassword(_T("FTP"), _T("Password"));
		m_strRoot = pApp->GetProfileString(_T("FTP"), _T("Root"));
	}
	m_nZip = pApp->GetProfileInt(_T("Action"), _T("Zip"), BST_UNCHECKED);
	m_fCanSend = pApp->GetProfileInt(_T("SMTP"), _T("Enable"), FALSE);
	if (m_fCanSend)
	{
		if (m_nZip == BST_CHECKED)
		{
			m_nSend = pApp->GetProfileInt(_T("Action"), _T("Send"), BST_UNCHECKED);
		}
		else {
			m_nSend = BST_UNCHECKED;
		}
		m_strFrom = pApp->GetProfileString(_T("SMTP"), _T("from"));
		m_strTo = pApp->GetProfileString(_T("SMTP"), _T("mailto"));
		m_strSubject = pApp->GetProfileString(_T("SMTP"), _T("subj"));
		m_strHost = pApp->GetProfileString(_T("SMTP"), _T("host"));
		m_nPort = pApp->GetProfileInt(_T("SMTP"), _T("port"), 25);
		pApp->GetProfileBinary(_T("SMTP"), _T("body"), &pbTemp, &cbBody);
		if (pbTemp != NULL && cbBody > 0)
		{
			memmove(m_strBody.GetBuffer(cbBody - 1), pbTemp, cbBody);
			m_strBody.ReleaseBuffer();
			delete[] pbTemp;
		}
	}
}

BOOL CActionPage::OnInitDialog(void)
{
	// invoke inherited handler
	BOOL fResult = CBetterPropPage::OnInitDialog();

	// adjust page as needed
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
	BOOL fSuccess = CBetterPropPage::OnSetActive();
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
	BOOL fSuccess = CBetterPropPage::OnKillActive();

	if (fSuccess)
	{
		CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
		ASSERT_VALID(pApp);
		pApp->WriteProfileInt(_T("Action"), _T("Action"), m_nAction);
		pApp->WriteProfileInt(_T("Action"), _T("Upload"), m_nUpload);
		if (m_nUpload == BST_CHECKED)
		{
			pApp->WriteProfileString(_T("FTP"), _T("Server"), m_strServer);
			pApp->WriteProfileInt(_T("FTP"), _T("Port"), m_nFtpPort);
			pApp->WriteProfileString(_T("FTP"), _T("Login"), m_strLogin);
			pApp->WriteProfilePassword(_T("FTP"), _T("Password"), m_strPassword);
			pApp->WriteProfileString(_T("FTP"), _T("Root"), m_strRoot);
		}
		pApp->WriteProfileInt(_T("Action"), _T("Zip"), m_nZip);
		if (m_fCanSend)
		{
			pApp->WriteProfileInt(_T("Action"), _T("Send"), m_nSend);
			if (m_nSend == BST_CHECKED)
			{
				pApp->WriteProfileString(_T("SMTP"), _T("from"), m_strFrom);
				pApp->WriteProfileString(_T("SMTP"), _T("mailto"), m_strTo);
				pApp->WriteProfileString(_T("SMTP"), _T("subj"), m_strSubject);
				pApp->WriteProfileString(_T("SMTP"), _T("host"), m_strHost);
				pApp->WriteProfileInt(_T("SMTP"), _T("port"), m_nPort);
				UINT cbBody = (m_strBody.GetLength() + 1) * sizeof(TCHAR);
				BYTE* pbTemp = reinterpret_cast<BYTE*>(m_strBody.GetBuffer(0));
				pApp->WriteProfileBinary(_T("SMTP"), _T("body"), pbTemp, cbBody);
				m_strBody.ReleaseBuffer();
			}
		}
	}

	return (fSuccess);
}

void CActionPage::DoDataExchange(CDataExchange* pDX)
{
	CBetterPropPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_COPY, m_nAction);
	DDX_Check(pDX, IDC_CHECK_UPLOAD, m_nUpload);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServer);
	DDV_MaxChars(pDX, m_strServer, 255);
	DDX_Text(pDX, IDC_EDIT_FTP_PORT, m_nFtpPort);
	DDV_MinMaxInt(pDX, m_nFtpPort, 1, 255);
	DDX_Text(pDX, IDC_EDIT_LOGIN, m_strLogin);
	DDV_MaxChars(pDX, m_strLogin, 64);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 64);
	DDX_Text(pDX, IDC_EDIT_ROOT, m_strRoot);
	DDV_MaxChars(pDX, m_strRoot, _MAX_PATH);
	DDX_Check(pDX, IDC_CHECK_ZIP, m_nZip);

	if (m_fCanSend)
	{
		DDX_Check(pDX, IDC_CHECK_SEND, m_nSend);
		DDX_Text(pDX, IDC_EDIT_FROM, m_strFrom);
		DDV_MaxChars(pDX, m_strFrom, 255);
		DDX_Text(pDX, IDC_EDIT_MAILTO, m_strTo);
		DDV_MaxChars(pDX, m_strTo, 255);
		DDX_Text(pDX, IDC_EDIT_SUBJECT, m_strSubject);
		DDV_MaxChars(pDX, m_strSubject, 255);
		DDX_Text(pDX, IDC_EDIT_HOST, m_strHost);
		DDV_MaxChars(pDX, m_strHost, 255);
		DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
		DDV_MinMaxInt(pDX, m_nPort, 1, 255);
		DDX_Text(pDX, IDC_EDIT_BODY, m_strBody);
		DDV_MaxChars(pDX, m_strBody, 1024);
	}
}

void CActionPage::OnCheckUpload(void)
{
	BOOL fEnable = IsDlgButtonChecked(IDC_CHECK_UPLOAD) == BST_CHECKED;
	if (fEnable)
	{
		CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
		ASSERT_VALID(pApp);

		// restore the most recently saved settings
		SetDlgItemText(IDC_EDIT_SERVER, pApp->GetProfileString(_T("FTP"), _T("Server")));
		SetDlgItemInt(IDC_EDIT_FTP_PORT, pApp->GetProfileInt(_T("FTP"), _T("Port"), 21), FALSE);
		SetDlgItemText(IDC_EDIT_LOGIN, pApp->GetProfileString(_T("FTP"), _T("Login")));
		SetDlgItemText(IDC_EDIT_PASSWORD, pApp->GetProfilePassword(_T("FTP"), _T("Password")));
		SetDlgItemText(IDC_EDIT_ROOT, pApp->GetProfileString(_T("FTP"), _T("Root")));
	}
	EnableFtpControls(fEnable);
}

void CActionPage::OnCheckZip(void)
{
	if (m_fCanSend)
	{
		CWnd* pCheckSend = GetDlgItem(IDC_CHECK_SEND);
		ASSERT(pCheckSend != NULL);
		if (IsDlgButtonChecked(IDC_CHECK_ZIP) == BST_CHECKED)
		{
			pCheckSend->EnableWindow();
		}
		else {
			if (IsDlgButtonChecked(IDC_CHECK_SEND) != BST_UNCHECKED)
			{
				CheckDlgButton(IDC_CHECK_SEND, BST_UNCHECKED);
				EnableMailControls(FALSE);
			}
			pCheckSend->EnableWindow(FALSE);
		}
	}
}

void CActionPage::OnCheckSend(void)
{
	BOOL fEnable = IsDlgButtonChecked(IDC_CHECK_SEND) == BST_CHECKED;
	if (fEnable)
	{
		CWinApp* pApp = AfxGetApp();
		ASSERT_VALID(pApp);

		// restore the most recently saved settings
		SetDlgItemText(IDC_EDIT_FROM, pApp->GetProfileString(_T("SMTP"), _T("from")));
		SetDlgItemText(IDC_EDIT_MAILTO, pApp->GetProfileString(_T("SMTP"), _T("mailto")));
		SetDlgItemText(IDC_EDIT_SUBJECT, pApp->GetProfileString(_T("SMTP"), _T("subj")));
		SetDlgItemText(IDC_EDIT_HOST, pApp->GetProfileString(_T("SMTP"), _T("host")));
		SetDlgItemInt(IDC_EDIT_PORT, pApp->GetProfileInt(_T("SMTP"), _T("port"), 25), FALSE);
	}
	EnableMailControls(fEnable);
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

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CActionPage::AssertValid(void) const
{
	// first perform inherited validity check...
	CBetterPropPage::AssertValid();

	// ...and then verify own state as well
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
		CBetterPropPage::Dump(dumpCtx);

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
		dumpCtx << "\nm_strFrom = " << m_strFrom;
		dumpCtx << "\nm_strTo = " << m_strTo;
		dumpCtx << "\nm_strSubject = " << m_strSubject;
		dumpCtx << "\nm_strHost = " << m_strHost;
		dumpCtx << "\nm_nPort = " << m_nPort;
		dumpCtx << "\nm_strBody = " << m_strBody;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
