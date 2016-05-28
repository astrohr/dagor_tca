VERSION 5.00
Begin VB.Form Form4 
   Caption         =   "IK 220 - Status"
   ClientHeight    =   5385
   ClientLeft      =   4860
   ClientTop       =   7050
   ClientWidth     =   9075
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   9.75
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "Form4.frx":0000
   LinkTopic       =   "Form4"
   ScaleHeight     =   5385
   ScaleWidth      =   9075
   Begin VB.CommandButton OK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   7680
      TabIndex        =   1
      Top             =   240
      Width           =   975
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Axis 8:"
      Height          =   240
      Index           =   7
      Left            =   360
      TabIndex        =   16
      Top             =   4560
      Width           =   615
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Axis 7:"
      Height          =   240
      Index           =   6
      Left            =   360
      TabIndex        =   15
      Top             =   3960
      Width           =   615
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Axis 6:"
      Height          =   240
      Index           =   5
      Left            =   360
      TabIndex        =   14
      Top             =   3360
      Width           =   615
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Axis 5:"
      Height          =   240
      Index           =   4
      Left            =   360
      TabIndex        =   13
      Top             =   2760
      Width           =   615
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Axis 4:"
      Height          =   240
      Index           =   3
      Left            =   360
      TabIndex        =   12
      Top             =   2160
      Width           =   615
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Axis 3:"
      Height          =   240
      Index           =   2
      Left            =   360
      TabIndex        =   11
      Top             =   1455
      Width           =   615
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Axis 2:"
      Height          =   240
      Index           =   1
      Left            =   360
      TabIndex        =   10
      Top             =   915
      Width           =   615
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Axis 1:"
      Height          =   240
      Index           =   0
      Left            =   360
      TabIndex        =   9
      Top             =   360
      Width           =   615
   End
   Begin VB.Label StatusTXT 
      Height          =   480
      Index           =   7
      Left            =   1320
      TabIndex        =   8
      Top             =   4560
      Width           =   5820
   End
   Begin VB.Label StatusTXT 
      Height          =   480
      Index           =   6
      Left            =   1320
      TabIndex        =   7
      Top             =   3960
      Width           =   5820
   End
   Begin VB.Label StatusTXT 
      Height          =   480
      Index           =   5
      Left            =   1320
      TabIndex        =   6
      Top             =   3360
      Width           =   5820
   End
   Begin VB.Label StatusTXT 
      Height          =   480
      Index           =   4
      Left            =   1320
      TabIndex        =   5
      Top             =   2760
      Width           =   5820
   End
   Begin VB.Label StatusTXT 
      Height          =   480
      Index           =   3
      Left            =   1320
      TabIndex        =   4
      Top             =   2160
      Width           =   5820
   End
   Begin VB.Label StatusTXT 
      Height          =   480
      Index           =   2
      Left            =   1320
      TabIndex        =   3
      Top             =   1455
      Width           =   5820
   End
   Begin VB.Label StatusTXT 
      Height          =   480
      Index           =   1
      Left            =   1320
      TabIndex        =   2
      Top             =   915
      Width           =   5820
   End
   Begin VB.Label StatusTXT 
      Height          =   480
      Index           =   0
      Left            =   1320
      TabIndex        =   0
      Top             =   360
      Width           =   5820
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "Form4"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private Sub Form_Load()
    DoStatus
End Sub

Private Sub OK_Click()
    Unload Me
End Sub

