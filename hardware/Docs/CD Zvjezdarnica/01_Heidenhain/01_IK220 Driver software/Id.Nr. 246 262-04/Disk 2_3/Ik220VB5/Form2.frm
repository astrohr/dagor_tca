VERSION 5.00
Begin VB.Form Form2 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "IK 220 - Setup"
   ClientHeight    =   4635
   ClientLeft      =   7755
   ClientTop       =   7605
   ClientWidth     =   7230
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "Form2.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4635
   ScaleWidth      =   7230
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'Fenstermitte
   Begin VB.Frame Frame2 
      Caption         =   "Encoder"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1575
      Left            =   2160
      TabIndex        =   7
      Top             =   240
      Width           =   1935
      Begin VB.OptionButton Option2 
         Caption         =   "EnDat"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   1
         Left            =   120
         TabIndex        =   9
         Top             =   960
         Width           =   1335
      End
      Begin VB.OptionButton Option2 
         Caption         =   "incremental"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   0
         Left            =   120
         TabIndex        =   8
         Top             =   360
         Width           =   1575
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Signal"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1575
      Left            =   360
      TabIndex        =   4
      Top             =   240
      Width           =   1575
      Begin VB.OptionButton Option1 
         Caption         =   "1Vss"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   11.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   1
         Left            =   240
         TabIndex        =   6
         Top             =   960
         Width           =   855
      End
      Begin VB.OptionButton Option1 
         Caption         =   "11µA"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   0
         Left            =   240
         TabIndex        =   5
         Top             =   360
         Width           =   1095
      End
   End
   Begin VB.TextBox Signal 
      Alignment       =   1  'Rechts
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2160
      TabIndex        =   2
      TabStop         =   0   'False
      Top             =   2760
      Width           =   1455
   End
   Begin VB.CommandButton Cancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   4800
      TabIndex        =   1
      Top             =   960
      Width           =   1575
   End
   Begin VB.CommandButton OK 
      Caption         =   "OK"
      Default         =   -1  'True
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   4800
      TabIndex        =   0
      Top             =   240
      Width           =   1575
   End
   Begin VB.Label SignalText 
      Caption         =   "Signal period:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   360
      TabIndex        =   3
      Top             =   2760
      Width           =   1575
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Cancel_Click()
    Unload Me
End Sub

Private Sub OK_Click()

Dim EnStatus As Integer
Dim EnType As Integer
Dim EnPeriod As Long
Dim EnStep As Long
Dim EnTurns As Integer
Dim EnRefDist As Integer
Dim EnCntDir As Integer
                    

    For Ax = 0 To 7
        If Form1.Check(Ax) = 1 Then
            If (IKCard(Ax) <> 0) Then
                If (Option1(0)) Then SignalType(Ax) = 0 Else SignalType(Ax) = 1
                If (Option2(0)) Then EncoderType(Ax) = 0 Else EncoderType(Ax) = 1
                SignalPeriode(Ax) = Form2.Signal
                    
                    If (IK220WritePar(Ax, 1, EncoderType(Ax)) = 0) Then Debug.Print "IK220WritePar returns error"
                    If (IK220WritePar(Ax, 2, SignalType(Ax)) = 0) Then Debug.Print "IK220WritePar returns error"

                    If (EncoderType(Ax) = 1) Then
                        If (IK220ResetEn(Ax, EnStatus) = 0) Then Debug.Print "IK220ResetEn returns error"
                        If (IK220ConfigEn(Ax, EnStatus, EnType, EnPeriod, EnStep, EnTurns, EnRefDist, EnCntDir) = 0) Then Debug.Print "IK220ConfigEn returns error"
                    End If

            End If
        End If
    Next Ax


    Unload Me
End Sub

