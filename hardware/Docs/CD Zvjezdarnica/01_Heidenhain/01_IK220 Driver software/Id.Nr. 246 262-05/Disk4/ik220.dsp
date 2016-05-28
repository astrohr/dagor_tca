# Microsoft Developer Studio Project File - Name="ik220" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ik220 - Win32 kernel driver (debug)
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "ik220.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "ik220.mak" CFG="ik220 - Win32 kernel driver (debug)"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "ik220 - Win32 kernel driver (release)" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ik220 - Win32 kernel driver (debug)" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ik220"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ik220 - Win32 kernel driver (release)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:\my\mySoft\IK220_NT\Disk4\Release"
# PROP Intermediate_Dir "C:\my\mySoft\IK220_NT\Disk4\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "NE2000_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MT /W3 /Z7 /O2 /Oy- /X /I "Include" /I "c:\MsvcJHCN\ddk\inc" /I "c:\MsvcJHCN\ddk\inc\ddk" /I "c:\MsvcJHCN\VC98\include" /D "NT_DDK" /D BINARY_COMPATIBLE=0 /D "NDEBUG" /D "WIN32" /D FPO=1 /D "CONDITION_HANDLING" /D "_X86_" /D "i386" /D "STD_CALL" /D "WIN32_LEAN_AND_MEAN" /D "NT_UP" /D "DEVL" /D "_DLL" /D "_UNICODE" /D NT_INST=0 /FD /Oxs /QIfdiv- /QIf /QI0f /GF /Oxs /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /x /i "Z:\MsvcJHCN\VC98\include" /i "C:\MsvcJHCN\VC98\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 c:\MsvcJHCN\ddk\lib\free\hal.lib c:\MsvcJHCN\ddk\lib\free\ntoskrnl.lib c:\MsvcJHCN\ddk\lib\free\wmilib.lib /nologo /base:"0x10000" /version:2.0 /entry:"DriverEntry@8" /map /debug /machine:I386 /nodefaultlib /out:"C:\my\mySoft\IK220_NT\Disk4\Release\IK220Drv.sys" /SUBSYSTEM:native
# SUBTRACT LINK32 /verbose /pdb:none

!ELSEIF  "$(CFG)" == "ik220 - Win32 kernel driver (debug)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\my\mySoft\IK220_NT\Disk4\Debug"
# PROP Intermediate_Dir "C:\my\mySoft\IK220_NT\Disk4\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NE2000_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MTd /W3 /Z7 /Od /Gy /X /I "Include" /I "c:\MsvcJHCN\ddk\inc" /I "c:\MsvcJHCN\ddk\inc\ddk" /I "c:\MsvcJHCN\VC98\include" /D "NT_DDK" /D BINARY_COMPATIBLE=0 /D "_DEBUG" /D "WIN32" /D "DBG" /D "RDRDBG" /D "SRVDBG" /D "_IDWBUILD" /D "CONDITION_HANDLING" /D "_X86_" /D "i386" /D "STD_CALL" /D "WIN32_LEAN_AND_MEAN" /D "NT_UP" /D "DEVL" /D "_DLL" /D "_UNICODE" /D NT_INST=0 /FD /QIfdiv- /QIf /QI0f /GF /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /x /i "Z:\MsvcJHCN\VC98\include" /i "C:\MsvcJHCN\VC98\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 c:\MsvcJHCN\ddk\lib\checked\hal.lib c:\MsvcJHCN\ddk\lib\checked\ntoskrnl.lib c:\MsvcJHCN\ddk\lib\checked\wmilib.lib /nologo /base:"0x10000" /version:2.0 /stack:0x40000,0x1000 /entry:"DriverEntry@8" /incremental:no /map /debug /machine:I386 /nodefaultlib /out:"C:\my\mySoft\IK220_NT\Disk4\Debug\IK220Drv.sys" /debug:FULL /SUBSYSTEM:native /merge:_PAGE=PAGE /merge:_TEXT=.text /section:INIT,D /section:.text,EP /opt:REF /opt:ICF /driver /optidata /ignore:4078
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ik220 - Win32 kernel driver (release)"
# Name "ik220 - Win32 kernel driver (debug)"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ik220.rc
# End Source File
# Begin Source File

SOURCE=.\iklog.mc

!IF  "$(CFG)" == "ik220 - Win32 kernel driver (release)"

# Begin Custom Build
InputPath=.\iklog.mc

"iklog.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mc iklog.mc

# End Custom Build

!ELSEIF  "$(CFG)" == "ik220 - Win32 kernel driver (debug)"

# Begin Custom Build
InputPath=.\iklog.mc

"iklog.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	mc iklog.mc

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\initunlo.c
# End Source File
# Begin Source File

SOURCE=.\ioctl.c
# End Source File
# Begin Source File

SOURCE=.\log.c
# End Source File
# Begin Source File

SOURCE=.\openclos.c
# End Source File
# Begin Source File

SOURCE=.\pnp.c
# End Source File
# Begin Source File

SOURCE=.\power.c
# End Source File
# Begin Source File

SOURCE=.\qsfile.c
# End Source File
# Begin Source File

SOURCE=.\registry.c
# End Source File
# Begin Source File

SOURCE=.\utils.c
# End Source File
# Begin Source File

SOURCE=.\wmi.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
