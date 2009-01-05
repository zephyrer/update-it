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

// FilesList.cpp - implementation of the CFilesList class

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"
#include "../Languages/English_USA.1252/Source/Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "FilesList.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// avoid unwanted ICL warnings

#if defined(__INTEL_COMPILER)
// remark #171: invalid type conversion
#pragma warning(disable: 171)
// remark #279: controlling expression is constant
#pragma warning(disable: 279)
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

IMPLEMENT_DYNAMIC(CFilesList, CSortingListCtrl)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CFilesList, CSortingListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CFilesList::CFilesList(void)
{
}

CFilesList::~CFilesList(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// operations

void CFilesList::InsertColumns(void)
{
	CRect rectClient;
	CString strHeading;

	int cxVScroll = ::GetSystemMetrics(SM_CXVSCROLL);
	GetClientRect(rectClient);
	int cxWidth = ((rectClient.Width() - cxVScroll) / NUM_COLUMNS);

	strHeading.LoadString(IDS_NAME);
	InsertColumn(I_NAME, strHeading, LVCFMT_LEFT, cxWidth, I_NAME);
	strHeading.LoadString(IDS_EXTENSION);
	InsertColumn(I_EXTENSION, strHeading, LVCFMT_LEFT, cxWidth, I_EXTENSION);
	strHeading.LoadString(IDS_PATH);
	InsertColumn(I_PATH, strHeading, LVCFMT_LEFT, cxWidth, I_PATH);
	strHeading.LoadString(IDS_DATE);
	InsertColumn(I_DATE, strHeading, LVCFMT_LEFT, cxWidth, I_DATE);
	strHeading.LoadString(IDS_TIME);
	InsertColumn(I_TIME, strHeading, LVCFMT_LEFT, cxWidth, I_TIME);
	strHeading.LoadString(IDS_SIZE);
	InsertColumn(I_SIZE, strHeading, LVCFMT_RIGHT, cxWidth, I_SIZE);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

int CFilesList::CompareItems(int iItemLhs, int iItemRhs)
{
#if (_MFC_VER >= 0x0700)
	__int64 nDelta;
#endif	// _MFC_VER

	FILE_DATA* pDataLhs = reinterpret_cast<FILE_DATA*>(GetItemData(iItemLhs));
	ASSERT(pDataLhs != NULL);
	FILE_DATA* pDataRhs = reinterpret_cast<FILE_DATA*>(GetItemData(iItemRhs));
	ASSERT(pDataRhs != NULL);

	switch (m_iSortColumn)
	{
	case I_NAME:
		return (_tcsicmp(pDataLhs->szName, pDataRhs->szName) * m_nSortOrder);
	case I_EXTENSION:
		return (_tcsicmp(pDataLhs->szExt, pDataRhs->szExt) * m_nSortOrder);
	case I_PATH:
		return (_tcsicmp(pDataLhs->szFolder, pDataRhs->szFolder) * m_nSortOrder);
	case I_SIZE:
#if (_MFC_VER < 0x0700)
		return ((pDataLhs->cbLength - pDataRhs->cbLength) * m_nSortOrder);
#else
		nDelta = pDataLhs->cbLength - pDataRhs->cbLength;
		return (nDelta != 0 ? static_cast<int>(nDelta / _abs64(nDelta)) * m_nSortOrder : 0);
#endif	// _MFC_VER
	case I_DATE:
	case I_TIME:
		if (pDataLhs->timeWrite < pDataRhs->timeWrite)
		{
			return (m_nSortOrder);
		}
		else if (pDataLhs->timeWrite > pDataRhs->timeWrite)
		{
			return (-m_nSortOrder);
		}
		// fall through
	default:
		return (0);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

void CFilesList::OnGetDispInfo(NMHDR* pHdr, LRESULT* /*pnResult*/)
{
	SYSTEMTIME st;
	CString strFormat;
	CString strSize;

	LVITEM& lvi = reinterpret_cast<NMLVDISPINFO*>(pHdr)->item;
	if ((lvi.mask & LVIF_TEXT) != 0)
	{
		FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(GetItemData(lvi.iItem));
		switch (lvi.iSubItem)
		{
		case I_NAME:
			_tcsncpy(lvi.pszText, pData->szName, lvi.cchTextMax);
			break;
		case I_EXTENSION:
			_tcsncpy(lvi.pszText, pData->szExt, lvi.cchTextMax);
			break;
		case I_PATH:
			_tcsncpy(lvi.pszText, pData->szFolder, lvi.cchTextMax);
			break;
		case I_DATE:
			pData->timeWrite.GetAsSystemTime(st);
			strFormat.LoadString(IDS_DATE_FORMAT);
			::GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, strFormat, lvi.pszText, lvi.cchTextMax);
			break;
		case I_TIME:
			pData->timeWrite.GetAsSystemTime(st);
			strFormat.LoadString(IDS_TIME_FORMAT);
			::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, strFormat, lvi.pszText, lvi.cchTextMax);
			break;
		case I_SIZE:
#if (_MFC_VER < 0x0700)
			_ultot(pData->cbLength, strSize.GetBuffer(32), 10);
#else
			_ui64tot(pData->cbLength, strSize.GetBuffer(64), 10);
#endif	// _MFC_VER
			strSize.ReleaseBuffer();
			SeparateThousands(strSize);
			_tcsncpy(lvi.pszText, strSize, lvi.cchTextMax);
			break;
		default:
			*lvi.pszText = 0;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

void CFilesList::SeparateThousands(CString& strNumber)
{
	if (!strNumber.IsEmpty())
	{
		struct lconv* pLConv = localeconv();
		TCHAR chrSep = *_A2T(pLConv->thousands_sep);
		if (chrSep != 0)
		{
			int iStart = strNumber.Find(*_A2T(pLConv->decimal_point));
			if (iStart < 0)
			{
				iStart = strNumber.GetLength();
			}
			int iStop = strNumber[0] == *_A2T(pLConv->negative_sign) ||
				strNumber[0] == *_A2T(pLConv->positive_sign) ? 1 : 0;
			while ((iStart -= *pLConv->grouping) > iStop)
			{
				strNumber.Insert(iStart, chrSep);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CFilesList::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
}

void CFilesList::Dump(CDumpContext& dumpCtx) const
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
