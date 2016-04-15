#define WANTVXDWRAPS
#include "basedef.h"
#include "vmm.h"
#include "debug.h"
#include "vwin32.h"
#include "vxdwraps.h"
#include "winerror.h"
#include "configmg.h"

#include "IK220Vxd.h"

/////////////////////////////////////////////////////////////////////////////
#define PAGEBITS 12					// Bits in a page
#define PAGEBYTES 4096				// Bytes in a page
#define Page(pa) ((pa)>>PAGEBITS)	// Page of an address

/////////////////////////////////////////////////////////////////////////////
// VxD wrappers needed, but not included in VXDWRAPS.H...

#pragma warning(disable:4035)

/////////////////////////////////////////////////////////////////////////////
// ULONG EXTERNAL _PageReserve(ULONG page, ULONG npages, ULONG flags);
ULONG VXDINLINE __declspec(naked)
_PageReserve(ULONG page, ULONG npages, ULONG flags)
{
	VMMJmp(_PageReserve);
}

/////////////////////////////////////////////////////////////////////////////
// ULONG EXTERNAL _PageCommitPhys(ULONG page, ULONG npages, ULONG physpg, ULONG flags);
ULONG VXDINLINE __declspec(naked)
_PageCommitPhys(ULONG page, ULONG npages, ULONG physpg, ULONG flags)
{
	VMMJmp(_PageCommitPhys);
}

/////////////////////////////////////////////////////////////////////////////
// VMMcall _CopyPageTable, <LinPgNum, nPages, <OFFSET32 PageBuf>, flags>
// mov     [Copied], eax           ; nonzero if copied, zero otherwise
ULONG VXDINLINE __declspec(naked)
_CopyPageTable (ULONG LinPgNum, ULONG nPages, ULONG * PageBuf, ULONG flags)
{
	VMMJmp(_CopyPageTable);
}
#pragma warning(default:4035)


////////////////////////////////////////////////////////////////////////////////////////
// Map continuous physical range (address and byte count) to a linear address
// Not checking for zero count -- allocates one page minimum
// Should be doing more error checking _PageReserve(), _PageCommitPhys(), _LinPageLock()!
////////////////////////////////////////////////////////////////////////////////////////
DWORD IK220_MapPhysToLin (PDIOCPARAMETERS p)
{
	DWORD linaddr;
	PHYSLIN * pl;
	
	if (!(pl = (PHYSLIN *)p->lpvInBuffer) || (p->cbInBuffer < sizeof(PHYSLIN)))
		return ERROR_INVALID_PARAMETER;
	// Record page count (for use by IK220_Unmap())
	// 1 plus page crossings (Not the same as 1 + Page(pl->bcnt))
	pl->pcnt = 1 + (Page(pl->phys + pl->bcnt) - Page(pl->phys));

	// Reserve and commit page(s)
	linaddr = _PageReserve(PR_SYSTEM, pl->pcnt, PR_FIXED);
	_PageCommitPhys (Page(linaddr), pl->pcnt, Page(pl->phys), PC_INCR | PC_WRITEABLE | PC_USER);
	_LinPageLock    (Page(linaddr), pl->pcnt, 0);

	// Record linear address
	(DWORD)pl->lin = linaddr | (pl->phys & (PAGEBYTES - 1));
	// Debug_Printf ("%8.8x (%8.8x): %8.8x\n", pl->phys, pl->lin, *((DWORD*)pl->lin));

	pl->error = IK220_ERR_SUCCESS;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
DWORD IK220_Unmap(PDIOCPARAMETERS p)
{	// Unlock and free pages allocated by IK220_MapPhysToLin()
	PHYSLIN * pl;
	
	if (!(pl = (PHYSLIN *)p->lpvInBuffer) || (p->cbInBuffer < sizeof(PHYSLIN)))
	{
		Debug_Printf ("IK220_Unmap p->lpvInBuffer:%8.8x p->cbInBuffer:%8.8x\n", p->lpvInBuffer, p->cbInBuffer);
		return ERROR_INVALID_PARAMETER;
	}

	if (!_LinPageUnlock(Page((DWORD)pl->lin), pl->pcnt, 0))
	{	// Failed
		Debug_Printf ("_LinPageUnlock failed\n");
		pl->error = IK220_ERR_UNMAPERR;
		return ERROR_INVALID_PARAMETER;
	}

	if (!_PageFree((VOID *)((DWORD)pl->lin & ~(PAGEBYTES - 1)), 0))
	{	// Failed
		Debug_Printf ("_PageFree failed\n");
		pl->error = IK220_ERR_UNMAPERR;
		return ERROR_INVALID_PARAMETER;
	}
	
	// Success!
	pl->error = IK220_ERR_SUCCESS;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
/*DWORD IK220_LinToPhys(PDIOCPARAMETERS p)
{	// Convert physical to linear address
	ULONG physpage;
	PHYSLIN * pl;
	
	if (!(pl = (PHYSLIN *)p->lpvInBuffer) || (p->cbInBuffer < sizeof(PHYSLIN)))
	{
		Debug_Printf ("IK220_LinToPhys p->lpvInBuffer:%8.8x p->cbInBuffer:%8.8x\n", p->lpvInBuffer, p->cbInBuffer);
		return ERROR_INVALID_PARAMETER;
	}

	if (!(_CopyPageTable(Page((DWORD)pl->lin), 1, &physpage, 0)))
	{
		Debug_Printf ("_CopyPageTable failed\n");
		pl->error = IK220_ERR_PAGETABLE;
		return ERROR_INVALID_PARAMETER;
	}
	// Success! Return the physical address
	pl->phys = (physpage & ~(PAGEBYTES - 1)) | ((DWORD)pl->lin & (PAGEBYTES - 1));
	pl->error = IK220_ERR_SUCCESS;
	return 0; 
}
*/

/////////////////////////////////////////////////////////////////////////////
DWORD IK220_PciConfigReg(PDIOCPARAMETERS p)
{	// Read/write PCI configuration register
	CONFIGREG * cr = (CONFIGREG *)p->lpvInBuffer;

	if (CM_Call_Enumerator_Function(
		cr->devnode,	// Handle of a device's DevNode
		cr->func,		// Function number that is one of the Pci.h #defines
		cr->reg,		// Offset within PCI config regs
		&cr->val,		// Pointer to a buffer to read or write PCI config data.
		cr->len,		// Number of bytes in PCI config space to read or write.
		0l				// Must be zero.
		) == CR_SUCCESS)
	{
		return 0;
	}
	return ERROR_INVALID_PARAMETER;
}



/////////////////////////////////////////////////////////////////////////////
BOOL OnSysDynamicDeviceInit()
{
	Debug_Printf (__FILE__" OnSysDynamicDeviceInit() VXD Loaded!\n");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
BOOL OnSysDynamicDeviceExit()
{
	Debug_Printf (__FILE__" OnSysDynamicDeviceExit() VXD Unloaded!\n");
	// (Need to destroy locked pages!!!)
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
DWORD OnDeviceIoControl (PDIOCPARAMETERS p)
{
	switch (p->dwIoControlCode)
	{
		case DIOC_OPEN:
			break;
		case DIOC_CLOSEHANDLE:
			return VXD_SUCCESS;
		case IK220_GETVERSION:
			// Return VxD's version
			if (!p->lpvOutBuffer || p->cbOutBuffer < sizeof(WORD))
				return ERROR_INVALID_PARAMETER;
			*(WORD*)p->lpvOutBuffer = (VER_MAJOR << 8) | VER_MINOR; // Major/Minor
			if (p->lpcbBytesReturned)
				*(PDWORD)p->lpcbBytesReturned = 2;
			break;
		case IK220_MAPPHYSTOLIN:
			return IK220_MapPhysToLin(p);
  
/*		case IK220_LINTOPHYS:
			return IK220_LinToPhys(p);
*/
		case IK220_UNMAP:
			return IK220_Unmap(p);
		case IK220_PCICONFIGREG:
			return IK220_PciConfigReg(p);
	}
	return 0;
}
