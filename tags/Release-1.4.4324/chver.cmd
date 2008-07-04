@echo off

REM UpdateIt! application.
REM Copyright (c) 2002-2008 by Elijah Zarezky,
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

REM chver.cmd - NT command script to change version numbers

copy /Y .\Source\UpdateIt.manifest.munge .\Source\UpdateIt.manifest
.\Tools\munge.exe .\Source\MungeScript.txt -k -t -L .\Source\UpdateIt.manifest

copy /Y .\Source\VersionInfo.rc.munge .\Source\VersionInfo.rc
.\Tools\munge.exe .\Source\MungeScript.txt -k -t -L .\Source\VersionInfo.rc

copy /Y .\Languages\English_USA.1252\Source\VersionInfo.rc.munge .\Languages\English_USA.1252\Source\VersionInfo.rc
.\Tools\munge.exe .\Source\MungeScript.txt -k -t -L .\Languages\English_USA.1252\Source\VersionInfo.rc

copy /Y .\Languages\Russian_Russia.1251\Source\VersionInfo.rc.munge .\Languages\Russian_Russia.1251\Source\VersionInfo.rc
.\Tools\munge.exe .\Source\MungeScript.txt -k -t -L .\Languages\Russian_Russia.1251\Source\VersionInfo.rc

copy /Y .\Source\Setup.iss.munge .\Source\Setup.iss
.\Tools\munge.exe .\Source\MungeScript.txt -k -t -L .\Source\Setup.iss

copy /Y .\Source\SetupUniversal.iss.munge .\Source\SetupUniversal.iss
.\Tools\munge.exe .\Source\MungeScript.txt -k -t -L .\Source\SetupUniversal.iss

REM end of file
