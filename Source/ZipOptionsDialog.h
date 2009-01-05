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

// ZipOptionsDialog.h - interface of the CZipOptionsDialog class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__ZipOptionsDialog_h)
#define __ZipOptionsDialog_h

class CZipOptionsDialog: public CCustomDialog
{
	DECLARE_DYNAMIC(CZipOptionsDialog)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CZipOptionsDialog(CWnd* pParentWnd = NULL);
	virtual ~CZipOptionsDialog(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// message map functions
protected:
	afx_msg void OnComboEncrMethodSelChange(void);
	afx_msg void OnEditPasswordChange(void);

// attributes
public:
	enum
	{
		// compression methods
		I_METHOD_DEFLATE = 0,
		I_METHOD_BZIP2,

		// compression levels
		I_LEVEL_STORE = 0,
		I_LEVEL_FASTEST,
		I_LEVEL_DEFAULT,
		I_LEVEL_BEST,

		// encryption methods
		I_METHOD_NONE = 0,
		I_METHOD_STANDARD,
		I_FIRST_PRO_METHOD,
		I_METHOD_AES_128 = I_FIRST_PRO_METHOD,
		I_METHOD_AES_192,
		I_METHOD_AES_256
	};

	// controls
	CComboBox m_comboComprMethod;
	CComboBox m_comboComprLevel;
	CComboBox m_comboEncrMethod;
	CStatic m_textPassword;
	CEdit m_editPassword;

	// user's input
	int m_iComprMethod;
	int m_iComprLevel;
	int m_iEncrMethod;
	CString m_strPassword;

// implementation helpers
private:
	void UpdateControls(void);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif   // __ZipOptionsDialog_h

// end of file
