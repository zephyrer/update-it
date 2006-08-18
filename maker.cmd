@echo off
echo.
echo Building executable...
echo.
msdev.com UpdateIt.dsw /MAKE "UpdateIt - Win32 Release" /REBUILD
echo.
echo.
echo Compiling documentation...
echo.
chdir .\HTML
if exist UpdateIt.chm del UpdateIt.chm
hhc.exe UpdateIt.hhp
chdir ..\
echo.
echo.
echo Creating installer...
echo.
if not exist .\Setup mkdir .\Setup
chdir .\Source
if exist ..\Setup\*.exe del ..\Setup\*.exe
iscc.exe Setup.iss
chdir ..
