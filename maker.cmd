@echo off

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
