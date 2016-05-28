Attribute VB_Name = "DllFunc"
Option Explicit

Public Declare Function IK220Find Lib "IK220DLL.DLL" (ByRef pBuffer16 As Long) As Boolean
Public Declare Function IK220Find256 Lib "IK220DLL.DLL" (ByRef pBuffer256 As Long) As Boolean
Public Declare Function IK220Init Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220Version Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pVersCard As Byte, ByRef pVersDrv As Byte, ByRef pVersDll As Byte) As Boolean

Public Declare Function IK220Reset Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220Start Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220Stop Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220ClearErr Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220Latch Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer) As Boolean

Public Declare Function IK220LatchInt Lib "IK220DLL.DLL" (ByVal Card As Integer) As Boolean
Public Declare Function IK220LatchExt Lib "IK220DLL.DLL" (ByVal Card As Integer) As Boolean

Public Declare Function IK220ResetRef Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220StartRef Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220StopRef Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220LatchRef Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean

Public Declare Function IK220Latched Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer, ByRef pStatus As Boolean) As Boolean
Public Declare Function IK220WaitLatch Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer) As Boolean

Public Declare Function IK220SetTimeOut Lib "IK220DLL.DLL" (ByVal TimOut As Long) As Boolean

Public Declare Function IK220Set Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal SetVal As Double) As Boolean
Public Declare Function IK220SetPreset Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal PresVal As Double) As Boolean
Public Declare Function IK220GetPreset Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pPresVal As Double) As Boolean

Public Declare Function IK220Read32 Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer, ByRef pData As Long) As Boolean
Public Declare Function IK220Read48 Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer, ByRef pData As Double) As Boolean
Public Declare Function IK220Get32 Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer, ByRef pData As Long) As Boolean
Public Declare Function IK220Get48 Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer, ByRef pData As Double) As Boolean
Public Declare Function IK220CntStatus Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer, ByRef pRefSta As Integer, ByRef pKorr00 As Integer, ByRef pKorr90 As Integer, ByRef pNKorr00 As Integer, ByRef pNKorr90 As Integer, ByRef pSamCnt As Integer) As Boolean

Public Declare Function IK220DoRef Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220CancelRef Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220RefActive Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pStatus As Boolean) As Boolean
Public Declare Function IK220WaitRef Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean

Public Declare Function IK220PositionRef Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pData As Double, ByRef pPeriod As Long, ByRef pIntpol As Integer) As Boolean
Public Declare Function IK220PositionRef2 Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pData1 As Double, ByRef pPeriod1 As Long, ByRef pIntpol1 As Integer, ByRef pData2 As Double, ByRef pPeriod2 As Long, ByRef pIntpol2 As Integer) As Boolean

Public Declare Function IK220Status Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pData As Long) As Boolean
Public Declare Function IK220DllStatus Lib "IK220DLL.DLL" (ByRef pStatus As Long, ByRef pInfo As Long) As Boolean
Public Declare Function IK220RefStatus Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pRef1 As Long, ByRef pRef2 As Long, ByRef pDiff As Long, ByRef pCode As Integer, ByRef pFlag As Integer) As Boolean
Public Declare Function IK220SignalStatus Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pFreq As Integer, ByRef pAmin As Integer, ByRef pAact As Integer, ByRef pAmax As Integer) As Boolean

Public Declare Function IK220GetCorrA Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pOfs0 As Integer, ByRef pOfs90 As Integer, ByRef pPha0 As Integer, ByRef pPha90 As Integer, ByRef pSym0 As Integer, ByRef pSym90 As Integer, ByRef pFlag1 As Integer, ByRef pFlag2 As Integer) As Boolean
Public Declare Function IK220GetCorrB Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pOfs0 As Integer, ByRef pOfs90 As Integer, ByRef pPha0 As Integer, ByRef pPha90 As Integer, ByRef pSym0 As Integer, ByRef pSym90 As Integer, ByRef pFlag1 As Integer, ByRef pFlag2 As Integer) As Boolean
Public Declare Function IK220LoadCorrA Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Ofs0 As Integer, ByVal Ofs90 As Integer, ByVal Pha0 As Integer, ByVal Pha90 As Integer, ByVal Sym0 As Integer, ByVal Sym90 As Integer) As Boolean
Public Declare Function IK220OctStatus Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pOct0 As Integer, ByRef pOct1 As Integer, ByRef pOct2 As Integer, ByRef pOct3 As Integer, ByRef pOct4 As Integer, ByRef pOct5 As Integer, ByRef pOct6 As Integer, ByRef pOct7 As Integer, ByRef pSamCnt) As Boolean

Public Declare Function IK220ChkSumPar Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pChkSum As Integer) As Boolean
Public Declare Function IK220ChkSumPrg Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pChkSum1 As Integer, ByRef pChkSum2 As Integer) As Boolean

Public Declare Function IK220WritePar Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal ParNum As Integer, ByVal ParVal As Integer) As Boolean
Public Declare Function IK220ReadPar Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal ParNum As Integer, ByRef pParVal As Integer) As Boolean

Public Declare Function IK220ResetEn Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pStatus As Integer) As Boolean
Public Declare Function IK220ConfigEn Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pStatus As Integer, ByRef pType As Integer, ByRef pPeriod As Long, ByRef pStep As Long, ByRef pTurns As Integer, ByRef pRefDist As Integer, ByRef pCntDir As Integer) As Boolean
Public Declare Function IK220ReadEn Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pStatus As Integer, ByRef pData As Double, ByRef pAlarm As Integer) As Boolean
Public Declare Function IK220ReadEnInc Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer, ByRef pStatus As Integer, ByRef pDataEn As Double, ByRef pAlarm As Integer, ByRef pDataInc As Double) As Boolean

Public Declare Function IK220ModeEnCont Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer, ByVal Mode As Integer, ByRef pStatus As Integer) As Boolean
Public Declare Function IK220ReadEnIncCont Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pStatus As Integer, ByRef pDataEn As Double, ByRef pAlarm As Integer, ByRef pDataInc As Double, ByRef pSigSta As Double) As Boolean

Public Declare Function IK220AlarmEn Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pStatus As Integer, ByRef pAlarm As Integer) As Boolean
Public Declare Function IK220WarnEn Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pStatus As Integer, ByRef pWarn As Integer) As Boolean
Public Declare Function IK220ReadMemEn Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Range As Integer, ByVal MemAdr As Integer, ByRef pMemData As Integer, ByRef pStatus As Integer) As Boolean
Public Declare Function IK220WriteMemEn Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Range As Integer, ByVal MemAdr As Integer, ByRef MemData As Integer, ByRef pStatus As Integer) As Boolean

Public Declare Function IK220ReadSSI Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pStatus As Integer, ByRef pData As Double) As Boolean
Public Declare Function IK220ReadSsiInc Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Latch As Integer, ByRef pStatus As Integer, ByRef pDataSsi As Double, ByRef pDataInc As Double) As Boolean

Public Declare Function IK220SetTimer Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal SetVal As Long, ByRef pTimVal As Long) As Boolean
Public Declare Function IK220ModeTimer Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Mode As Integer) As Boolean

Public Declare Function IK220ModeRam Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Mode As Integer) As Boolean
Public Declare Function IK220ResetRam Lib "IK220DLL.DLL" (ByVal Axis As Integer) As Boolean
Public Declare Function IK220GetRam Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pData As Double, ByRef pRead As Integer, ByRef pWrite As Integer, ByRef pStatus As Integer) As Boolean
Public Declare Function IK220BurstRam Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal maxCount As Integer, ByRef pData As Double, ByRef Count As Integer, ByRef pStatus As Integer) As Boolean
Public Declare Function IK220GetSig Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pPeriod As Integer, ByRef pAmp0 As Integer, ByRef pAmp90 As Integer, ByRef pRead As Integer, ByRef pWrite As Integer, ByRef pStatus As Integer) As Boolean
Public Declare Function IK220BurstSig Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal maxCount As Integer, ByRef pPeriod As Integer, ByRef pAmp0 As Integer, ByRef pAmp90 As Integer, ByRef Count As Integer, ByRef pStatus As Integer) As Boolean

Public Declare Function IK220Led Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Mode As Integer) As Boolean
Public Declare Function IK220SysLed Lib "IK220DLL.DLL" (ByVal Card As Integer, ByVal Mode As Integer) As Boolean
Public Declare Function IK220GetPort Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pPortInfo As Integer, ByRef pRising As Integer, ByRef pFalling As Integer) As Boolean

Public Declare Function IK220InputW Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Adr As Integer, ByRef pData As Integer) As Boolean
Public Declare Function IK220InputL Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Adr As Integer, ByRef pData As Long) As Boolean
Public Declare Function IK220Output Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Adr As Integer, ByVal Data As Integer) As Boolean

Public Declare Function IK220RamRead Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Adr As Integer, ByRef pData As Integer) As Boolean
Public Declare Function IK220RamWrite Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Adr As Integer, ByVal Data As Integer) As Boolean

Public Declare Function IK220DownLoad Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pPgmData As Integer, ByVal PgmSize As Long) As Boolean

Public Declare Function IK220SetEnClock Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal State As Boolean, ByRef pStatus As Integer) As Boolean
Public Declare Function IK220SetEnData Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal State As Boolean, ByRef pStatus As Integer) As Boolean
Public Declare Function IK220ReadEnData Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pState As Boolean) As Boolean

Public Declare Function IK220RefEval Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Mode As Integer) As Boolean

Public Declare Function IK220SetBw Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Mode As Integer) As Boolean
Public Declare Function IK220SetTrm Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Mode As Integer) As Boolean

Public Declare Function IK220SetOpMode Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByVal Mode As Integer) As Boolean
Public Declare Function IK220GetOpMode Lib "IK220DLL.DLL" (ByVal Axis As Integer, ByRef pMode As Integer) As Boolean

Public Declare Function IK220SetLatInt "IK220DLL.DLL" (ByVal Card As Integer, ByVal Mode As Integer) As Boolean
Public Declare Function IK220SetLatExt "IK220DLL.DLL" (ByVal Card As Integer, ByVal Mode As Integer) As Boolean

