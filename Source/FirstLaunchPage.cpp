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

// FirstLaunchPage.cpp - implementation of the CFirstLaunchPage class

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
#include "ZipOptionsDialog.h"
#include "BrowseFtpDialog.h"
#include "AuthenticationDialog.h"
#include "ActionPage.h"
#include "ProgressPageBase.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"
#include "UpdateItApp.h"
#include "../Common/IniDefines.h"
#include "../Common/Registry.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CFirstLaunchPage, CBetterPropPage)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CFirstLaunchPage, CBetterPropPage)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

//! Default ctor. Invokes inherited ctor first and then sets the PSP_PREMATURE
//! flag, which causes the page to be created when the property sheet is created.
//! @brief constructs a CFirstLaunchPage object
CFirstLaunchPage::CFirstLaunchPage(void):
CBetterPropPage(IDD_PAGE_FIRST_LAUNCH),
m_nSmtpPort(IPPORT_SMTP)
{
	m_psp.dwFlags |= PSP_PREMATURE;

	::SHGetSpecialFolderPath(NULL, m_strAppDataPath.GetBuffer(_MAX_PATH), CSIDL_APPDATA, TRUE);
	m_strAppDataPath.ReleaseBuffer();
#if defined(UPDATE_IT_PRO)
	m_strAppDataPath += _T("\\Elijah Zarezky\\UpdateItPro\\");
#else
	m_strAppDataPath += _T("\\Elijah Zarezky\\UpdateIt\\");
#endif   // UPDATE_IT_PRO
}

CFirstLaunchPage::~CFirstLaunchPage(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

//! This member function is called in by the framework response to the WM_INITDIALOG
//! message. Its implementation first invokes the inherited method. Then it assigns
//! the big and small icons to the wizard's main window; obtains and shows version
//! info; and finally setups hyper-links.
//! @brief responses to the WM_INITDIALOG message
//! @return TRUE to direct the system to set the keyboard focus to the first control
//! in the page.
BOOL CFirstLaunchPage::OnInitDialog(void)
{
	__super::OnInitDialog();

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	HICON hInfoIcon = pApp->LoadStandardIcon(IDI_INFORMATION);
	m_iconInfo.SetIcon(hInfoIcon);

	CString strIniPath(m_strAppDataPath);
	strIniPath += INI_FILE_NAME;
	CString strGuarantee;
	strGuarantee.Format(IDS_EMAIL_GUARANTEE_FORMAT, static_cast<LPCTSTR>(strIniPath));
	m_textEmailGuarantee.SetWindowText(strGuarantee);
	m_textEmailGuarantee.SetTextColor(RGB(0, 128, 0));

	// warning RC4206: title string too long; truncated at 256
	CString strEmailInfo(MAKEINTRESOURCE(IDS_EMAIL_INFO));
	m_textEmailInfo.SetWindowText(strEmailInfo);

	return (TRUE);
}

//! This member function is called by the framework when the page is chosen by the
//! user and becomes the active page. Its implementation first invokes the inherited
//! method and then configures the wizard's buttons (via SetWizardButtons call).
//! @brief called when the page becomes the active page
//! @return nonzero if the page was successfully set active; otherwise 0.
BOOL CFirstLaunchPage::OnSetActive(void)
{
	BOOL fSuccess = __super::OnSetActive();
	if (fSuccess)
	{
		CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
		ASSERT(pWiz != NULL);
		pWiz->SetWizardButtons(PSWIZB_NEXT);
	}
	return (fSuccess);
}

BOOL CFirstLaunchPage::OnKillActive(void)
{
	BOOL fSuccess = __super::OnKillActive();
	if (fSuccess)
	{
		CString strIniPath(m_strAppDataPath);
		strIniPath += INI_FILE_NAME;
		CIniAppSettings iniSettings(strIniPath);

		if (!m_strEmailAddress.IsEmpty())
		{
			iniSettings.WriteString(SZ_REGK_SMTP, SZ_REGV_SMTP_FROM, m_strEmailAddress);
		}
		
		if (!m_strSmtpServer.IsEmpty())
		{
			iniSettings.WriteString(SZ_REGK_SMTP, SZ_REGV_SMTP_HOST, m_strSmtpServer);
		}
		else {
		}

		iniSettings.WriteInt(SZ_REGK_SMTP, SZ_REGV_SMTP_PORT, m_nSmtpPort);
	}
	return (fSuccess);
}

//! This member function is called by the framework to exchange and validate page
//! data. Its implementation first invokes the inherited method. Then it associates
//! each of dialog box control with a corresponding member variable (via DDX calls).
//! @brief exchanges and validates page data
//! @param pDX a pointer to a CDataExchange object.
void CFirstLaunchPage::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ICON_INFO, m_iconInfo);
	DDX_Control(pDX, IDC_TEXT_EMAIL_GUARANTEE, m_textEmailGuarantee);
	DDX_Control(pDX, IDC_TEXT_EMAIL_INFO, m_textEmailInfo);

	DDX_Text(pDX, IDC_EDIT_CRASH_FROM, m_strEmailAddress);
	DDX_Text(pDX, IDC_EDIT_CRASH_SMTP, m_strSmtpServer);
	DDXV_Word(pDX, IDC_EDIT_CRASH_PORT, m_nSmtpPort, 1, INTERNET_MAX_PORT_NUMBER_VALUE);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CFirstLaunchPage::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify own state as well
	ASSERT_VALID(&m_iconInfo);
	ASSERT_VALID(&m_textEmailGuarantee);
	ASSERT_VALID(&m_textEmailInfo);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CFirstLaunchPage::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_strAppDataPath = " << m_strAppDataPath;
		dumpCtx << "\nm_iconInfo = " << m_iconInfo;
		dumpCtx << "\nm_textEmailGuarantee = " << m_textEmailGuarantee;
		dumpCtx << "\nm_textEmailInfo = " << m_textEmailInfo;
		dumpCtx << "\nm_strEmailAddress = " << m_strEmailAddress;
		dumpCtx << "\nm_strSmtpServer = " << m_strSmtpServer;
		dumpCtx << "\nm_nSmtpPort = " << m_nSmtpPort;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
