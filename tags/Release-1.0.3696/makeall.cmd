@echo off
echo.
echo Building executable - "Win32 MBCS Debug"...
echo.
devenv.com UpdateIt71.sln /rebuild MBCS_Debug /project UpdateIt /nologo
echo Building executable - "Win32 MBCS Release"...
echo.
devenv.com UpdateIt71.sln /rebuild MBCS_Release /project UpdateIt /nologo
echo.
