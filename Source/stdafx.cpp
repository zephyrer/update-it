// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// stdafx.cpp - source file that includes just the standard includes

#include "stdafx.h"

#if (_MSC_VER >= 1300) && defined(_DLL)

extern "C" __int64 __cdecl _abs64(__int64 num)
{
	return (num < 0 ? -num : num);
}

#endif	// _MSC_VER

// import libraries
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

// linker options
#if (_MSC_VER < 1300)
#pragma comment(linker, "/osversion:4.1")
#pragma comment(linker, "/subsystem:windows,4.10")
#pragma comment(linker, "/ignore:4089")		// all references to DLL discarded by /opt:ref
#endif	// _MSC_VER

// end of file
