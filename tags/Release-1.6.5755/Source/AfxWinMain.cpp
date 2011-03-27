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

// AfxWinMain.cpp  Version 1.1
//
// Copyright © 1998 Bruce Dawson
//
// This module contains the boilerplate code that you need to add to any
// MFC app in order to wrap the main thread in an exception handler.
// AfxWinMain() in this source file replaces the AfxWinMain() in the MFC
// library.
//
// Author:       Bruce Dawson
//               brucedawson@cygnus-software.com
//
// Modified by:  Hans Dietrich
//               hdietrich2@hotmail.com
//
// Modified by:  Elijah Zarezky
//               elijah.zarezky@gmail.com
//
// A paper by the original author can be found at:
//     http://www.cygnus-software.com/papers/release_debugging.html

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "ExceptionHandler.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// This is the initial entry point into an MFC app. Normally this is in the
// MFC library:  mfc\src\winmain.cpp

int AFXAPI AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Wrap WinMain in a structured exception handler (different from C++
	// exception handling) in order to make sure that all access violations
	// and other exceptions are displayed - regardless of when they happen.
	// This should be done for each thread, if at all possible, so that exceptions
	// will be reliably caught, even inside the debugger.
	__try
	{
		TRACE(_T("in AfxWinMain.cpp - AfxWinMain\n"));

		// The code inside the __try block is the MFC version of AfxWinMain(),
		// copied verbatim from the MFC source code. See winmain.cpp for details.
		ASSERT(hPrevInstance == NULL);

		int nReturnCode = -1;
		CWinThread* pThread = AfxGetThread();
		CWinApp* pApp = AfxGetApp();

		// AFX internal initialization
		if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
			goto InitFailure;

		// App global initializations (rare)
		if (pApp != NULL && !pApp->InitApplication())
			goto InitFailure;

		// Perform specific initializations
		if (!pThread->InitInstance())
		{
			if (pThread->m_pMainWnd != NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: Destroying non-NULL m_pMainWnd\n");
				pThread->m_pMainWnd->DestroyWindow();
			}
			nReturnCode = pThread->ExitInstance();
			goto InitFailure;
		}
		nReturnCode = pThread->Run();

	InitFailure:
#ifdef _DEBUG
		// Check for missing AfxLockTempMap calls
		if (AfxGetModuleThreadState()->m_nTempMapLock != 0)
		{
			TRACE(traceAppMsg, 0, "Warning: Temp map lock count non-zero (%ld).\n",
				AfxGetModuleThreadState()->m_nTempMapLock);
		}
		AfxLockTempMaps();
		AfxUnlockTempMaps(-1);
#endif

		AfxWinTerm();
		return nReturnCode;
	}
	__except(RecordExceptionInfo(GetExceptionInformation(), _T("AfxWinMain.cpp - AfxWinMain")))
	{
		// Do nothing here - RecordExceptionInfo() has already done
		// everything that is needed. Actually this code won't even
		// get called unless you return EXCEPTION_EXECUTE_HANDLER from
		// the __except clause.
	}
	return 0;
}

// end of file
