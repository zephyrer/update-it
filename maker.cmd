@echo off
echo.
echo Building executable...
echo.
devenv.com UpdateIt70.sln /rebuild MBCS_Release /project UpdateIt /nologo
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
iscc.exe SetupUniversal.iss
chdir ..
