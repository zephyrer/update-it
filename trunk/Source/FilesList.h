// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

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
