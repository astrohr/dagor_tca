# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=IK220Drv - Win32 NT kernel driver (debug)
!MESSAGE No configuration specified.  Defaulting to IK220Drv - Win32 NT kernel\
 driver (debug).
!ENDIF 

!IF "$(CFG)" != "IK220Drv - Win32 NT kernel driver (release)" && "$(CFG)" !=\
 "IK220Drv - Win32 NT kernel driver (debug)"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "IK220Drv.mak"\
 CFG="IK220Drv - Win32 NT kernel driver (debug)"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IK220Drv - Win32 NT kernel driver (release)" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IK220Drv - Win32 NT kernel driver (debug)" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "IK220Drv - Win32 NT kernel driver (debug)"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "IK220Drv - Win32 NT kernel driver (release)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "c:\temp"
# PROP Intermediate_Dir "c:\temp"
OUTDIR=c:\temp
INTDIR=c:\temp

ALL : ".\release\IK220Drv.sys"

CLEAN : 
	-@erase "$(INTDIR)\IK220Drv.obj"
	-@erase ".\release\IK220Drv.sys"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /G4 /Gz /MT /W3 /Ox /Gf /Gy /I "d:\Ddk\inc" /D "NDEBUG" /D "_X86_" /D "i386" /D "STD_CALL" /D "CONDITION_HANDLING" /D "WIN32_LEAN_AND_MEAN" /D "NT_UP" /D "__HEIDENHAIN__" /U "NT_INST" /c
# SUBTRACT CPP /Ot /Oa /Ow /Og /Oi /Os /Fr /YX
CPP_PROJ=/nologo /G4 /Gz /MT /W3 /Ox /Gf /Gy /I "d:\Ddk\inc" /D "NDEBUG" /D\
 "_X86_" /D "i386" /D "STD_CALL" /D "CONDITION_HANDLING" /D\
 "WIN32_LEAN_AND_MEAN" /D "NT_UP" /D "__HEIDENHAIN__" /U "NT_INST"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=c:\temp/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/IK220Drv.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ntoskrnl.lib hal.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x10000 /entry:"DriverEntry@8" /pdb:"mousecl.pdb" /machine:I386 /nodefaultlib /out:"C:\IK220_NT\IK220Drv\release\IK220Drv.sys" /SUBSYSTEM:native
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=ntoskrnl.lib hal.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x10000 /entry:"DriverEntry@8"\
 /incremental:no /pdb:"mousecl.pdb" /machine:I386 /nodefaultlib\
 /out:"C:\IK220_NT\IK220Drv\release\IK220Drv.sys" /SUBSYSTEM:native 
LINK32_OBJS= \
	"$(INTDIR)\IK220Drv.obj"

".\release\IK220Drv.sys" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IK220Drv - Win32 NT kernel driver (debug)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\IK220_NT\IK220Drv\debug"
# PROP Intermediate_Dir "c:\temp"
OUTDIR=C:\IK220_NT\IK220Drv\debug
INTDIR=c:\temp

ALL : "$(OUTDIR)\IK220Drv.sys" "$(OUTDIR)\IK220Drv.bsc"

CLEAN : 
	-@erase "$(INTDIR)\IK220Drv.obj"
	-@erase "$(INTDIR)\IK220Drv.sbr"
	-@erase "$(OUTDIR)\IK220Drv.bsc"
	-@erase "$(OUTDIR)\IK220Drv.sys"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /G5 /Gz /MTd /W3 /Z7 /Oi /Gf /Gy /I "d:\Ddk\inc" /D "_DEBUG" /D "RDRDBG" /D "SRVDBG" /D "DBG" /D "_IDWBUILD" /D "_X86_" /D "i386" /D "STD_CALL" /D "CONDITION_HANDLING" /D "WIN32_LEAN_AND_MEAN" /D "NT_UP" /D "__HEIDENHAIN__" /U "NT_INST" /Fr /Zel /c
CPP_PROJ=/nologo /G5 /Gz /MTd /W3 /Z7 /Oi /Gf /Gy /I "d:\Ddk\inc" /D "_DEBUG"\
 /D "RDRDBG" /D "SRVDBG" /D "DBG" /D "_IDWBUILD" /D "_X86_" /D "i386" /D\
 "STD_CALL" /D "CONDITION_HANDLING" /D "WIN32_LEAN_AND_MEAN" /D "NT_UP" /D\
 "__HEIDENHAIN__" /U "NT_INST" /Fr"$(INTDIR)/" /Fo"$(INTDIR)/" /Zel /c 
CPP_OBJS=c:\temp/
CPP_SBRS=c:\temp/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/IK220Drv.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\IK220Drv.sbr"

"$(OUTDIR)\IK220Drv.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 ntoskrnl.lib hal.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x10000 /entry:"DriverEntry@8" /pdb:none /debug /machine:I386 /nodefaultlib /out:"C:\IK220_NT\IK220Drv\debug\IK220Drv.sys" /DEBUG:full /SUBSYSTEM:native
# SUBTRACT LINK32 /map
LINK32_FLAGS=ntoskrnl.lib hal.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x10000 /entry:"DriverEntry@8"\
 /pdb:none /debug /machine:I386 /nodefaultlib /out:"$(OUTDIR)/IK220Drv.sys"\
 /DEBUG:full /SUBSYSTEM:native 
LINK32_OBJS= \
	"$(INTDIR)\IK220Drv.obj"

"$(OUTDIR)\IK220Drv.sys" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "IK220Drv - Win32 NT kernel driver (release)"
# Name "IK220Drv - Win32 NT kernel driver (debug)"

!IF  "$(CFG)" == "IK220Drv - Win32 NT kernel driver (release)"

!ELSEIF  "$(CFG)" == "IK220Drv - Win32 NT kernel driver (debug)"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\IK220Drv.c
DEP_CPP_IK220=\
	".\ik220.h"\
	"d:\Ddk\inc\alpharef.h"\
	"d:\Ddk\inc\bugcodes.h"\
	"d:\Ddk\inc\exlevels.h"\
	"d:\Ddk\inc\ntddk.h"\
	"d:\Ddk\inc\ntdef.h"\
	"d:\Ddk\inc\ntiologc.h"\
	"d:\Ddk\inc\ntpoapi.h"\
	"d:\Ddk\inc\ntstatus.h"\
	

!IF  "$(CFG)" == "IK220Drv - Win32 NT kernel driver (release)"


"$(INTDIR)\IK220Drv.obj" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Drv - Win32 NT kernel driver (debug)"


"$(INTDIR)\IK220Drv.obj" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"

"$(INTDIR)\IK220Drv.sbr" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
