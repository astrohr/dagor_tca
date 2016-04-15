UNIT ik220_base_unit;

interface
(******************************************************************)
uses
  Windows,Dialogs;
(******************************************************************)
(* Typen                                                          *)
(******************************************************************)
type
  Bool32    =  LongInt;
  Bool32Ptr = ^Bool32;
  Long16    =  array [0..15] of longint;
  Long16Ptr = ^Long16;
  BytePtr   = ^Byte;
  TChar     =  array [0..255] of Char;
  CharPtr   = ^TChar;
  IntPtr    = ^SmallInt;
  WordPtr   = ^Word;
  LongPtr   = ^LongInt;
  DoublePtr = ^Double;


  TIK220Find = function (pBuffer16: Long16Ptr) : Bool32; StdCall;
  TIK220Init = function (Axis: Word) : Bool32; StdCall;
  TIK220Version = function (Axis: Word; pVersCard: CharPtr; pVersDrv: CharPtr; pVersDll: CharPtr) : Bool32; StdCall;
  TIK220Reset = function (Axis: Word) : Bool32; StdCall;
  TIK220Start = function (Axis: Word) : Bool32; StdCall;
  TIK220Stop = function (Axis: Word) : Bool32; StdCall;
  TIK220ClearErr = function (Axis: Word) : Bool32; StdCall;
  TIK220Latch = function (Axis: Word; Latch: Word) : Bool32; StdCall;
  TIK220LatchInt = function (Card: Word) : Bool32; StdCall;
  TIK220LatchExt = function (Card: Word) : Bool32; StdCall;
  TIK220ResetRef = function (Axis: Word) : Bool32; StdCall;
  TIK220StartRef = function (Axis: Word) : Bool32; StdCall;
  TIK220StopRef = function (Axis: Word) : Bool32; StdCall;
  TIK220LatchRef = function (Axis: Word) : Bool32; StdCall;
  TIK220Latched = function (Axis: Word; Latch: Word; pStatus: Bool32Ptr) : Bool32; StdCall;
  TIK220WaitLatch = function (Axis: Word; Latch: Word) : Bool32; StdCall;
  TIK220Read32 = function (Axis: Word; Latch: Word; pData: LongPtr)   : Bool32; StdCall;
  TIK220Read48 = function	(Axis: Word; Latch: Word; pData: DoublePtr) : Bool32; StdCall;
  TIK220Get32 = function (Axis: Word; Latch: Word; pData: LongPtr)   : Bool32; StdCall;
  TIK220Get48 = function (Axis: Word; Latch: Word; pData: DoublePtr) : Bool32; StdCall;
  TIK220CntStatus = function (Axis: Word; Latch: Word; pRefSta: WordPtr; pKorr00: IntPtr; pKorr90: IntPtr; pNKorr00: IntPtr; pNKorr90: IntPtr; pSamCnt: WordPtr) : Bool32; StdCall;
  TIK220DoRef = function (Axis: Word) : Bool32; StdCall;
  TIK220RefActive = function (Axis: Word; pStatus: Bool32Ptr) : Bool32; StdCall;
  TIK220WaitRef = function (Axis: Word) : Bool32; StdCall;
  TIK220CancelRef = function (Axis: Word) : Bool32; StdCall;
  TIK220PositionRef = function (Axis: Word; pData: DoublePtr; pPeriod: LongPtr; pIntpol: WordPtr) : Bool32; StdCall;
  TIK220Status = function (Axis: Word; pStatus: LongPtr) : Bool32; StdCall;
  TIK220DllStatus = function (pStatus: LongPtr; pInfo: LongPtr) : Bool32; StdCall;
  TIK220RefStatus = function (Axis: Word; pRef1: LongPtr; pRef2: LongPtr; pDiff: LongPtr; pCode: LongPtr; pFlag: WordPtr) : Bool32; StdCall;
  TIK220SignalStatus = function (Axis: Word; pFreq: WordPtr; pAmin: WordPtr; pAact: WordPtr; pAmax: WordPtr) : Bool32; StdCall;
  TIK220GetCorrA = function (Axis: Word; pOfs0: IntPtr;  pOfs90: IntPtr;   pPha0: IntPtr;  pPha90: IntPtr;  pSym0: IntPtr;  pSym90: IntPtr; pFlag1: WordPtr; pFlag2: WordPtr) : Bool32; StdCall;
  TIK220GetCorrB = function (Axis: Word; pOfs0: IntPtr;  pOfs90: IntPtr;   pPha0: IntPtr;  pPha90: IntPtr;  pSym0: IntPtr;  pSym90: IntPtr; pFlag1: WordPtr; pFlag2: WordPtr) : Bool32; StdCall;
  TIK220LoadCorrA = function (Axis: Word;  Ofs0: Integer;  Ofs90: Integer;   Pha0: Integer;  Pha90: Integer;  Sym0: Integer;  Sym90: Integer)				  : Bool32; StdCall;
  TIK220OctStatus = function (Axis: Word; pOkt0: WordPtr; pOct1: WordPtr; pOct2: WordPtr; pOct3: WordPtr; pOct4: WordPtr; pOct5: WordPtr; pOct6: WordPtr; pOct7: WordPtr)	  : Bool32; StdCall;
  TIK220ChkSumPar = function (Axis: Word; pChkSum: WordPtr) : Bool32; StdCall;
  TIK220ChkSumPrg = function (Axis: Word; pChkSum1: WordPtr; pChkSum2: WordPtr) : Bool32; StdCall;
  TIK220WritePar = function (Axis: Word; ParNum: Word;  ParVal: LongInt) : Bool32; StdCall;
  TIK220ReadPar = function (Axis: Word; ParNum: Word; pParVal: LongPtr) : Bool32; StdCall;
  TIK220ResetEn = function (Axis: Word; pStatus: WordPtr) : Bool32; StdCall;
  TIK220ConfigEn = function (Axis: Word; pStatus: WordPtr; pType: WordPtr; pPeriod: LongPtr; pStep: LongPtr; pTurns: WordPtr; pRefDis: WordPtr; pCntDir: WordPtr) : Bool32; StdCall;
  TIK220ReadEn = function (Axis: Word; pStatus: WordPtr; pData:   DoublePtr; pAlarm: WordPtr) : Bool32; StdCall;
  TIK220ReadEnInc = function (Axis: Word; Latch: Word; pStatus: WordPtr; pDataEn: DoublePtr; pAlarm: WordPtr; pDataInc: DoublePtr) : Bool32; StdCall;
  TIK220ModeEnCont = function (Axis: Word; Latch: Word; Mode: Word; pStatus: WordPtr) : Bool32; StdCall;
  TIK220ReadEnIncCont = function (Axis: Word; pStatus: WordPtr; pDataEn: DoublePtr; pAlarm: WordPtr; pDataInc: DoublePtr;  pSigStat: WordPtr) : Bool32; StdCall;
  TIK220AlarmEn = function (Axis: Word; pStatus: WordPtr; pAlarm: WordPtr) : Bool32; StdCall;
  TIK220WarnEn = function (Axis: Word; pStatus: WordPtr; pWarn:  WordPtr) : Bool32; StdCall;
  TIK220ReadMemEn = function (Axis: Word; Range: Word; MemAdr: Word; pMemData: WordPtr; pStatus: WordPtr) : Bool32; StdCall;
  TIK220WriteMemEn = function (Axis: Word; Range: Word; MemAdr: Word;  MemData: Word;    pStatus: WordPtr) : Bool32; StdCall;
  TIK220ReadSSI = function (Axis: Word; pStatus: WordPtr; pData: DoublePtr) : Bool32; StdCall;
  TIK220ReadSsiInc = function (Axis: Word; Latch: Word; pStatus: WordPtr; pDataSsi: DoublePtr; pDataInc: DoublePtr) : Bool32; StdCall;
  TIK220SetTimer = function (Axis: Word; SetVal: LongInt; pTimVal: LongPtr) : Bool32; StdCall;
  TIK220ModeTimer = function (Axis: Word; Mode: Word) : Bool32; StdCall;
  TIK220ModeRam = function (Axis: Word; Mode: Word) : Bool32; StdCall;
  TIK220ResetRam = function (Axis: Word) : Bool32; StdCall;
  TIK220GetRam = function (Axis: Word; pData: DoublePtr; pRead: WordPtr; pWrite: WordPtr; pStatus: WordPtr) : Bool32; StdCall;
  TIK220BurstRam = function (Axis: Word; maxCount: Word; pData:DoublePtr; pCount: WordPtr; pStatus: WordPtr) : Bool32; StdCall;
  TIK220Led = function (Axis: Word; Mode: Word) : Bool32; StdCall;
  TIK220SysLed = function (Card: Word; Mode: Word) : Bool32; StdCall;
  TIK220GetPort = function (Axis: Word; pPortInfo: WordPtr; pRising: WordPtr; pFalling: WordPtr) : Bool32; StdCall;
  TIK220InputW = function (Axis: Word; Adr: Word; pData: WordPtr) : Bool32; StdCall;
  TIK220InputL = function (Axis: Word; Adr: Word; pData: LongPtr) : Bool32; StdCall;
  TIK220Output = function (Axis: Word; Adr: Word;  Data: Word)    : Bool32; StdCall;
  TIK220RamRead = function (Axis: Word; Adr: Word; pData: WordPtr) : Bool32; StdCall;
  TIK220RamWrite = function (Axis: Word; Adr: Word;  Data: Word)    : Bool32; StdCall;
  TIK220DownLoad = function (Axis: Word; pPgmData: WordPtr; PgmSize: LongInt) : Bool32; StdCall;
  TIK220SetEnClock = function (Axis: Word;  State: Bool32; pStatus: WordPtr) : Bool32; StdCall;
  TIK220SetEnData = function (Axis: Word;  State: Bool32; pStatus: WordPtr) : Bool32; StdCall;
  TIK220ReadEnData = function (Axis: Word; pState: Bool32Ptr) : Bool32; StdCall;
  TIK220SetTimeOut = function (TimeOut: LongInt) : Bool32; StdCall;

  TIK220GetSig = function (Axis: Word; Period: WordPtr; adc00: WordPtr; adc90: WordPtr; pRead: WordPtr; pWrite: WordPtr; pStatus: WordPtr) : Bool32; StdCall;
  TIK220BurstSig = function (Axis: Word; maxCount: Word; Period: WordPtr; adc00: WordPtr; adc90: WordPtr; pCount: WordPtr; pStatus: WordPtr) : Bool32; StdCall;

function LoadIK220DLL(DLLName:String):Bool32;  //Funktion zum laden der DLL-Funktionen
function FreeIK220DLL:Bool32; //freigeben von IK220DLL.dll

var
  boardadressen : long16;
  adrzeiger     : long16ptr;

  Status        : Bool32;


  IK220Find: TIK220Find;
  IK220Init: TIK220Init;
  IK220Version: TIK220Version;
  IK220Reset: TIK220Reset;
  IK220Start: TIK220Start;
  IK220Stop: TIK220Stop;
  IK220ClearErr: TIK220ClearErr;
  IK220Latch: TIK220Latch;
  IK220LatchInt: TIK220LatchInt;
  IK220LatchExt: TIK220LatchExt;
  IK220ResetRef: TIK220ResetRef;
  IK220StartRef: TIK220StartRef;
  IK220StopRef: TIK220StopRef;
  IK220LatchRef: TIK220LatchRef;
  IK220Latched: TIK220Latched;
  IK220WaitLatch: TIK220WaitLatch;
  IK220Read32: TIK220Read32;
  IK220Read48: TIK220Read48;
  IK220Get32: TIK220Get32;
  IK220Get48: TIK220Get48;
  IK220CntStatus: TIK220CntStatus;
  IK220DoRef: TIK220DoRef;
  IK220RefActive: TIK220RefActive;
  IK220WaitRef: TIK220WaitRef;
  IK220CancelRef: TIK220CancelRef;
  IK220PositionRef: TIK220PositionRef;
  IK220Status: TIK220Status;
  IK220DllStatus: TIK220DllStatus;
  IK220RefStatus: TIK220RefStatus;
  IK220SignalStatus: TIK220SignalStatus;
  IK220GetCorrA: TIK220GetCorrA;
  IK220GetCorrB: TIK220GetCorrB;
  IK220LoadCorrA: TIK220LoadCorrA;
  IK220OctStatus: TIK220OctStatus;
  IK220ChkSumPar: TIK220ChkSumPar;
  IK220ChkSumPrg: TIK220ChkSumPrg;
  IK220WritePar: TIK220WritePar;
  IK220ReadPar: TIK220ReadPar;
  IK220ResetEn: TIK220ResetEn;
  IK220ConfigEn: TIK220ConfigEn;
  IK220ReadEn: TIK220ReadEn;
  IK220ReadEnInc: TIK220ReadEnInc;
  IK220ModeEnCont: TIK220ModeEnCont;
  IK220ReadEnIncCont: TIK220ReadEnIncCont;
  IK220AlarmEn: TIK220AlarmEn;
  IK220WarnEn: TIK220WarnEn;
  IK220ReadMemEn: TIK220ReadMemEn;
  IK220WriteMemEn: TIK220WriteMemEn;
  IK220ReadSSI: TIK220ReadSSI;
  IK220ReadSsiInc: TIK220ReadSsiInc;
  IK220SetTimer: TIK220SetTimer;
  IK220ModeTimer: TIK220ModeTimer;
  IK220ModeRam: TIK220ModeRam;
  IK220ResetRam: TIK220ResetRam;
  IK220GetRam: TIK220GetRam;
  IK220BurstRam: TIK220BurstRam;
  IK220Led: TIK220Led;
  IK220SysLed: TIK220SysLed;
  IK220GetPort: TIK220GetPort;
  IK220InputW: TIK220InputW;
  IK220InputL: TIK220InputL;
  IK220Output: TIK220Output;
  IK220RamRead: TIK220RamRead;
  IK220RamWrite: TIK220RamWrite;
  IK220DownLoad: TIK220DownLoad;
  IK220SetEnClock: TIK220SetEnClock;
  IK220SetEnData: TIK220SetEnData;
  IK220ReadEnData: TIK220ReadEnData;
  IK220SetTimeOut: TIK220SetTimeOut;

  IK220GetSig: TIK220GetSig;
  IK220BurstSig: TIK220BurstSig;

implementation

var
  DLLHandle: THandle;

//******************************************************************************
function LoadIK220DLL(DLLName:String):Bool32;
var
  err:Boolean;

begin
try
DllHandle:= LoadLibrary(PChar(DLLName)); //îffnen der IK220DLL.dll
if DllHandle = 0 then
  begin
  ShowMessage('Error loading IK220 DLL');
  Result:=0;
  exit;
  end;

@IK220Find:= GetProcAddress(DllHandle,'IK220Find');  //laden der Funktion IK220Find
//Abfangen des Fehlers wenn die Funktion nicht gefunden wurde oder einen
//anderen Header besitzt; die nachfolgenden vier Zeilen sollten nach jedem
//Aufruf von <GetProcAdress> stehen;
if @IK220Find = nil then
  begin
  ShowMessage('Error loading >>IK220Find<< function')
  end;

@IK220Init:= GetProcAddress(DllHandle,'IK220Init');
@IK220Version:= GetProcAddress(DllHandle,'IK220Version');
@IK220Reset:= GetProcAddress(DllHandle,'IK220Reset');
@IK220Start:= GetProcAddress(DllHandle,'IK220Start');
@IK220Stop:= GetProcAddress(DllHandle,'IK220Stop');
@IK220ClearErr:= GetProcAddress(DllHandle,'IK220ClearErr');
@IK220Latch:= GetProcAddress(DllHandle,'IK220Latch');
@IK220LatchInt:= GetProcAddress(DllHandle,'IK220LatchInt');
@IK220LatchExt:= GetProcAddress(DllHandle,'IK220LatchExt');
@IK220ResetRef:= GetProcAddress(DllHandle,'IK220ResetRef');
@IK220StartRef:= GetProcAddress(DllHandle,'IK220StartRef');
@IK220StopRef:= GetProcAddress(DllHandle,'IK220StopRef');
@IK220LatchRef:= GetProcAddress(DllHandle,'IK220LatchRef');
@IK220Latched:= GetProcAddress(DllHandle,'IK220Latched');
@IK220WaitLatch:= GetProcAddress(DllHandle,'IK220WaitLatch');
@IK220Read32:= GetProcAddress(DllHandle,'IK220Read32');
@IK220Read48:= GetProcAddress(DllHandle,'IK220Read48');
@IK220Get32:= GetProcAddress(DllHandle,'IK220Get32');
@IK220Get48:= GetProcAddress(DllHandle,'IK220Get48');
@IK220CntStatus:= GetProcAddress(DllHandle,'IK220CntStatus');
@IK220DoRef:= GetProcAddress(DllHandle,'IK220DoRef');
@IK220RefActive:= GetProcAddress(DllHandle,'IK220RefActive');
@IK220WaitRef := GetProcAddress(DllHandle,'IK220WaitRef');
@IK220CancelRef:= GetProcAddress(DllHandle,'IK220CancelRef');
@IK220PositionRef:= GetProcAddress(DllHandle,'IK220PositionRef');
@IK220Status:= GetProcAddress(DllHandle,'IK220Status');
@IK220DllStatus:= GetProcAddress(DllHandle,'IK220DllStatus');
@IK220RefStatus:= GetProcAddress(DllHandle,'IK220RefStatus');
@IK220SignalStatus:= GetProcAddress(DllHandle,'IK220SignalStatus');
@IK220GetCorrA:= GetProcAddress(DllHandle,'IK220GetCorrA');
@IK220GetCorrB:= GetProcAddress(DllHandle,'IK220GetCorrB');
@IK220LoadCorrA:= GetProcAddress(DllHandle,'IK220LoadCorrA');
@IK220OctStatus:= GetProcAddress(DllHandle,'IK220OctStatus');
@IK220ChkSumPar:= GetProcAddress(DllHandle,'IK220ChkSumPar');
@IK220ChkSumPrg:= GetProcAddress(DllHandle,'IK220ChkSumPrg');
@IK220WritePar:= GetProcAddress(DllHandle,'IK220WritePar');
@IK220ReadPar:= GetProcAddress(DllHandle,'IK220ReadPar');
@IK220ResetEn:= GetProcAddress(DllHandle,'IK220ResetEn');
@IK220ConfigEn:= GetProcAddress(DllHandle,'IK220ConfigEn');
@IK220ReadEn:= GetProcAddress(DllHandle,'IK220ReadEn');
@IK220ReadEnInc:= GetProcAddress(DllHandle,'IK220ReadEnInc');
@IK220ModeEnCont:= GetProcAddress(DllHandle,'IK220ModeEnCont');
@IK220ReadEnIncCont:= GetProcAddress(DllHandle,'IK220ReadEnIncCont');
@IK220AlarmEn:= GetProcAddress(DllHandle,'IK220AlarmEn');
@IK220WarnEn:= GetProcAddress(DllHandle,'IK220WarnEn');
@IK220ReadMemEn:= GetProcAddress(DllHandle,'IK220ReadMemEn');
@IK220WriteMemEn:= GetProcAddress(DllHandle,'IK220WriteMemEn');
@IK220ReadSSI:= GetProcAddress(DllHandle,'IK220ReadSSI');
@IK220ReadSsiInc:= GetProcAddress(DllHandle,'IK220ReadSsiInc');
@IK220SetTimer:= GetProcAddress(DllHandle,'IK220SetTimer');
@IK220ModeTimer:= GetProcAddress(DllHandle,'IK220ModeTimer');
@IK220ModeRam:= GetProcAddress(DllHandle,'IK220ModeRam');
@IK220ResetRam:= GetProcAddress(DllHandle,'IK220ResetRam');
@IK220GetRam:= GetProcAddress(DllHandle,'IK220GetRam');
@IK220BurstRam:= GetProcAddress(DllHandle,'IK220BurstRam');
@IK220Led:= GetProcAddress(DllHandle,'IK220Led');
@IK220SysLed:= GetProcAddress(DllHandle,'IK220SysLed');
@IK220GetPort:= GetProcAddress(DllHandle,'IK220GetPort');
@IK220InputW:= GetProcAddress(DllHandle,'IK220InputW');
@IK220InputL:= GetProcAddress(DllHandle,'IK220InputL');
@IK220Output:= GetProcAddress(DllHandle,'IK220Output');
@IK220RamRead:= GetProcAddress(DllHandle,'IK220RamRead');
@IK220RamWrite:= GetProcAddress(DllHandle,'IK220RamWrite');
@IK220DownLoad:= GetProcAddress(DllHandle,'IK220DownLoad');
@IK220SetEnClock:= GetProcAddress(DllHandle,'IK220SetEnClock');
@IK220SetEnData:= GetProcAddress(DllHandle,'IK220SetEnData');
@IK220ReadEnData:= GetProcAddress(DllHandle,'IK220ReadEnData');
@IK220SetTimeOut:= GetProcAddress(DllHandle,'IK220SetTimeOut');

@IK220GetSig:= GetProcAddress(DllHandle,'IK220GetSig');
@IK220BurstSig:= GetProcAddress(DllHandle,'IK220BurstSig');


except
  err:= FreeLibrary(DLLHandle);
  if not err then
    begin
    ShowMessage('cannot remove DLL');
    Result:=0;
    exit;
    end;
  end;
Result:=$FFFFFF;
end;  //end of function

//******************************************************************************
function FreeIK220DLL:Bool32;
var
  err:Boolean;

begin
if DLLHandle <> 0 then //prÅfen ob Handle belegt ist
  begin
  err:= FreeLibrary(DLLHandle);  //freigeben der dll
  if not err then
    begin
    ShowMessage('cannot remove IK220DLL.dll');
    Result:=0;
    exit;
    end;
  Result:=$FFFFFF;
  exit;
  end;
Result:=0;
end;
//******************************************************************************
end.

