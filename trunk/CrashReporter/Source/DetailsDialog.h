// CrashReporter application.
// Copyright (c) 2009 by Elijah Zarezky,
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

// DetailsDialog.h  Version 1.0
//
// Author:          Hans Dietrich
//                  hdietrich2@hotmail.com
//
// Modified by:     Elijah Zarezky
//                  elijah.zarezky@gmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like, except
// that you may not sell this source code.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.

#ifndef __DETAILS_DIALOG_H
#define __DETAILS_DIALOG_H

#include "ROEdit.h"

struct FILEDETAILS
{
	FILEDETAILS()
	{
		strFilePath    = _T("");
		strDescription = _T("");
		strFileSize    = _T("");
		strType        = _T("");
		bSend          = TRUE;
	}

	CString strFilePath;
	CString strDescription;
	CString strFileSize;
	CString strType;
	BOOL    bSend;
};

/////////////////////////////////////////////////////////////////////////////
// CDetailsDialog dialog

class CDetailsDialog : public CDialog
{
// Construction
public:
	CDetailsDialog(CDWordArray * paFileDetails, CWnd* pParent = NULL);
	virtual ~CDetailsDialog();

// Dialog Data
	//{{AFX_DATA(CDetailsDialog)
	enum { IDD = IDD_XCRASHREPORT_DETAILS };
	CXColorStatic	m_Banner;
	CStatic			m_Name;
	CStatic			m_Desc;
	CROEdit			m_FileContents;
	CXListCtrl		m_FileList;
	//}}AFX_DATA

	CDWordArray *	m_paFileDetails;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailsDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_FileContentsFont;

	void DisplayFile(FILEDETAILS *fd);
	void DisplayHex(const char * lpBuf, DWORD dwSize);
	void FillList(CXListCtrl& list);
	void InitListCtrl(CXListCtrl& list);

	// Generated message map functions
	//{{AFX_MSG(CDetailsDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__DETAILS_DIALOG_H
