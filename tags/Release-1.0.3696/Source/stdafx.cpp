// UpdateIt! application.
// Copyright (c) 2002-2006 by Elijah Zarezky,
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

#if defined(_DEBUG)
#pragma comment(lib, "ssleay32MDd.lib")
#pragma comment(lib, "libeay32MDd.lib")
#else
#pragma comment(lib, "ssleay32MD.lib")
#pragma comment(lib, "libeay32MD.lib")
#endif	// _DEBUG

// linker options

#if (_MSC_VER < 1300)
#pragma comment(linker, "/osversion:4.1")
#pragma comment(linker, "/subsystem:windows,4.10")
#pragma comment(linker, "/ignore:4089")		// all references to DLL discarded by /opt:ref
#endif	// _MSC_VER

// end of file
