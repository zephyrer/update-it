// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// stdafx.cpp - source file that includes just the standard includes

#include "stdafx.h"

// import libraries
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

// static libraries
#if defined(__INTEL_COMPILER)
#if defined(__STL_CONFIG_H)
#if defined(_DEBUG)
#pragma comment(linker, "/nodefaultlib:msvcprtd.lib")
#pragma comment(lib, "libciod.lib")
#else
#pragma comment(linker, "/nodefaultlib:msvcprt.lib")
#pragma comment(lib, "libcio.lib")
#endif	// _DEBUG
#endif	// __STL_CONFIG_H
#endif	// __INTEL_COMPILER

// linker options
#pragma comment(linker, "/osversion:4.1")
#pragma comment(linker, "/subsystem:windows,4.10")
#pragma comment(linker, "/ignore:4089")		// all references to DLL discarded by /opt:ref

// end of file
