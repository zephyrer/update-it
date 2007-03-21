// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// AboutPage.h - interface of the CAboutPage class

#if !defined(__AboutPage_h)
#define __AboutPage_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

//! Encapsulates the "Weclome!" step of the UpdateIt! wizard.
class CAboutPage: public CBetterPropPage
{
	DECLARE_DYNAMIC(CAboutPage)
	DECLARE_MESSAGE_MAP()

// construction
public:
	CAboutPage(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
	virtual BOOL OnSetActive(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// attributes
public:
	CHyperLink m_linkZarezky;
	CHyperLink m_linkZLib;
	CHyperLink m_linkArtpol;
	CHyperLink m_linkRSA;
	CHyperLink m_linkNaughter;

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __AboutPage_h

// end of file
