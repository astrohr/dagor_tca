VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  'Fester Dialog
   Caption         =   "Info über IK220App"
   ClientHeight    =   3555
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   5730
   ClipControls    =   0   'False
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2453.724
   ScaleMode       =   0  'Benutzer
   ScaleWidth      =   5380.766
   ShowInTaskbar   =   0   'False
   Begin VB.PictureBox picIcon 
      AutoSize        =   -1  'True
      ClipControls    =   0   'False
      Height          =   540
      Left            =   240
      Picture         =   "frmAbout.frx":0000
      ScaleHeight     =   337.12
      ScaleMode       =   0  'Benutzer
      ScaleWidth      =   337.12
      TabIndex        =   1
      Top             =   240
      Width           =   540
   End
   Begin VB.CommandButton cmdOK 
      Cancel          =   -1  'True
      Caption         =   "&OK"
      Default         =   -1  'True
      Height          =   345
      Left            =   4245
      TabIndex        =   0
      Top             =   2625
      Width           =   1260
   End
   Begin VB.CommandButton cmdSysInfo 
      Caption         =   "&System-Info..."
      Height          =   345
      Left            =   4260
      TabIndex        =   2
      Top             =   3075
      Width           =   1245
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      BorderStyle     =   6  'Innen ausgefüllt
      Index           =   1
      X1              =   84.515
      X2              =   5309.398
      Y1              =   1687.583
      Y2              =   1687.583
   End
   Begin VB.Label lblDescription 
      Caption         =   "Dr. Johannes Heidehain GmbH"
      ForeColor       =   &H00000000&
      Height          =   330
      Left            =   1050
      TabIndex        =   3
      Top             =   1125
      Width           =   3885
   End
   Begin VB.Label lblTitle 
      Caption         =   "IK220App, Versio 1.0"
      ForeColor       =   &H00000000&
      Height          =   480
      Left            =   1050
      TabIndex        =   4
      Top             =   240
      Width           =   3885
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00FFFFFF&
      BorderWidth     =   2
      Index           =   0
      X1              =   98.6
      X2              =   5309.398
      Y1              =   1697.936
      Y2              =   1697.936
   End
   Begin VB.Label lblVersion 
      Caption         =   "Copyright 1997"
      Height          =   225
      Left            =   1050
      TabIndex        =   5
      Top             =   780
      Width           =   3885
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

' Optionen für Registrierungsschlüssel Security ...
Const READ_CONTROL = &H20000
Const KEY_QUERY_VALUE = &H1
Const KEY_SET_VALUE = &H2
Const KEY_CREATE_SUB_KEY = &H4
Const KEY_ENUMERATE_SUB_KEYS = &H8
Const KEY_NOTIFY = &H10
Const KEY_CREATE_LINK = &H20
Const KEY_ALL_ACCESS = KEY_QUERY_VALUE + KEY_SET_VALUE + _
                       KEY_CREATE_SUB_KEY + KEY_ENUMERATE_SUB_KEYS + _
                       KEY_NOTIFY + KEY_CREATE_LINK + READ_CONTROL
                     
' Typen für Registrierungssschlüssel ROOT ...
Const HKEY_LOCAL_MACHINE = &H80000002
Const ERROR_SUCCESS = 0
Const REG_SZ = 1         ' Mit NUL abgeschlossene Unicode-Zeichenfolge
Const REG_DWORD = 4      ' 32-Bit-Zahl

Const gREGKEYSYSINFOLOC = "SOFTWARE\Microsoft\Shared Tools Location"
Const gREGVALSYSINFOLOC = "MSINFO"
Const gREGKEYSYSINFO = "SOFTWARE\Microsoft\Shared Tools\MSINFO"
Const gREGVALSYSINFO = "PATH"

Private Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, ByRef phkResult As Long) As Long
Private Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, ByRef lpType As Long, ByVal lpData As String, ByRef lpcbData As Long) As Long
Private Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long


Private Sub cmdSysInfo_Click()
  Call StartSysInfo
End Sub

Private Sub cmdOK_Click()
  Unload Me
End Sub

Private Sub Form_Load()
    Me.Caption = App.Title & "-Info"
    lblVersion.Caption = "Version " & App.Major & "." & App.Minor & "." & App.Revision
    lblTitle.Caption = App.Title
End Sub

Public Sub StartSysInfo()
    On Error GoTo SysInfoErr
  
    Dim rc As Long
    Dim SysInfoPath As String
    
    ' Auslesen von Pfad und Namen des Systeminformations-
    ' programms aus der Registrierung ...
    If GetKeyValue(HKEY_LOCAL_MACHINE, gREGKEYSYSINFO, gREGVALSYSINFO, SysInfoPath) Then
    ' Auslesen allein des Pfads des Systeminformationsprogramms.
    ElseIf GetKeyValue(HKEY_LOCAL_MACHINE, gREGKEYSYSINFOLOC, gREGVALSYSINFOLOC, SysInfoPath) Then
        ' Prüfen, ob die bekannte Datei in der 32-Bit-Version
        ' vorhanden ist.
        If (Dir(SysInfoPath & "\MSINFO32.EXE") <> "") Then
            SysInfoPath = SysInfoPath & "\MSINFO32.EXE"
            
        ' Fehler - Datei wurde nicht gefunden...
        Else
            GoTo SysInfoErr
        End If
    ' Fehler - Registrierungseintrag wurde nicht gefunden.
    Else
        GoTo SysInfoErr
    End If
    
    Call Shell(SysInfoPath, vbNormalFocus)
    
    Exit Sub
SysInfoErr:
    MsgBox "System-Informationen sind augenblicklich nicht verfügbar.", vbOKOnly
End Sub

Public Function GetKeyValue(KeyRoot As Long, KeyName As String, SubKeyRef As String, ByRef KeyVal As String) As Boolean
    Dim i As Long             ' Schleifenzählvariable
    Dim rc As Long            ' Rückgabe-Code
    Dim hKey As Long          ' Zugriffsnummer auf einen
                              ' geöffneten Registrierungsschlüssel
    Dim hDepth As Long        '
    Dim KeyValType As Long    ' Datentyp eines Registrierungsschlüssels
    Dim tmpVal As String      ' Temporärer Speicher für
                              ' einen Registrierungsschlüssel
    Dim KeyValSize As Long    ' Größe einer Registrierungsschlüsselvariablen
    '------------------------------------------------------------
    ' Öffnen des Registrierungsschlüssels unter dem
    ' Schlüsselstammverzeichnis {HKEY_LOCAL_MACHINE...}
    '------------------------------------------------------------
    rc = RegOpenKeyEx(KeyRoot, KeyName, 0, KEY_ALL_ACCESS, hKey) ' Öffnen des Registrierungsschlüssels.
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError    ' Fehlerbehandlung...
    
    tmpVal = String$(1024, 0)           ' Zuordnen des Variablenspeichers
    KeyValSize = 1024                   ' Festlegen für variable Größe.
    
    '------------------------------------------------------------
    ' Ermitteln des Registrierungsschlüsselwerts...
    '------------------------------------------------------------
    rc = RegQueryValueEx(hKey, SubKeyRef, 0, _
                         KeyValType, tmpVal, KeyValSize) ' Auslesen/Erzeugen des Schlüsselwerts.
                        
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError       ' Fehlerbehandlung
    
    If (Asc(Mid(tmpVal, KeyValSize, 1)) = 0) Then    ' Win95 fügt eine mit Null abgeschlossene
                                                     ' Zeichenfolge hinzu.
        tmpVal = Left(tmpVal, KeyValSize - 1)        ' Null wurde gefunden, Zeichenfolge ohne sie auslesen.
    Else                                             ' WinNT hängt keine Null an...
        tmpVal = Left(tmpVal, KeyValSize)            ' Null nicht gefunden. Nur Zeichenfolge auslesen.
    End If
    
    '------------------------------------------------------------
    ' Bestimmen des Schlüsselwerttyps für die Umwandlung...
    '------------------------------------------------------------
    Select Case KeyValType                 ' Suchen der Datentypen...
    Case REG_SZ                            ' Zeichenfolgen-Datentyp des Registrierungsschlüssels
        KeyVal = tmpVal                    ' Kopieren des Zeichenfolgenwerts.
    Case REG_DWORD                         ' Registrierungsschlüssel-Datentyp Double Word
        For i = Len(tmpVal) To 1 Step -1   ' Jedes Bit konvertieren
            ' Zusammensetzen des Werts Zeichen für Zeichen.
            KeyVal = KeyVal + Hex(Asc(Mid(tmpVal, i, 1)))
        Next
        KeyVal = Format$("&h" + KeyVal)    ' Umwandeln von Double Word in String
    End Select
    
    GetKeyValue = True          ' Übermitteln des Erfolgs
    rc = RegCloseKey(hKey)      ' Schließen der Registrierung
    Exit Function               ' Beenden
    
GetKeyError:      ' Aufräumen nach Auftritt eines Fehlers...
    KeyVal = ""                ' Rückgabewert wird leere Zeichenfolge zugewiesen.
    GetKeyValue = False        ' Übermitteln des Mißerfolgs
    rc = RegCloseKey(hKey)     ' Schließen des Registrierungsschlüssels.
End Function
