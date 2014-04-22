; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPC_SimpleDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PC_Simple.h"

ClassCount=3
Class1=CPC_SimpleApp
Class2=CPC_SimpleDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_PC_SIMPLE_DIALOG

[CLS:CPC_SimpleApp]
Type=0
HeaderFile=PC_Simple.h
ImplementationFile=PC_Simple.cpp
Filter=N

[CLS:CPC_SimpleDlg]
Type=0
HeaderFile=PC_SimpleDlg.h
ImplementationFile=PC_SimpleDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_RECEIVEBYTE

[CLS:CAboutDlg]
Type=0
HeaderFile=PC_SimpleDlg.h
ImplementationFile=PC_SimpleDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PC_SIMPLE_DIALOG]
Type=1
Class=CPC_SimpleDlg
ControlCount=6
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_RECEIVEBYTE,edit,1350631552
Control5=IDC_SEND,button,1342242816
Control6=IDC_STATIC,static,1342308352

