Attribute VB_Name = "Module2"
Option Explicit

Public OldSta(7) As Long


Public Sub DoStatus()
    Dim Stat As Long
    Dim Ax As Integer
    
    
    Stat = 0
    For Ax = 0 To 7
        If (IKCard(Ax) <> 0) Then
            If IK220Status(Ax, Stat) Then
            If OldSta(Ax) <> Stat Then
                OldSta(Ax) = Stat
                    
                Form4.StatusTXT(Ax) = ""
                If (Stat And &H1) <> 0 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "Latch 0, "
                If (Stat And &H2) <> 0 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "Latch 1, "
                If (Stat And &H4) <> 0 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "Latch 2 ,"
                
                If (Stat And &H20) <> 0 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "started, " Else Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "stopped, "
                If (Stat And &H40) <> 0 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "REF function active, "
                If (Stat And &H80) <> 0 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "Frequency Error, "
                
                If (Stat And &HC000) = &H0 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "No REF"
                If (Stat And &HC000) = &H4000 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "Wait 1. REF"
                If (Stat And &HC000) = &H8000 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "Wait 2. REF"
                If (Stat And &HC000) = &HC000 Then Form4.StatusTXT(Ax) = Form4.StatusTXT(Ax) + "REF"
                
                
            End If
                 
           Else
                Form4.StatusTXT(Ax) = "   --- Status Error ---"
                Debug.Print "IKStatus returns error"
            End If
        Else
             Form4.StatusTXT(Ax) = "   --- No IK 220 installed ---"
        End If
    Next Ax
End Sub


