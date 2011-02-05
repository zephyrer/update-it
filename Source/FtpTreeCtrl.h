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

// FtpTreeCtrl.h - interface of the CFtpTreeCtrl class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__FtpTreeCtrl_h)
#define __FtpTreeCtrl_h

//////////////////////////////////////////////////////////////////////////////////////////////
// CFtpTreeCtrl

class CFtpTreeCtrl: public CTreeCtrl
{
	DECLARE_DYNAMIC(CFtpTreeCtrl)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CFtpTreeCtrl(UINT idcStatusText = 0);
	virtual ~CFtpTreeCtrl(void);

// operations
public:
	BOOL Connect(LPCTSTR pszServer, LPCTSTR pszLogin, LPCTSTR pszPassword, INTERNET_PORT nPort, BOOL fPassive);
	void Disconnect(void);
	HTREEITEM InsertRootItem(LPCTSTR pszRootText = _T("/"));

// message map functions
protected:
	afx_msg void OnItemExpanding(NMHDR* pHdr, LRESULT* pnResult);
	afx_msg void OnSelChanged(NMHDR* pHdr, LRESULT* pnResult);

// attributes
public:
	CInternetSession m_ftpSession;
	ATL::CAutoPtr<CFtpConnection> m_ptrFtpConn;
	UINT m_idcStatusText;
	CImageList m_imageList;
	int m_iFolderImg;
	int m_iFolderOpenImg;
	int m_iFtpRootImg;

// implementation helpers
private:
	void SetStatusText(LPCTSTR pszText);
	void SearchForFolders(HTREEITEM hParentItem);
	CString GetCurrentPath(void);
	CString GetItemPath(HTREEITEM hItem);
	void DeleteChildItems(HTREEITEM hParentItem);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

//////////////////////////////////////////////////////////////////////////////////////////////
// inlines

inline CString CFtpTreeCtrl::GetCurrentPath(void)
{
	return (GetItemPath(GetSelectedItem()));
}

#endif   // __FtpTreeCtrl_h

// end of file
