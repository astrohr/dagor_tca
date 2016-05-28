VERSION 5.00
Begin VB.Form Form3 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "About IK 220"
   ClientHeight    =   2535
   ClientLeft      =   5940
   ClientTop       =   8115
   ClientWidth     =   6615
   ControlBox      =   0   'False
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   9.75
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "Form3.frx":0000
   LinkTopic       =   "Form3"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2535
   ScaleWidth      =   6615
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'Fenstermitte
   Begin VB.CommandButton OK 
      Caption         =   "OK"
      Default         =   -1  'True
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   5160
      TabIndex        =   1
      Top             =   480
      Width           =   975
   End
   Begin VB.Image Image1 
      Height          =   480
      Left            =   840
      Picture         =   "Form3.frx":000C
      Top             =   1080
      Width           =   480
   End
   Begin VB.Label Label3 
      Caption         =   "Dr. Johannes Heidenhain GmbH"
      Height          =   375
      Left            =   1800
      TabIndex        =   3
      Top             =   1800
      Width           =   3015
   End
   Begin VB.Label Label2 
      Caption         =   "Copyright 2000"
      Height          =   255
      Left            =   1800
      TabIndex        =   2
      Top             =   1200
      Width           =   2535
   End
   Begin VB.Label Label1 
      Caption         =   "IK220App, Version 3.00"
      Height          =   255
      Left            =   1800
      TabIndex        =   0
      Top             =   600
      Width           =   2415
   End
End
Attribute VB_Name = "Form3"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub OK_Click()
    Unload Me
End Sub
