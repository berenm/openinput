# Microsoft Developer Studio Project File - Name="OpenInput" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OpenInput - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OpenInput.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OpenInput.mak" CFG="OpenInput - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OpenInput - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OpenInput - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OpenInput - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENINPUT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /O2 /I "." /I "..\\" /I "..\include" /I "..\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENINPUT_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "NDEBUG"
# ADD RSC /l 0x406 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENINPUT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\include" /I "..\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENINPUT_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "_DEBUG"
# ADD RSC /l 0x406 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "OpenInput - Win32 Release"
# Name "OpenInput - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\action.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\appstate.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\debug.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\device.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\events.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\joystick.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\keyboard.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\keynames.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\main.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\mouse.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\queue.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\unixsignal\unixsignal.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\win32.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\win32\win32_events.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\win32\win32_translate.c

!IF  "$(CFG)" == "OpenInput - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenInput - Win32 Debug"

# ADD CPP /Zd

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\bootstrap.h
# End Source File
# Begin Source File

SOURCE=..\config.h
# End Source File
# Begin Source File

SOURCE=..\src\internal.h
# End Source File
# Begin Source File

SOURCE=..\include\openinput.h
# End Source File
# Begin Source File

SOURCE=..\include\openinput_action.h
# End Source File
# Begin Source File

SOURCE=..\include\openinput_api.h
# End Source File
# Begin Source File

SOURCE=..\include\openinput_events.h
# End Source File
# Begin Source File

SOURCE=..\include\openinput_joystick.h
# End Source File
# Begin Source File

SOURCE=..\include\openinput_keyboard.h
# End Source File
# Begin Source File

SOURCE=..\include\openinput_mouse.h
# End Source File
# Begin Source File

SOURCE=..\include\openinput_types.h
# End Source File
# Begin Source File

SOURCE=..\src\private.h
# End Source File
# Begin Source File

SOURCE=..\src\unixsignal\unixsignal.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\win32.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
