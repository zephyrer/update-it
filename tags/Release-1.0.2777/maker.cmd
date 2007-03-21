@echo off
echo.
echo Building executable...
echo.
msdev.com UpdateIt.dsw /MAKE "UpdateIt - Win32 Release" /REBUILD
echo.
echo.
echo Compiling documentation...
echo.
cd .\HTML
if exist UpdateIt.chm del UpdateIt.chm
hhc.exe UpdateIt.hhp
echo.
echo.
echo Creating installer...
echo.
cd ..\Source
if exist ..\Setup\*.exe del ..\Setup\*.exe
iscc.exe Setup.iss
cd ..
