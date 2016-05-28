; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSetupDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "IK220app.h"
LastPage=0

ClassCount=7
Class1=CIK220TSTApp
Class2=CAboutDlg
Class3=CIK220TSTDlg
Class4=CSetupDlg

ResourceCount=11
Resource1=IDD_IK220APP_DIALOG
Resource2=IDD_ABOUTBOX
Resource3=IDD_STATUS
Class5=CStatus
Resource4=IDD_Status
Class6=CStatusDlg
Resource5=IDD_SETUP
Class7=CINITMSG
Resource6=IDD_INITMSG
Resource7=IDD_SETUP (German (Germany))
Resource8=IDD_STATUS (German (Germany))
Resource9=IDD_ABOUTBOX (German (Germany))
Resource10=IDD_IK220APP_DIALOG (German (Germany))
Resource11=IDD_INITMSG (German (Germany))

[CLS:CIK220TSTApp]
Type=0
BaseClass=CWinApp
HeaderFile=IK220App.h
ImplementationFile=IK220App.cpp
Filter=N
LastObject=CIK220TSTApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=IK220AppDlg.cpp
ImplementationFile=IK220AppDlg.cpp
LastObject=CAboutDlg
Filter=D

[CLS:CIK220TSTDlg]
Type=0
BaseClass=CDialog
HeaderFile=IK220AppDlg.h
ImplementationFile=IK220AppDlg.cpp
Filter=D
LastObject=CIK220TSTDlg
VirtualFilter=dWC

[CLS:CSetupDlg]
Type=0
BaseClass=CDialog
HeaderFile=SetupDlg.h
ImplementationFile=SetupDlg.cpp
LastObject=IDC_Current
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_SETUP]
Type=1
Class=CSetupDlg
ControlCount=10
Control1=IDC_SIGNAL,button,1342308361
Control2=IDC_SIGNAL1,button,1342177289
Control3=IDC_STATIC,button,1342308359
Control4=IDC_ENCODER,button,1342308361
Control5=IDC_ENCODER1,button,1342177289
Control6=IDC_STATIC,button,1342308359
Control7=IDC_STATIC,static,1342308864
Control8=IDC_PERIOD,edit,1350631424
Control9=IDOK,button,1342242817
Control10=IDCANCEL,button,1342242816

[DLG:IDD_Status]
Type=1
Class=CStatus
ControlCount=1
Control1=IDOK,button,1342242817

[CLS:CStatus]
Type=0
HeaderFile=Status.h
ImplementationFile=Status.cpp
BaseClass=CDialog
Filter=D

[DLG:IDD_STATUS]
Type=1
Class=CStatusDlg
ControlCount=17
Control1=IDOK,button,1342242817
Control2=IDC_STA1,static,1342308352
Control3=IDC_STA2,static,1342308352
Control4=IDC_STA3,static,1342308352
Control5=IDC_STA4,static,1342308352
Control6=IDC_STA5,static,1342308352
Control7=IDC_STA6,static,1342308352
Control8=IDC_STA7,static,1342308352
Control9=IDC_STA8,static,1342308352
Control10=IDC_STEXT1,static,1342308352
Control11=IDC_STEXT2,static,1342308352
Control12=IDC_STEXT3,static,1342308352
Control13=IDC_STEXT4,static,1342308352
Control14=IDC_STEXT5,static,1342308352
Control15=IDC_STEXT6,static,1342308352
Control16=IDC_STEXT7,static,1342308352
Control17=IDC_STEXT8,static,1342308352

[CLS:CStatusDlg]
Type=0
HeaderFile=statusdlg.h
ImplementationFile=statusdlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_INITMSG]
Type=1
Class=CINITMSG
ControlCount=2
Control1=IDC_STATIC,static,1342308353
Control2=IDC_PROGRESS1,msctls_progress32,1350565888

[CLS:CINITMSG]
Type=0
HeaderFile=INITMSG.h
ImplementationFile=INITMSG.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_PROGRESS1
VirtualFilter=dWC

[DLG:IDD_IK220APP_DIALOG]
Type=1
Class=CIK220TSTDlg
ControlCount=28
Control1=IDC_RESET,button,1342242816
Control2=IDC_START,button,1342242816
Control3=IDC_STOP,button,1342242816
Control4=IDC_RESET_RI,button,1342242816
Control5=IDC_START_RI,button,1342242816
Control6=IDC_STOP_RI,button,1342242816
Control7=IDC_CHECK0,button,1342373891
Control8=IDC_CHECK1,button,1342242819
Control9=IDC_CHECK2,button,1342242819
Control10=IDC_CHECK3,button,1342242819
Control11=IDC_CHECK4,button,1342242819
Control12=IDC_CHECK5,button,1342242819
Control13=IDC_CHECK6,button,1342242819
Control14=IDC_CHECK7,button,1342242819
Control15=IDCANCEL,button,1342242817
Control16=IDC_Axis0,edit,1350568192
Control17=IDC_Axis1,edit,1350567936
Control18=IDC_Axis2,edit,1350567936
Control19=IDC_Axis3,edit,1350567936
Control20=IDC_Axis4,edit,1350567936
Control21=IDC_Axis5,edit,1350567936
Control22=IDC_Axis6,edit,1350567936
Control23=IDC_Axis7,edit,1350567936
Control24=IDC_Setup,button,1342242816
Control25=IDC_HEADLINE,static,1342312449
Control26=IDC_STATIC,static,1350569998
Control27=IDC_STATIC,static,1342177287
Control28=IDC_STATUS,button,1342242816

[DLG:IDD_ABOUTBOX (German (Germany))]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_IK220APP_DIALOG (German (Germany))]
Type=1
Class=?
ControlCount=28
Control1=IDC_RESET,button,1342242816
Control2=IDC_START,button,1342242816
Control3=IDC_STOP,button,1342242816
Control4=IDC_RESET_RI,button,1342242816
Control5=IDC_START_RI,button,1342242816
Control6=IDC_STOP_RI,button,1342242816
Control7=IDC_CHECK0,button,1342373891
Control8=IDC_CHECK1,button,1342242819
Control9=IDC_CHECK2,button,1342242819
Control10=IDC_CHECK3,button,1342242819
Control11=IDC_CHECK4,button,1342242819
Control12=IDC_CHECK5,button,1342242819
Control13=IDC_CHECK6,button,1342242819
Control14=IDC_CHECK7,button,1342242819
Control15=IDCANCEL,button,1342242817
Control16=IDC_Axis0,edit,1350568192
Control17=IDC_Axis1,edit,1350567936
Control18=IDC_Axis2,edit,1350567936
Control19=IDC_Axis3,edit,1350567936
Control20=IDC_Axis4,edit,1350567936
Control21=IDC_Axis5,edit,1350567936
Control22=IDC_Axis6,edit,1350567936
Control23=IDC_Axis7,edit,1350567936
Control24=IDC_Setup,button,1342242816
Control25=IDC_HEADLINE,static,1342312449
Control26=IDC_STATIC,static,1350569998
Control27=IDC_STATIC,static,1342177287
Control28=IDC_STATUS,button,1342242816

[DLG:IDD_SETUP (German (Germany))]
Type=1
Class=?
ControlCount=10
Control1=IDC_SIGNAL,button,1342308361
Control2=IDC_SIGNAL1,button,1342177289
Control3=IDC_STATIC,button,1342308359
Control4=IDC_ENCODER,button,1342308361
Control5=IDC_ENCODER1,button,1342177289
Control6=IDC_STATIC,button,1342308359
Control7=IDC_STATIC,static,1342308864
Control8=IDC_PERIOD,edit,1350631424
Control9=IDOK,button,1342242817
Control10=IDCANCEL,button,1342242816

[DLG:IDD_STATUS (German (Germany))]
Type=1
Class=?
ControlCount=17
Control1=IDOK,button,1342242817
Control2=IDC_STA1,static,1342308352
Control3=IDC_STA2,static,1342308352
Control4=IDC_STA3,static,1342308352
Control5=IDC_STA4,static,1342308352
Control6=IDC_STA5,static,1342308352
Control7=IDC_STA6,static,1342308352
Control8=IDC_STA7,static,1342308352
Control9=IDC_STA8,static,1342308352
Control10=IDC_STEXT1,static,1342308352
Control11=IDC_STEXT2,static,1342308352
Control12=IDC_STEXT3,static,1342308352
Control13=IDC_STEXT4,static,1342308352
Control14=IDC_STEXT5,static,1342308352
Control15=IDC_STEXT6,static,1342308352
Control16=IDC_STEXT7,static,1342308352
Control17=IDC_STEXT8,static,1342308352

[DLG:IDD_INITMSG (German (Germany))]
Type=1
Class=?
ControlCount=2
Control1=IDC_STATIC,static,1342308353
Control2=IDC_PROGRESS1,msctls_progress32,1350565888

