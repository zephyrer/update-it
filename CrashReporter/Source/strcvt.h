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

#ifndef __STRCVT_H
#define __STRCVT_H

#define _wcstombsz xwcstombsz
#define _mbstowcsz xmbstowcsz

int xwcstombsz(char* mbstr, const wchar_t* wcstr, size_t count);
int xmbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count);

#endif   // __STRCVT_H
