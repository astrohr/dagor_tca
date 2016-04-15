# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=IK220Dll - Win32 W95 Debug
!MESSAGE No configuration specified.  Defaulting to IK220Dll - Win32 W95 Debug.
!ENDIF 

!IF "$(CFG)" != "IK220Dll - Win32 Release" && "$(CFG)" !=\
 "IK220Dll - Win32 Debug" && "$(CFG)" != "IK220Dll - Win32 W95 Debug" &&\
 "$(CFG)" != "IK220Dll - Win32 W95 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "IK220Dll.mak" CFG="IK220Dll - Win32 W95 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IK220Dll - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IK220Dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IK220Dll - Win32 W95 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IK220Dll - Win32 W95 Release" (based on\
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
# PROP Target_Last_Scanned "IK220Dll - Win32 W95 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "IK220Dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\IK220Dll.dll"

CLEAN : 
	-@erase "$(INTDIR)\DLLMain.obj"
	-@erase "$(INTDIR)\Exports.obj"
	-@erase "$(INTDIR)\IK220Pci.obj"
	-@erase "$(OUTDIR)\IK220Dll.dll"
	-@erase "$(OUTDIR)\IK220Dll.exp"
	-@erase "$(OUTDIR)\IK220Dll.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/IK220Dll.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/IK220Dll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/IK220Dll.pdb" /machine:I386 /def:".\IK220Dll.def"\
 /out:"$(OUTDIR)/IK220Dll.dll" /implib:"$(OUTDIR)/IK220Dll.lib" 
DEF_FILE= \
	".\IK220Dll.def"
LINK32_OBJS= \
	"$(INTDIR)\DLLMain.obj" \
	"$(INTDIR)\Exports.obj" \
	"$(INTDIR)\IK220Pci.obj"

"$(OUTDIR)\IK220Dll.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IK220Dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\IK220Dll.dll" "$(OUTDIR)\IK220Dll.bsc"

CLEAN : 
	-@erase "$(INTDIR)\DLLMain.obj"
	-@erase "$(INTDIR)\DLLMain.sbr"
	-@erase "$(INTDIR)\Exports.obj"
	-@erase "$(INTDIR)\Exports.sbr"
	-@erase "$(INTDIR)\IK220Pci.obj"
	-@erase "$(INTDIR)\IK220Pci.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\IK220Dll.bsc"
	-@erase "$(OUTDIR)\IK220Dll.dll"
	-@erase "$(OUTDIR)\IK220Dll.exp"
	-@erase "$(OUTDIR)\IK220Dll.ilk"
	-@erase "$(OUTDIR)\IK220Dll.lib"
	-@erase "$(OUTDIR)\IK220Dll.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/IK220Dll.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/IK220Dll.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DLLMain.sbr" \
	"$(INTDIR)\Exports.sbr" \
	"$(INTDIR)\IK220Pci.sbr"

"$(OUTDIR)\IK220Dll.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/IK220Dll.pdb" /debug /machine:I386 /def:".\IK220Dll.def"\
 /out:"$(OUTDIR)/IK220Dll.dll" /implib:"$(OUTDIR)/IK220Dll.lib" 
DEF_FILE= \
	".\IK220Dll.def"
LINK32_OBJS= \
	"$(INTDIR)\DLLMain.obj" \
	"$(INTDIR)\Exports.obj" \
	"$(INTDIR)\IK220Pci.obj"

"$(OUTDIR)\IK220Dll.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IK220Dll"
# PROP BASE Intermediate_Dir "IK220Dll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug95"
# PROP Intermediate_Dir "Debug95"
# PROP Target_Dir ""
OUTDIR=.\Debug95
INTDIR=.\Debug95

ALL : "$(OUTDIR)\IK220Dll.dll" "$(OUTDIR)\IK220Dll.bsc"

CLEAN : 
	-@erase "$(INTDIR)\DLLMain.obj"
	-@erase "$(INTDIR)\DLLMain.sbr"
	-@erase "$(INTDIR)\Exports.obj"
	-@erase "$(INTDIR)\Exports.sbr"
	-@erase "$(INTDIR)\IK220Pci.obj"
	-@erase "$(INTDIR)\IK220Pci.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\IK220Dll.bsc"
	-@erase "$(OUTDIR)\IK220Dll.dll"
	-@erase "$(OUTDIR)\IK220Dll.exp"
	-@erase "$(OUTDIR)\IK220Dll.ilk"
	-@erase "$(OUTDIR)\IK220Dll.lib"
	-@erase "$(OUTDIR)\IK220Dll.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_Windows95" /FR /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_Windows95" /FR"$(INTDIR)/" /Fp"$(INTDIR)/IK220Dll.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug95/
CPP_SBRS=.\Debug95/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/IK220Dll.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DLLMain.sbr" \
	"$(INTDIR)\Exports.sbr" \
	"$(INTDIR)\IK220Pci.sbr"

"$(OUTDIR)\IK220Dll.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/IK220Dll.pdb" /debug /machine:I386 /def:".\IK220Dll.def"\
 /out:"$(OUTDIR)/IK220Dll.dll" /implib:"$(OUTDIR)/IK220Dll.lib" 
DEF_FILE= \
	".\IK220Dll.def"
LINK32_OBJS= \
	"$(INTDIR)\DLLMain.obj" \
	"$(INTDIR)\Exports.obj" \
	"$(INTDIR)\IK220Pci.obj"

"$(OUTDIR)\IK220Dll.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "IK220Dl0"
# PROP BASE Intermediate_Dir "IK220Dl0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release95"
# PROP Intermediate_Dir "Release95"
# PROP Target_Dir ""
OUTDIR=.\Release95
INTDIR=.\Release95

ALL : "$(OUTDIR)\IK220Dll.dll"

CLEAN : 
	-@erase "$(INTDIR)\DLLMain.obj"
	-@erase "$(INTDIR)\Exports.obj"
	-@erase "$(INTDIR)\IK220Pci.obj"
	-@erase "$(OUTDIR)\IK220Dll.dll"
	-@erase "$(OUTDIR)\IK220Dll.exp"
	-@erase "$(OUTDIR)\IK220Dll.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_Windows95" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_Windows95" /Fp"$(INTDIR)/IK220Dll.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release95/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/IK220Dll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /version:1.0 /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/IK220Dll.pdb" /machine:I386 /def:".\IK220Dll.def"\
 /out:"$(OUTDIR)/IK220Dll.dll" /implib:"$(OUTDIR)/IK220Dll.lib" 
DEF_FILE= \
	".\IK220Dll.def"
LINK32_OBJS= \
	"$(INTDIR)\DLLMain.obj" \
	"$(INTDIR)\Exports.obj" \
	"$(INTDIR)\IK220Pci.obj"

"$(OUTDIR)\IK220Dll.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "IK220Dll - Win32 Release"
# Name "IK220Dll - Win32 Debug"
# Name "IK220Dll - Win32 W95 Debug"
# Name "IK220Dll - Win32 W95 Release"

!IF  "$(CFG)" == "IK220Dll - Win32 Release"

!ELSEIF  "$(CFG)" == "IK220Dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Debug"

!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Exports.c
DEP_CPP_EXPOR=\
	"..\include\DRVFunc.h"\
	"..\include\G28.h"\
	"..\include\IK220.h"\
	".\DLLGLob.h"\
	".\PGM220.H"\
	

!IF  "$(CFG)" == "IK220Dll - Win32 Release"


"$(INTDIR)\Exports.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Dll - Win32 Debug"


"$(INTDIR)\Exports.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"

"$(INTDIR)\Exports.sbr" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Debug"


"$(INTDIR)\Exports.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"

"$(INTDIR)\Exports.sbr" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Release"


"$(INTDIR)\Exports.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DLLMain.c
DEP_CPP_DLLMA=\
	".\DLLGLob.h"\
	

!IF  "$(CFG)" == "IK220Dll - Win32 Release"


"$(INTDIR)\DLLMain.obj" : $(SOURCE) $(DEP_CPP_DLLMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Dll - Win32 Debug"


"$(INTDIR)\DLLMain.obj" : $(SOURCE) $(DEP_CPP_DLLMA) "$(INTDIR)"

"$(INTDIR)\DLLMain.sbr" : $(SOURCE) $(DEP_CPP_DLLMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Debug"


"$(INTDIR)\DLLMain.obj" : $(SOURCE) $(DEP_CPP_DLLMA) "$(INTDIR)"

"$(INTDIR)\DLLMain.sbr" : $(SOURCE) $(DEP_CPP_DLLMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Release"


"$(INTDIR)\DLLMain.obj" : $(SOURCE) $(DEP_CPP_DLLMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IK220Pci.c
DEP_CPP_IK220=\
	".\IK220Pci.h"\
	".\pci.h"\
	

!IF  "$(CFG)" == "IK220Dll - Win32 Release"


"$(INTDIR)\IK220Pci.obj" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Dll - Win32 Debug"


"$(INTDIR)\IK220Pci.obj" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"

"$(INTDIR)\IK220Pci.sbr" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Debug"


"$(INTDIR)\IK220Pci.obj" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"

"$(INTDIR)\IK220Pci.sbr" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Release"


"$(INTDIR)\IK220Pci.obj" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IK220Dll.def

!IF  "$(CFG)" == "IK220Dll - Win32 Release"

!ELSEIF  "$(CFG)" == "IK220Dll - Win32 Debug"

!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Debug"

!ELSEIF  "$(CFG)" == "IK220Dll - Win32 W95 Release"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
