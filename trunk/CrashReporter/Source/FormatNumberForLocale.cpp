// CrashReporter application.
// Copyright (c) 2009 by Elijah Zarezky,
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

// FormatNumberForLocale.cpp  Version 1.0
//
// Author:                    Hans Dietrich
//                            hdietrich2@hotmail.com
//
// Modified by:               Elijah Zarezky
//                            elijah.zarezky@gmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like, except
// that you may not sell this source code.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.

#include "stdafx.h"
#include "FormatNumberForLocale.h"

BOOL      CFormatNumberForLocale::m_bInitialized = FALSE;
NUMBERFMT CFormatNumberForLocale::m_nf = { 0 };
TCHAR     CFormatNumberForLocale::m_szThousandSep[20] = { 0 };
TCHAR     CFormatNumberForLocale::m_szDecimalSep[20] = { 0 };
