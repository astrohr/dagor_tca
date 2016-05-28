

#ifndef _TIMEOUT_H_
#define _TIMEOUT_H_


#define maxTimNum		6

#define TimDLL			0			// Timer for DLL functions
#define TimLCL			1			// Timer for local functions
#define TimDLY			2			// Timer for delay
#define TimOutCmd1		3			// Timer for OutCmd1
#define TimWaitLatch	4			// Timer for IK220WaitLatch
#define TimWaitRef		5			// Timer for IK220WaitRef


BOOL InitTimeOut	(USHORT TimNum);
BOOL TimeOut		(USHORT TimNum, __int64 PerfTimeout);



#define DLLStatus_ErrorIK					0x00000001				// Bit  0 = IK reports error
#define DLLStatus_TimeOut					0x00000002				// Bit  1 = Time out error in DLL function
#define DLLStatus_WrongAck					0x00000004				// Bit  2 = Wrong command acknowledge


#define DLLStatus_OutOfRange				0x00100000				// Bit 20 = Out of range
#define DLLStatus_RefActive					0x00200000				// Bit 21 = REF is already active
#define DLLStatus_TimNumHigh				0x00400000				// Bit 22 = Timer number too high
#define DLLStatus_ErrDevIoControl			0x00800000				// Bit 23 = Error device i/o control

#define DLLStatus_ErrByteCount				0x01000000				// Bit 24 = Error wrong byte count
#define DLLStatus_WrongMode					0x02000000				// Bit 25 = Wrong mode
#define DLLStatus_EnDatAlarm				0x04000000				// Bit 26 = Alarm from EnDat encoder
#define DLLStatus_AxisNumberHigh			0x08000000				// Bit 27 = Axis number too high

#define DLLStatus_AxisNotInstalled			0x10000000				// Bit 28 = Axis not installed
#define DLLStatus_AddressHigh				0x20000000				// Bit 29 = Adress to high
#define DLLStatus_LatchNumberHigh			0x40000000				// Bit 30 = Latch number high
#define DLLStatus_BufferAddressInvalid		0x80000000				// Bit 31 = Buffer address invalid




#define DLLInfo_NoHighSpeedTimer 			0x00000001				// Bit  0 = High speed timer not available
#define DLLInfo_NoDelayTimer				0x00000002				// Bit  1 = High speed timer not used for delay
#define DLLInfo_NoIK220						0x00010000				// Bit 16 = IK220 not present


#endif