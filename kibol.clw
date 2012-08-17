; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CKibolDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "kibol.h"

ClassCount=3
Class1=CKibolApp
Class2=CKibolDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_KIBOL_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_KIBOL_DIALOG (English (U.S.))
Resource6=IDR_MENU1

[CLS:CKibolApp]
Type=0
HeaderFile=kibol.h
ImplementationFile=kibol.cpp
Filter=N

[CLS:CKibolDlg]
Type=0
HeaderFile=kibolDlg.h
ImplementationFile=kibolDlg.cpp
Filter=D
LastObject=ID_GRA_NOWA
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=kibolDlg.h
ImplementationFile=kibolDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_KIBOL_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CKibolDlg

[DLG:IDD_KIBOL_DIALOG (English (U.S.))]
Type=1
Class=CKibolDlg
ControlCount=0

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[MNU:IDR_MENU1]
Type=1
Class=CKibolDlg
Command1=ID_GRA_NOWA
Command2=ID_GRA_MARKERY
Command3=ID_GRA_KONIEC
Command4=ID_TRYB_HUMVSHUM
Command5=ID_TRYB_HUMVSKOMP
Command6=ID_TRYB_KOMPVSHUM
CommandCount=6

