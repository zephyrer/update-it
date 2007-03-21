// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// OptionsPage.cpp - implementation of the COptionsPage class

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

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(COptionsPage, CBetterPropPage)

// message map
BEGIN_MESSAGE_MAP(COptionsPage, CBetterPropPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SOURCE, OnButtonSource)
	ON_BN_CLICKED(IDC_BUTTON_TARGET, OnButtonTarget)
	ON_BN_CLICKED(IDC_CHECK_CLEANUP, OnCheckCleanup)
END_MESSAGE_MAP()

COptionsPage::COptionsPage(void):
CBetterPropPage(IDD_PAGE_OPTIONS)
{
	m_psp.dwFlags |= PSP_PREMATURE;
	CWinApp* pApp = AfxGetApp();
	m_strSource = pApp->GetProfileString(_T("Options"), _T("Source"));
	m_nRecurse = pApp->GetProfileInt(_T("Options"), _T("Recurse"), BST_CHECKED);
	m_strExclude = pApp->GetProfileString(_T("Options"), _T("Exclude"));
	m_strTarget = pApp->GetProfileString(_T("Options"), _T("Target"));
	m_nCleanup = pApp->GetProfileInt(_T("Options"), _T("Cleanup"), BST_CHECKED);
	if (m_nCleanup == BST_CHECKED) {
		m_nRecycle = pApp->GetProfileInt(_T("Options"), _T("Recycle"), BST_UNCHECKED);
	}
	else {
		m_nRecycle = BST_UNCHECKED;
	}
	m_timeWrite = m_strSource.IsEmpty() ? -1 : pApp->GetProfileInt(_T("Times"), m_strSource, -1);
	m_fCompare = pApp->GetProfileInt(_T("Options"), _T("Compare"), BST_UNCHECKED);
}

BOOL COptionsPage::OnInitDialog(void)
{
	// invoke inherited handler
	BOOL fResult = CBetterPropPage::OnInitDialog();

	// setup controls as needed
	m_dtpWrite.SetFormat(_T("dd.MM.yyyy   HH:mm:ss"));
	long tz_delta = _timezone / 3600;
	CTime timeMin(1970, 1, 1, 0 - tz_delta, 0, 0);
	CTime timeMax(2038, 1, 18, 0 - tz_delta, 0, 0);
	m_dtpWrite.SetRange(&timeMin, &timeMax);
	if (m_timeWrite != -1) {
		m_dtpWrite.SetTime(&m_timeWrite);
	}

	// assign tool tips
	CToolTipCtrl& tipWnd = GetToolTipCtrl();
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_SOURCE));
	tipWnd.AddTool(GetDlgItem(IDC_BUTTON_SOURCE));
	tipWnd.AddTool(GetDlgItem(IDC_CHECK_RECURSE));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_EXCLUDE));
	tipWnd.AddTool(GetDlgItem(IDC_EDIT_TARGET));
	tipWnd.AddTool(GetDlgItem(IDC_BUTTON_TARGET));
	tipWnd.AddTool(GetDlgItem(IDC_CHECK_CLEANUP));
	tipWnd.AddTool(GetDlgItem(IDC_CHECK_RECYCLE));
	tipWnd.AddTool(GetDlgItem(IDC_DTP_WRITE));
	tipWnd.AddTool(GetDlgItem(IDC_CHECK_COMPARE));
	tipWnd.Activate(TRUE);

	// initialized
	return (fResult);
}

BOOL COptionsPage::OnSetActive(void)
{
	BOOL fSuccess = CBetterPropPage::OnSetActive();
	if (fSuccess) {
		GetDlgItem(IDC_CHECK_RECYCLE)->EnableWindow(m_nCleanup == BST_CHECKED);
		CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
		ASSERT(pWiz != NULL);
		pWiz->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	}
	return (fSuccess);
}

BOOL COptionsPage::OnKillActive(void)
{
	BOOL fSuccess = CBetterPropPage::OnKillActive();
	if (fSuccess) {
		CWinApp* pApp = AfxGetApp();
		pApp->WriteProfileString(_T("Options"), _T("Source"), m_strSource);
		pApp->WriteProfileInt(_T("Options"), _T("Recurse"), m_nRecurse);
		pApp->WriteProfileString(_T("Options"), _T("Exclude"), m_strExclude);
		pApp->WriteProfileString(_T("Options"), _T("Target"), m_strTarget);
		pApp->WriteProfileInt(_T("Options"), _T("Cleanup"), m_nCleanup);
		pApp->WriteProfileInt(_T("Options"), _T("Recycle"), m_nRecycle);
		pApp->WriteProfileInt(_T("Options"), _T("Compare"), m_fCompare);
	}
	return (fSuccess);
}

LRESULT COptionsPage::OnWizardNext(void)
{
	CWnd* pWnd;

	pWnd = GetDlgItem(IDC_EDIT_SOURCE);
	ASSERT(pWnd != NULL);
	if (pWnd->GetWindowTextLength() == 0) {
		AfxMessageBox(IDS_NO_SOURCE, MB_ICONSTOP | MB_OK);
		pWnd->SetFocus();
		return (-1);
	}
	pWnd = GetDlgItem(IDC_EDIT_TARGET);
	ASSERT(pWnd != NULL);
	if (pWnd->GetWindowTextLength() == 0) {
		AfxMessageBox(IDS_NO_TARGET, MB_ICONSTOP | MB_OK);
		pWnd->SetFocus();
		return (-1);
	}
	m_dtpWrite.GetTime(m_timeWrite);
	return (CBetterPropPage::OnWizardNext());
}

void COptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CBetterPropPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SOURCE, m_strSource);
	DDV_MaxChars(pDX, m_strSource, _MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_EXCLUDE, m_strExclude);
	DDX_Check(pDX, IDC_CHECK_RECURSE, m_nRecurse);
	DDX_Text(pDX, IDC_EDIT_TARGET, m_strTarget);
	DDV_MaxChars(pDX, m_strTarget, _MAX_PATH);
	DDX_Check(pDX, IDC_CHECK_CLEANUP, m_nCleanup);
	DDX_Check(pDX, IDC_CHECK_RECYCLE, m_nRecycle);
	DDX_Control(pDX, IDC_DTP_WRITE, m_dtpWrite);
	DDX_Check(pDX, IDC_CHECK_COMPARE, m_fCompare);
}

HBRUSH COptionsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT uCtlColor)
{
	HBRUSH hbr;

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_EDIT_SOURCE:
	case IDC_EDIT_TARGET:
		if (uCtlColor == CTLCOLOR_STATIC) {
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			hbr = ::GetSysColorBrush(COLOR_WINDOW);
			break;
		}
	default:
		hbr = CBetterPropPage::OnCtlColor(pDC, pWnd, uCtlColor);
	}
	return (hbr);
}

void COptionsPage::OnButtonSource(void)
{
	CString strPrompt;

	strPrompt.LoadString(IDS_CHOOSE_SOURCE);
	CFolderDialog dlgFolder(strPrompt, m_strSource, this);
	if (dlgFolder.DoModal() == IDOK) {
		m_strSource = dlgFolder.GetFolderPath();
		SetDlgItemText(IDC_EDIT_SOURCE, m_strSource);
		CWinApp* pApp = AfxGetApp();
		if ((m_timeWrite = pApp->GetProfileInt(_T("Times"), m_strSource, -1)) != -1) {
			m_dtpWrite.SetTime(&m_timeWrite);
		}
		CString strDefTarget = m_strSource + _T(".Update");
		SetDlgItemText(IDC_EDIT_TARGET, pApp->GetProfileString(_T("Targets"), m_strSource, strDefTarget));
	}
}

void COptionsPage::OnButtonTarget(void)
{
	CString strPrompt;

	strPrompt.LoadString(IDS_CHOOSE_TARGET);
	CFolderDialog dlgFolder(strPrompt, m_strTarget, this);
	if (dlgFolder.DoModal() == IDOK) {
		m_strTarget = dlgFolder.GetFolderPath();
		SetDlgItemText(IDC_EDIT_TARGET, m_strTarget);
	}
}

void COptionsPage::OnCheckCleanup(void)
{
	CWnd* pCheckRecycle = GetDlgItem(IDC_CHECK_RECYCLE);
	ASSERT(pCheckRecycle != NULL);
	if (IsDlgButtonChecked(IDC_CHECK_CLEANUP) == BST_CHECKED) {
		pCheckRecycle->EnableWindow();
	}
	else {
		CheckDlgButton(IDC_CHECK_RECYCLE, BST_UNCHECKED);
		pCheckRecycle->EnableWindow(FALSE);
	}
}

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void COptionsPage::AssertValid(void) const
{
	// first perform inherited validity check...
	CBetterPropPage::AssertValid();
	// ...and then verify own state as well
	ASSERT_VALID(&m_dtpWrite);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void COptionsPage::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CBetterPropPage::Dump(dumpCtx);
		// ...and then dump own unique members
		dumpCtx << "m_strSource = " << m_strSource;
		dumpCtx << "\nm_nRecurse = " << m_nRecurse;
		dumpCtx << "\nm_strExclude = " << m_strExclude;
		dumpCtx << "\nm_strTarget = " << m_strTarget;
		dumpCtx << "\nm_nCleanup = " << m_nCleanup;
		dumpCtx << "\nm_nRecycle = " << m_nRecycle;
		dumpCtx << "\nm_dtpWrite = " << m_dtpWrite;
		dumpCtx << "\nm_timeWrite = " << m_timeWrite;
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
