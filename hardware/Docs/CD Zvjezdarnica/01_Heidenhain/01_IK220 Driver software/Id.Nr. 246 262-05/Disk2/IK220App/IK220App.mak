# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=IK220App - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to IK220App - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "IK220App - Win32 Release" && "$(CFG)" !=\
 "IK220App - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "IK220App.mak" CFG="IK220App - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IK220App - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "IK220App - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "IK220App - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "IK220App - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\IK220App.exe"

CLEAN : 
	-@erase "$(INTDIR)\IK220App.obj"
	-@erase "$(INTDIR)\IK220App.pch"
	-@erase "$(INTDIR)\IK220App.res"
	-@erase "$(INTDIR)\IK220AppDlg.obj"
	-@erase "$(INTDIR)\INITMSG.obj"
	-@erase "$(INTDIR)\SetupDlg.obj"
	-@erase "$(INTDIR)\StatusDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\IK220App.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)/IK220App.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG"
RSC_PROJ=/l 0x407 /fo"$(INTDIR)/IK220App.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/IK220App.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 IK220DLL.LIB /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=IK220DLL.LIB /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/IK220App.pdb" /machine:I386 /out:"$(OUTDIR)/IK220App.exe" 
LINK32_OBJS= \
	"$(INTDIR)\IK220App.obj" \
	"$(INTDIR)\IK220App.res" \
	"$(INTDIR)\IK220AppDlg.obj" \
	"$(INTDIR)\INITMSG.obj" \
	"$(INTDIR)\SetupDlg.obj" \
	"$(INTDIR)\StatusDlg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\IK220App.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "IK220App - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\IK220App.exe" "$(OUTDIR)\IK220App.bsc"

CLEAN : 
	-@erase "$(INTDIR)\IK220App.obj"
	-@erase "$(INTDIR)\IK220App.pch"
	-@erase "$(INTDIR)\IK220App.res"
	-@erase "$(INTDIR)\IK220App.sbr"
	-@erase "$(INTDIR)\IK220AppDlg.obj"
	-@erase "$(INTDIR)\IK220AppDlg.sbr"
	-@erase "$(INTDIR)\INITMSG.obj"
	-@erase "$(INTDIR)\INITMSG.sbr"
	-@erase "$(INTDIR)\SetupDlg.obj"
	-@erase "$(INTDIR)\SetupDlg.sbr"
	-@erase "$(INTDIR)\StatusDlg.obj"
	-@erase "$(INTDIR)\StatusDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\IK220App.bsc"
	-@erase "$(OUTDIR)\IK220App.exe"
	-@erase "$(OUTDIR)\IK220App.ilk"
	-@erase "$(OUTDIR)\IK220App.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/IK220App.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG"
RSC_PROJ=/l 0x407 /fo"$(INTDIR)/IK220App.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/IK220App.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\IK220App.sbr" \
	"$(INTDIR)\IK220AppDlg.sbr" \
	"$(INTDIR)\INITMSG.sbr" \
	"$(INTDIR)\SetupDlg.sbr" \
	"$(INTDIR)\StatusDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\IK220App.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 IK220DLL.LIB /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=IK220DLL.LIB /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/IK220App.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/IK220App.exe" 
LINK32_OBJS= \
	"$(INTDIR)\IK220App.obj" \
	"$(INTDIR)\IK220App.res" \
	"$(INTDIR)\IK220AppDlg.obj" \
	"$(INTDIR)\INITMSG.obj" \
	"$(INTDIR)\SetupDlg.obj" \
	"$(INTDIR)\StatusDlg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\IK220App.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "IK220App - Win32 Release"
# Name "IK220App - Win32 Debug"

!IF  "$(CFG)" == "IK220App - Win32 Release"

!ELSEIF  "$(CFG)" == "IK220App - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\IK220App.cpp
DEP_CPP_IK220=\
	".\IK220App.h"\
	".\IK220AppDlg.h"\
	".\INITMSG.h"\
	".\StatusDlg.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "IK220App - Win32 Release"


"$(INTDIR)\IK220App.obj" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ELSEIF  "$(CFG)" == "IK220App - Win32 Debug"


"$(INTDIR)\IK220App.obj" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"

"$(INTDIR)\IK220App.sbr" : $(SOURCE) $(DEP_CPP_IK220) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IK220AppDlg.cpp
DEP_CPP_IK220A=\
	"..\IK220Drv\IK220.h"\
	".\IK220App.h"\
	".\IK220AppDlg.h"\
	".\INITMSG.h"\
	".\SetupDlg.h"\
	".\StatusDlg.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "IK220App - Win32 Release"


"$(INTDIR)\IK220AppDlg.obj" : $(SOURCE) $(DEP_CPP_IK220A) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ELSEIF  "$(CFG)" == "IK220App - Win32 Debug"


"$(INTDIR)\IK220AppDlg.obj" : $(SOURCE) $(DEP_CPP_IK220A) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"

"$(INTDIR)\IK220AppDlg.sbr" : $(SOURCE) $(DEP_CPP_IK220A) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "IK220App - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/IK220App.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\IK220App.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "IK220App - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/IK220App.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\IK220App.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IK220App.rc
DEP_RSC_IK220AP=\
	".\res\bitmap1.bmp"\
	".\res\icon1.ico"\
	".\res\IK220App.rc2"\
	".\resource.hm"\
	

"$(INTDIR)\IK220App.res" : $(SOURCE) $(DEP_RSC_IK220AP) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SetupDlg.cpp
DEP_CPP_SETUP=\
	".\IK220App.h"\
	".\SetupDlg.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "IK220App - Win32 Release"


"$(INTDIR)\SetupDlg.obj" : $(SOURCE) $(DEP_CPP_SETUP) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ELSEIF  "$(CFG)" == "IK220App - Win32 Debug"


"$(INTDIR)\SetupDlg.obj" : $(SOURCE) $(DEP_CPP_SETUP) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"

"$(INTDIR)\SetupDlg.sbr" : $(SOURCE) $(DEP_CPP_SETUP) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StatusDlg.cpp
DEP_CPP_STATU=\
	".\IK220App.h"\
	".\IK220AppDlg.h"\
	".\INITMSG.h"\
	".\StatusDlg.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "IK220App - Win32 Release"


"$(INTDIR)\StatusDlg.obj" : $(SOURCE) $(DEP_CPP_STATU) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ELSEIF  "$(CFG)" == "IK220App - Win32 Debug"


"$(INTDIR)\StatusDlg.obj" : $(SOURCE) $(DEP_CPP_STATU) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"

"$(INTDIR)\StatusDlg.sbr" : $(SOURCE) $(DEP_CPP_STATU) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\INITMSG.cpp
DEP_CPP_INITM=\
	".\IK220App.h"\
	".\INITMSG.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "IK220App - Win32 Release"


"$(INTDIR)\INITMSG.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ELSEIF  "$(CFG)" == "IK220App - Win32 Debug"


"$(INTDIR)\INITMSG.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"

"$(INTDIR)\INITMSG.sbr" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"\
 "$(INTDIR)\IK220App.pch"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
