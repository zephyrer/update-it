// UpdateIt! application.
// Copyright (c) 2002-2007 by Elijah Zarezky,
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

// ZipOptionsDialog.cpp - implementation of the CZipOptionsDialog class

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
#include "ZipOptionsDialog.h"
#include "BetterPropPage.h"
#include "AboutPage.h"
#include "OptionsPage.h"
#include "FilesList.h"
#include "FilesPage.h"
#include "AuthenticationDialog.h"
#include "ActionPage.h"
#include "ProgressPageBase.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"
#include "UpdateItApp.h"
#include "Registry.h"
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

IMPLEMENT_DYNAMIC(CZipOptionsDialog, CCustomDialog)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CZipOptionsDialog, CCustomDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ENCR_METHOD, OnComboEncrMethodSelChange)
	ON_EN_CHANGE(IDC_EDIT_ZIP_PASSWORD, OnEditPasswordChange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CZipOptionsDialog::CZipOptionsDialog(CWnd* pParentWnd):
CCustomDialog(IDD_ZIP_OPTIONS, pParentWnd),
m_iComprLevel(I_LEVEL_DEFAULT),
m_iEncrMethod(I_METHOD_NONE)
{
	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	// initialize and validate initial input values

	CArgsParser& argsParser = pApp->m_argsParser;

	// "Compression level"
	if (!argsParser.GetIntValue(SZ_ARG_ZIP_COMPRESSION, m_iComprLevel, 10))
	{
		m_iComprLevel = pApp->GetProfileInt(SZ_REGK_ZIP, SZ_REGV_ZIP_COMPR_LEVEL, I_LEVEL_DEFAULT);
	}
	if (m_iComprLevel < I_LEVEL_STORE || m_iComprLevel > I_LEVEL_BEST)
	{
		m_iComprLevel = I_LEVEL_DEFAULT;
	}

	// "Encryption method"
	if (!argsParser.GetIntValue(SZ_ARG_ZIP_ENCRYPTION, m_iEncrMethod, 10))
	{
		m_iEncrMethod = pApp->GetProfileInt(SZ_REGK_ZIP, SZ_REGV_ZIP_ENCR_METHOD, I_METHOD_NONE);
	}
	if (m_iEncrMethod < I_METHOD_NONE || m_iEncrMethod > I_METHOD_AES_256)
	{
		m_iEncrMethod = I_METHOD_NONE;
	}

	// "Password"
	if (!argsParser.HasKey(SZ_ARG_ZIP_PASSWORD))
	{
		m_strPassword = pApp->GetProfilePassword(SZ_REGK_ZIP, SZ_REGV_ZIP_PASSWORD);
	}
	else {
		m_strPassword = argsParser.GetStringValue(SZ_ARG_ZIP_PASSWORD);
	}
}

CZipOptionsDialog::~CZipOptionsDialog(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CZipOptionsDialog::OnInitDialog(void)
{
	__super::OnInitDialog();

#if !defined(_UPDATE_IT_PRO)
	while (m_comboEncrMethod.GetCount() > I_FIRST_PRO_METHOD)
	{
		m_comboEncrMethod.DeleteString(I_FIRST_PRO_METHOD);
	}
	if (m_iEncrMethod >= I_FIRST_PRO_METHOD)
	{
		m_iEncrMethod = I_METHOD_NONE;
	}
#endif   // _UPDATE_IT_PRO

	UpdateControls();

	return (TRUE);
}

void CZipOptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_ZIP_LEVEL, m_comboComprLevel);
	DDX_Control(pDX, IDC_COMBO_ENCR_METHOD, m_comboEncrMethod);
	DDX_Control(pDX, IDC_TEXT_ZIP_PASSWORD, m_textPassword);
	DDX_Control(pDX, IDC_EDIT_ZIP_PASSWORD, m_editPassword);

	DDX_CBIndex(pDX, IDC_COMBO_ZIP_LEVEL, m_iComprLevel);
	DDX_CBIndex(pDX, IDC_COMBO_ENCR_METHOD, m_iEncrMethod);
	DDX_Text(pDX, IDC_EDIT_ZIP_PASSWORD, m_strPassword);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

void CZipOptionsDialog::OnComboEncrMethodSelChange(void)
{
	UpdateControls();
}

void CZipOptionsDialog::OnEditPasswordChange(void)
{
	UpdateControls();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

void CZipOptionsDialog::UpdateControls(void)
{
	// precondition
	ASSERT(::IsWindow(m_hWnd));

	CWnd* pButtonOK = GetDlgItem(IDOK);
	ASSERT_VALID(pButtonOK);

	int iEncrMethod = m_comboEncrMethod.GetCurSel();

	if (iEncrMethod != I_METHOD_NONE)
	{
		m_textPassword.EnableWindow(TRUE);
		m_editPassword.EnableWindow(TRUE);
		pButtonOK->EnableWindow(m_editPassword.GetWindowTextLength() > 0);
	}
	else {
		m_textPassword.EnableWindow(FALSE);
		m_editPassword.EnableWindow(FALSE);
		pButtonOK->EnableWindow(TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CZipOptionsDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
	ASSERT_VALID(&m_comboComprLevel);
	ASSERT_VALID(&m_comboEncrMethod);
	ASSERT_VALID(&m_textPassword);
	ASSERT_VALID(&m_editPassword);
}

void CZipOptionsDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_comboComprLevel = " << m_comboComprLevel;
		dumpCtx << "\nm_comboEncrMethod = " << m_comboEncrMethod;
		dumpCtx << "\nm_textPassword" << m_textPassword;
		dumpCtx << "\nm_editPassword" << m_editPassword;
		dumpCtx << "\nm_iComprLevel" << m_iComprLevel;
		dumpCtx << "\nm_iEncrMethod" << m_iEncrMethod;
		dumpCtx << "\nm_strPassword" << m_strPassword;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
