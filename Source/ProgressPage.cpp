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

// ProgressPage.cpp - implementation of the CProgressPage class

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
#if (_MFC_VER >= 0x0700)
#include "UpdateItApp.h"
#endif	// _MFC_VER
#include "Registry.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// avoid unwanted ICL warnings

#if defined(__INTEL_COMPILER)
// remark #171: invalid type conversion
#pragma warning(disable: 171)
// remark #279: controlling expression is constant
#pragma warning(disable: 279)
// remark #383: value copied to temporary, reference to temporary used
#pragma warning(disable: 383)
// remark #797: a class-qualified name is not allowed
#pragma warning(disable: 797)
// remark #981: operands are evaluated in unspecified order
#pragma warning(disable: 981)
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

IMPLEMENT_DYNAMIC(CProgressPage, CProgressPageBase)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CProgressPage, CProgressPageBase)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction

CProgressPage::CProgressPage(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

void CProgressPage::ZipTargetFolder(LPCTSTR pszTarget, const CListCtrl& listFiles, LPCTSTR pszZipPath)
{
	CZipArchive zipArch;

	try
	{
		zipArch.SetCallback(this, CZipActionCallback::cbAdd);
		zipArch.Open(pszZipPath, CZipArchive::zipCreate);
		CString strTemp(pszTarget);
		zipArch.SetRootPath(strTemp.Left(strTemp.ReverseFind(_T('\\'))));

		CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
		ASSERT(pWiz != NULL);
		CActionPage* pActionPage = DYNAMIC_DOWNCAST(CActionPage, pWiz->GetPage(CMainWizard::I_ACTION));
		ASSERT(pActionPage != NULL);
		CZipOptionsDialog& dlgZipOpts = pActionPage->m_dlgZipOpts;

		int iEcncrMethod = dlgZipOpts.m_iEncrMethod;
		if (iEcncrMethod != CZipOptionsDialog::I_METHOD_NONE)
		{
			static CZipCryptograph::EncryptionMethod aeEncrMethods[] =
			{
				CZipCryptograph::encNone,
				CZipCryptograph::encStandard,
				CZipCryptograph::encWinZipAes128,
				CZipCryptograph::encWinZipAes192,
				CZipCryptograph::encWinZipAes256
			};
			zipArch.SetPassword(dlgZipOpts.m_strPassword);
			zipArch.SetEncryptionMethod(aeEncrMethods[iEcncrMethod]);
		}

		static CZipCompressor::CompressionLevel aeComprLevels[] =
		{
			CZipCompressor::levelStore,
			CZipCompressor::levelFastest,
			CZipCompressor::levelDefault,
			CZipCompressor::levelBest
		};

		int cFiles = listFiles.GetItemCount();

		for (int i = 0; i < cFiles; ++i)
		{
			CString strFilePath(pszTarget);
			if (strFilePath[strFilePath.GetLength() - 1] != _T('\\'))
			{
				strFilePath += _T('\\');
			}
			FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(listFiles.GetItemData(i));
			ASSERT(pData != NULL);
			if (pData->szFolder[0] != 0)
			{
				strFilePath += pData->szFolder;
				strFilePath += _T('\\');
			}
			strFilePath += pData->szName;
			strFilePath += _T('.');
			strFilePath += pData->szExt;
			zipArch.AddNewFile(strFilePath, aeComprLevels[dlgZipOpts.m_iComprLevel], false);
		}
		zipArch.Close();
	}
	catch (CZipException* pErr)
	{
		AfxMessageBox(pErr->GetErrorDescription(), MB_ICONSTOP | MB_OK);
		delete pErr;
		zipArch.Close(CZipArchive::afAfterException);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CZipActionCallback overridables

bool CProgressPage::Callback(ZIP_SIZE_TYPE uProgress)
{
	m_textTotal.SetWindowText(m_szFileInZip);
	m_progressTotal.OffsetPos(uProgress);
	PumpWaitingMessages();
	return (true);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CProgressPage::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify own state as well
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CProgressPage::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
