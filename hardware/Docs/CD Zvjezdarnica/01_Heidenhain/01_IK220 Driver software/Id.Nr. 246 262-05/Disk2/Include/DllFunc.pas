UNIT DLLFunc;

interface
(******************************************************************)
uses
  Windows;
(******************************************************************)
(* Typen                                                          *)
(******************************************************************)
type
  Long16    =  array [0..15] of longint;
  Long16Ptr = ^Long16;
  Long256    =  array [0..255] of longint;
  Long16Ptr = ^Long256;
  BytePtr   = ^Byte;
  TCharPtr	=  array [0..255] of Char;
  CharPtr   = ^TCharPtr;
  IntPtr    = ^SmallInt;
  WordPtr   = ^Word;
  Bool32	=  LongInt;
  Bool32Ptr = ^Bool32;
  LongPtr   = ^LongInt;
  DoublePtr = ^Double;

(******************************************************************)
(* DLL-functions                                                  *)
(******************************************************************)
Function IK220Find			(pBuffer16 : Long16Ptr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Find256		(pBuffer256: Long256Ptr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Init			(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Version		(Axis: Word; pVersCard: CharPtr; pVersDrv: CharPtr; pVersDll: CharPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220Reset			(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Start			(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Stop			(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ClearErr		(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Latch			(Axis: Word; Latch: Word) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220LatchInt		(Card: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220LatchExt		(Card: Word) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220ResetRef		(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220StartRef		(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220StopRef		(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220LatchRef		(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220Latched		(Axis: Word; Latch: Word; pStatus: Bool32Ptr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220WaitLatch		(Axis: Word; Latch: Word) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220SetTimeOut	(TimeOut: LongInt) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220Set			(Axis: Word; SetVal   : Double) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220SetPreset		(Axis: Word; PresVal  : Double) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220GetPreset		(Axis: Word; pPresVal : DoublePtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220Read32		(Axis: Word; Latch: Word; pData: LongPtr)   : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Read48		(Axis: Word; Latch: Word; pData: DoublePtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Get32			(Axis: Word; Latch: Word; pData: LongPtr)   : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Get48			(Axis: Word; Latch: Word; pData: DoublePtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220CntStatus		(Axis: Word; Latch: Word; pRefSta: WordPtr; pKorr00: IntPtr; pKorr90: IntPtr; pNKorr00: IntPtr; pNKorr90: IntPtr; pSamCnt: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220DoRef			(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220CancelRef		(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220RefActive		(Axis: Word; pStatus: Bool32Ptr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220WaitRef		(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220PositionRef	(Axis: Word; pData:  DoublePtr; pPeriod:  LongPtr; pIntpol:  WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220PositionRef2	(Axis: Word; pData1: DoublePtr; pPeriod1: LongPtr; pIntpol1: WordPtr; pData2: DoublePtr; pPeriod2: LongPtr; pIntpol2: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220Status		(Axis: Word; pStatus: LongPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220DllStatus		(pStatus: LongPtr; pInfo: LongPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220RefStatus		(Axis: Word; pRef1: LongPtr; pRef2: LongPtr; pDiff: LongPtr; pCode: LongPtr; pFlag: WordPtr) :	Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220SignalStatus  (Axis: Word; pFreq: WordPtr; pAmin: WordPtr; pAact: WordPtr; pAmax: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220GetCorrA     	(Axis: Word; pOfs0: IntPtr;  pOfs90: IntPtr;   pPha0: IntPtr;  pPha90: IntPtr;  pSym0: IntPtr;  pSym90: IntPtr; pFlag1: WordPtr; pFlag2: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220GetCorrB     	(Axis: Word; pOfs0: IntPtr;  pOfs90: IntPtr;   pPha0: IntPtr;  pPha90: IntPtr;  pSym0: IntPtr;  pSym90: IntPtr; pFlag1: WordPtr; pFlag2: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220LoadCorrA		(Axis: Word;  Ofs0: Integer;  Ofs90: Integer;   Pha0: Integer;  Pha90: Integer;  Sym0: Integer;  Sym90: Integer)				  : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220OctStatus		(Axis: Word; pOkt0: WordPtr; pOct1: WordPtr; pOct2: WordPtr; pOct3: WordPtr; pOct4: WordPtr; pOct5: WordPtr; pOct6: WordPtr; pOct7: WordPtr)	  : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220ChkSumPar		(Axis: Word; pChkSum: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ChkSumPrg		(Axis: Word; pChkSum1: WordPtr; pChkSum2: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220WritePar		(Axis: Word; ParNum: Word;  ParVal: LongInt) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ReadPar		(Axis: Word; ParNum: Word; pParVal: LongPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220ResetEn		(Axis: Word; pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ConfigEn		(Axis: Word; pStatus: WordPtr; pType: WordPtr; pPeriod: LongPtr; pStep: LongPtr; pTurns: WordPtr; pRefDis: WordPtr; pCntDir: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ReadEn		(Axis: Word; pStatus: WordPtr; pData:   DoublePtr; pAlarm: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ReadEnInc		(Axis: Word; Latch: Word; pStatus: WordPtr; pDataEn: DoublePtr; pAlarm: WordPtr; pDataInc: DoublePtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220ModeEnCont	(Axis: Word; Latch: Word; Mode: Word; pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ReadEnIncCont	(Axis: Word; pStatus: WordPtr; pDataEn: DoublePtr; pAlarm: WordPtr; pDataInc: DoublePtr;  pSigStat: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220AlarmEn		(Axis: Word; pStatus: WordPtr; pAlarm: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220WarnEn		(Axis: Word; pStatus: WordPtr; pWarn:  WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ReadMemEn		(Axis: Word; Range: Word; MemAdr: Word; pMemData: WordPtr; pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220WriteMemEn	(Axis: Word; Range: Word; MemAdr: Word;  MemData: Word;    pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220ReadSSI		(Axis: Word; pStatus: WordPtr; pData: DoublePtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ReadSsiInc	(Axis: Word; Latch: Word; pStatus: WordPtr; pDataSsi: DoublePtr; pDataInc: DoublePtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220SetTimer		(Axis: Word; SetVal: LongInt; pTimVal: LongPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ModeTimer		(Axis: Word; Mode: Word) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220ModeRam		(Axis: Word; Mode: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ResetRam		(Axis: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220GetRam		(Axis: Word; pData: DoublePtr; pRead: WordPtr; pWrite: WordPtr; pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220BurstRam		(Axis: Word; maxCount: Word; pData: DoublePtr; pCount: WordPtr; pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220GetSig		(Axis: Word; pPeriod: WordPtr; pAmp0: WordPtr; pAmp90: WordPtr; pRead: WordPtr; pWrite: WordPtr; pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220BurstSig		(Axis: Word; maxCount: Word; pPeriod: WordPtr; pAmp0: WordPtr; pAmp90: WordPtr; pCount: WordPtr; pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220Led			(Axis: Word; Mode: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220SysLed		(Card: Word; Mode: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220GetPort		(Axis: Word; pPortInfo: WordPtr; pRising: WordPtr; pFalling: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220InputW		(Axis: Word; Adr: Word; pData: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220InputL		(Axis: Word; Adr: Word; pData: LongPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220Output		(Axis: Word; Adr: Word;  Data: Word)    : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220RamRead		(Axis: Word; Adr: Word; pData: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220RamWrite		(Axis: Word; Adr: Word;  Data: Word)    : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220DownLoad		(Axis: Word; pPgmData: WordPtr; PgmSize: LongInt) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220SetEnClock	(Axis: Word;  State: Bool32; pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220SetEnData 	(Axis: Word;  State: Bool32; pStatus: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220ReadEnData 	(Axis: Word; pState: Bool32Ptr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220RefEval		(Axis: Word; Mode: Word) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220SetBw			(Axis: Word; Mode: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220SetTrm		(Axis: Word; Mode: Word) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220SetOpMode		(Axis: Word;  Mode: Word)    : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220GetOpMode		(Axis: Word; pMode: WordPtr) : Bool32; StdCall; External 'IK220DLL.DLL';

Function IK220SetLatInt		(Card: Word; Mode: Word) : Bool32; StdCall; External 'IK220DLL.DLL';
Function IK220SetLatExt		(Card: Word; Mode: Word) : Bool32; StdCall; External 'IK220DLL.DLL';



(******************************************************************)
(*                                                                *)
(******************************************************************)
var
  boardadressen : long16;
  adrzeiger     : long16ptr;
  status        : Bool32;
(******************************************************************)
(*                                                                *)
(******************************************************************)
implementation
end.
