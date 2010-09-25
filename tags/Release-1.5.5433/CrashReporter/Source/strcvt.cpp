// CrashReporter application.
// Copyright (c) 2009-2010 by Elijah Zarezky,
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

// from strcore.cpp

#include "stdafx.h"
#include "strcvt.h"
#include <crtdbg.h>

#pragma warning(disable:4127)	// for _ASSERTE

int xwcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
{
	if (count == 0 && mbstr != NULL)
		return 0;

	int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1,
		mbstr, count, NULL, NULL);
	_ASSERTE(mbstr == NULL || result <= (int)count);
	if (result > 0)
		mbstr[result-1] = 0;
	return result;
}

int xmbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count)
{
	if (count == 0 && wcstr != NULL)
		return 0;

	int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1,
		wcstr, count);
	_ASSERTE(wcstr == NULL || result <= (int)count);
	if (result > 0)
		wcstr[result-1] = 0;
	return result;
}
