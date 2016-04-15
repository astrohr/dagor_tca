/*++

	Copyright (c) 1997,  Dr. Johannes Heidenhain GmbH

	Module Name:	IK220Pci.c
	
	Version 0.4:	23.03.1999

	Abstract:		Windows 95 functions for IK 220

	Notes:			This DLL provides IK 220 card functions for IK 220.
					IK220 hardware is accessed through IK220Drv.Sys or IK220VxD.Vxd.

--*/

#ifdef _Windows95


#include "IK220Pci.h"
#include "..\..\Disk2\include\IK220.h"


#define VXD_FILENAME	"\\\\.\\IK220VxD.VXD"
#define DevVendID		0x905010B5					// Device and Vendor ID of IK 220
#define	SDevVendIK220	0x117210B5					// Sub-Vendor and Sub-System ID for Dr. Heidenhain GmbH IK220


#define BUFLEN			256

extern USHORT* PortBase [2*MAX_CARDS];
extern ULONG*  ConfReg[MAX_CARDS];

static USHORT  PortPages[2*MAX_CARDS];


static HANDLE hdevice = INVALID_HANDLE_VALUE;
int Card = 0;



/////////////////////////////////////////////////////////////////
// Read PCI configuration register (DWORD) from IK 220 PCI bridge
/////////////////////////////////////////////////////////////////
ULONG pciReadConfigDword (PCI_SITE site, UINT reg)
{  
	CONFIGREG cr = {site.devnode, reg, sizeof(DWORD), PCI_ENUM_FUNC_GET_DEVICE_INFO};

	DeviceIoControl (hdevice,
					 IK220_PCICONFIGREG,
					 &cr,
					 sizeof(CONFIGREG),
					 &cr,
					 sizeof(CONFIGREG),
					 NULL,
					 NULL);
	return (DWORD)cr.val;
}

///////////////////////////////////////////////
// Maps physical memory to linear address space
///////////////////////////////////////////////	
USHORT* MapPhysToLin (ULONG physaddr, ULONG count)
{	
	PHYSLIN pl;

	if (!count) return NULL;

	pl.phys = physaddr;
	pl.bcnt = count;

	if (!DeviceIoControl (hdevice,
						  IK220_MAPPHYSTOLIN,
						  &pl,
						  sizeof(PHYSLIN),
						  NULL,
						  0,
						  NULL,
						  NULL))
	{	// Uh-oh, failure in OS/VxD level
		return NULL;
	}
	return (USHORT*)pl.lin;

}


//////////////////////////////////////////
// Unmaps IK 220 from linear address space
//////////////////////////////////////////
BOOL UnMapIK220 ()
{	
	UINT Card;
	PHYSLIN pl;
	
	for (Card=0; Card<2*MAX_CARDS; Card++)
		if (PortBase[Card])
		{
			pl.lin = PortBase[Card];
		    pl.pcnt= 32;
			if (!DeviceIoControl (hdevice,
								  IK220_UNMAP,
								  &pl,
								  sizeof(PHYSLIN),
								  NULL,
								  0,
								  NULL,
								  NULL)) return FALSE;
		}

	for (Card=0; Card<MAX_CARDS; Card++)
		if (ConfReg[Card])
		{
			pl.lin = ConfReg[Card];
		    pl.pcnt= 54;
			if (!DeviceIoControl (hdevice,
								  IK220_UNMAP,
								  &pl,
								  sizeof(PHYSLIN),
								  NULL,
								  0,
								  NULL,
								  NULL)) return FALSE;
		}


	return TRUE;
}




// Search for IK 220 cards in registry
/////////////////////////////////////////////////////////////////////////////
BOOL SearchForIK220(void)
{
	PCI_SITE site;
	DWORD i;
	HKEY hEnum;
	HKEY hDevnode;
	char dnstr[BUFLEN];
	DWORD dnlen;
	char hwkstr[BUFLEN];
	DWORD hwklen;

	if (RegOpenKeyEx (HKEY_DYN_DATA, "Config Manager\\Enum", 0, KEY_READ, &hEnum) != ERROR_SUCCESS) return FALSE;

	// Each key under "Config Manager\Enum" represents a devnode in the system
	// Open each devnode key and check for "PCI\" in the HardWareKey value
	for (i=0; ; i++)
	{
		dnlen = BUFLEN;
		if (RegEnumKeyEx (hEnum, i, dnstr, &dnlen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) break;

		// Open the devnode subkey
		if (RegOpenKeyEx (hEnum, dnstr, 0, KEY_READ, &hDevnode) != ERROR_SUCCESS) continue;

		// Read the HardWareKey for this devnode
		hwklen = BUFLEN;
		if (RegQueryValueEx (hDevnode, "HardWareKey", NULL, NULL, (unsigned char *)hwkstr, &hwklen) != ERROR_SUCCESS)
		{
			hwkstr[0] = '\0';
		}

		// HardWareKey should look like the following three-part device ID string:
		// PCI\VEN_10B5&DEV_9050\BUS_00&DEV_0B&FUNC_00
		// 0         1         2         3         4
		// 0123456789012345678901234567890123456789012
		if (strncmp (hwkstr, "PCI\\", 4) != 0) continue;	// Not a PCI device

		// For compatablity with DOS version, discard non-FUNC_00 devices
		// Edi: Removed 990730 because of problems finding card in PC with PCI-to-PCI-Bridge!!!!!!!
//		if (strncmp (hwkstr+41, "00", 2) != 0) continue;	// Not function 0

		site.devnode = strtoul(dnstr,     NULL, 16);
		site.bus	 = strtoul(hwkstr+26, NULL, 16);
		site.dev	 = strtoul(hwkstr+33, NULL, 16);

		if (pciReadConfigDword(site, 0x00) == DevVendID)
			if (pciReadConfigDword(site, 0x2C) == SDevVendIK220)
				if (Card<2*MAX_CARDS)
				{
					ConfReg[Card>>1] = (ULONG*)MapPhysToLin (pciReadConfigDword(site, 0x10), 54);

					PortBase[Card]   = MapPhysToLin (pciReadConfigDword(site, 0x18), 32);
					Card++;
					PortBase[Card]   = MapPhysToLin (pciReadConfigDword(site, 0x1C), 32);
					Card++;

				}
	}
	return TRUE;
}




BOOL InitPci (void)
{
	int i;

	for (i=0; i<2*MAX_CARDS; i++)
	{
		PortBase[i]=0;
		ConfReg [i]=0;
	}

	// Open IK 220 VxD
	hdevice = CreateFile (VXD_FILENAME,
						  0,
						  0,
						  NULL,
						  0, 
						  FILE_FLAG_DELETE_ON_CLOSE,
						  NULL);
	if (hdevice == INVALID_HANDLE_VALUE) return FALSE;

	return SearchForIK220();
}

#endif
