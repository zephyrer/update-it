// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
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

// FilesList.h - interface of the CFilesList class

#if !defined(__FilesList_h)
#define __FilesList_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

class CFilesList: public CSortingListCtrl
{
	DECLARE_DYNAMIC(CFilesList)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CFilesList(void);
	virtual ~CFilesList(void);

// operations
public:
	void InsertColumns(void);

// overridables
protected:
	virtual int CompareItems(int iItemLhs, int iItemRhs);

// message map functions
protected:
	afx_msg void OnGetDispInfo(NMHDR* pHdr, LRESULT* pnResult);

// attributes
public:
	enum {
		// column indices
		I_NAME,
		I_EXTENSION,
		I_PATH,
		I_DATE,
		I_TIME,
		I_SIZE,
		NUM_COLUMNS		// should be the LAST enumerator!
	};

// implementation helpers
private:
	void SeparateThousands(CString& strNumber);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __FilesList_h

// end of file
