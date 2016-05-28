
#ifndef _IK220PCI_H_
#define _IK220PCI_H_


#include "windows.h"
#include "pci.h"

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

typedef struct pci_site
{	// Site (location) of a PCI device
	UINT bus;
	UINT dev;		// (Device in upper five bits, function in lower three bits)
	DWORD devnode;	// WIN32 extension
} PCI_SITE;

typedef struct physlin
{	// Physical/linear mappings
	VOID * lin;
	DWORD phys;
	DWORD bcnt;			// Byte count
	DWORD pcnt;			// Page count
	BOOL active;		// This mapping is "in use"
	UINT error;			// Error -- set by our VxD
} PHYSLIN;


/////////////////////////////////////////////////////////////////////////////
// Control Codes -- used by Win32 app that call IK 220 VxD's DeviceIoControl
enum plxcode {
 //								// Application's calling syntax:
 // PLX_xxxx					// (hDev, PLX_xxxx, &ibuf, cbibuf, &obuf, cbobuf, &numret, &ovl)
	IK220_GETVERSION = 0x8000,	// (hDev, PLX_xxxx, NULL, 0, &Version, sizeof(Version, &numret, NULL)
	IK220_MAPPHYSTOLIN,			// (hDev, PLX_xxxx, &PhysAddr, Size, &LinAddr, sizeof(LinAddr), NULL, NULL)
	IK220_UNMAP,				// (hDev, PLX_xxxx, &LinAddr, Size, &Bool, sizeof(Bool), NULL, NULL)
	IK220_LINTOPHYS,			// (hDev, PLX_xxxx, &LinAddr, Size, &PtrToPhys, sizeof(PtrToPhys), NULL, NULL)
	IK220_PCICONFIGREG			// (hDev, PLX_xxxx, &PciConfigReg, sizeof(PciConfigReg), NULL, 0, NULL, NULL)
};


enum plxerr {
	IK220_ERR_SUCCESS = 0,		// (Success -- MUST be zero)
	IK220_ERR_UNKNOWN,			// Hmm - this should never occur -- it's an error's error
	IK220_ERR_BADPARM,			// 
	IK220_ERR_MAPLISTFULL,		// Application (R3) code: out of room for phys/lin mappings
	IK220_ERR_MAPNOTFOUND,		// Application (R3) code: could not find phys/lin in list
	IK220_ERR_UNMAPERR,			// Problem in PLX_Unmap(): unable to unmap 
	IK220_ERR_PAGETABLE			// Error reading page tables
};


BOOL InitPci ();
BOOL UnMapIK220 ();

#endif
