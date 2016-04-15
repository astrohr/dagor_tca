//
//	Definitions for G38
//
//	Version 0.4:	23.03.1999
//
//	ES,  Dr. Johannes Heidenhain GmbH
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////




#ifndef G38_H
#define G38_H



// register defintion for G38
/////////////////////////////
#define G28Port0		(USHORT) 0		// Address of port  0
#define G28Port1		(USHORT) 1		// Address of port  1
#define G28Port2		(USHORT) 2		// Address of port  2
#define G28Port3		(USHORT) 3		// Address of port  3
#define G28Port4		(USHORT) 4		// Address of port  4
#define G28Port5		(USHORT) 5		// Address of port  5
#define G28Port6		(USHORT) 6		// Address of port  6
#define G28Port7		(USHORT) 7		// Address of port  7
#define G28Port8		(USHORT) 8		// Address of port  8
#define G28Port9		(USHORT) 9		// Address of port  9
#define G28Port10		(USHORT)10		// Address of port 10

// write register
#define SetFlag0Reg		(USHORT)11		// Address of set flag 0 register
#define ControlReg		(USHORT)12		// Address of control register

// read register
#define ClrFlag1Reg		(USHORT)11		// Address of clear flag 1 register
#define StatusReg		(USHORT)12		// Address of status register
#define Flag0Reg		(USHORT)13		// Address of flag 0 register
#define Flag1Reg		(USHORT)14		// Address of flag 1 register
#define CodeReg			(USHORT)15		// Address of code register


//  command definition for G28
#define	WaitStates		(USHORT)0x0000 					// no wait states

#define RunMode			(USHORT)(0x0000 | WaitStates)	// CPU run         mode
#define BootMode		(USHORT)(0x0001 | WaitStates)	// CPU boot        mode
#define SingleMode		(USHORT)(0x0002 | WaitStates)	// CPU single step mode
#define DebugMode		(USHORT)(0x0003 | WaitStates)	// CPU debug	   mode

#define ReadRamMode		(USHORT)(0x0004 | BootMode)		// CPU boot        mode and read  ram
#define WriteRamMode	(USHORT)(0x0008 | BootMode)		// CPU boot        mode and write ram

#define DoStepMode		(USHORT)(0x0004 | SingleMode)	// CPU single step mode and step

#define ReadRegs		(USHORT)(0x0004 | DebugMode)	// CPU debug	   mode and read  register
#define WriteRegs		(USHORT)(0x0008 | DebugMode)	// CPU debug	   mode and write register


#define CPUHalt			(USHORT)0x0001					// CPU halted
#define PipeEmpty		(USHORT)0x0002					// CPU pipeline empty


#define G28Sem0			(USHORT)0x0001				// Semaphore  0 from G28
#define G28Sem1			(USHORT)0x0002				// Semaphore  1 from G28
#define G28Sem2			(USHORT)0x0004				// Semaphore  2 from G28
#define G28Sem3			(USHORT)0x0008				// Semaphore  3 from G28
#define G28Sem4			(USHORT)0x0010				// Semaphore  4 from G28
#define G28Sem5			(USHORT)0x0020				// Semaphore  5 from G28
#define G28Sem6			(USHORT)0x0040				// Semaphore  6 from G28
#define G28Sem7			(USHORT)0x0080				// Semaphore  7 from G28
#define G28Sem8			(USHORT)0x0100				// Semaphore  8 from G28
#define G28Sem9			(USHORT)0x0200				// Semaphore  9 from G28
#define G28Sem10		(USHORT)0x0400				// Semaphore 10 from G28




#define CONFREG_CNTRL	(0x50>>2)						// PLX 9052 PCI Configuration Register 0x50 (CNTRL)

#endif