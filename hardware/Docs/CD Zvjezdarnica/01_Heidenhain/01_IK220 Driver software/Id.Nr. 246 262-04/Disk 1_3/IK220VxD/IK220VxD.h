#ifndef IK220VXD_H
#define IK220VXD_H

#include <pci.h>

#define VER_MAJOR 0x00
#define VER_MINOR 0x01
#define VXD_FILENAME "\\\\.\\IK220VxD.VXD"

///////////////////////////////////////////////////////////////////////////
// Control Codes -- used by Win32 app that call IK220 VxD's DeviceIoControl
enum IK220code {
 								// Application's calling syntax:
	IK220_GETVERSION = 0x8000,	// (hDev, PLX_xxxx, NULL, 0, &Version, sizeof(Version, &numret, NULL)
	IK220_MAPPHYSTOLIN,			// (hDev, PLX_xxxx, &PhysAddr, Size, &LinAddr, sizeof(LinAddr), NULL, NULL)
	IK220_UNMAP,				// (hDev, PLX_xxxx, &LinAddr, Size, &Bool, sizeof(Bool), NULL, NULL)
	IK220_LINTOPHYS,			// (hDev, PLX_xxxx, &LinAddr, Size, &PtrToPhys, sizeof(PtrToPhys), NULL, NULL)
	IK220_PCICONFIGREG			// (hDev, PLX_xxxx, &PciConfigReg, sizeof(PciConfigReg), NULL, 0, NULL, NULL)
};

enum IK220err {
	IK220_ERR_SUCCESS = 0,		// (Success -- MUST be zero)
	IK220_ERR_UNKNOWN,			// Hmm - this should never occur -- it's an error's error
	IK220_ERR_BADPARM,			// 
	IK220_ERR_MAPLISTFULL,		// Application (R3) code: out of room for phys/lin mappings
	IK220_ERR_MAPNOTFOUND,		// Application (R3) code: could not find phys/lin in list
	IK220_ERR_UNMAPERR,			// Problem in IK220_Unmap(): unable to unmap 
	IK220_ERR_PAGETABLE			// Error reading page tables
};

/////////////////////////////////////////////////////////////////////////////
// Read and Write PCI configuration registers
typedef struct configreg
{
	DWORD devnode;
	UINT  reg;
	UINT  len;		// sizeof(BYTE), sizeof(WORD), sizeof(DWORD), 
	UINT  func;		// Read or write configuration register function
	DWORD val;
} CONFIGREG;

typedef struct physlin
{	// Physical/linear mappings
	VOID* lin;
	DWORD phys;
	DWORD bcnt;			// Byte count
	DWORD pcnt;			// Page count
	BOOL  active;		// This mapping is "in use"
	UINT  error;		// Error -- set by VxD
} PHYSLIN;

#endif // IK220VXD_H

