//
//	Header file for IK 220 commands
//
//	Version 0.4:	23.03.1999
//
//	ES,  Dr. Johannes Heidenhain GmbH
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////




#ifndef _IK220_H_
#define _IK220_H_

////////////////////////////////
// Port definition for IK 220
////////////////////////////////

#define CmdPort		G28Port0
#define DatPort0	G28Port0
#define DatPort1	G28Port1
#define DatPort2	G28Port2
#define DatPort3	G28Port3
#define DatPort4	G28Port4
#define DatPort5	G28Port5
#define DatPort6	G28Port6
#define DatPort7	G28Port7
#define DatPort8	G28Port8
#define DatPort9	G28Port9

#define StaPort		G28Port10




///////////////////////////////
// Command defintion for IK 220
///////////////////////////////

#define cmdReset   			(USHORT)0x0101		// Command: reset counter
#define cmdStart   			(USHORT)0x0102		// Command: start counter
#define cmdStop	   			(USHORT)0x0103		// Command: stop  counter
#define cmdClearErr			(USHORT)0x0104		// Command: clear frequency and amplitude error

#define cmdLatch0			(USHORT)0x0105		// Command: store counter in latch 0
#define cmdLatch1			(USHORT)0x0106		// Command: store counter in latch 1
#define cmdLatch2			(USHORT)0x0107		// Command: store counter in latch 2


#define cmdResetREF			(USHORT)0x0201		// Command: reset  counter with next reference mark
#define cmdStartREF			(USHORT)0x0202		// Command: start  counter with next reference mark
#define cmdStopREF 			(USHORT)0x0203		// Command: stop   counter with next reference mark
#define cmdLatchREF2		(USHORT)0x0204		// Command: store  counter in latch 2 with next reference mark
#define cmdTraverseREF		(USHORT)0x0205		// Command: start  ref procedure as defined in parameter 5
#define cmdCancelREF		(USHORT)0x0206		// Command: cancel ref procedure as defined in parameter 5

#define cmdPositionREF		(USHORT)0x0207		// Command: get  position of reference mark at rising edge
#define cmdPositionREF2		(USHORT)0x0208		// Command: get  position of reference mark at rising and falling edge
#define cmdPositionREF3		(USHORT)0x0209		// Command: read position of reference mark at falling edge

#define cmdReadCnt0			(USHORT)0x0301		// Command: read counter 0 value
#define cmdReadCnt1			(USHORT)0x0302		// Command: read counter 1 value
#define cmdGetCnt0			(USHORT)0x0303		// Command: get  counter 0 value
#define cmdGetCnt1			(USHORT)0x0304		// Command: get  counter 1 value
#define cmdGetCnt2			(USHORT)0x0305		// Command: get  counter 2 value


#define cmdGetRefStat		(USHORT)0x0401		// Command: get ref status
#define cmdGetSigStat		(USHORT)0x0402		// Command: get signal status

#define cmdGetCorrA			(USHORT)0x0403		// Command: get aligned    correction values
#define cmdGetCorrB			(USHORT)0x0404		// Command: get calculated correction values
#define cmdLoadCorrA		(USHORT)0x0405		// Command: load correction values

#define cmdGetOktCnt		(USHORT)0x0406		// Command: get oktand counter
#define cmdGetVersion		(USHORT)0x0407		// Command: get software version
#define cmdGetPort			(USHORT)0x0408		// Command: get port info

#define cmdGetChkSumPar		(USHORT)0x0409		// Command: get check sum of parameters
#define cmdGetChkSumPrg		(USHORT)0x040A		// Command: get check sum of program

#define cmdGetCntStatus		(USHORT)0x040B		// Command: get count status


#define cmdWritePar 		(USHORT)0x0501		// Command: write parameter
#define cmdReadPar			(USHORT)0x0502		// Command: read  parameter


#define cmdResetEn			(USHORT)0x0601		// Command: reset EnDat
#define cmdConfigEn			(USHORT)0x0602		// Command: read  EnDat configuration
#define cmdReadEn			(USHORT)0x0603		// Command: read  EnDat counter value

#define cmdReadAlarmEn		(USHORT)0x0604		// Command: read   EnDat alarm
#define cmdReadWarnEn		(USHORT)0x0605		// Command: read   EnDat warning
#define cmdClearAlarmEn		(USHORT)0x0606		// Command: clear  EnDat alarm
#define cmdClearWarnEn		(USHORT)0x0607		// Command: clear  EnDat warning

#define cmdCommLEn			(USHORT)0x0608		// Command: long  EnDat protocol
#define cmdCommSEn			(USHORT)0x0609		// Command: short EnDat protocol

#define cmdReadEnInc0		(USHORT)0x060A		// Command: read EnDat and incremental counter value latch 0
#define cmdReadEnInc1		(USHORT)0x060B		// Command: read EnDat and incremental counter value latch 1

#define cmdEnContMode0		(USHORT)0x060C		// Command: stop continous EnDat clock			
#define cmdEnContMode1		(USHORT)0x060D		// Command: set mode for continous EnDat clock with    CRC check
#define cmdEnContMode2		(USHORT)0x060E		// Command: set mode for continous EnDat clock without CRC check

#define cmdReadEnIncCont	(USHORT)0x060F		// Command: read EnDat and incremental counter value with continous clock


#define cmdTimerMode		(USHORT)0x0701		// Command: set Timer mode


#define cmdRamMode			(USHORT)0x0801		// Command: set mode for RAM buffer
#define cmdResetRam			(USHORT)0x0802		// Command: reset RAM buffer
#define cmdGetRam			(USHORT)0x0803		// Command: get counter value from RAM buffer


#define cmdReadSSI			(USHORT)0x0901		// Command: read SSI counter value
#define cmdConfigSSI		(USHORT)0x0902		// Command: read SSI configuration
#define cmdReadSsiInc0		(USHORT)0x0903		// Command: read SSI and incremental counter value latch 0
#define cmdReadSsiInc1		(USHORT)0x0904		// Command: read SSI and incremental counter value latch 1



#define cmdWritePort 		(USHORT)0x1001		// Command: write port
#define cmdReadPort			(USHORT)0x1002		// Command: read  port
#define cmdWriteRAM 		(USHORT)0x1003		// Command: write RAM
#define cmdReadRAM			(USHORT)0x1004		// Command: read  RAM
#define cmdSetBw			(USHORT)0x1005		// Command: set bandwidth
#define cmdSetTrm			(USHORT)0x1006		// Command: set termination
#define cmdRefEval			(USHORT)0x1007		// Command: set ref evaluation

#define cmdLedOff			(USHORT)0x1010		// Command: led off
#define cmdLedOn    		(USHORT)0x1011		// Command: led on
#define cmdLedFlash			(USHORT)0x1012		// Command: led flash

#define cmdResetEnClock		(USHORT)0x1020		// Command: set EnDat clock line low
#define cmdSetEnClock		(USHORT)0x1021		// Command: set EnDat clock line high

#define cmdResetEnData		(USHORT)0x1022		// Command: set EnDat data line low
#define cmdSetEnData		(USHORT)0x1023		// Command: set EnDat data line high

#define cmdReadEnData		(USHORT)0x1024		// Command: read EnDat data line

#define cmdSetOpMode		(USHORT)0x1030		// Command: set operation mode flag
#define cmdGetOpMode		(USHORT)0x1031		// Command: get operation mode flag



#define cmdWrongCommand		(USHORT)0xFEDC		// Command:	wrong command
#define cmdWrongCommandEn	(USHORT)0xFEDD		// Command: wrong command while continous EnDat clock




/////////////////////////////////
//
//	EnDat mode commands of IK 220
//
/////////////////////////////////

#define ModeSelRangeEn		(USHORT)0x0E
#define ModeWriteMemEn		(USHORT)0X1C
#define ModeReadMemEn		(USHORT)0X23



///////////////////////////////
//
//	Status definition of IK 220
//
///////////////////////////////

#define statBufferOverflow	(USHORT)0x0001			// RAM buffer overflow
#define statBufferNoVal		(USHORT)0x0002			// RAM buffer empty (no value stored)
#define statBufferEmpty		(USHORT)0x0004			// RAM buffer empty (last value read)
#define statBufferFailed	(USHORT)0x8000			// RAM buffer transfer failed




//////////////////////////////////
//
//	Parameter definition of IK 220
//
//////////////////////////////////

#define parTimerValue	11		// Select timer value
#define parPreScaler	12		// Prescaler for timer




#endif