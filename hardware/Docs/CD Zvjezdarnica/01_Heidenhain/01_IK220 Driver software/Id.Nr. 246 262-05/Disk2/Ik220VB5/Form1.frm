VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "HEIDENHAIN IK 220"
   ClientHeight    =   6225
   ClientLeft      =   4635
   ClientTop       =   6045
   ClientWidth     =   9075
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   6225
   ScaleWidth      =   9075
   StartUpPosition =   2  'Bildschirmmitte
   Begin VB.CommandButton Status 
      Caption         =   "Stat&us"
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
      Left            =   3480
      TabIndex        =   25
      Top             =   5520
      Width           =   1695
   End
   Begin VB.CheckBox Check 
      Caption         =   "Axis &8:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   270
      Index           =   7
      Left            =   360
      TabIndex        =   24
      Top             =   4920
      Width           =   1335
   End
   Begin VB.CheckBox Check 
      Caption         =   "Axis &7:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   270
      Index           =   6
      Left            =   360
      TabIndex        =   23
      Top             =   4320
      Width           =   1335
   End
   Begin VB.CheckBox Check 
      Caption         =   "Axis &6:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   270
      Index           =   5
      Left            =   360
      TabIndex        =   22
      Top             =   3720
      Width           =   1335
   End
   Begin VB.CheckBox Check 
      Caption         =   "Axis &5:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   270
      Index           =   4
      Left            =   360
      TabIndex        =   21
      Top             =   3120
      Width           =   1335
   End
   Begin VB.CheckBox Check 
      Caption         =   "Axis &4:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   270
      Index           =   3
      Left            =   360
      TabIndex        =   20
      Top             =   2520
      Width           =   1335
   End
   Begin VB.CheckBox Check 
      Caption         =   "Axis &3:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   270
      Index           =   2
      Left            =   360
      TabIndex        =   19
      Top             =   1920
      Width           =   1335
   End
   Begin VB.CheckBox Check 
      Caption         =   "Axis &2:"
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
      Left            =   360
      TabIndex        =   18
      Top             =   1320
      Width           =   1335
   End
   Begin VB.CheckBox Check 
      Caption         =   "Axis &1:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   270
      Index           =   0
      Left            =   360
      TabIndex        =   17
      Top             =   840
      Width           =   1335
   End
   Begin VB.TextBox Axis 
      Alignment       =   1  'Rechts
      BackColor       =   &H80000000&
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
      Index           =   7
      Left            =   1920
      Locked          =   -1  'True
      TabIndex        =   16
      Top             =   4920
      Width           =   2415
   End
   Begin VB.TextBox Axis 
      Alignment       =   1  'Rechts
      BackColor       =   &H80000000&
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
      Index           =   6
      Left            =   1920
      Locked          =   -1  'True
      TabIndex        =   15
      Top             =   4320
      Width           =   2415
   End
   Begin VB.TextBox Axis 
      Alignment       =   1  'Rechts
      BackColor       =   &H80000000&
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
      Index           =   5
      Left            =   1920
      Locked          =   -1  'True
      TabIndex        =   14
      Top             =   3720
      Width           =   2415
   End
   Begin VB.TextBox Axis 
      Alignment       =   1  'Rechts
      BackColor       =   &H80000000&
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
      Index           =   4
      Left            =   1920
      Locked          =   -1  'True
      TabIndex        =   13
      Top             =   3120
      Width           =   2415
   End
   Begin VB.TextBox Axis 
      Alignment       =   1  'Rechts
      BackColor       =   &H80000000&
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
      Index           =   3
      Left            =   1920
      Locked          =   -1  'True
      TabIndex        =   12
      Top             =   2520
      Width           =   2415
   End
   Begin VB.TextBox Axis 
      Alignment       =   1  'Rechts
      BackColor       =   &H80000000&
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
      Index           =   2
      Left            =   1920
      Locked          =   -1  'True
      TabIndex        =   11
      Top             =   1920
      Width           =   2415
   End
   Begin VB.TextBox Axis 
      Alignment       =   1  'Rechts
      BackColor       =   &H80000000&
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
      Left            =   1920
      Locked          =   -1  'True
      TabIndex        =   10
      Top             =   1320
      Width           =   2415
   End
   Begin VB.CommandButton Cancel 
      Caption         =   "E&xit"
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
      Left            =   6720
      TabIndex        =   2
      Top             =   5520
      Width           =   1935
   End
   Begin VB.CommandButton Stop_RI 
      Caption         =   "St&op with REF"
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
      Left            =   6960
      TabIndex        =   9
      Top             =   2160
      Width           =   1815
   End
   Begin VB.CommandButton Start_RI 
      Caption         =   "St&art with REF"
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
      Left            =   6960
      TabIndex        =   8
      Top             =   1440
      Width           =   1815
   End
   Begin VB.CommandButton Reset_RI 
      Caption         =   "Rese&t with REF"
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
      Left            =   6960
      TabIndex        =   7
      Top             =   720
      Width           =   1815
   End
   Begin VB.CommandButton Stop 
      Caption         =   "Sto&p"
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
      Left            =   4920
      TabIndex        =   6
      Top             =   2160
      Width           =   1695
   End
   Begin VB.CommandButton Start 
      Caption         =   " &Start"
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
      Left            =   4920
      TabIndex        =   5
      Top             =   1440
      Width           =   1695
   End
   Begin VB.CommandButton Reset 
      Cancel          =   -1  'True
      Caption         =   " &Reset"
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
      Left            =   4920
      TabIndex        =   4
      Top             =   720
      Width           =   1695
   End
   Begin VB.Timer Timer1 
      Interval        =   150
      Left            =   4440
      Top             =   3600
   End
   Begin VB.CommandButton Setup 
      Caption         =   "&Setup"
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
      Left            =   360
      TabIndex        =   3
      Top             =   5520
      Width           =   1695
   End
   Begin VB.TextBox Axis 
      Alignment       =   1  'Rechts
      BackColor       =   &H80000000&
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
      Left            =   1920
      Locked          =   -1  'True
      TabIndex        =   0
      Top             =   720
      Width           =   2415
   End
   Begin VB.Image Image1 
      Height          =   1995
      Left            =   5880
      Picture         =   "Form1.frx":030A
      ToolTipText     =   "Dr. Johannes Heidenhain GmbH"
      Top             =   3120
      Width           =   1995
   End
   Begin VB.Label HeadLine 
      Alignment       =   2  'Zentriert
      BorderStyle     =   1  'Fest Einfach
      Caption         =   "Test Program for IK 220"
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
      TabIndex        =   1
      Top             =   120
      Width           =   8325
   End
   Begin VB.Menu INFO 
      Caption         =   "&About IK220App..."
      NegotiatePosition=   1  'Links
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Cancel_Click()
    Unload Form4
    Unload Me
End Sub

Private Sub Check_Click(Ax As Integer)
    If IKCard(Ax) = 0 Then Check(Ax) = 0
End Sub


Private Sub Form_Load()
    
    If IK220Find(IKCard(0)) = 0 Then Debug.Print "IK220Find returns error"
    For Ax = 0 To 7
        
        SignalPeriode(Ax) = 0.02
        
        If (IKCard(Ax) <> 0) Then
            If IK220Init(Ax) = 0 Then Debug.Print "IK220Init returns error"
            
            If (IK220ReadPar(Ax, 1, EncoderType(Ax)) = 0) Then Debug.Print "IK220ReadPar returns error"
            If (IK220ReadPar(Ax, 2, SignalType(Ax)) = 0) Then Debug.Print "IK220ReadPar returns error"
            
            Check(Ax) = 1
        End If
    Next Ax
    

End Sub


Private Sub Form_Unload(Cancel As Integer)
    Unload Form4
End Sub


Private Sub INFO_Click()
    Form3.Show vbModal
End Sub

Private Sub Reset_Click()
    For Ax = 0 To 7
        If Check(Ax) = 1 Then
            If (IKCard(Ax) <> 0) Then
                If IK220Reset(Ax) = 0 Then Debug.Print "IK220Reset returns error"
            End If
        End If
    Next Ax
End Sub

Private Sub Reset_RI_Click()
    For Ax = 0 To 7
        If Check(Ax) = 1 Then
            If (IKCard(Ax) <> 0) Then
                If IK220ResetRef(Ax) = 0 Then Debug.Print "IK220ResetRef returns error"
            End If
        End If
    Next Ax
End Sub

Private Sub Setup_Click()

    Ax = 0
    Do While ((IKCard(Ax) = 0) Or (Check(Ax) = 0))
        Ax = Ax + 1
        If (Ax > 7) Then Exit Do
    Loop
    
    If (Ax < 8) Then

        If (IK220ReadPar(Ax, 1, EncoderType(Ax)) = 0) Then Debug.Print "IK220ReadPar returns error"
        If (IK220ReadPar(Ax, 2, SignalType(Ax)) = 0) Then Debug.Print "IK220ReadPar returns error"

        If (SignalType(Ax) = 0) Then Form2.Option1(0) = True Else Form2.Option1(1) = True
        If (EncoderType(Ax) = 0) Then Form2.Option2(0) = True Else Form2.Option2(1) = True
        Form2.Signal = SignalPeriode(Ax)
    
        Form2.Show vbModal
        
    Else
        MsgBox ("No axis selected")
    End If
        
End Sub

Private Sub Start_Click()
    For Ax = 0 To 7
        If Check(Ax) = 1 Then
            If (IKCard(Ax) <> 0) Then
                If IK220Start(Ax) = 0 Then Debug.Print "IK220Start returns error"
            End If
        End If
    Next Ax
End Sub

Private Sub Start_RI_Click()
    For Ax = 0 To 7
        If Check(Ax) = 1 Then
            If (IKCard(Ax) <> 0) Then
                If IK220StartRef(Ax) = 0 Then Debug.Print "IK220StartRef returns error"
            End If
        End If
    Next Ax
End Sub

Private Sub Status_Click()
    For Ax = 0 To 7
        OldSta(Ax) = &HFFFF
    Next Ax
    Form4.Show
End Sub

Private Sub Stop_Click()
    For Ax = 0 To 7
        If Check(Ax) = 1 Then
            If (IKCard(Ax) <> 0) Then
                If IK220Stop(Ax) = 0 Then Debug.Print "IK220Stop returns error"
            End If
        End If
    Next Ax
End Sub

Private Sub Stop_RI_Click()
    For Ax = 0 To 7
        If Check(Ax) = 1 Then
            If (IKCard(Ax) <> 0) Then
                If IK220StopRef(Ax) = 0 Then Debug.Print "IK220StopRef returns error"
            End If
        End If
    Next Ax
End Sub

Private Sub Timer1_Timer()
Dim Count As Double

Dim EnStatus As Integer
Dim EnAlarm As Integer

    For Ax = 0 To 7
        If (IKCard(Ax) <> 0) Then
        
            ' Read incremental value
            If (EncoderType(Ax) = 0) Then
                If (IK220Read48(Ax, 0, Count) = 0) Then
                    Debug.Print "IK220Read48 returns error"
                    Form1.Axis(Ax) = "   E r r o r"
                Else
                    Count = Count * SignalPeriode(Ax)
                    Form1.Axis(Ax) = Format(Count, "0.000000")
                End If
            End If
            
          
            ' Read EnDat value
            If (EncoderType(Ax) = 1) Then
                If (IK220ReadEn(Ax, EnStatus, Count, EnAlarm) = 0) Then
                    Debug.Print "IK220ReadEn returns error"
                    Form1.Axis(Ax) = "   E r r o r"
                Else
                    Count = Count * SignalPeriode(Ax)
                    Form1.Axis(Ax) = Format(Count, "0.000000")
                End If
            End If
            
        End If
    Next Ax
    DoStatus
End Sub

