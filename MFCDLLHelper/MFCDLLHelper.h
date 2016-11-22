// MFCDLLHelper.h : main header file for the MFCDLLHelper DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCDLLHelperApp
// See MFCDLLHelper.cpp for the implementation of this class
//

class CMFCDLLHelperApp : public CWinApp
{
public:
	CMFCDLLHelperApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
