/*++

	Copyright (c) 1998,  Dr. Johannes Heidenhain GmbH

	Module Name:	Locals.c
	
	Abstract:		Local functions for Windows DLL for IK 220


	History:		- Function InitTimeOut and TimeOut added for time out function		ES	17.06.1999
					- Function OutCmd1 added for commands with user defined time out	ES	23.06.1999
					- Local variable "RegW instead of global RegW (reentrance)			ES	21.10.1999


--*/

#include <windows.h>
#include <winioctl.h>

#include "..\..\Disk2\include\G28.h"
#include "..\..\Disk2\include\IK220.h"
#include "..\..\Disk2\include\DRVFunc.h"

#include "TimeOut.h"



extern ULONG Exists[2*MAX_CARDS];								// Successful initialized axes

extern USHORT OEM_Adr  [2*MAX_CARDS][2];
extern USHORT CORR_Adr [2*MAX_CARDS][2];


extern __int64 PerfCount;										// Number of counter ticks per second 
extern __int64 UserTimeOut1;									// User time out value for commands
extern __int64 PerfStart[maxTimNum+1];							// Start value of counter
extern __int64 TIM_1us, TIM_1ms, TIM_2s;						// Counter value for specified time


extern ULONG	DLLStatus;										// Status of DLL functions
extern ULONG	DLLInfo;										// Info   of DLL functions



#ifdef _Windows95
	#include <conio.h>

	extern USHORT* PortBase[2*MAX_CARDS];
	extern ULONG*  ConfReg[MAX_CARDS];
#else
	extern	HANDLE	m_hIK220Drv;								// Handle for device driver
#endif



//////////////////////////////////////////////////////////////////////////////
// Reads actual counter value and saves value as start value of counter TimNum
//////////////////////////////////////////////////////////////////////////////
BOOL InitTimeOut (USHORT TimNum)
{																		
	if (TimNum>maxTimNum)														// Check timer number
	{
		DLLStatus = DLLStatus | DLLStatus_TimNumHigh;			return FALSE;	// Timer number too high!
	}
	if (!QueryPerformanceCounter ((LARGE_INTEGER*)&PerfStart[TimNum]))			// Read actual tick count
	{
		DLLInfo = DLLInfo | DLLInfo_NoHighSpeedTimer;			return FALSE;	// High speed timer not available!
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Compares difference between actual and start value of counter TimNum with given delay time
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL TimeOut (USHORT TimNum, __int64 PerfTimeout)
{
	if (PerfTimeout==0)											return FALSE;	// Time out not active!!!
	
	if (TimNum>maxTimNum)														// Check timer number
	{
		DLLStatus = DLLStatus | DLLStatus_TimNumHigh;			return FALSE;	// Timer number too high!
	}


	if (!QueryPerformanceCounter ((LARGE_INTEGER*)&PerfCount))					// Read actual tick count
	{
		DLLInfo = DLLInfo | DLLInfo_NoHighSpeedTimer;			return FALSE;	// High speed timer not available!
	}																			// No time out active!


	if	(PerfCount - PerfStart[TimNum] >= PerfTimeout)			return TRUE;	// Time elapsed
	return FALSE;																// Time not elapsed
}




#ifdef _Windows95


////////////////////////////////////////////
// Windows95
// delay_us: CPU and clock independent delay
////////////////////////////////////////////
void delay_us (USHORT us)
{
	BYTE lo, hi;
	long summ,c,d,e,max;
	__int64 Delay;


	if ( InitTimeOut (TimDLY) )								// Use high speed timer for clock/cpu independent delay
	{									
		Delay = (__int64)(us) * TIM_1us;							// Number of clock ticks for given delay
		do {
		} while ( !TimeOut (TimDLY, Delay) );
	}
	else													// Use access of isa keyboard hardware for clock/cpu independent delay
	{
		DLLInfo = DLLInfo | DLLInfo_NoDelayTimer;					// High speed timer not used for delay!

		max=us*2380L/1000L;
		summ=0L;
		_outp(67,0);					
		lo=_inp(64); 					// Read low  byte
		hi=_inp(64); 					// Read high byte
		d=(hi<<8)+lo;					// Start time
		while(summ<max)
		{
			_outp(67,0);
			lo=_inp(64);  				// Read low  byte
			hi=_inp(64);  				// Read high byte
			c=(hi<<8)+lo; 				// Current time
			e=d-c;						// Elapsed time
			if(e<0L) e=e+65535L;		// If negative correct overflow
			summ=summ+e;	
			d=c;
		}
	}
}




//////////////////////////////////////////////////////////
// Windows95
// InputW: Returns 1 word read from Axis at Adr to pBuffer
//////////////////////////////////////////////////////////
BOOL InputW (USHORT Axis, USHORT Adr, USHORT* pBuffer)
{
	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!PortBase[Axis])														// Check address of axis
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (Adr>15)																// Check address of register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AddressHigh;			return FALSE;	// Adress to high!
	}
	if (!pBuffer)																// Check buffer address
	{
	 	DLLStatus = DLLStatus | DLLStatus_BufferAddressInvalid;	return FALSE;	// Buffer address invalid!
	}
	
	*pBuffer = *(PortBase[Axis]+Adr);											// Read word value from register to pBuffer
	return TRUE;
}


/////////////////////////////////////////////////////////////////////
// Windows95
// InputL: Returns 2 words read from Axis at Adr and Adr+1 to pBuffer
/////////////////////////////////////////////////////////////////////
BOOL InputL (USHORT Axis, USHORT Adr, ULONG* pBuffer)
{
	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!PortBase[Axis])														// Check address of axis
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (Adr>14)																// Check address of register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AddressHigh;			return FALSE;	// Adress to high!
	}
	if (!pBuffer)																// Check buffer address
	{
	 	DLLStatus = DLLStatus | DLLStatus_BufferAddressInvalid;	return FALSE;	// Buffer address invalid!
	}
	
	*((USHORT*) pBuffer)    =  *(PortBase[Axis]+Adr);
	*((USHORT*)(pBuffer)+1) =  *(PortBase[Axis]+Adr+1);
	return TRUE;
}


//////////////////////////////////////////////////
// Windows95
// OutputW: Writes 1 word from Data to Axis at Adr
//////////////////////////////////////////////////
BOOL OutputW (USHORT Axis, USHORT Adr, USHORT Data)
{
	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!PortBase[Axis])														// Check address of axis
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (Adr>15)																// Check address of register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AddressHigh;			return FALSE;	// Adress to high!
	}
	
	*(PortBase[Axis]+Adr) = Data;
	return TRUE;
}



////////////////////////////////////////////////////////////////
// Windows95
// LatchInt: Generates latch pulse for internal syncronous latch
////////////////////////////////////////////////////////////////
BOOL LatchInt (USHORT Card)
{
	if (Card>(MAX_CARDS-1))																	// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if ( !Exists[Card<<1] )														// Check address of card
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (!ConfReg[Card])															// Check address of configuration register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	
	*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) & 0xFFFFFEFF;	// Latch Int low
	delay_us (2);																	// Delay more than 1.5µs
	*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) | 0x00000100;	// Latch Int high
	return TRUE;
}



////////////////////////////////////////////////////////////////
// Windows95
// LatchExt: Generates latch pulse for external syncronous latch
////////////////////////////////////////////////////////////////
BOOL LatchExt (USHORT Card)
{
	if (Card>(MAX_CARDS-1))														// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Card<<1])														// Check card address
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (!ConfReg[Card])															// Check address of configuration register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	
	*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) & 0xFFFFF7FF;	// Latch Ext low
	delay_us (2);																	// Delay more than 1.5µs
	*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) | 0x00000800;	// Latch Ext high
	return TRUE;
}



///////////////////////////////////
// Windows95
// SysLed: Sets state of system LED
///////////////////////////////////
BOOL SysLed (USHORT Card, USHORT Mode)
{
	if (Card>(MAX_CARDS-1))														// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Card<<1])														// Check card address
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (!ConfReg[Card])															// Check address of configuration register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	
	if (Mode==0)
		*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) | 0x00000004;	// System LED off
	else
		*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) & 0xFFFFFFFB;	// System LED on
	return TRUE;
}


/////////////////////////////////////////////////
// Windows95
// SetLatInt: Sets state of output Internal Latch
/////////////////////////////////////////////////
BOOL SetLatInt (USHORT Card, USHORT Mode)
{
	if (Card>(MAX_CARDS-1))														// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Card<<1])														// Check card address
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (!ConfReg[Card])															// Check address of configuration register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	
	if (Mode==0)
		*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) & 0xFFFFFEFF;	// Latch Int. low
	else
		*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) | 0x00000100;	// Latch Int. high
	return TRUE;

}



/////////////////////////////////////////////////
// Windows95
// SetLatExt: Sets state of output External Latch
/////////////////////////////////////////////////
BOOL SetLatExt (USHORT Card, USHORT Mode)
{
	if (Card>(MAX_CARDS-1))														// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Card<<1])														// Check card address
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (!ConfReg[Card])															// Check address of configuration register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	
	if (Mode==0)
		*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) & 0xFFFFF7FF;	// Latch Ext. low
	else
		*(ConfReg[Card]+CONFREG_CNTRL) = *(ConfReg[Card]+CONFREG_CNTRL) | 0x00000800;	// Latch Ext. high
	return TRUE;

}



///////////////////////////////////////////////////////////////////////
// Windows95
// IKFind: Returns port address(es) of installed card('s) in pBuffer256
///////////////////////////////////////////////////////////////////////

BOOL IKFind (ULONG* pBuffer256)
{
USHORT	RegW;					// Register value word
USHORT	Ax;

	if (!pBuffer256)															// Check buffer address
	{
	 	DLLStatus = DLLStatus | DLLStatus_BufferAddressInvalid;	return FALSE;	// Buffer address invalid!
	}

	
	for (Ax=0; Ax<MAX_CARDS; Ax++)
	{
		if (PortBase[Ax])
		{
			if (!InputW (Ax, CodeReg, &RegW))					return FALSE;	// Read hardware code G28
			if (RegW == 0x0007)
				*(pBuffer256+Ax)=(ULONG)PortBase[Ax];							// Return address of IK 220
			else 
				*(pBuffer256+Ax)=0;												// Clear address entry
		}
		else
			*(pBuffer256+Ax)=0;													// Clear address entry
	}

	return TRUE;
}




#else		// Ifdef _Windows95



////////////////////////////////////////////////////////
// WindowsNT
// InputW: Returns word read from Axis at Adr to pBuffer
////////////////////////////////////////////////////////
BOOL InputW (USHORT Axis, USHORT Adr, USHORT* pBuffer)
{
	struct p { USHORT axis, adr, data; } port;
	DWORD  byteCount;
   
	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Axis])															// Check address of axis
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (Adr>15)																	// Check address of register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AddressHigh;			return FALSE;	// Adress to high!
	}
	if (!pBuffer)																// Check buffer address!
	{
	 	DLLStatus = DLLStatus | DLLStatus_BufferAddressInvalid;	return FALSE;	// Buffer address invalid
	}


	port.axis = Axis; port.adr  = Adr;
	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_INPUT,			// I/O control code
							  &port,						// Pointer to input buffer
							  (DWORD) sizeof(port),			// Input buffer size
							  pBuffer,						// Pointer to output buffer
							  (DWORD) sizeof(*pBuffer),		// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control
	}

	if (byteCount!=2)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;			return FALSE;	// Error byte count wrong
	}
	else return TRUE;
}


///////////////////////////////////////////////////////////////
// WindowsNT
// InputL: Returns double-word read from Axis at Adr to pBuffer
///////////////////////////////////////////////////////////////
BOOL InputL (USHORT Axis, USHORT Adr, ULONG* pBuffer)
{
	struct p { USHORT axis, adr, data; } port;
	DWORD  byteCount;

	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Axis])															// Check address of axis
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	if (Adr>14)																	// Check address of register!
	{
	 	DLLStatus = DLLStatus | DLLStatus_AddressHigh;			return FALSE;	// Adress to high
	}
	if (!pBuffer)																// Check buffer address
	{
	 	DLLStatus = DLLStatus | DLLStatus_BufferAddressInvalid;	return FALSE;	// Buffer address invalid!
	}


	port.axis = Axis; port.adr  = Adr;
	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_INPUT,			// I/O control code
							  &port,						// Pointer to input buffer
							  (DWORD) sizeof(port),			// Input buffer size
							  pBuffer,						// Pointer to output buffer
							  (DWORD) sizeof(*pBuffer),		// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control
	}

	if (byteCount!=4)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;			return FALSE;	// Error byte count wrong
	}
	else return TRUE;
}


/////////////////////////////////////////////
// WindowsNT
// OutputW: Writes 1 word Data to Axis at Adr
/////////////////////////////////////////////
BOOL OutputW (USHORT Axis, USHORT Adr, USHORT Data)
{
	struct p { USHORT axis, adr, data; } port;
	DWORD  byteCount;

	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high
	}
	if (!Exists[Axis])															// Check address of axis
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed
	}
	if (Adr>15)																	// Check address of register
	{
	 	DLLStatus = DLLStatus | DLLStatus_AddressHigh;			return FALSE;	// Adress to high
	}


	port.axis = Axis; port.adr = Adr; port.data = Data;
	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_OUTPUT,			// I/O control code
							  &port,						// Pointer to input buffer
							  (DWORD) sizeof(port),			// Input buffer size
							  &port,						// Pointer to output buffer
							  (DWORD) sizeof(port),			// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control!
	}

	if (byteCount!=2)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;			return FALSE;	// Error byte count wrong!
	}
	else return TRUE;
}



////////////////////////////////////////////////////////////////
// WindowsNT
// LatchInt: Generates latch pulse for internal syncronous latch
////////////////////////////////////////////////////////////////
BOOL LatchInt (USHORT Card)
{
  	DWORD  byteCount;
	
	if (Card>(MAX_CARDS-1))														// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Card<<1])														// Check card address
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	
	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_LATCHI,			// I/O control code
							  &Card,			   			// Pointer to input buffer
							  (DWORD) sizeof(Card),			// Input buffer size
							  NULL,							// Pointer to output buffer
							  (DWORD) 0,					// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control!
	}

	if (byteCount!=1)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;			return FALSE;	// Error byte count wrong!
	}
	else return TRUE;
}


////////////////////////////////////////////////////////////////
// WindowsNT
// LatchExt: Generates latch pulse for external syncronous latch
////////////////////////////////////////////////////////////////
BOOL LatchExt (USHORT Card)
{
  	DWORD  byteCount;
	
	if (Card>(MAX_CARDS-1))																	// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Card<<1])														// Check card address
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	
	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_LATCHE,			// I/O control code
							  &Card,			   			// Pointer to input buffer
							  (DWORD) sizeof(Card),			// Input buffer size
							  NULL,							// Pointer to output buffer
							  (DWORD) 0,					// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control!
	}

	if (byteCount!=1)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;			return FALSE;	// Error byte count wrong!
	}
	else return TRUE;
}


///////////////////////////////////
// WindowsNT
// SysLed: Sets state of system LED
///////////////////////////////////
BOOL SysLed (USHORT Card, USHORT Mode)
{
  	DWORD  byteCount;
	struct p { USHORT card, mode; } port;

	if (Card>(MAX_CARDS-1))														// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Card<<1])														// Check card address
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	

	port.card = Card; port.mode  = Mode;
	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_SYSLED,			// I/O control code
							  &port,			   			// Pointer to input buffer
							  (DWORD) sizeof(port),			// Input buffer size
							  NULL,							// Pointer to output buffer
							  (DWORD) 0,					// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control!
	}

	if (byteCount!=1)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;			return FALSE;	// Error byte count wrong!
	}
	else return TRUE;
}



/////////////////////////////////////////////////
// WindowsNT
// SetLatInt: Sets state of output Internal Latch
/////////////////////////////////////////////////
BOOL SetLatInt (USHORT Card, USHORT Mode)
{
  	DWORD  byteCount;
	struct p { USHORT card, mode; } port;

	if (Card>(MAX_CARDS-1))														// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Card<<1])														// Check card address
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	

	port.card = Card; port.mode  = Mode;
	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_SETI,			// I/O control code
							  &port,			   			// Pointer to input buffer
							  (DWORD) sizeof(port),			// Input buffer size
							  NULL,							// Pointer to output buffer
							  (DWORD) 0,					// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control!
	}

	if (byteCount!=1)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;			return FALSE;	// Error byte count wrong!
	}
	else return TRUE;
}



/////////////////////////////////////////////////
// WindowsNT
// SetLatExt: Sets state of output External Latch
/////////////////////////////////////////////////
BOOL SetLatExt (USHORT Card, USHORT Mode)
{
  	DWORD  byteCount;
	struct p { USHORT card, mode; } port;

	if (Card>(MAX_CARDS-1))														// Check card number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high!
	}
	if (!Exists[Card<<1])														// Check card address
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed!
	}
	

	port.card = Card; port.mode  = Mode;
	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_SETE,			// I/O control code
							  &port,			   			// Pointer to input buffer
							  (DWORD) sizeof(port),			// Input buffer size
							  NULL,							// Pointer to output buffer
							  (DWORD) 0,					// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control!
	}

	if (byteCount!=1)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;			return FALSE;	// Error byte count wrong!
	}
	else return TRUE;
}



///////////////////////////////////////////////////////////////////////
// WindowsNT
// IKFind: Returns port address(es) of installed card('s) in pBuffer256
///////////////////////////////////////////////////////////////////////
BOOL IKFind (ULONG* pBuffer256)
{
	DWORD  byteCount;

	if (!pBuffer256)																// Check buffer address
	{
	 	DLLStatus = DLLStatus | DLLStatus_BufferAddressInvalid;		return FALSE;	// Buffer address invalid!
	}


	if  (!DeviceIoControl	( m_hIK220Drv,					// Handle to device driver
							  (DWORD)IOCTL_STATUS,			// I/O control code
							  pBuffer256,					// Pointer to input buffer
							  (2*MAX_CARDS)*4,				// Input buffer size
							  pBuffer256,					// Pointer to output buffer
							  (2*MAX_CARDS)*4,				// Output buffer size
							  &byteCount,					// Bytes returned
							  NULL ))						// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;			return FALSE;	// Error device i/o control!
	}

	if (byteCount!=(2*MAX_CARDS)*4)
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;				return FALSE;	// Error byte count wrong!
	}
	else return TRUE;
}


#endif		// Ifdef _Windows_95




////////////////////////////////////////////////////////////////////////////
// OutCmd: Write command and wait for acknowledge of IK 220 with 2s time out
////////////////////////////////////////////////////////////////////////////
BOOL OutCmd (USHORT Axis, USHORT IK220Cmd)
{
USHORT	RegW;					// Register value word

	DLLInfo = DLLInfo & 0x00FFFFFF;
	DLLInfo=DLLInfo+0x01000000;															// -- 1 --
	if (!OutputW (Axis, CmdPort, IK220Cmd))				return FALSE;	// Write command to IK 220


	DLLInfo=DLLInfo+0x01000000;															// -- 2 --
	InitTimeOut (TimLCL);												// Init timer for time out
	do 
	{
		if (!InputW (Axis, Flag1Reg, &RegW))			return FALSE;	// Wait until finished
	}																	// or time out
	while ( !(RegW & G28Sem0) && !TimeOut (TimLCL, TIM_2s) );

	DLLInfo=DLLInfo+0x01000000;															// -- 3 --
	if (!InputW (Axis, Flag1Reg, &RegW))				return FALSE;	// Finished ?
	if ( !(RegW & G28Sem0) )
	{
		DLLStatus = DLLStatus | DLLStatus_TimeOut;		return FALSE;	// No, time out
	}

	DLLInfo=DLLInfo+0x01000000;															// -- 4 --
	if (!InputW (Axis, DatPort0, &RegW))				return FALSE;	// Verify acknowledge
	if (RegW!=IK220Cmd)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongAck;		return FALSE;	// Wrong command acknowledge
	}
	
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////
// OutCmd1: Write command and wait for acknowledge of IK 220 with user defined time out
///////////////////////////////////////////////////////////////////////////////////////
BOOL OutCmd1 (USHORT Axis, USHORT IK220Cmd)
{
USHORT	RegW;					// Register value word

	DLLInfo = DLLInfo & 0x00FFFFFF;
	DLLInfo=DLLInfo+0x01000000;											// -- 1 --
	if (!OutputW (Axis, CmdPort, IK220Cmd))				return FALSE;	// Write command to IK 220


	DLLInfo=DLLInfo+0x01000000;											// -- 2 --
	InitTimeOut (TimOutCmd1);											// Init timer for time out
	do 
	{
		if (!InputW (Axis, Flag1Reg, &RegW))			return FALSE;	// Wait until finished
	}																	// or time out
	while ( !(RegW & G28Sem0) && !TimeOut (TimOutCmd1, UserTimeOut1) );

	DLLInfo=DLLInfo+0x01000000;											// -- 3 --
	if (!InputW (Axis, Flag1Reg, &RegW))				return FALSE;	// Finished ?
	if ( !(RegW & G28Sem0) )
	{
		DLLStatus = DLLStatus | DLLStatus_TimeOut;		return FALSE;	// No, time out
	}

	DLLInfo=DLLInfo+0x01000000;											// -- 4 --
	if (!InputW (Axis, DatPort0, &RegW))				return FALSE;	// Verify acknowledge
	if (RegW!=IK220Cmd)
	{
		DLLStatus = DLLStatus | DLLStatus_WrongAck;		return FALSE;	// Wrong command acknowledge
	}
	
	return TRUE;
}






#ifdef _Windows95

////////////////////////////////////////////
// Windows95
// GetVers: Returns software version of card
////////////////////////////////////////////
BOOL GetVers (USHORT Axis, char* pVersCard, char* pVersDrv)
{

	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high
	}
	if (!PortBase[Axis])														// Check address of axis
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed
	}
	if (!pVersCard)																// Check buffer address
	{
	 	DLLStatus = DLLStatus | DLLStatus_BufferAddressInvalid;	return FALSE;	// Buffer address invalid
	}
	if (!pVersDrv)																// Check buffer address
	{
	 	DLLStatus = DLLStatus | DLLStatus_BufferAddressInvalid;	return FALSE;	// Buffer address invalid
	}


	*(PortBase[Axis]+CmdPort) = cmdGetVersion;												// Command:  to IK 220
	
	InitTimeOut (TimLCL);																	// Init timer for time out
	do {
	} while ( !(*(PortBase[Axis]+Flag1Reg) & G28Sem0) && !TimeOut (TimLCL, TIM_1ms) );		// Wait until finished

	if ( !(*(PortBase[Axis]+Flag1Reg) & G28Sem0) )											// Finished ?
	{
		DLLStatus = DLLStatus | DLLStatus_TimeOut;				return FALSE;				// No, time out
	}

	if ( *(PortBase[Axis]+DatPort0)==cmdGetVersion )										// Verify command
	{
		*pVersCard++ = *(PortBase[Axis]+DatPort1) >> 8;										// Command successful
		*pVersCard++ = *(PortBase[Axis]+DatPort1) &  0x00FF;

		*pVersCard++ = *(PortBase[Axis]+DatPort2) >> 8;
		*pVersCard++ = *(PortBase[Axis]+DatPort2) &  0x00FF;
	
		*pVersCard++ = *(PortBase[Axis]+DatPort3) >> 8;
		*pVersCard++ = *(PortBase[Axis]+DatPort3) &  0x00FF;
	 
		*pVersCard++ = *(PortBase[Axis]+DatPort4) >> 8;
		*pVersCard++ = *(PortBase[Axis]+DatPort4) &  0x00FF;
	 
		*pVersCard++ = *(PortBase[Axis]+DatPort5) >> 8;
		*pVersCard++ = *(PortBase[Axis]+DatPort5) &  0x00FF;
	 
		*pVersCard++ = *(PortBase[Axis]+DatPort6) >> 8;
		*pVersCard++ = *(PortBase[Axis]+DatPort6) &  0x00FF;
	 
		*pVersCard++ = 0;
	

		*pVersDrv++  = '-';
		*pVersDrv++  = '-';
		*pVersDrv++  = '-';
		*pVersDrv++  = 0;

		return TRUE;
	}
	else
		return FALSE;																	// Command error
}


#else


///////////////////////////////////////////////////////
// WindowsNT
// GetVers: Returns software version of card and driver
///////////////////////////////////////////////////////
BOOL GetVers (USHORT Axis, char* pVersCard, char* pVersDrv)
{
	DWORD  byteCount;
	USHORT i;
	char Buffer[80];

	if  (!DeviceIoControl	( m_hIK220Drv,				// Handle to device driver
							  (DWORD)IOCTL_VERSION,		// I/O control code
							  &Axis,					// Pointer to input buffer
							  (DWORD)sizeof(Axis),		// Input buffer size
							  &Buffer,					// Pointer to output buffer
							  (DWORD) sizeof(Buffer),	// Output buffer size
							  &byteCount,				// Bytes returned
							  NULL ))					// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control!
	}



	if (byteCount)
	{
		i=0;
		do {
			*pVersCard++=Buffer[i];
		} while (Buffer[i++]!=0);


		do {
			*pVersDrv++=Buffer[i];
		} while (Buffer[i++]!=0);

		return TRUE;
	}
	else
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;				return FALSE;	// Error byte count wrong!
	}
}

#endif


	

////////////////////////////////////////////////////////////////
// ReadRam: Returns word read from RAM of Axis at Adr to pBuffer
////////////////////////////////////////////////////////////////
BOOL RamRead (USHORT Axis, USHORT Adr, USHORT* pData)
{
USHORT	RegW;					// Register value word

	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high
	}
	if ( !Exists[Axis] )
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed
	}

	if (!OutputW (Axis, ControlReg, BootMode))					return FALSE;	// Set CPU to boot mode

	InitTimeOut (TimLCL);													 	// Init timer for time out
	do 
	{
		if (!InputW (Axis, StatusReg, &RegW))					return FALSE;	// Pipe empty ?
	} while ( !(RegW & PipeEmpty) && !TimeOut (TimLCL, TIM_1ms) );			 	// or time out
    

	if ( !InputW (Axis, StatusReg, &RegW) )						return FALSE;	// Pipe empty ?
	if ( !(RegW & PipeEmpty) )
	{
		DLLStatus = DLLStatus | DLLStatus_TimeOut;				return FALSE; 	// No, time out
	}



	if (!OutputW (Axis, G28Port0,   Adr))						return FALSE;		// Write address
	if (!OutputW (Axis, ControlReg, ReadRamMode))				return FALSE;		// Read from address
	
	InitTimeOut (TimLCL);															// Init timer for time out
	do 
	{
		if (!InputW (Axis, Flag1Reg, &RegW))					return FALSE;		// Register 1 written
	} while ( !(RegW & G28Sem1) && !TimeOut (TimLCL, TIM_1ms) );					// or time out

	if (!InputW (Axis, Flag1Reg, &RegW))						return FALSE;		// Read flag register 1
	if ( !(RegW & G28Sem1) )
	{
		DLLStatus = DLLStatus | DLLStatus_TimeOut;				return FALSE;		// No, time out
	}


	if (!InputW (Axis, G28Port1, pData))						return FALSE;		// Read data

	return TRUE;
}


///////////////////////////////////////////////////
// RamWrite: Writes word Data to RAM in Axis at Adr
///////////////////////////////////////////////////
BOOL RamWrite (USHORT Axis, USHORT Adr, USHORT Data)
{
USHORT	RegW;					// Register value word

	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high
	}
	if ( !Exists[Axis] )
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed
	}

	if (!OutputW (Axis, ControlReg, BootMode))					return FALSE;	// Set CPU to boot mode

	InitTimeOut (TimLCL);														// Init timer for time out
	do 
	{
		if (!InputW (Axis, StatusReg, &RegW))					return FALSE;	// Pipe emtpy ?
	} while ( !(RegW & PipeEmpty) && !TimeOut (TimLCL, TIM_1ms) );				// or time out
    
	if ( !InputW (Axis, StatusReg, &RegW) )						return FALSE;	// Pipe emtpy ?
	if ( !(RegW & PipeEmpty) )
	{
		DLLStatus = DLLStatus | DLLStatus_TimeOut;				return FALSE;	// No, time out
	}


	if (!OutputW (Axis, G28Port0,   Adr))						return FALSE;	// Write address
    if (!OutputW (Axis, G28Port1,   Data))						return FALSE;	// Write data
	if (!OutputW (Axis, ControlReg, WriteRamMode))				return FALSE;	// Write data to address

	return TRUE;
}



#ifdef _Windows95

//////////////////////////////////////////////////////////
// Windows95
// DownLoad: Loads firmware from pPgmData into RAM of Axis
//////////////////////////////////////////////////////////
BOOL DownLoad (USHORT Axis, USHORT* pPgmData, ULONG PgmSize)
{
	ULONG  i;
	USHORT RamAdr;	

	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high
	}
	if ( !Exists[Axis] )
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed
	}
	if ( (PgmSize<(3*2)) | (PgmSize>(65536+2)*2) )
	{
	DLLStatus = DLLStatus | DLLStatus_OutOfRange;				return FALSE;	// Address out of range!
	}

	pPgmData++;
	RamAdr=*pPgmData++;

	for (i=1; i<=(PgmSize/2)-2; i++)
		if (!(RamWrite (Axis, RamAdr++, *pPgmData++)))			return FALSE;

	return TRUE;
}


#else


//////////////////////////////////////////////////////////
// WindowsNT
// DownLoad: Loads firmware from pPgmData into RAM of Axis
//////////////////////////////////////////////////////////
BOOL DownLoad (USHORT Axis, USHORT* pPgmData, ULONG PgmSize)
{
	DWORD  byteCount;

	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high
	}
	if ( !Exists[Axis] )
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed
	}
	if ( (PgmSize<(3*2)) | (PgmSize>(65536+2)*2) )
	{
	DLLStatus = DLLStatus | DLLStatus_OutOfRange;				return FALSE;	// Address out of range!
	}


	*pPgmData = Axis;
	if  (!DeviceIoControl	( m_hIK220Drv,				// Handle to device driver
							  (DWORD)IOCTL_DOWNLOAD,	// I/O control code
							  pPgmData,					// Pointer to input buffer
							  (DWORD)PgmSize,			// Input buffer size
							  pPgmData,					// Pointer to output buffer
							  (DWORD)PgmSize,			// Output buffer size
							  &byteCount,				// Bytes returned
							  NULL ))					// Not overlapped
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrDevIoControl;		return FALSE;	// Error device i/o control!
	}
	
	if ( byteCount!=(PgmSize-4) )
	{
	 	DLLStatus = DLLStatus | DLLStatus_ErrByteCount;			return FALSE;	// Error byte count wrong!
	}
	else
		return TRUE;
}

#endif


//////////////////////////////////////////////////////
// WritePar: Writes ParVal to parameter ParNum of Axis
//////////////////////////////////////////////////////
BOOL WritePar (USHORT Axis, USHORT ParNum, ULONG ParVal)
{
USHORT	RegW;					// Register value word

	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high
	}
	if (!Exists[Axis])
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed
	}

	if (!OutputW (Axis, DatPort1, ParNum))						return FALSE;
	if (!OutputW (Axis, DatPort2, (USHORT)(ParVal & 0xFFFF)))	return FALSE;
	if (!OutputW (Axis, DatPort3, (USHORT)(ParVal>>16)))		return FALSE;
	if (!OutCmd  (Axis, cmdWritePar))							return FALSE;

	if (!InputW  (Axis, DatPort2, &RegW))						return FALSE;
	if (RegW!=0)												return FALSE;

	return TRUE;
}



////////////////////////////////////////////////////
// ReadPar: Reads parameter ParNum of Axis to ParVal
////////////////////////////////////////////////////
BOOL ReadPar (USHORT Axis, USHORT ParNum, ULONG* pParVal)
{
USHORT	RegW;					// Register value word

	*pParVal = 0;
	
	if (Axis>MAX_AXIS)																// Check axis number
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNumberHigh;		return FALSE;	// Axis number too high
	}
	if (!Exists[Axis])
	{
	 	DLLStatus = DLLStatus | DLLStatus_AxisNotInstalled;		return FALSE;	// Axis not installed
	}


	if (!OutputW (Axis, DatPort1, ParNum))						return FALSE;
	if (!OutCmd  (Axis, cmdReadPar))							return FALSE;

	if (!InputW  (Axis, DatPort2, &RegW))						return FALSE;
	if (RegW!=0)												return FALSE;

	if (!InputL  (Axis, DatPort3, pParVal))						return FALSE;

	return  TRUE;
}



//////////////////////////////////////////////////////////////////////////////
// FindEnRange: Converts linear MemAdr into segmented MrsCode:MemAdr for EnDat
//////////////////////////////////////////////////////////////////////////////
BOOL FindEnRange (USHORT Axis, USHORT* pMemAdr, USHORT* pMrsCodeEn)
{
	USHORT i, StartAdr, EndAdr;
	USHORT LinAdr[16] = {0,31,63,64,127,255,-1,-1,-1,-1,-1,-1,-1,-1,0xFFFF};
	BYTE*  pAdr = NULL;

	if (*pMrsCodeEn==0xA9)				// Parameter OEM
	{
		pAdr=(BYTE*)&OEM_Adr[Axis][0];
	}

	if (*pMrsCodeEn==0xB1)				// Correction values
	{
		pAdr=(BYTE*)&CORR_Adr[Axis][0];
	}

	if (pAdr==NULL)
	{
	 	DLLStatus = DLLStatus | DLLStatus_BufferAddressInvalid;		return FALSE;	// Buffer address invalid!
	}

	

	StartAdr = 0;
	EndAdr	 = -1;

	for (i=0; i<4; i++)
	{
		if (*pAdr!=0xFF)
		{
			StartAdr = EndAdr+1;
			EndAdr   = StartAdr+(LinAdr[*pAdr>>4 & 0x0F] - LinAdr[*pAdr & 0x0F]);

			if ( (StartAdr<=*pMemAdr) && (EndAdr>=*pMemAdr) )			// Range found ?
			{
				*pMrsCodeEn = *pMrsCodeEn + (i<<1);							// Yes, modify MrsCodeEn
				*pMemAdr    = *pMemAdr-StartAdr+LinAdr[*pAdr & 0x0F];		//		and MrsAdr
				return TRUE;
			}
		}
		pAdr++;
	}

	DLLStatus = DLLStatus | DLLStatus_OutOfRange;	return FALSE;	// Address out of range!
}