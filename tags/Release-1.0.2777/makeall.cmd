@echo off
echo.
echo Building executable - "Win32 Debug"...
echo.
msdev.com UpdateIt.dsw /MAKE "UpdateIt - Win32 Debug" /REBUILD
echo.
echo Building executable - "Win32 Release"...
echo.
msdev.com UpdateIt.dsw /MAKE "UpdateIt - Win32 Release" /REBUILD
echo.
