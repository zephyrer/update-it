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

// FtpManagerDialog.h - interface of the CFtpManagerDialog class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__FtpManagerDialog_h)
#define __FtpManagerDialog_h

//////////////////////////////////////////////////////////////////////////////////////////////
// dependent includes

#include "CustomDialog.h"
#include "FtpPropertiesDialog.h"
#include "WindowsVersion.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CFtpManagerDialog

class CFtpManagerDialog: public CCustomDialog
{
	DECLARE_DYNAMIC(CFtpManagerDialog)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CFtpManagerDialog(CWnd* pParentWnd = NULL);
	virtual ~CFtpManagerDialog(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK(void);

// message map functions
protected:
	afx_msg void OnListItemChanged(NMHDR* pHdr, LRESULT* pnResult);
	afx_msg void OnButtonEdit(void);
	afx_msg void OnButtonRemove(void);

// attributes
public:
	enum
	{
		// column indices
		I_NAME = 0,
		I_COMMENT,
		NUM_COLUMNS   // should be the LAST enumerator!
	};

	struct SITE_DATA
	{
		TCHAR szName[CFtpPropertiesDialog::MAX_FTP_NAME_LENGTH];
		TCHAR szComment[CFtpPropertiesDialog::MAX_FTP_COMMENT_LENGTH];
		TCHAR szServer[CFtpPropertiesDialog::MAX_FTP_SERVER_LENGTH];
		INTERNET_PORT nPort;
		TCHAR szLogin[CFtpPropertiesDialog::MAX_FTP_LOGIN_LENGTH];
		TCHAR szPassword[CFtpPropertiesDialog::MAX_FTP_PASSWORD_LENGTH];
		TCHAR szRoot[_MAX_PATH];
		BOOL fPassive;
	};

	CArray<SITE_DATA, SITE_DATA&> m_arrData;
	int m_iCurItem;
	BOOL GetSiteData(SITE_DATA* pSiteData) const;

	CImageList m_imageList;
	int m_iFtpRootImg;
	CWindowsVersion m_winVer;

	// controls
	CListCtrl m_listSites;

// implementation helpers
private:
	INT_PTR RegQueryData(void);
	int PutDataToList(void);
	void UpdateControls(void);

// properties
private:
	UINT GetFtpRootIcoID(void);
	__declspec(property(get = GetFtpRootIcoID)) UINT m_idrFtpRoot;

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

//////////////////////////////////////////////////////////////////////////////////////////////
// inlines

__forceinline BOOL CFtpManagerDialog::GetSiteData(SITE_DATA* pSiteData) const
{
	if (pSiteData != NULL && m_iCurItem >= 0 && m_iCurItem < m_arrData.GetCount())
	{
		*pSiteData = m_arrData[m_iCurItem];
		return (TRUE);
	}
	else
	{
		return (FALSE);
	}
}

inline UINT CFtpManagerDialog::GetFtpRootIcoID(void)
{
	return (m_winVer.WinNT() < _WIN32_WINNT_VISTA ? IDI_FTP_ROOT_XP : IDI_FTP_ROOT_VISTA);
}

#endif   // __FtpManagerDialog_h

// end of file
