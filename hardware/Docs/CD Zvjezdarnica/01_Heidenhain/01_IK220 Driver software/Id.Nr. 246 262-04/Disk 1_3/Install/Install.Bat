@Echo Off
CLS
ECHO.
ECHO.
ECHO.

if "%OS%" == "Windows_NT" goto SetNT


REM Setup for Windows 95/98
REM =======================

ECHO    /--------------------------------\
ECHO    /                                \
ECHO    /      Setup for IK 220          \
ECHO               Software               
ECHO    \      for Windows 95/98         /
ECHO    \                                /
ECHO    \--------------------------------/
ECHO.

Echo Copy device driver to Windows system directory.
if exist ..\IK220VxD\IK220VxD.VxD goto okVXD
Echo ---- File not found ----
:okVXD
copy ..\IK220VxD\IK220VxD.VxD %Windir%\System\IK220VxD.VxD /Y /V  > nul

Echo Copy DLL to Windows system directory.
if exist ..\IK220Dll\Release95\IK220Dll.Dll goto okDLL95
Echo ---- File not found ----
:okDLL95
copy ..\IK220Dll\Release95\IK220Dll.Dll %Windir%\System\IK220Dll.Dll /Y /V  > nul

goto End


:SetNT
REM Setup for Windows NT/2000
REM =========================

ECHO    /--------------------------------\
ECHO    /                                \
ECHO    /      Setup for IK 220          \
ECHO               Software               
ECHO    \     for Windows NT/2000        /
ECHO    \                                /
ECHO    \--------------------------------/
ECHO.

Echo Copy device driver to Windows drivers directory.
if exist ..\IK220Drv\Release\IK220Drv.Sys goto okSYS
Echo ---- File not found ----
:okSYS
Echo Trying to stop device driver...
net stop ik220drv > nul
copy ..\IK220Drv\Release\IK220Drv.Sys %SystemRoot%\System32\Drivers\IK220Drv.Sys /V > nul
Echo Trying to start device driver...
net start ik220drv 

Echo Copy DLL to Windows System32 directory.
if exist ..\IK220Dll\Release\IK220Dll.Dll goto okDLL
Echo ---- File not found ----
:okDLL
copy ..\IK220Dll\Release\IK220Dll.Dll %SystemRoot%\System32\IK220Dll.Dll /V > nul

ECHO Add Registry entry.
if exist IK220Drv.reg goto okREG
Echo ---- File not found ----
:okREG
RegEdit /s IK220Drv.reg


Echo.
Echo ***************************************************************
Echo * You have to reboot now to finish device driver installation *
Echo ***************************************************************


:End
Echo.
Echo Setup finished.
Echo.
Pause
