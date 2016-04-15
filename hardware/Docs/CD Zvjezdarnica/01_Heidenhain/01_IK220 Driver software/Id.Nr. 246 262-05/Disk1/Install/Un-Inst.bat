@Echo Off
CLS
ECHO.
ECHO.
ECHO.

if "%OS%" == "Windows_NT" goto SetNT


REM Uninstall for Windows 95/98
REM ===========================

ECHO    /--------------------------------\
ECHO    /                                \
ECHO    /      Setup for IK 220          \
ECHO               Software               
ECHO    \      for Windows 95/98         /
ECHO    \                                /
ECHO    \--------------------------------/
ECHO.

Echo Delete device driver
del %Windir%\System\IK220VxD.VxD > nul

Echo Delete DLL
del %Windir%\System\IK220Dll.Dll > nul

goto End


:SetNT
REM Unistall for Windows NT/2000
REM ============================

ECHO    /--------------------------------\
ECHO    /                                \
ECHO    /      Setup for IK 220          \
ECHO               Software               
ECHO    \     for Windows NT/2000        /
ECHO    \                                /
ECHO    \--------------------------------/
ECHO.

Echo Trying to stop device driver...
net stop ik220drv > nul

ECHO Delete device driver
del %SystemRoot%\System32\Drivers\IK220Drv.Sys > nul

Echo Delete DLL
del %SystemRoot%\System32\IK220Dll.Dll > nul

ECHO Remove Registry entry
RegEdit /s Uninstall.reg


:End
Echo.
Echo Uninstall finished.
Echo.
Pause
