/*++

	Copyright (c) 1999,  Dr. Johannes Heidenhain GmbH

	Module Name:	Exports.c
	
	Abstract:		Windows DLL for IK 220

	Notes:			This DLL provides IK 220 card functions for IK 220.
					IK220 hardware is accessed through IK220Drv.Sys or IK220VxD.Vxd.


	History:		- Error if axis is not present							Version  0.08	ES	17.06.1999
					- Time out for all functions							Version  0.08	ES	17.06.1999
					- DLLStatus & DLLInfo added								Versiom  0.08	ES	17.06.1999
					- New firmware version 									Version  0.09	ES	22.06.1999
					- User defined time out for functions					Version  0.10	ES	23.06.1999
					  IK220WaitLatch & IK220PositionRef
					- IK220SysLed to set state of system LED				Version  0.11	ES	24.06.1999
					- User defined time out for function IK220WaitRef		Version	 0.12	ES	25.06.1999
					- New firmware version	 								Version  0.12	ES	25.06.1999
					- New firmware version									Version  0.15	ES	29.06.1999
					- Time out in IK220ReadBurst							Version  0.15	ES	29.06.1999
					- Checking PCI function number (FUNC_00) removed		Version  0.16	ES	30.07.1999
					  in IK220Pci.c (SearchForIK220)
					- Some error reporting to DLLStatus	added				Version  0.17	ES	04.08.1999
					- Error in SetEnData & SetEnClock						Version  0.18	ES	04.08.1999
					- Version 1.00											Version  1.00	ES	05.08.1999
					----------------------------------------------------------------------------------------------
					- New version of firmware 								Version  1.10   ES	01.10.1999
    				- IK220OctStatus parameter pSamCnt added				Version  1.10   ES	01.10.1999
					- Error's in IK220ReadMemEn & IK220WriteMemEn fixed		Version  1.20   ES	05.10.1999
					- New firmware version  								Version  1.30   ES	08.10.1999
					- New firmware version	 								Version  1.40   ES	15.10.1999
					- New firmware version	 								Version  1.40   ES	15.10.1999
					- New firmware version	 								Version  1.50   ES	18.10.1999
					- New firmware version	 								Version  1.60   ES	18.10.1999
					- New firmware version	 								Version  1.70   ES	19.10.1999
					- Release Version 2.00									Version  2.00	ES	19.10.1999
					----------------------------------------------------------------------------------------------
					- IK220ReadEn reports error if EnDat alarm 				Version  2.01	ES	20.10.1999
					- Critical section for each axis		 				Version  2.10	ES	21.10.1999
					- Local variable "RegW" instead of						Version  2.10	ES	21.10.1999
					  global "RegW" (reentrance)
					- IK220CntStatus variables for each axis (reentrance)	Version  2.10	ES	21.10.1999
					- New firmware version	 								Version  2.20   ES	16.11.1999
					- New firmware version	 								Version  2.30   ES	18.11.1999
					- Return error in GetCntStatus & IK220ModeEnCont		Version  2.40   ES	07.12.1999
					- New firmware version									Version  2.50   ES  21.02.2000   
					- New firmware version									Version  2.60   ES  20.04.2000   
					- Additional function PositionRef2						Version  2.60   ES  20.04.2000   
					- New firmware version									Version  2.70   ES  03.05.2000   
					- New modes for IK220ModeRam to store amplitude			Version  2.70   ES  03.05.2000   
					  values in RAM buffer
					- IK220GetSig and IK220BurstSig to read amplitude		Version  2.70   ES  03.05.2000   
					  values form RAM buffer
					- New firmware version									Version  2.80   ES  05.05.2000
					- Variable and function for preset						Version  2.90	ES	22.05.2000
					- New firmware version 03E								Version  2.90   ES  22.05.2000
					- New firmware version 03E								Version  2.91   ES  23.05.2000
					- Firmware version 03D for test version					Version  2.92   ES  29.05.2000
					- Firmware version 03E (2000-05-30/09:09)				Version  2.93   ES  30.05.2000
					- Firmware version 03  (2000-06-08/13:02)				Version  3.00   ES  08.06.2000
					- Release Version 3.00									Version  3.00	ES	13.06.2000
					----------------------------------------------------------------------------------------------
					- Firmware version 04A  (2000-07-31/13:32)				Version  3.01   ES  21.08.2000
					- Firmware version 04B  (2000-09-13/10:47)				Version  3.02	ES	13.09.2000
					- Error in IK220PositionRef2 (Intpol2 >> 4)				Version  3.03	ES	26.09.2000
					- Firmware version 04C  (2000-10-04/10:37)				Version  3.04	ES	04.10.2000
					- Function IK220CntStatus reads values from card		Version  3.05	ES	06.10.2000
					  instead of returning stored values
					- Firmware version 04C  (2000-10-11/12:50)				Version  3.06	ES	11.10.2000
					- Firmware version 04C  (2000-10-12/13:32)				Version  3.07	ES	12.10.2000
					- Firmware version 04D  (2000-10-24/08:45)				Version  3.08	ES	24.10.2000
					- DLL does not report error when starting up if			Version  3.10	ES	26.10.2000
					  no IK 220 is present, to allow software to load
					  DLL without IK 220 installed (Software that uses	
					  different cards)
					- Firmware version 04E  (2000-12-08/11:06)				Version  3.11	ES	08.12.2000
					- Firmware version 04F  (2001-01-18/13:45)				Version  3.12	ES	25.01.2001
					- Firmware version 04F  (2001-01-26/11:25)				Version  3.13	ES	29.01.2001
					- Firmware version 04F  (2001-01-29/11:13)				Version  3.14	ES	30.01.2001
					- Firmware version 04F  (2001-02-19/10:16)				Version  3.15	ES	19.02.2001
					- Firmware version 04G  (2001-04-03/11:22)				Version  3.16	ES	04.04.2001
					- New function IK220RefEval to switch evaluation of		Version  3.16	ES	04.04.2001
					  ref signal 
					- Firmware version 04H  (2001-08-06/15:06)				Version  3.17	ES	07.08.2001
					- Firmware version 04H  (2001-08-06/15:06)				Version  3.18	ES	26.09.2001
					- Firmware version 04K  (2001-10-09/15:03)				Version  3.19	ES	09.10.2001
					- New function IK220SetBw to set bandwidth of			Version	 3.20   ES  22.10.2001
					  incremental encoder signals
					- New function IK220SetTrm to set termination of		Version	 3.20   ES  22.10.2001
					  of 1Vss encoder signals
					- New function IK220SetOpMode and IK220GetOpMode		Version	 3.21   ES  26.10.2001
					  to set and read IK operation mode flag
					- Firmware version 04  (2001-10-29/08:33)				Version  4.00	ES	29.10.2001
					- Release Version 4.00									Version  4.00	ES	29.10.2001
					----------------------------------------------------------------------------------------------
					  
										

--*/


#define _VERSION	" 4.00"

#include <math.h>
#include <windows.h>
#include "dllglob.h"

#include "..\..\Disk2\include\G28.h"
#include "..\..\Disk2\include\IK220.h"
#include "..\..\Disk2\include\DRVFunc.h"

#include "Exports.h"
#include "Pgm220.h"
#include "TimVal.h"
#include "TimeOut.h"




#ifdef _Windows95
  #ifdef _DEBUG
	const char DLLVers[20] = "DLL95: V"_VERSION"(D)";
  #else
	const char DLLVers[20] = "DLL95: V"_VERSION"(R)";
  #endif
#else
  #ifdef _DEBUG
	const char DLLVers[20] = "DLLNT: V"_VERSION"(D)";
  #else
	const char DLLVers[20] = "DLLNT: V"_VERSION"(R)";
  #endif				
#endif






// Global variable definitions

double  Preset[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};


double FactorEn[16] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

ULONG Exists[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};



USHORT OEM_Adr  [16][2] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
						   0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};


USHORT CORR_Adr [16][2] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
						   0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};



__int64 PerfFreq, PerfCount;
__int64 UserTimeOut1 = 0;						// Time out value for functions IK220WaitLatch, IK220WaitRef & IK220PositionRef
__int64 PerfStart[maxTimNum+1];
__int64 TIM_1us, TIM_1ms, TIM_2s;


ULONG	DLLStatus = 0;							// Status of DLL functions
ULONG	DLLInfo   = 0;							// Info   of DLL functions


extern CRITICAL_SECTION	CritSect[17];



#ifdef _Windows95
	#include <conio.h>

	// Global variable definitions for Windows95
	USHORT* PortBase[16];
	ULONG*  ConfReg[8];


#else
	#include <winioctl.h>

	// Global variable definitions for WindowsNT
	HANDLE	m_hIK220Drv;		// Handle for device driver

#endif





///////////////////////////////////////////////////////////////
// IK220DownLoad: Loads firmware from pPgmData into RAM of Axis
///////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220DownLoad (USHORT Axis, USHORT* pPgmData, ULONG PgmSize)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!DownLoad (Axis, pPgmData, PgmSize))
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}

}



/////////////////////////////////////////////////////////////////////////
// IK220Find: Returns port address(es) of installed card('s) in pBuffer16
/////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Find (ULONG* pBuffer16)
{
USHORT	Axis;
ULONG*	pExists=pBuffer16;
BOOL	IkExists=FALSE;

	EnterCriticalSection (&CritSect[16]);
	if (!IKFind (pBuffer16)) 
	{
		LeaveCriticalSection (&CritSect[16]);
		DLLInfo=DLLInfo | DLLInfo_NoIK220;
		return FALSE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[16]);
		for (Axis=0; Axis<16; Axis++) if (*pExists++) IkExists=TRUE;

		if (!IkExists) DLLInfo=DLLInfo | DLLInfo_NoIK220;
		return (IkExists);
	}
}



//////////////////////////////
// IK220Init: Initializes Axis
//////////////////////////////
DLLEXPORT BOOL WINAPI IK220Init (USHORT Axis)
{
USHORT	RegW;					// Register value word

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);

	if (!DownLoad (Axis, &Pgm220[0], sizeof(Pgm220)) )		goto ErrExit;	// Load firmware

	if (!InputW  (Axis, ClrFlag1Reg, &RegW))				goto ErrExit;	// Clear all semaphore bit's
	if (!OutputW (Axis, ControlReg, RunMode))				goto ErrExit;	// Set CPU to run mode

	InitTimeOut (TimDLL);													// Init timer for time out
	do 
	{
		if (!InputW (Axis, Flag1Reg, &RegW))				goto ErrExit;	// Wait until status-register is updated
	}																		// or time out
	while ( !(RegW & G28Sem10) && !TimeOut (TimDLL, TIM_2s) );

	if ( !InputW (Axis, Flag1Reg, &RegW) )					goto ErrExit;	// Status-register updated ?
	if ( !(RegW & G28Sem10) )												// No, time out
	{	
		DLLStatus = DLLStatus | DLLStatus_TimeOut;			goto ErrExit;		
	}

	if (!InputW (Axis, ClrFlag1Reg, &RegW))					goto ErrExit;	// Yes, clear all semaphore bit's
	
    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);
			 return FALSE;
}



/////////////////////////////////////////////////////////////////
// IK220Version: Returns software version of card, driver and dll
/////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Version (USHORT Axis, char* pVersCard, char* pVersDrv, char* pVersDll)
{

	USHORT i;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	
	if (!GetVers (Axis, pVersCard, pVersDrv))					goto ErrExit;	// Get version of firmware and device driver
	for (i=0; i<sizeof(DLLVers); i++) *pVersDll++=DLLVers[i];					// Version of DLL

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);
			 return FALSE;
}



//////////////////////////
// IK220Reset: Resets Axis
//////////////////////////
DLLEXPORT BOOL WINAPI IK220Reset (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdReset))												// Command: Reset incremental counter
	{	
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}


//////////////////////////
// IK220Start: Starts Axis
//////////////////////////
DLLEXPORT BOOL WINAPI IK220Start (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdStart))												// Command: Start incremental counter
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



////////////////////////
// IK220Stop: Stops Axis
////////////////////////
DLLEXPORT BOOL WINAPI IK220Stop (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdStop))												// Command: Stop incremental counter
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



///////////////////////////////////////////////////////////////////////////
// IK220ClearErr: Clears error bits in Axis (frequency and amplitude error)
///////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ClearErr (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdClearErr))											// Command: Clear signal errors
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



//////////////////////////////////////////////////
// IK220Latch: Stores counter value in latch 0/1/2
//////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Latch (USHORT Axis, USHORT Latch)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	if (Latch>2)																// Check latch number
	{
		DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;		// Latch number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd(Axis, (USHORT)(cmdLatch0+Latch)))								// Command: Store counter in latch
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



//////////////////////////////////////////////////////
// IK220LatchInt: Generates internal synchronous latch
//////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220LatchInt (USHORT Card)
{
    if (Card>7)
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Card<<1]);
	if ( !LatchInt (Card) )													// Internal (on board) latch signal
	{
	    LeaveCriticalSection (&CritSect[Card<<1]);			return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Card<<1]);			return TRUE;
	}
}



//////////////////////////////////////////////////////
// IK220LatchExt: Generates external synchronous latch
//////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220LatchExt (USHORT Card)
{
    if (Card>7)
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Card<<1]);
	if ( !LatchExt (Card) )													// External latch signal
	{
	    LeaveCriticalSection (&CritSect[Card<<1]);			return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Card<<1]);			return TRUE;
	}
}



//////////////////////////////////////////////////////
// IK220ResetRef: Resets Axis with next reference mark
//////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ResetRef (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd(Axis, cmdResetREF))												// Command: Reset counter with next ref
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



//////////////////////////////////////////////////////
// IK220StartRef: Starts Axis with next reference mark
//////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220StartRef (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd(Axis, cmdStartREF))												// Command: Start counter with next ref
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



////////////////////////////////////////////////////
// IK220StopRef: Stops Axis with next reference mark
////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220StopRef (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd(Axis, cmdStopREF))												// Command: Stop counter with next ref
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



/////////////////////////////////////////////////////////////////////////////
// IK220LatchRef: Stores counter value in register 2 with next reference mark
/////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220LatchRef (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{																				 
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd(Axis, cmdLatchREF2))											// Command: Store counter with next ref in latch 2
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



/////////////////////////////////////////////////////////////////////////
// IK220Latched: Checks if counter value of Axis is stored in Latch 0/1/2
/////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Latched (USHORT Axis, USHORT Latch, BOOL* pStatus)
{
USHORT	RegW;					// Register value word

	*pStatus = FALSE;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	if (Latch>2)																// Check latch number
	{
		DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;		// Latch number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!InputW (Axis, StaPort, &RegW))						goto ErrExit;		// Read status
	if ((RegW & (1<<Latch))) *pStatus = TRUE;									// Test if latched

    LeaveCriticalSection (&CritSect[Axis]);					return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



////////////////////////////////////////////////////////////////////////////
// IK220WaitLatch: Wait until counter value of Axis is stored in Latch 0/1/2
////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220WaitLatch (USHORT Axis, USHORT Latch)
{
USHORT	RegW;					// Register value word

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	if (Latch>2)																// Check latch number
	{
	 	DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;		// Latch number too high!
	}

	InitTimeOut (TimWaitLatch);
	do
	{
		EnterCriticalSection (&CritSect[Axis]);
		if ( !InputW (Axis, StaPort, &RegW) )				goto ErrExit;		// Read status
		LeaveCriticalSection (&CritSect[Axis]);
	} while ( !(RegW & (1<<Latch)) && !TimeOut(TimWaitLatch, UserTimeOut1) );	// Wait until latched
																				// or time out

	if ( !InputW (Axis, StaPort, &RegW) )					return FALSE;		// Read status
	if ( !(RegW & (1<<Latch)) )													// Latched ?
	{																			
		DLLStatus = DLLStatus | DLLStatus_TimeOut;			return FALSE;		// No, time out!
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}





/////////////////////////////////////////////////
// IK220Set: Set counter value of Axis to PresVal
/////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Set (USHORT Axis, double SetVal)
{
	__int64	Count=0;
	USHORT* pCount = (USHORT*)&Count;


	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, (USHORT)(cmdReadCnt0)))				goto ErrExit;		// Command: Store counter value

	if (!InputL (Axis, DatPort1, (ULONG*)pCount))			goto ErrExit;		// Read counter value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort3, pCount))					goto ErrExit;
    LeaveCriticalSection (&CritSect[Axis]);


	if (*pCount & 0x8000)														// Calculate float counter value
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	Preset[Axis] = SetVal - (double) (Count / 65536.0);
	return TRUE;


	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}


//////////////////////////////////////////////////////
// IK220SetPreset: Set preset value of Axis to PresVal
//////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SetPreset (USHORT Axis, double PresVal)
{

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	Preset[Axis] = PresVal;
	return TRUE;
}


////////////////////////////////////////////////////////
// IK220GetPreset: Gets preset value of Axis to pPresVal
////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220GetPreset (USHORT Axis, double* pPresVal)
{

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	*pPresVal = Preset[Axis];
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////
// IK220Read32: Stores and reads 32-bit count value of Axis from Latch 0/1 to pData
///////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Read32 (USHORT Axis, USHORT Latch, LONG* pData)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	if (Latch>1)																// Check latch number
	{
	 	DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;		// Latch number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, (USHORT)(cmdReadCnt0+Latch)))		goto ErrExit;		// Command: Store counter value
	if (!InputL (Axis, DatPort1, pData))					goto ErrExit;		// Return counter value

    LeaveCriticalSection (&CritSect[Axis]);					return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////////////////////////////////////////////////
// IK220Read48: Stores and reads 48-bit count value of Axis from Latch 0/1 to pData
///////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Read48 (USHORT Axis, USHORT Latch, double* pData)
{
	__int64	Count=0;
	USHORT* pCount = (USHORT*)&Count;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	if (Latch>1)																// Check latch number
	{
	 	DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;		// Latch number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, (USHORT)(cmdReadCnt0+Latch)))		goto ErrExit;		// Command: Store counter value

	if (!InputL (Axis, DatPort1, (ULONG*)pCount))			goto ErrExit;		// Read counter value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort3, pCount))					goto ErrExit;

	if (*pCount & 0x8000)														// Calculate float counter value
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	*pData = Preset[Axis] + (double) Count / 65536.0;


    LeaveCriticalSection (&CritSect[Axis]);					return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



////////////////////////////////////////////////////////////////////////////////
// IK220Get32: Reads stored 32-bit count value of Axis from Latch 0/1/2 to pData
////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Get32 (USHORT Axis, USHORT Latch, LONG* pData)
{
USHORT	RegW;					// Register value word

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	if (Latch>2)																// Check latch number
	{
	 	DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;		// Latch number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!InputW (Axis, StaPort, &RegW))						goto ErrExit;		// Read status
	if (!(RegW & (1<<Latch)))								goto ErrExit;		// Error if not latched

	if (!OutCmd (Axis, (USHORT)(cmdGetCnt0+Latch)))			goto ErrExit;		// Command: Read counter value
	if (!InputL (Axis, DatPort1, pData))					goto ErrExit;		// Return counter value


    LeaveCriticalSection (&CritSect[Axis]);					return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



//////////////////////////////////////////////////////////////////////////////
// IK220Get48: Reads stored 48-bit count value of Axis from Latch 0/1 to pData
//////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Get48 (USHORT Axis, USHORT Latch, double* pData)
{
USHORT	RegW;					// Register value word
__int64	Count=0;
USHORT* pCount = (USHORT*)&Count;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	if (Latch>2)																// Check latch number
	{
	 	DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;		// Latch number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!InputW (Axis, StaPort, &RegW))						goto ErrExit;		// Read status
	if (!(RegW & (1<<Latch)))								goto ErrExit;		// Error if not latched

	if (!OutCmd (Axis, (USHORT)(cmdGetCnt0+Latch)))			goto ErrExit;		// Command: Read counter value

	if (!InputL (Axis, DatPort1, (ULONG*)pCount))			goto ErrExit;
	pCount = pCount+2;
	if (!InputW ( Axis, DatPort3, pCount))					goto ErrExit;

	if (*pCount & 0x8000)														// Calculate float counter value
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	*pData = Preset[Axis] + (double) Count / 65536.0;


    LeaveCriticalSection (&CritSect[Axis]);					return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



//////////////////////////////////////////////////////////////
// IK220CntStatus: Reads counter status from Axis of Latch 0/1
//////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220CntStatus (USHORT Axis, USHORT Latch, USHORT* pRefSta, SHORT* pKorr0, SHORT* pKorr90,
								      SHORT* pNKorr0, SHORT* pNKorr90, USHORT* pSamCnt)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	if (Latch>1)
	{
	 	DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;		// Latch number too high!
	}


	EnterCriticalSection (&CritSect[Axis]);
	if (!OutputW (Axis, DatPort1, Latch))					goto ErrExit;
	if (!OutCmd (Axis, (USHORT)(cmdGetCntStatus)))			goto ErrExit;		// Command: Read counter value

	if (!InputW ( Axis, DatPort1, pRefSta  ))				goto ErrExit;
	if (!InputW ( Axis, DatPort2, pKorr0   ))				goto ErrExit;
	if (!InputW ( Axis, DatPort3, pKorr90  ))				goto ErrExit;
	if (!InputW ( Axis, DatPort4, pNKorr0  ))				goto ErrExit;
	if (!InputW ( Axis, DatPort5, pNKorr90 ))				goto ErrExit;
	if (!InputW ( Axis, DatPort6, pSamCnt  ))				goto ErrExit;

	LeaveCriticalSection (&CritSect[Axis]);					return TRUE;


	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////////////////////////
// IK220DoRef: Starts REF procedure as defined in parameter 5
/////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220DoRef (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdTraverseREF))											// Command: Start REF procedure
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



///////////////////////////////////////
// IK220CancelRef: Stopps REF procedure
///////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220CancelRef (USHORT Axis)
{
	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd(Axis, cmdCancelREF))											// Command: Stop REF procedure
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);				return TRUE;
	}
}



/////////////////////////////////////////////////////////////
// IK220RefActive: Reads REF-active status of Axis to pStatus
/////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220RefActive (USHORT Axis, BOOL* pStatus)
{
USHORT	RegW;					// Register value word

	*pStatus = FALSE;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!InputW (Axis, StaPort, &RegW))						goto ErrExit;		// Read status
    LeaveCriticalSection (&CritSect[Axis]);
	if (RegW & 0x0040)			 *pStatus = TRUE;								// Reset, start or stop with REF

	if (RegW & 0xC000 == 0x4000) *pStatus = TRUE;								// Waiting for 1st REF
	if (RegW & 0xC000 == 0x8000) *pStatus = TRUE;								// Waiting for 2nd REF

	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////////////////
// IK220WaitRef: Waits until REF function is finished
/////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220WaitRef (USHORT Axis)
{
USHORT	RegW;					// Register value word
BOOL	Status;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	InitTimeOut (TimWaitRef);
	do
	{
		EnterCriticalSection (&CritSect[Axis]);
		if (!InputW (Axis, StaPort, &RegW))					goto ErrExit;		// Read status
		LeaveCriticalSection (&CritSect[Axis]);

		Status = FALSE;

		if (RegW & 0x0040)			 Status = TRUE;								// Reset, start, stop or latch with REF

		if (RegW & 0xC000 == 0x4000) Status = TRUE;								// Waiting for 1st REF
		if (RegW & 0xC000 == 0x8000) Status = TRUE;								// Waiting for 2nd REF
	} while ( Status && !TimeOut(TimWaitRef, UserTimeOut1) );					// REF finished or time out

	
	EnterCriticalSection (&CritSect[Axis]);
	if (!InputW (Axis, StaPort, &RegW))						goto ErrExit;		// Read status
	LeaveCriticalSection (&CritSect[Axis]);
	
	Status = FALSE;
	if (RegW & 0x0040)			 Status = TRUE;									// Reset, start, stop or latch with REF
	if (RegW & 0xC000 == 0x4000) Status = TRUE;									// Waiting for 1st REF
	if (RegW & 0xC000 == 0x8000) Status = TRUE;									// Waiting for 2nd REF

	if ( !Status )																// REF finished?
	{
		DLLStatus = DLLStatus | DLLStatus_TimeOut;			return FALSE;		// No, time out!
	}


	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////////////////////////////////////
// IK220PositionRef: Reads counter value at rising edge of reference mark
/////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220PositionRef (USHORT Axis, double* pData, LONG* pPeriod, USHORT* pIntpol)
{
	USHORT	Status;
	__int64	Count=0;
	USHORT* pCount = (USHORT*)&Count;

	*pData =   0.0;
	*pPeriod = 0;
	*pIntpol = 0;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);

	if (!OutCmd1 (Axis, cmdPositionREF))					goto ErrExit;		// Command: Read counter value at reference mark
																				// With user programmed time out control !!!!!!

	if (!InputW (Axis, DatPort1, &Status)) 					goto ErrExit;		// Read status
   	if (Status)																	// Error?
	{
		DLLStatus = DLLStatus | DLLStatus_RefActive;		goto ErrExit;		// Yes, REF is active!
	}

	if (!InputW (Axis, DatPort2, pIntpol)) 					goto ErrExit;		// Read interpolation
	if (!InputL (Axis, DatPort3, pPeriod)) 					goto ErrExit;		// Read counter value

	*pCount = *pIntpol;
	pCount = pCount+1;
	*(LONG*)pCount = *pPeriod;
	pCount = pCount+1;


   	if (*pCount & 0x8000)														// Calculate float counter value
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	*pData = (double) Count / 65536.0;


    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}








//////////////////////////////////////////////////////////////////////////////////////
// IK220PositionRef2: Reads counter value at rising and falling edge of reference mark
//////////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220PositionRef2 (USHORT Axis, double* pData1, LONG* pPeriod1, USHORT* pIntpol1, double* pData2, LONG* pPeriod2, USHORT* pIntpol2)
{
	USHORT	Status;
	__int64	Count=0;
	USHORT* pCount = (USHORT*)&Count;

	*pData1 =   0.0;
	*pPeriod1 = 0;
	*pIntpol1 = 0;

	*pData2 =   0.0;
	*pPeriod2 = 0;
	*pIntpol2 = 0;


	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
   

	///////////////////
	// Read rising edge
	///////////////////

	if (!OutCmd1 (Axis, cmdPositionREF2))					goto ErrExit;		// Command: Read counter value at rising edge of reference mark
																				// With user programmed time out control !!!!!!

	if (!InputW (Axis, DatPort1, &Status)) 					goto ErrExit;		// Read status
   	if (Status)																	// Error?
	{
		DLLStatus = DLLStatus | DLLStatus_RefActive;		goto ErrExit;		// Yes, REF is active!
	}

	if (!InputW (Axis, DatPort2, pIntpol1)) 				goto ErrExit;		// Read interpolation rising edge
	if (!InputL (Axis, DatPort3, pPeriod1)) 				goto ErrExit;		// Read counter value rising edge

	*pCount = *pIntpol1;
	pCount = pCount+1;
	*(LONG*)pCount = *pPeriod1;
	pCount = pCount+1;


   	if (*pCount & 0x8000)														// Calculate float counter value at rising edge
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	*pData1 = (double) Count / 65536.0;




	////////////////////
	// Read falling edge
	////////////////////

	if (!OutCmd (Axis, cmdPositionREF3))					goto ErrExit;		// Command: Read counter value at falling edge of reference mark

	if (!InputW (Axis, DatPort1, &Status)) 					goto ErrExit;		// Read status
   	if (Status)																	// Error?
	{
		DLLStatus = DLLStatus | DLLStatus_RefActive;		goto ErrExit;		// Yes, REF is active!
	}

	if (!InputW (Axis, DatPort2, pIntpol2)) 				goto ErrExit;		// Read interpolation falling edge
	if (!InputL (Axis, DatPort3, pPeriod2)) 				goto ErrExit;		// Read counter value falling edge


	pCount = (USHORT*)&Count;

	*pCount = *pIntpol2;
	pCount = pCount+1;
	*(LONG*)pCount = *pPeriod2;
	pCount = pCount+1;


   	if (*pCount & 0x8000)														// Calculate float counter value at falling edge
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	*pData2 = (double) Count / 65536.0;



    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}






///////////////////////////////////////////////////////
// IK220Status: Reads counter status of Axis to pStatus
///////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Status (USHORT Axis, ULONG* pStatus)
{
USHORT	RegW;					// Register value word

	*pStatus = 0;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;		// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!InputW (Axis, StaPort, &RegW))						goto ErrExit;		// Read status
    LeaveCriticalSection (&CritSect[Axis]);
	*pStatus = (ULONG) RegW;													// Return status
																		
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



////////////////////////////////////////////////////////////////////////////////////////////
// IK220DllStatus: Returns DLL status/info to pDLLStatus/pDLLInfo and clears DLL status/info
////////////////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220DllStatus (ULONG* pDLLStatus, ULONG* pDLLInfo)
{
	*pDLLStatus  = DLLStatus;									// Return DLL status
	*pDLLInfo    = DLLInfo;										// Return DLL info

	DLLStatus = 0;												// Clear DLL status
	DLLInfo   = 0;												// Clear DLL info
	return TRUE;
}



///////////////////////////////////////////
// IK220RefStatus: Reads REF-status of Axis
///////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220RefStatus (USHORT Axis, LONG* pRef1, LONG* pRef2, LONG* pDiff, LONG* pCode, USHORT* pFlag)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd   (Axis, cmdGetRefStat))					goto ErrExit;	// Command: read ref-status

	if (!InputL (Axis, DatPort1, pRef1))					goto ErrExit;	// Read value 1. REF
	if (!InputL (Axis, DatPort3, pRef2))					goto ErrExit;	// Read value 2. REF
	if (!InputL (Axis, DatPort5, pDiff))					goto ErrExit;	// Read difference (1. REF - 2. REF)
	if (!InputL (Axis, DatPort7, pCode))					goto ErrExit;	// Read distance coded offset
	if (!InputW (Axis, DatPort9, pFlag))					goto ErrExit;	// Read REF-Status


    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////////////////////////////////////////////////
// IK220SignalStatus: Reads signal-status of Axis to pStatFreq, pStatMin and pStatMax
/////////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SignalStatus (USHORT Axis, USHORT* pFreq, USHORT* pAmin, USHORT* pAact, USHORT* pAmax)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdGetSigStat))						goto ErrExit;	// Command: read signal-status

	if (!InputW (Axis, DatPort1, pFreq))					goto ErrExit;	// Read status frequency error
	if (!InputW (Axis, DatPort2, pAmin))					goto ErrExit;	// Read status actual  amplitude
	if (!InputW (Axis, DatPort3, pAact))					goto ErrExit;	// Read status minimal amplitude
	if (!InputW (Axis, DatPort4, pAmax))					goto ErrExit;	// Read status maximal amplitude

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



//////////////////////////////////////////
// IK220ReadCorrA: Get correction values A
//////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220GetCorrA (USHORT Axis, SHORT* pOfs0, SHORT* pOfs90, SHORT* pPha0, SHORT* pPha90, SHORT* pSym0, SHORT* pSym90, USHORT* pFlag1, USHORT* pFlag2)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdGetCorrA))						goto ErrExit;	// Command: Get correction values A

	if (!InputW (Axis, DatPort1, pOfs0 ))					goto ErrExit;	// Read offset    0 degree
	if (!InputW (Axis, DatPort2, pOfs90))					goto ErrExit;	// Read offset   90 degree
	if (!InputW (Axis, DatPort3, pPha0 ))					goto ErrExit;	// Read phase     0 degree
	if (!InputW (Axis, DatPort4, pPha90))					goto ErrExit;	// Read phase    90 degree
	if (!InputW (Axis, DatPort5, pSym0 ))					goto ErrExit;	// Read symmetry  0 degree
	if (!InputW (Axis, DatPort6, pSym90))					goto ErrExit;	// Read symmetry 90 degree
	if (!InputW (Axis, DatPort7, pFlag1))					goto ErrExit;	// Read flag 1
	if (!InputW (Axis, DatPort8, pFlag2))					goto ErrExit;	// Read flag 2

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////
// IK220GetCorrB: Get correction values B
/////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220GetCorrB	(USHORT Axis, SHORT*  pOfs0, SHORT* pOfs90, SHORT* pPha0, SHORT*  pPha90, SHORT*  pSym0, SHORT* pSym90, USHORT* pFlag1, USHORT* pFlag2)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdGetCorrB))						goto ErrExit;	// Command: Get correction values B

	if (!InputW (Axis, DatPort1, pOfs0 ))					goto ErrExit;	// Read offset    0 degree
	if (!InputW (Axis, DatPort2, pOfs90))					goto ErrExit;	// Read offset   90 degree
	if (!InputW (Axis, DatPort3, pPha0 ))					goto ErrExit;	// Read phase     0 degree
	if (!InputW (Axis, DatPort4, pPha90))					goto ErrExit;	// Read phase    90 degree
	if (!InputW (Axis, DatPort5, pSym0 ))					goto ErrExit;	// Read symmetry  0 degree
	if (!InputW (Axis, DatPort6, pSym90))					goto ErrExit;	// Read symmetry 90 degree
	if (!InputW (Axis, DatPort7, pFlag1))					goto ErrExit;	// Read flag 1
	if (!InputW (Axis, DatPort8, pFlag2))					goto ErrExit;	// Read flag 2

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////////
// IK220LoadCorrA: Load correction values A
///////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220LoadCorrA (USHORT Axis, SHORT Ofs0, SHORT Ofs90, SHORT Pha0, SHORT Pha90,
										           SHORT Sym0, SHORT Sym90)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutputW (Axis, DatPort1, Ofs0 ))					goto ErrExit;	// Load offset    0 degree
	if (!OutputW (Axis, DatPort2, Ofs90))					goto ErrExit;	// Load offset   90 degree
	if (!OutputW (Axis, DatPort3, Pha0 ))					goto ErrExit;	// Load phase     0 degree
	if (!OutputW (Axis, DatPort4, Pha90))					goto ErrExit;	// Load phase    90 degree
	if (!OutputW (Axis, DatPort5, Sym0 ))					goto ErrExit;	// Load symmetry  0 degree
	if (!OutputW (Axis, DatPort6, Sym90))					goto ErrExit;	// Load symmetry 90 degree

	if (!OutCmd  (Axis, cmdLoadCorrA))						goto ErrExit;	// Command: Load correction values A

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		 return FALSE;
}



////////////////////////////////////////
// IK220OctStatus: Reads octand counters
////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220OctStatus (USHORT Axis, USHORT* pOkt0, USHORT* pOkt1, USHORT* pOkt2, USHORT* pOkt3, USHORT* pOkt4, USHORT* pOkt5, USHORT* pOkt6, USHORT* pOkt7, USHORT* pSamCnt)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd   (Axis, cmdGetOktCnt))						goto ErrExit;	// Command: read oktand counters

	if (!InputW (Axis, DatPort1, pOkt0))					goto ErrExit;	// Read oktand counter 0
	if (!InputW (Axis, DatPort2, pOkt1))					goto ErrExit;	// Read oktand counter 1
	if (!InputW (Axis, DatPort3, pOkt2))					goto ErrExit;	// Read oktand counter 2
	if (!InputW (Axis, DatPort4, pOkt3))					goto ErrExit;	// Read oktand counter 3
	if (!InputW (Axis, DatPort5, pOkt4))					goto ErrExit;	// Read oktand counter 4
	if (!InputW (Axis, DatPort6, pOkt5))					goto ErrExit;	// Read oktand counter 5
	if (!InputW (Axis, DatPort7, pOkt6))					goto ErrExit;	// Read oktand counter 6
	if (!InputW (Axis, DatPort8, pOkt7))					goto ErrExit;	// Read oktand counter 7

	if (!InputW (Axis, DatPort9, pSamCnt))					goto ErrExit;	// Read sample counter

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



////////////////////////////////////////////////
// IK220ChkSumPara: Reads check sum of parameter
////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ChkSumPar (USHORT Axis, USHORT* pChkSum)
{
	*pChkSum=0;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdGetChkSumPar))					goto ErrExit;	// Command: Get check sum parameter

	if (!InputW (Axis, DatPort1, pChkSum ))					goto ErrExit;	// Return check sum parameter

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////////
// IK220ChkSumPrg: Reads check sum of program
/////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ChkSumPrg (USHORT Axis, USHORT* pChkSum1, USHORT* pChkSum2)
{
	USHORT Adr, PrgStart, PrgEnd;

	*pChkSum1=0;
	*pChkSum2=0;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdGetChkSumPrg))					goto ErrExit;	// Command: Get check sum program

	if (!InputW (Axis, DatPort1, pChkSum1 ))				goto ErrExit;	// Read check sum program
	if (!InputW (Axis, DatPort2, &PrgStart))				goto ErrExit;	// Read start address program
	if (!InputW (Axis, DatPort3, &PrgEnd  ))				goto ErrExit;	// Read end   address program

    LeaveCriticalSection (&CritSect[Axis]);

	for (Adr=PrgStart+2; Adr<PrgEnd+2; Adr++)
	{	
		if (Adr>sizeof(Pgm220)/2)											// Check program address
		{
	 		DLLStatus = DLLStatus | DLLStatus_AddressHigh;	return FALSE;	// Program address is too high!
		}
		*pChkSum2=*pChkSum2+Pgm220[Adr];
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////////////////////////
// IK220WritePar: Writes ParVal to parameter ParNum of Axis
///////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220WritePar (USHORT Axis, USHORT ParNum, ULONG ParVal)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!WritePar (Axis, ParNum, ParVal))									// Write parameter
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);
		return TRUE;
	}
}



/////////////////////////////////////////////////////////
// IK220ReadPar: Reads parameter ParNum of Axis to ParVal
/////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ReadPar (USHORT Axis, USHORT ParNum, ULONG* pParVal)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if(!ReadPar (Axis, ParNum, pParVal))									// Read Parameter
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);
		return TRUE;
	}
}



/////////////////////////////////////
// IK220ResetEn: Resets EnDat-Encoder
/////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ResetEn (USHORT Axis, USHORT* pStatus)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdResetEn))							goto ErrExit;	// Command: Reset EnDat encoder
	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Return EnDat status
	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			goto ErrExit;	// IK reports error
	}

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



//////////////////////////////////////////////////////
// IK220ConfigEn: Reads configuration of EnDat-Encoder
//////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ConfigEn (USHORT Axis, USHORT* pStatus, USHORT* pType, ULONG* pPeriod, ULONG* pStep,
									 USHORT* pTurns, USHORT* pRefDist, USHORT* pCntDir)
{

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdConfigEn))						goto ErrExit;	// Command: Read EnDat encoder configuration
	if (!InputW (Axis, DatPort1, pStatus  ))				goto ErrExit;	// Return EnDat status
	if (!InputW (Axis, DatPort2, pType    ))				goto ErrExit;	// Return EnDat type

	if (!InputL (Axis, DatPort3, pPeriod  ))				goto ErrExit;	// Return incremental signal period
	if (!InputL (Axis, DatPort5, pStep    ))				goto ErrExit;	// Return EnDat step
		
	if (!InputW (Axis, DatPort7, pTurns   ))				goto ErrExit;	// Return number of EnDat turns

	if (!InputW (Axis, DatPort8, pRefDist ))				goto ErrExit;	// Return Ref distance
    if (!InputW (Axis, DatPort9, pCntDir  ))				goto ErrExit;	// Return counting direction

    LeaveCriticalSection (&CritSect[Axis]);


	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			return FALSE;	// IK reports error
	}

	if ( (*pType==0x4001) || (*pType==6001) )								// Correction factor for Linear encoder
	{
		if (*pPeriod!=0) FactorEn[Axis] = (double)*pStep / (double)*pPeriod;
	}


	if ( (*pType==0xC001) || (*pType==0xE001) )								// Correction factor for Rotary encoder
	{
		if (*pStep!=0) FactorEn[Axis] = (double)*pPeriod / (double)*pStep;
	}



	if ( !IK220ReadMemEn (Axis, 1,  9, &OEM_Adr[Axis][0], pStatus) )  return FALSE;	// Read memory range 1/2 for OEM parameters
	if ( !IK220ReadMemEn (Axis, 1, 10, &OEM_Adr[Axis][1], pStatus) )  return FALSE;	// Read memory range 3/4 for OEM parameters
	if ( !IK220ReadMemEn (Axis, 1, 11, &CORR_Adr[Axis][0], pStatus) ) return FALSE;	// Read memory range 1/2 for correction values
	if ( !IK220ReadMemEn (Axis, 1, 12, &CORR_Adr[Axis][1], pStatus) ) return FALSE;	// Read memory range 3/4 for correction values

	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);				  return FALSE;
}



////////////////////////////////////////////////////
// IK220ReadEn: Reads counter value of EnDat-Encoder
////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ReadEn (USHORT Axis, USHORT* pStatus, double* pData, USHORT* pAlarm)
{
	__int64	Count=0;
	USHORT* pCount = (USHORT*)&Count;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdReadEn))							goto ErrExit;	// Command: Read absolute EnDat counter value
	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Return EnDat status
	
	if (!InputL (Axis, DatPort2, (ULONG*)pCount))			goto ErrExit;	// Read counter value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort4, pCount))					goto ErrExit;

	if (!InputW (Axis, DatPort5, pAlarm))					goto ErrExit;	// Return EnDat alarm bit

    LeaveCriticalSection (&CritSect[Axis]);


	if (*pCount & 0x8000)													// Calculate float counter value
	{
		pCount=pCount+1;
	    *pCount=0xFFFF;
	}
	*pData=(double)Count * FactorEn[Axis];


	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			return FALSE;	// IK reports error
	}

	if (*pAlarm!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_EnDatAlarm;		return FALSE;	// Alarm from EnDat encoder
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////////////////////////////////////////////
// IK220ReadEnInc: Reads counter value of EnDat-Encoder and incremental counter
///////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ReadEnInc (USHORT Axis, USHORT Latch, USHORT* pStatus, double* pDataEn, USHORT* pAlarm, double* pDataInc)
{
	__int64	Count;
	USHORT* pCount;


	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if (Latch>1)
	{
	 	DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;	// Latch number too high!
		return FALSE;
	}


	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, (USHORT)(cmdReadEnInc0+Latch)))		goto ErrExit; 	// Command: Read absolute and incrmental EnDat counter value
	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit; 	// Return EnDat status
	

	Count=0;
	pCount = (USHORT*)&Count;

	if (!InputL (Axis, DatPort2, (ULONG*)pCount))			goto ErrExit;	// Read EnDat value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort4, pCount))					goto ErrExit;

	if (!InputW (Axis, DatPort5, pAlarm))					goto ErrExit;	// Return EnDat alarm bit


	if (*pCount & 0x8000)
	{
		pCount=pCount+1;
	    *pCount=0xFFFF;
	}
	*pDataEn=(double)Count * FactorEn[Axis];



	Count=0;
	pCount = (USHORT*)&Count;
	
	if (!InputL (Axis, DatPort6, (ULONG*)pCount))			goto ErrExit;	// Read incremental value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort8, pCount))					goto ErrExit;

    LeaveCriticalSection (&CritSect[Axis]);


	if (*pCount & 0x8000)
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	*pDataInc =  Preset[Axis] + (double)Count / 65536.0;


	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			return FALSE;	// IK reports error
	}

	if (*pAlarm!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_EnDatAlarm;		return FALSE;	// Alarm from EnDat encoder
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////////////////
// IK220EnContMode: Sets EnDat continous clock mode
///////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ModeEnCont (USHORT Axis, USHORT Latch, USHORT Mode, USHORT* pStatus)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}
	
	if (Latch!=0)
	{
	 	DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;	// Latch number too high!
	}

	if (Mode>2)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutputW (Axis, DatPort1, Latch))					goto ErrExit;

	if (!OutCmd (Axis, (USHORT)(cmdEnContMode0+Mode)))		goto ErrExit;	// Command:	0 = Continous mode: off
																			//			1 = Continous mode: with    CRC check
																			//			2 = Continous mode: without CRC check

	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Return EnDat status

    LeaveCriticalSection (&CritSect[Axis]);


	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			return FALSE;	// IK reports error
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IK220ReadEnIncCont: Reads counter value of EnDat-Encoder and incremental counter with continous EnDat-clock
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ReadEnIncCont (USHORT Axis, USHORT* pStatus, double* pDataEn, USHORT* pAlarm, double* pDataInc, USHORT* pSigSta)
{
USHORT	RegW;					// Register value word
__int64	 Count;
USHORT* pCount;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, (USHORT)(cmdReadEnIncCont)))			goto ErrExit;	// Command: Read absolute and incremental value
																			//			with continous EnDat clock
	if (!InputW (Axis, DatPort1, pStatus))		 			goto ErrExit;	// Return EnDat status

	
	Count=0;
	pCount = (USHORT*)&Count;

	if (!InputL (Axis, DatPort2, (ULONG*)pCount))			goto ErrExit;	// Read absolute value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort4, pCount))					goto ErrExit;

	if (*pCount & 0x8000)
	{
		pCount=pCount+1;
	    *pCount=0xFFFF;
	}
	*pDataEn=(double)Count * FactorEn[Axis];

	if (!InputW (Axis, DatPort5, pAlarm))					goto ErrExit;	// Return EnDat alarm bit



	Count=0;
	pCount = (USHORT*)&Count;
	
	if (!InputL (Axis, DatPort6, (ULONG*)pCount))			goto ErrExit;	// Read incremental value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort8, pCount))					goto ErrExit;


	if (*pCount & 0x8000)
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	*pDataInc = Preset[Axis] + (double)Count / 65536.0;


	if (!InputW (Axis, StatusReg, &RegW))		 			goto ErrExit;	// Read signal status
	*pSigSta = (RegW>>3) & 0x3F;

    LeaveCriticalSection (&CritSect[Axis]);


	if (*pStatus)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			return FALSE;	// IK reports error
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



//////////////////////////////////////////////
// IK220AlarmEn: Reads and clears alarm status
//////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220AlarmEn (USHORT Axis, USHORT* pStatus, USHORT* pAlarm)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdReadAlarmEn))						goto ErrExit;	// Command: Read EnDat alarm status
	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Return EnDat status
	if (!InputW (Axis, DatPort2, pAlarm ))					goto ErrExit;	// Return EnDat alarm status

	if (!OutCmd (Axis, cmdClearAlarmEn))					goto ErrExit;	// Command: Clear EnDat alarm 

    LeaveCriticalSection (&CritSect[Axis]);


	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			return FALSE;	// IK reports error
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////////////
// IK220WarnEn: Reads and clears warning status
///////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220WarnEn (USHORT Axis, USHORT* pStatus, USHORT* pWarn)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdReadWarnEn))						goto ErrExit;	// Command: Read EnDat warning status
	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Return EnDat status
	if (!InputW (Axis, DatPort2, pWarn  ))					goto ErrExit;	// Return EnDat warning status

	if (!OutCmd (Axis, cmdClearWarnEn))						goto ErrExit;	// Command: Clear EnDat warning status

    LeaveCriticalSection (&CritSect[Axis]);


	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			return FALSE;	// IK reports error
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////////////////////////////////////////
// IK220ReadMemEn: Reads word from EnDat memory from Range/MemAdr to pMemData
/////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ReadMemEn (USHORT Axis, USHORT Range, USHORT MemAdr, USHORT* pMemData, USHORT* pStatus)
{
	USHORT MrsCodeEn;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}


	switch (Range)
	{
	// Status
	case 0:	if (MemAdr>3)														// Check address
			{
	 			DLLStatus = DLLStatus | DLLStatus_OutOfRange;	return FALSE;	// Address out of range!
			}
			MrsCodeEn = 0xB9;													// MRS-Code for status
			break;

	// MS-Parameter
	case 1:	if ((MemAdr<4) | (MemAdr>47))										// Check address
			{
	 			DLLStatus = DLLStatus | DLLStatus_OutOfRange;	return FALSE;	// Address out of range!
			}
			MrsCodeEn = 0xA1 + ((MemAdr >> 4)<<1);								// MRS-Code for manufacturer parameter
			MemAdr = MemAdr & 0x0F;
			break;

	// Parameter
	case 2:	if (MemAdr>15)														// Check address
			{
	 			DLLStatus = DLLStatus | DLLStatus_OutOfRange;	return FALSE;	// Address out of range!
			}
			MrsCodeEn = 0xA7;													// MRS-Code for system parameter
			break;

	// OEM-Parameter
	case 3:	MrsCodeEn = 0xA9;
			if (!FindEnRange (Axis, &MemAdr, &MrsCodeEn))		return FALSE;	// Convert linear address to MRS/offset
			break;

	// MS-Correction values
	case 4:	MrsCodeEn = 0xB1;
			if (!FindEnRange (Axis, &MemAdr, &MrsCodeEn))		return FALSE;	// Convert linear address to MRS/offset
			break;

	default: DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}



	EnterCriticalSection (&CritSect[Axis]);

	
	// Select mode and memory range
	if (!OutputW (Axis, DatPort1, ModeSelRangeEn))			goto ErrExit;	// Mode: Send MRS-Code
	if (!OutputW (Axis, DatPort2, MrsCodeEn))				goto ErrExit;
	
	if (!OutCmd  (Axis, cmdCommLEn)) 						goto ErrExit;	// Command: Execute EnDat command

	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Read status
	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			goto ErrExit;	// IK reports error
	}



	// Read memory value	
	if (!OutputW (Axis, DatPort1, ModeReadMemEn))			goto ErrExit;	// Mode: Read memory
	if (!OutputW (Axis, DatPort2, MemAdr       ))			goto ErrExit;
	
	if (!OutCmd (Axis, cmdCommLEn))							goto ErrExit;

	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Read status
	if (!InputW (Axis, DatPort3, pMemData))					goto ErrExit;	// Read data
	
	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			goto ErrExit;	// IK reports error
	}


    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////////////////////////////////////
// IK220WriteMemEn: Writes word MemData to EnDat memory at Range/MemAdr
///////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220WriteMemEn (USHORT Axis, USHORT Range, USHORT MemAdr, USHORT MemData, USHORT* pStatus)
{
	USHORT MrsCodeEn;

	if (Axis>15)																// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}


	switch (Range)
	{
	// Status
	case 0:	if (MemAdr>3)														// Check address
			{
	 			DLLStatus = DLLStatus | DLLStatus_OutOfRange;	return FALSE;	// Address out of range!
			}
			MrsCodeEn = 0xB9;													// MRS-Code for status
			break;

	// MS-Parameter
	case 1:	if ((MemAdr<4) | (MemAdr>47))										// Check address
			{
	 			DLLStatus = DLLStatus | DLLStatus_OutOfRange;	return FALSE;	// Address out of range!
			}
			MrsCodeEn = 0xA1 + ((MemAdr >> 4)<<1);								// MRS-Code for manufacturer parameter
			MemAdr = MemAdr & 0x0F;
			break;

	// Parameter
	case 2:	if (MemAdr>15)														// Check address
			{
	 			DLLStatus = DLLStatus | DLLStatus_OutOfRange;	return FALSE;	// Address out of range!
			}
			MrsCodeEn = 0xA7;													// MRS-Code for system parameter
			break;

	// OEM-Parameter
	case 3:	MrsCodeEn = 0xA9;
			if (!FindEnRange (Axis, &MemAdr, &MrsCodeEn))		return FALSE;	// Convert linear address to MRS/offset
			break;

	// MS-Correction values
	case 4:	MrsCodeEn = 0xB1;
			if (!FindEnRange (Axis, &MemAdr, &MrsCodeEn))		return FALSE;	// Convert linear address to MRS/offset
			break;

	default: DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}



	EnterCriticalSection (&CritSect[Axis]);

	
	// Select mode and memory range
	if (!OutputW (Axis, DatPort1, ModeSelRangeEn))			goto ErrExit;	// Mode: Send MRS-Code
	if (!OutputW (Axis, DatPort2, MrsCodeEn))				goto ErrExit;
	
	if (!OutCmd  (Axis, cmdCommLEn)) 						goto ErrExit;	// Command: Execute EnDat command

	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Read status
	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			goto ErrExit;	// IK reports error
	}



	// Write memory value	
	if (!OutputW (Axis, DatPort1, ModeWriteMemEn))			goto ErrExit;	// Mode: Write memory
	if (!OutputW (Axis, DatPort2, MemAdr        ))			goto ErrExit;
	if (!OutputW (Axis, DatPort3, MemData))					goto ErrExit;
	
	if (!OutCmd (Axis, cmdCommLEn))							goto ErrExit;	// Command: Execute EnDat command

	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Read status
	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			goto ErrExit;	// IK reports error
	}


    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////////////////
// IK220ReadSSI: Reads counter value of SSI-Encoder
///////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ReadSSI (USHORT Axis, USHORT* pStatus, double* pData)
{
	__int64	Count=0;
	USHORT* pCount = (USHORT*)&Count;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdReadSSI))							goto ErrExit;	// Command: Read absolute SSI value
	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Return SSI status
	
	if (!InputL (Axis, DatPort2, (ULONG*)pCount))			goto ErrExit;	// Read absolute value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort4, pCount))					goto ErrExit;

    LeaveCriticalSection (&CritSect[Axis]);

	if (*pCount & 0x8000)													// Convert to float value
	{
		pCount=pCount+1;
	    *pCount=0xFFFF;
	}
	*pData=(double) Count;

	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			return FALSE;	// IK reports error
	}


	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



//////////////////////////////////////////////////////////////////////////////
// IK220ReadSsiInc: Reads counter value of SSI-Encoder and incremental counter
//////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ReadSsiInc (USHORT Axis, USHORT Latch, USHORT* pStatus, double* pDataSsi, double* pDataInc)
{
	__int64	 Count;
	USHORT* pCount;


	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if (Latch>1)
	{
		DLLStatus = DLLStatus | DLLStatus_LatchNumberHigh;	return FALSE;	// Latch number too high!
	}


	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, (USHORT)(cmdReadSsiInc0+Latch)))		goto ErrExit;	// Command: Read incremental an absolute SSI value
	if (!InputW (Axis, DatPort1, pStatus))					goto ErrExit;	// Return SSI status
	
	Count=0;
	pCount = (USHORT*)&Count;

	if (!InputL (Axis, DatPort2, (ULONG*)pCount))			goto ErrExit;	// Read absolute SSI value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort4, pCount))					goto ErrExit;
															
	if (*pCount & 0x8000)													// Convert to float value
	{
		pCount=pCount+1;
	    *pCount=0xFFFF;
	}
	*pDataSsi=(double) Count;



	Count=0;
	pCount = (USHORT*)&Count;
	
	if (!InputL (Axis, DatPort6, (ULONG*)pCount))			goto ErrExit;	// Return invrmental SSI value
	pCount = pCount+2;
	if (!InputW (Axis, DatPort8, pCount))					goto ErrExit;

	if (*pCount & 0x8000)													// Convert to float value
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	*pDataInc = Preset[Axis] + (double) Count / 65536.0;

    LeaveCriticalSection (&CritSect[Axis]);


	if (*pStatus!=0)
	{
		DLLStatus = DLLStatus | DLLStatus_ErrorIK;			return FALSE;	// IK reports error
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IK220SetTimer: Loads Timer counter with nearest possible SetVal[µs] and returns this value in pTimVal[µs]
////////////////////////////////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SetTimer (USHORT Axis, ULONG SetVal, ULONG* pTimVal)
{
	USHORT i;
	ULONG  CValue, SValue;
	ULONG  TimDiff[CntValSize];
	ULONG  TimSel;


	if (SetVal>3200)
	{
		for (i=0; i<CntValSize; i++) TimDiff[i] = (CntVal[i]-(SetVal % CntVal[i])) % CntVal[i];

		CValue=CntValSize-1;
		TimSel=CntVal[CValue];
		for (i=(CntValSize-1); i>0; i--)
		{
			if ((TimDiff[i]<TimSel) && (CntVal[i]>=100))
			{
				TimSel=TimDiff[i];
				CValue=i;
			}
		}
		SValue = (SetVal / CntVal[CValue]);
		if (TimDiff[CValue]) SValue++;
	}
	else
	{
		SValue=1;
		for (CValue=0; CValue<CntValSize; CValue++)
		{
			if ((CntVal[CValue]>=SetVal) && (CntVal[CValue]>=100)) break;
		}
	}
	*pTimVal=SValue*CntVal[CValue];


	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!WritePar (Axis, parPreScaler,  SValue))			goto ErrExit;	// Store value for prescaler
	if (!WritePar (Axis, parTimerValue, CValue))			goto ErrExit;	// Store value for timer
    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: 
			if (!ReadPar (Axis, parPreScaler,  &SValue))	goto ErrExit1;	// Read value for prescaler
			if (!ReadPar (Axis, parTimerValue, &CValue))	goto ErrExit1;	// Read value for timer
			*pTimVal=SValue*CntVal[CValue];									// Return actual values
	
	ErrExit1:
			LeaveCriticalSection (&CritSect[Axis]);			return FALSE;
}



//////////////////////////////////
// IK220ModeTimer: Sets Timer mode
//////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ModeTimer (USHORT Axis, USHORT Mode)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if (Mode>1)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutputW (Axis, DatPort1, Mode))					goto ErrExit;
	if (!OutCmd  (Axis, cmdTimerMode))						goto ErrExit;	// Command: Set timer mode

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////
// IK220ModeRam: Sets mode for RAM buffer
/////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ModeRam (USHORT Axis, USHORT Mode)
{
	USHORT Status;


	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if ((Mode & 0x000F)>4)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}

	if ((Mode >> 4)>2)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}


	EnterCriticalSection (&CritSect[Axis]);
	if (!OutputW (Axis, DatPort1, Mode))					goto ErrExit;
	if (!OutCmd  (Axis, cmdRamMode))						goto ErrExit;	// Command: Set RAM mode

	if (!InputW  (Axis, DatPort1, &Status))					goto ErrExit;	// Read status

    LeaveCriticalSection (&CritSect[Axis]);


	if (Status)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////
// IK220ResetRam: Resets RAM buffer
///////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ResetRam (USHORT Axis)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdResetRam))						goto ErrExit;	// Command: Reset RAM buffer

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



////////////////////////////////////////////////////////////////////////
// IK220GetRam: Reads stored 48-bit count value from RAM buffer to pData
////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220GetRam (USHORT Axis, double* pData, USHORT* pRead, USHORT* pWrite, USHORT* pStatus)
{
	__int64	Count=0;
	USHORT* pCount = (USHORT*)&Count;


	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd   (Axis, cmdGetRam))						goto ErrExit;	// Command: Get counter value RAM buffer

	if (!InputL (Axis, DatPort1, (ULONG*)pCount))			goto ErrExit;	// Read counter value
	pCount = pCount+2;
	if (!InputW ( Axis, DatPort3, pCount))					goto ErrExit;

	if (!InputW ( Axis, DatPort4, pRead))					goto ErrExit;
	if (!InputW ( Axis, DatPort5, pWrite))					goto ErrExit;
	if (!InputW ( Axis, DatPort6, pStatus))					goto ErrExit;

    LeaveCriticalSection (&CritSect[Axis]);


	if (*pCount & 0x8000)													// Convert to double
	{
		pCount=pCount+1;
		*pCount=0xFFFF;
	}
	*pData = Preset[Axis] + (double) Count / 65536.0;
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
// IK220GetSig: Reads stored signal amplitude and 16bit of counter from RAM buffer to pPeriod/pAmp0/pAmp90
//////////////////////////////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220GetSig (USHORT Axis, USHORT* pPeriod, SHORT* pAmp0, SHORT* pAmp90, USHORT* pRead, USHORT* pWrite, USHORT* pStatus)
{

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd   (Axis, cmdGetRam))						goto ErrExit;	// Command: Get counter value RAM buffer

	if (!InputW ( Axis, DatPort1, pAmp0  ))					goto ErrExit;	// Read amplitude  0 degree
	if (!InputW ( Axis, DatPort2, pAmp90 ))					goto ErrExit;	// Read amplitude 90 degree
	if (!InputW ( Axis, DatPort3, pPeriod))					goto ErrExit;	// Read lower 16bit of counter

	if (!InputW ( Axis, DatPort4, pRead))					goto ErrExit;	// Read read  pointer
	if (!InputW ( Axis, DatPort5, pWrite))					goto ErrExit;	// Read write pointer
	if (!InputW ( Axis, DatPort6, pStatus))					goto ErrExit;	// Read status

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}


	

#ifdef _Windows95

////////////////////////////////////////////////////////////////////////////////
// Windows95
// IK220BurstRam: Reads up to maxCount count value(s) from RAM buffer to pData[]
////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220BurstRam (USHORT Axis, USHORT maxCount, double* pData, USHORT* pCount, USHORT* pStatus)
{
	USHORT i;
	__int64	Value;
	USHORT* pValue;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if ( !Exists[Axis] )
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;	return FALSE;	// Axis not installed
	}

	if ( (maxCount<1) | (maxCount>8191) )									// Check transfer count 
	{
		DLLStatus = DLLStatus | DLLStatus_OutOfRange;		return FALSE;	// Out of range!
	}


	EnterCriticalSection (&CritSect[Axis]);

	i=0;
	*pStatus=0;
	*pCount =0;
	do
	{
		*(PortBase[Axis]+CmdPort) = cmdGetRam;												// Command:  to IK 220
	
		InitTimeOut (TimDLL);																// Init timer for time out
		do {
		} while ( !(*(PortBase[Axis]+Flag1Reg) & G28Sem0) && !TimeOut (TimDLL, TIM_1ms) );	// Wait until finished
	
		if ( !(*(PortBase[Axis]+Flag1Reg) & G28Sem0) )										// Finished ?
		{
			*pStatus = *pStatus | statBufferFailed;
			DLLStatus = DLLStatus | DLLStatus_ErrorIK;		break;							// Error from IK 220!
		}


		if ( *(PortBase[Axis]+DatPort0)!=cmdGetRam )
		{
			*pStatus = *pStatus | statBufferFailed;
			DLLStatus = DLLStatus | DLLStatus_TimeOut;		break;							// Error time out
		}

		*pStatus = *pStatus | *(PortBase[Axis]+DatPort6);
		if (*pStatus & statBufferNoVal)						break;							// RAM buffer empty


		// Convert 48 bit counter value to double
		Value=0;
		pValue = (USHORT*)&Value;

		*pValue++=*(PortBase[Axis]+DatPort1);
		*pValue++=*(PortBase[Axis]+DatPort2);
		*pValue  =*(PortBase[Axis]+DatPort3);

		if (*pValue & 0x8000)
		{
			pValue=pValue+1;
			*pValue=0xFFFF;
		}
		*pData++ = Preset[Axis] + (double) Value / 65536.0;

		i++;
	} while ( (i<maxCount) && !(*pStatus & statBufferEmpty) );

	if ( !(*pStatus & statBufferFailed) )
	{
		LeaveCriticalSection (&CritSect[Axis]);
		*pCount  = i;
		return TRUE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);
		*pCount  = 0;
		return FALSE;
	}
 }


#else


////////////////////////////////////////////////////////////////////////////////
// WindowsNT
// IK220BurstRam: Reads up to maxCount count value(s) from RAM buffer to pData[]
////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220BurstRam (USHORT Axis, USHORT maxCount, double* pData, USHORT* pCount, USHORT* pStatus)
{

	DWORD  byteCount;
	USHORT i;
	USHORT 	Buffer[1+8192*3];		// Buffer for Status and counter values
	USHORT* pBuffer = &Buffer[1];	// Pointer to Buffer
	__int64	Value;
	USHORT* pValue;


	*pCount  = 0;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
		return FALSE;
	}

	if ( (maxCount<1) | (maxCount>8191) )									// Check transfer count 
	{
		DLLStatus = DLLStatus | DLLStatus_OutOfRange;		return FALSE;	// Out of range!
	}



	EnterCriticalSection (&CritSect[Axis]);

	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_BURSTRAM,		// I/O control code
							  &Axis,						// Pointer to input buffer
							  (DWORD)sizeof(Axis),			// Input buffer size
							  &Buffer,						// Pointer to output buffer
							  (DWORD)((maxCount*3+1)*2),	// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;	goto ErrExit;	// Error device i/o control
	}

 

	// Convert 48 bit counter values to double
	if (byteCount>2)
	{
		for (i=0; i<(((byteCount/2)-1)/3); i++)
		{
			Value=0;
			pValue = (USHORT*)&Value;

			*pValue++=*pBuffer++;
			*pValue++=*pBuffer++;
			*pValue  =*pBuffer++;

			if (*pValue & 0x8000)
			{
				pValue=pValue+1;
				*pValue=0xFFFF;
			}
			*pData++ = Preset[Axis] + (double) Value / 65536.0;
		}
		*pCount  = i;
		*pStatus = Buffer[0];
		LeaveCriticalSection (&CritSect[Axis]);
		return TRUE;
	}
	else
	{
		if ( Buffer[0] & 0x8000 )
		{
			*pStatus = 0x8000;
			if ( Buffer[0] & 0x0001)
			{
				DLLStatus = DLLStatus | DLLStatus_ErrorIK;				// Error from IK 220!
			}

			if ( Buffer[0] & 0x0002)
			{
				DLLStatus = DLLStatus | DLLStatus_TimeOut;				// Error time out!
			}
		}
		else
		{
			*pCount  = 0;
			*pStatus = Buffer[0];
			LeaveCriticalSection (&CritSect[Axis]);
			return TRUE;
		}
	}

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}

#endif



#ifdef _Windows95

///////////////////////////////////////////////////////////////////////////////////////////////////
// Windows95
// IK220BurstSig: Reads up to maxCount count value(s) from RAM buffer to pPeriod[]/pAmp0[]/pAmp90[]
///////////////////////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220BurstSig (USHORT Axis, USHORT maxCount, USHORT* pPeriod, SHORT* pAmp0, SHORT* pAmp90, USHORT* pCount, USHORT* pStatus)
{
	USHORT i;


	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if ( !Exists[Axis] )
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;	return FALSE;	// Axis not installed
	}

	if ( (maxCount<1) | (maxCount>8191) )									// Check transfer count 
	{
		DLLStatus = DLLStatus | DLLStatus_OutOfRange;		return FALSE;	// Out of range!
	}


	EnterCriticalSection (&CritSect[Axis]);

	i=0;
	*pStatus=0;
	*pCount =0;
	do
	{
		*(PortBase[Axis]+CmdPort) = cmdGetRam;												// Command:  to IK 220
	
		InitTimeOut (TimDLL);																// Init timer for time out
		do {
		} while ( !(*(PortBase[Axis]+Flag1Reg) & G28Sem0) && !TimeOut (TimDLL, TIM_1ms) );	// Wait until finished
	
		if ( !(*(PortBase[Axis]+Flag1Reg) & G28Sem0) )										// Finished ?
		{
			*pStatus = *pStatus | statBufferFailed;
			DLLStatus = DLLStatus | DLLStatus_ErrorIK;		break;							// Error from IK 220!
		}


		if ( *(PortBase[Axis]+DatPort0)!=cmdGetRam )
		{
			*pStatus = *pStatus | statBufferFailed;
			DLLStatus = DLLStatus | DLLStatus_TimeOut;		break;							// Error time out
		}

		*pStatus = *pStatus | *(PortBase[Axis]+DatPort6);
		if (*pStatus & statBufferNoVal)						break;							// RAM buffer empty


		// Copy amplitude and counter values
		*pAmp0++   =*(PortBase[Axis]+DatPort1);
		*pAmp90++  =*(PortBase[Axis]+DatPort2);
		*pPeriod++ =*(PortBase[Axis]+DatPort3);

		i++;
	} while ( (i<maxCount) && !(*pStatus & statBufferEmpty) );

	if ( !(*pStatus & statBufferFailed) )
	{
		LeaveCriticalSection (&CritSect[Axis]);
		*pCount  = i;
		return TRUE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);
		*pCount  = 0;
		return FALSE;
	}
 }


#else


////////////////////////////////////////////////////////////////////////////////
// WindowsNT
// IK220BurstSig: Reads up to maxCount count value(s) from RAM buffer to pData[]
////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220BurstSig (USHORT Axis, USHORT maxCount, USHORT* pPeriod, SHORT* pAmp0, SHORT* pAmp90, USHORT* pCount, USHORT* pStatus)
{

	DWORD  byteCount;
	USHORT i;
	USHORT 	Buffer[1+8192*3];		// Buffer for Status, amplitude and counter values
	USHORT* pBuffer = &Buffer[1];	// Pointer to Buffer


	*pCount  = 0;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
		return FALSE;
	}

	if ( (maxCount<1) | (maxCount>8191) )									// Check transfer count 
	{
		DLLStatus = DLLStatus | DLLStatus_OutOfRange;		return FALSE;	// Out of range!
	}



	EnterCriticalSection (&CritSect[Axis]);

	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_BURSTRAM,		// I/O control code
							  &Axis,						// Pointer to input buffer
							  (DWORD)sizeof(Axis),			// Input buffer size
							  &Buffer,						// Pointer to output buffer
							  (DWORD)((maxCount*3+1)*2),	// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;	goto ErrExit;	// Error device i/o control
	}

 

	// Copy amplitude and counter values
	if (byteCount>2)
	{
		for (i=0; i<(((byteCount/2)-1)/3); i++)
		{
			*pAmp0++  =*pBuffer++;
			*pAmp90++ =*pBuffer++;
			*pPeriod++=*pBuffer++;
		}
		*pCount  = i;
		*pStatus = Buffer[0];
		LeaveCriticalSection (&CritSect[Axis]);
		return TRUE;
	}
	else
	{
		if ( Buffer[0] & 0x8000 )
		{
			*pStatus = 0x8000;
			if ( Buffer[0] & 0x0001)
			{
				DLLStatus = DLLStatus | DLLStatus_ErrorIK;				// Error from IK 220!
			}

			if ( Buffer[0] & 0x0002)
			{
				DLLStatus = DLLStatus | DLLStatus_TimeOut;				// Error time out!
			}
		}
		else
		{
			*pCount  = 0;
			*pStatus = Buffer[0];
			LeaveCriticalSection (&CritSect[Axis]);
			return TRUE;
		}
	}

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}

#endif



////////////////////////////////
// IK220Led: Defines mode of LED
////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Led (USHORT Axis, USHORT Mode)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if (Mode>2)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, (USHORT)(cmdLedOff+Mode)))			goto ErrExit;	// Command: Set axis LED mode

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////
// IK220Led: Defines mode of system LED
///////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SysLed (USHORT Card, USHORT Mode)
{
    if (Card>7)
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}
	
	if ( Mode>1 )
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}

	EnterCriticalSection (&CritSect[Card<<1]);
	if ( !SysLed (Card, Mode) )												// Command: Set system LED mode
	{
	    LeaveCriticalSection (&CritSect[Card<<1]);			return FALSE;
	}
	else
	{
	    LeaveCriticalSection (&CritSect[Card<<1]);
		return TRUE;
	}
}



//////////////////////////////
// IK220GetPort: Get port info
//////////////////////////////
DLLEXPORT BOOL WINAPI IK220GetPort (USHORT Axis, USHORT* pPortInfo, USHORT* pPortSet, USHORT* pPortReset)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdGetPort))							goto ErrExit;	// Command: Read input port

	if (!InputW (Axis, DatPort1, pPortInfo  ))				goto ErrExit;	// Return actual port state
	if (!InputW (Axis, DatPort2, pPortSet   ))				goto ErrExit;	// Return port set   state
	if (!InputW (Axis, DatPort3, pPortReset ))				goto ErrExit;	// Return port reset state 

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////////////////////////
// IK220InputW: Returns word read from Axis at Adr to pBuffer
/////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220InputW (USHORT Axis, USHORT Adr, USHORT* pBuffer)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);

	if (InputW (Axis, Adr, pBuffer))										// Read word from IK 220 communication register
	{
		LeaveCriticalSection (&CritSect[Axis]);
		return TRUE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
}



////////////////////////////////////////////////////////////////////
// IK220InputL: Returns double-word read from Axis at Adr to pBuffer
////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220InputL (USHORT Axis, USHORT Adr, ULONG* pData)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);

	if (InputL (Axis, Adr, pData))											// Read doubleword from IK 220 communication register
	{
		LeaveCriticalSection (&CritSect[Axis]);
		return TRUE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
}



////////////////////////////////////////////////////
// IK220Output: Writes word from Data to Axis at Adr
////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220Output (USHORT Axis, USHORT Adr, USHORT Data)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);

	if (OutputW (Axis, Adr, Data))											// Write word to IK 220 communication register
	{								
		LeaveCriticalSection (&CritSect[Axis]);				
		return TRUE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}

}



/////////////////////////////////////////////////////////////////////
// IK220RamRead: Returns word read from RAM of Axis at Adr to pBuffer
/////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220RamRead (USHORT Axis, USHORT Adr, USHORT* pData)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	
	if (RamRead (Axis, Adr, pData))											// Read word for IK 220 internal RAM
	{
		LeaveCriticalSection (&CritSect[Axis]);
		return TRUE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
}



////////////////////////////////////////////////////////
// IK220RamWrite: Writes word Data to RAM in Axis at Adr
////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220RamWrite (USHORT Axis, USHORT Adr, USHORT Data)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	
	if (RamWrite (Axis, Adr, Data))											// Write word to IK 220 internal RAM
	{
		LeaveCriticalSection (&CritSect[Axis]);
		return TRUE;
	}
	else
	{
		LeaveCriticalSection (&CritSect[Axis]);				return FALSE;
	}
}


/////////////////////////////////////////
// IK220SetEnClock: Sets EnDat clock line
/////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SetEnClock (USHORT Axis, BOOL State, USHORT* pStatus)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);

	if (State)
	{
		if (!OutCmd (Axis, (USHORT)(cmdSetEnClock  )))		goto ErrExit;	// Set EnDat clock line high
	}
	else
	{
		if (!OutCmd (Axis, (USHORT)(cmdResetEnClock)))		goto ErrExit;	// Set EnDat clock line low
	}														
 
	if (!InputW ( Axis, DatPort1, pStatus))					goto ErrExit;	// Read state of EnDat clock line

	if (*pStatus!=0)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrorIK;			goto ErrExit;	// Error from IK 220!
	}

    LeaveCriticalSection (&CritSect[Axis]);					
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}


///////////////////////////////////////
// IK220SetEnData: Sets EnDat data line
///////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SetEnData (USHORT Axis, BOOL State, USHORT* pStatus)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);

	if (State)
	{
		if (!OutCmd (Axis, (USHORT)(cmdSetEnData  )))		goto ErrExit;	// Set EnDat data line high
	}
	else
	{
		if (!OutCmd (Axis, (USHORT)(cmdResetEnData)))		goto ErrExit;	// Set EnDat data line low
	}
 
	if (!InputW ( Axis, DatPort1, pStatus))					goto ErrExit;	// Read state of EnDat data line

	if (*pStatus!=0)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrorIK;			goto ErrExit;	// Error from IK 220!
	}


    LeaveCriticalSection (&CritSect[Axis]);					
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}


/////////////////////////////////////////
// IK220ReadEnData: Reads EnDat data line
/////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220ReadEnData (USHORT Axis, BOOL* pState)
{
USHORT	RegW;					// Register value word

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);

	if (!OutCmd (Axis, (USHORT)(cmdReadEnData)))			goto ErrExit;	// Read EnDat data line
 
	if (!InputW ( Axis, DatPort3, &RegW))					goto ErrExit;

	if (RegW==0) *pState=FALSE;
	else         *pState=TRUE;

    LeaveCriticalSection (&CritSect[Axis]);				
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}




/////////////////////////////////////////////////////////////////////////////////////////
// IK220SetTimeOut: Initilizes time out start value for IK220WaitLatch & IK220PositionRef
/////////////////////////////////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SetTimeOut (ULONG TimeOut)
{
	UserTimeOut1 = (__int64)(TimeOut) * TIM_1ms;
	return TRUE;
}



////////////////////////////////////////////////////////////
// IK220RefEval: Defines mode of reference signal evaluation
////////////////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220RefEval (USHORT Axis, USHORT Mode)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if (Mode>1)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}

	EnterCriticalSection (&CritSect[Axis]);

	if (!OutputW (Axis, DatPort1, Mode))					goto ErrExit;	// Write REF evaluation mode
	if (!OutCmd (Axis, (USHORT)(cmdRefEval)))				goto ErrExit;	// Command: Set REF evaluation

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



//////////////////////////////////////////////
// IK220SetBw: Sets bandwidth of encoder input
//////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SetBw (USHORT Axis, USHORT Mode)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if (Mode>1)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutputW (Axis, DatPort1, Mode))					goto ErrExit;
	if (!OutCmd  (Axis, cmdSetBw))							goto ErrExit;	// Command: Set bandwith

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



/////////////////////////////////////////////////
// IK220SetTrm: Sets termination of encoder input
/////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SetTrm (USHORT Axis, USHORT Mode)
{
	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	if (Mode>1)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongMode;		return FALSE;	// Wrong mode
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutputW (Axis, DatPort1, Mode))					goto ErrExit;
	if (!OutCmd  (Axis, cmdSetTrm))							goto ErrExit;	// Command: Set termination

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



//////////////////////////////////////////////////
// IK220SetOpMode: Sets IK 220 operation mode flag
//////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220SetOpMode (USHORT Axis, USHORT Mode)
{
	USHORT RegW;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutputW (Axis, DatPort1, Mode))					goto ErrExit;
	if (!OutCmd  (Axis, cmdSetOpMode))						goto ErrExit;	// Command: Set operation mode


	if (!InputW (Axis, DatPort1, &RegW))					goto ErrExit;	// Verify acknowledge
	if (RegW!=Mode)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongAck;			goto ErrExit;	// Wrong command acknowledge
	}
	
    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



///////////////////////////////////////////////////
// IK220GetOpMode: Reads IK 220 operation mode flag
///////////////////////////////////////////////////
DLLEXPORT BOOL WINAPI IK220GetOpMode (USHORT Axis, USHORT* pMode)
{
	*pMode=0xFFFF;

	if (Axis>15)															// Check axis number
	{
		DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;	return FALSE;	// Axis number too high!
	}

	EnterCriticalSection (&CritSect[Axis]);
	if (!OutCmd (Axis, cmdGetOpMode))						goto ErrExit;	// Command: Get operation mode flag

	if (!InputW (Axis, DatPort1, pMode ))					goto ErrExit;	// Return operation mode flag

    LeaveCriticalSection (&CritSect[Axis]);
	return TRUE;

	ErrExit: LeaveCriticalSection (&CritSect[Axis]);		return FALSE;
}



