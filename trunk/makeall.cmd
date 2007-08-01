@echo off

echo.
echo Building executable - "Win32 MBCS Debug"...
echo.
devenv.com UpdateIt71.sln /rebuild "MBCS Debug" /project UpdateIt /nologo
echo.

echo.
echo Building executable - "Win32 MBCS Release"...
echo.
devenv.com UpdateIt71.sln /rebuild "MBCS Release" /project UpdateIt /nologo
echo.
