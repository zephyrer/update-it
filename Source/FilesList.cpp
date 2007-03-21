// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// FilesList.cpp - implementation of the CFilesList class

#include "stdafx.h"
#include "FilesList.h"
#include "Resource.h"

#if defined(__INTEL_COMPILER)
// remark #171: invalid type conversion
#pragma warning(disable: 171)
// remark #279: controlling expression is constant
#pragma warning(disable: 279)
// remark #981: operands are evaluated in unspecified order
#pragma warning(disable: 981)
#endif	// __INTEL_COMPILER

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CFilesList, CSortingListCtrl)

// message map
BEGIN_MESSAGE_MAP(CFilesList, CSortingListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
END_MESSAGE_MAP()

CFilesList::CFilesList(void)
{
}

CFilesList::~CFilesList(void)
{
}

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

int CFilesList::CompareItems(int iItemLhs, int iItemRhs)
{
	FILE_DATA* pDataLhs = reinterpret_cast<FILE_DATA*>(GetItemData(iItemLhs));
	ASSERT(pDataLhs != NULL);
	FILE_DATA* pDataRhs = reinterpret_cast<FILE_DATA*>(GetItemData(iItemRhs));
	ASSERT(pDataRhs != NULL);

	switch (m_iSortColumn)
	{
	case I_NAME:
		return (::lstrcmpi(pDataLhs->szName, pDataRhs->szName) * m_nSortOrder);
	case I_EXTENSION:
		return (::lstrcmpi(pDataLhs->szExt, pDataRhs->szExt) * m_nSortOrder);
	case I_PATH:
		return (::lstrcmpi(pDataLhs->szFolder, pDataRhs->szFolder) * m_nSortOrder);
	case I_SIZE:
		return ((pDataLhs->cbLength - pDataRhs->cbLength) * m_nSortOrder);
	case I_DATE:
	case I_TIME:
		if (pDataLhs->timeWrite < pDataRhs->timeWrite) {
			return (m_nSortOrder);
		}
		else if (pDataLhs->timeWrite > pDataRhs->timeWrite) {
			return (-m_nSortOrder);
		}
		// fall through
	default:
		return (0);
	}
}

void CFilesList::OnGetDispInfo(NMHDR* pHdr, LRESULT* /*pnResult*/)
{
	SYSTEMTIME st;
	CString strFormat;
	CString strSize;

	LVITEM& lvi = reinterpret_cast<NMLVDISPINFO*>(pHdr)->item;
	if ((lvi.mask & LVIF_TEXT) != 0) {
		FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(GetItemData(lvi.iItem));
		switch (lvi.iSubItem)
		{
		case I_NAME:
			::lstrcpyn(lvi.pszText, pData->szName, lvi.cchTextMax);
			break;
		case I_EXTENSION:
			::lstrcpyn(lvi.pszText, pData->szExt, lvi.cchTextMax);
			break;
		case I_PATH:
			::lstrcpyn(lvi.pszText, pData->szFolder, lvi.cchTextMax);
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
			_ultot(pData->cbLength, strSize.GetBuffer(32), 10);
			strSize.ReleaseBuffer();
			SeparateThousands(strSize);
			::lstrcpyn(lvi.pszText, strSize, lvi.cchTextMax);
			break;
		default:
			*lvi.pszText = 0;
			break;
		}
	}
}

void CFilesList::SeparateThousands(CString& strNumber)
{
	if (!strNumber.IsEmpty()) {
		struct lconv* pLConv = localeconv();
		TCHAR chrSep = *_A2T(pLConv->thousands_sep);
		if (chrSep != 0) {
			int iStart = strNumber.Find(*_A2T(pLConv->decimal_point));
			if (iStart < 0) {
				iStart = strNumber.GetLength();
			}
			int iStop = strNumber[0] == *_A2T(pLConv->negative_sign) ||
				strNumber[0] == *_A2T(pLConv->positive_sign) ? 1 : 0;
			while ((iStart -= *pLConv->grouping) > iStop) {
				strNumber.Insert(iStart, chrSep);
			}
		}
	}
}

#if defined(_DEBUG)

void CFilesList::AssertValid(void) const
{
	// first perform inherited validity check...
	CSortingListCtrl::AssertValid();
	// ...and then verify our own state as well
}

void CFilesList::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CSortingListCtrl::Dump(dumpCtx);
		// ...and then dump own unique members
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
