@echo off

REM UpdateIt! application.
REM Copyright (c) 2002-2007 by Elijah Zarezky,
REM All rights reserved.

REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM See the License for the specific language governing permissions and
REM limitations under the License.

REM maker.cmd - NT command script to build release

echo.
echo Updating version numbers...
echo.
call chver.cmd

echo.
echo Building executable...
echo.
devenv.com UpdateIt71.sln /rebuild "MBCS Release" /project UpdateIt /nologo

echo Compiling documentation...
echo.
chdir .\HTML
if exist UpdateIt.chm del UpdateIt.chm
hhc.exe UpdateIt.hhp
chdir ..\

if not exist .\Setup mkdir .\Setup
if exist .\Setup\*.exe del .\Setup\*.exe

echo.
echo.
echo Creating standard installer...
echo.
chdir .\Source
iscc.exe Setup.iss
chdir ..\

echo.
echo.
echo Creating universal installer...
echo.
chdir .\Source
iscc.exe SetupUniversal.iss
chdir ..\
