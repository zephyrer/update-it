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

// FtpPropertiesDialog.cpp - implementation of the CFtpPropertiesDialog class

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"
#include "../Languages/English_USA.1252/Source/Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "FtpPropertiesDialog.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CFtpPropertiesDialog, CCustomDialog)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CFtpPropertiesDialog, CCustomDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CFtpPropertiesDialog::CFtpPropertiesDialog(UINT idsCaption, CWnd* pParentWnd):
CCustomDialog(IDD_FTP_PROPERTIES, pParentWnd),
m_idsCaption(idsCaption),
m_nPort(INTERNET_DEFAULT_FTP_PORT),
m_strRoot(_T("/")),
m_fPassive(FALSE)
{
}

CFtpPropertiesDialog::~CFtpPropertiesDialog(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CFtpPropertiesDialog::OnInitDialog(void)
{
	__super::OnInitDialog();

	if (m_idsCaption != 0)
	{
		CString strCaption(MAKEINTRESOURCE(m_idsCaption));
		SetWindowText(strCaption);
	}

	return (TRUE);
}

void CFtpPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_FTP_NAME, m_strName);
	DDV_MinMaxChars(pDX, m_strName, MIN_FTP_NAME_LENGTH, MAX_FTP_NAME_LENGTH);
	DDX_Text(pDX, IDC_EDIT_FTP_COMMENT, m_strComment);
	DDV_MaxChars(pDX, m_strComment, MAX_FTP_COMMENT_LENGTH);
	DDX_Text(pDX, IDC_EDIT_FTP_SERVER, m_strServer);
	DDV_MinMaxChars(pDX, m_strServer, MIN_FTP_SERVER_LENGTH, MAX_FTP_SERVER_LENGTH);
	DDXV_Word(pDX, IDC_EDIT_FTP_PORT, m_nPort, 1, INTERNET_MAX_PORT_NUMBER_VALUE);
	DDX_Text(pDX, IDC_EDIT_FTP_LOGIN, m_strLogin);
	DDV_MaxChars(pDX, m_strLogin, MAX_FTP_LOGIN_LENGTH);
	DDX_Text(pDX, IDC_EDIT_FTP_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, MAX_FTP_PASSWORD_LENGTH);
	DDX_Text(pDX, IDC_EDIT_FTP_ROOT, m_strRoot);
	DDV_MaxChars(pDX, m_strRoot, _MAX_PATH);
	DDX_Check(pDX, IDC_CHECK_FTP_PASSIVE, m_fPassive);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CFtpPropertiesDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
}

void CFtpPropertiesDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_idsCaption = " << m_idsCaption;
		dumpCtx << "\nm_strName = " << m_strName;
		dumpCtx << "\nm_strComment = " << m_strComment;
		dumpCtx << "\nm_strServer = " << m_strServer;
		dumpCtx << "\nm_nPort = " << m_nPort;
		dumpCtx << "\nm_strLogin = " << m_strLogin;
		dumpCtx << "\nm_strPassword = " << m_strPassword;
		dumpCtx << "\nm_strRoot = " << m_strRoot;
		dumpCtx << "\nm_fPassive = " << m_fPassive;
	}
	catch (CFileException* pErr)
	{
		pErr->ReportError();
		pErr->Delete();
	}
}

#endif   // _DEBUG

// end of file
