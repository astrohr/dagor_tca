/*


	Copyright (c) 1999,  Dr. Johannes Heidenhain GmbH

	Module Name:	IK220Drv.c
	
	Abstract:		Windows NT kernel-mode device driver for IK 220

	Notes:			This kernel driver allows Win32 applications to access
					IK 220 hardware through DeviceIOControl.


	History:		-													Version  0.08	ES	1999-06-24
					- Time out in IOCTL_BURSTRAM						Version  0.15	ES	1999-07-29
					- Save bus number in DEVICE_EXTENSION				Version  0.16	ES	1999-07-30
					- Change in resource descripton						Version  0.17	ES  1999-08-04
					- Version 1.0 release								Version  1.00   ES  1999-08-04
					------------------------------------------------------------------------------------------
					- Version 2.0 release								Version  2.00   ES  1999-10-19
					------------------------------------------------------------------------------------------
					- Version 3.0 release								Version  3.00   ES  2000-06-13
					------------------------------------------------------------------------------------------
					- Version 4.0 release								Version  4.00   ES  2001-10-29
					------------------------------------------------------------------------------------------
					- Set functions for Latch-Int & Latch-Ext			Version  4.01	ES	2003-06-05
					- Change Compiler to Visual C++ 6.0					Version  4.04	ES	2004-11-10
					- Version 5.0 release								Version  5.00   ES  2005-03-16
					------------------------------------------------------------------------------------------



*/


#define _VERSION	" 5.00"


#include "ntddk.h"
#include "string.h"
#include "IK220DRV.h"



#ifdef DBG
  #define kdPrint(arg)	DbgPrint arg
  #define kdBreak		DbgBreakPoint
#else
  #define kdPrint(arg)
  #define kdBreak
#endif




typedef struct _DEVICE_EXTENSION
{	PDEVICE_OBJECT	DeviceObject;
	ULONG			DeviceType;
	ULONG			BusNumber[MAX_CARDS];
	PUSHORT			PortBase[MAX_CARDS*2];
	PULONG			ConfBase[MAX_CARDS];
	ULONG			DevStat;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

typedef struct _BUS_TAG
{	ULONG BusType;
    ULONG BusNumber;
} BUS, *PBUS;


// Function prototypes
VOID		IK220DrvUnload	 ( IN PDRIVER_OBJECT	DriverObject );
NTSTATUS	IK220DrvDispatch ( IN PDEVICE_OBJECT	DeviceObject,
							   IN PIRP				Irp );


VOID IK220WriteErrorLog		 ( IN PDEVICE_OBJECT	DeviceObject, 
							   IN ULONG				EventID, 
							   IN ULONG				ErrorValue ); 




NTSTATUS DriverEntry ( IN PDRIVER_OBJECT  DriverObject,				// Pointer to driver object 
					   IN PUNICODE_STRING RegistryPath)				// Pointer to registry path


{

	PDEVICE_OBJECT		deviceObject;								// Pointer to device object
	PDEVICE_EXTENSION	deviceExtension;							// Pointer to device extension
	NTSTATUS			ntStatus;									// NT-Status
	BOOLEAN				Translated;									// Status of HalTranslateBusAddress
	WCHAR nameText[] = L"\\Device\\IK220Drv";						// Name of device driver
	WCHAR linkText[] = L"\\DosDevices\\IK220Drv";					// DOS-Name of device driver
	UNICODE_STRING		nameString, linkString;						// Device name

	PHYSICAL_ADDRESS	PortAddress[MAX_CARDS*2], MappedAddress;	// Port address of IK 220
	PHYSICAL_ADDRESS	ConfAddress[MAX_CARDS];						// Address of PCI configuration register
	ULONG				MemType			= 0;						// PCI bus memory mapped
	ULONG				MemorySize		= 0x20;						// Card memory space size 32 Byte
	ULONG				ConfSize		= 0x54;						// Configuration register space size 84 Byte
	ULONG				index;										// Card index
	ULONG				NumberOfCards	= 0;						// Number of registered card's

	PCM_RESOURCE_LIST				ResourceList	   = NULL;		// Pointer to resource usage list to report to system
	CM_RESOURCE_LIST				EmptyResourceList;		 		// Empty resource usage list
	ULONG							SizeOfResourceList = 0;			// Size of resource list
	PCM_FULL_RESOURCE_DESCRIPTOR	PFullResDesc;					// Full resource descriptor
	PCM_PARTIAL_RESOURCE_DESCRIPTOR	PPartResDesc;					// Partial resource descriptor
	BOOLEAN							ResourceConflict   = TRUE;		// Resouruce conflict

	ULONG				Zero		= 0x0;

	BUS					Interface;									//
	PCI_COMMON_CONFIG	PciCommonConfig;							//
	ULONG				Slot;										// Counter for PCI slots
	ULONG				bytes;										//



	kdPrint (( "\nIK220Drv: DriverEntry started (V"_VERSION")\n" ));



	// Create device
	////////////////

	RtlInitUnicodeString ( &nameString, nameText );

	ntStatus = IoCreateDevice ( DriverObject,						// Pointer to driver object
								sizeof(DEVICE_EXTENSION),			// Size of device extension
								&nameString,						// Device name
								FILE_DEVICE_IK220,					// Device type
								0,									// Device characteristics
								TRUE,								// Exclusive device
								&deviceObject );					// Newly created device object
										
 
	if (!NT_SUCCESS (ntStatus))
	{
		kdPrint (( "IK220Drv: Can't create Device\n" ));
		IK220WriteErrorLog (deviceObject, 0 ,ntStatus);
		return ntStatus;
	}


	// Fill device object
	/////////////////////

	deviceObject->Flags |= DO_BUFFERED_IO;

	deviceExtension = (PDEVICE_EXTENSION) deviceObject->DeviceExtension;
	RtlZeroMemory ( deviceExtension, sizeof(*deviceExtension) );
	deviceExtension->DeviceObject = deviceObject;
	deviceExtension->DeviceType   = FILE_DEVICE_IK220;



	// Create symbolic link
	///////////////////////

	RtlInitUnicodeString ( &linkString, linkText );
	ntStatus = IoCreateSymbolicLink  ( &linkString, &nameString );

	if (!NT_SUCCESS (ntStatus))
	{
		kdPrint (( "IK220Drv: Can't create symbolic link\n" ));
		IK220WriteErrorLog (deviceObject, 3 ,ntStatus);
		IoDeleteDevice (DriverObject->DeviceObject);
		return ntStatus;
	}



	DriverObject->DriverUnload = IK220DrvUnload;							// Driver unload 
	DriverObject->MajorFunction[IRP_MJ_CREATE] =
	DriverObject->MajorFunction[IRP_MJ_CLOSE]  =
	DriverObject->MajorFunction[IRP_MJ_READ]   =
	DriverObject->MajorFunction[IRP_MJ_WRITE] =
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IK220DrvDispatch;	// Driver dispatch 



	// Search for IK 220
	////////////////////

	kdPrint (( "IK220Drv: Search for IK 220\n" ));


	Interface.BusType   = PCIBus;
	Interface.BusNumber = 0;


	for (index=0; index<(MAX_CARDS*2); index++)
	{
		PortAddress[index].HighPart = 0L;
		PortAddress[index].LowPart  = 0L;
	}

	for (index=0; index<MAX_CARDS; index++)
	{
		ConfAddress[index].HighPart = 0L;
		ConfAddress[index].LowPart  = 0L;
	}


	index=0;
	do 
	{
		for (Slot=0; Slot<PCI_MAX_DEVICES; Slot++)
		{
	   		bytes = HalGetBusData ( PCIConfiguration,
									Interface.BusNumber,
									Slot,
									&PciCommonConfig,
									sizeof (PCI_COMMON_CONFIG) );

	   
			if (bytes==0)
			{
				if (Interface.BusNumber==0)
				{
					kdPrint (( "IK220Drv: PCI bus doesn't exist\n" ));
					IK220WriteErrorLog (deviceObject, 9, bytes);
					IoDeleteDevice (DriverObject->DeviceObject);
					return STATUS_DEVICE_CONFIGURATION_ERROR;
				}
				else break;
			}
			else
			{
				if  ( (PciCommonConfig.VendorID == VendorID_PLX) && (PciCommonConfig.DeviceID == DeviceID_PLX) )
	 				if  ( (PciCommonConfig.u.type0.SubVendorID == SVendorID_PLX) && (PciCommonConfig.u.type0.SubSystemID == SSystemID_IK220) )
					{
					   
						deviceExtension->BusNumber[index>>1] = Interface.BusNumber;
						
						ConfAddress[index>>1].LowPart = PciCommonConfig.u.type0.BaseAddresses[0] & 0xFFFFFFF0;
						kdPrint (( "IK220Drv: Bus %2d Slot %2d configuration register at 0x%08lX\n", Interface.BusNumber, Slot,  ConfAddress[index>>1].LowPart ));
					
						PortAddress[index].LowPart	   = PciCommonConfig.u.type0.BaseAddresses[2] & 0xFFFFFFF0;
						kdPrint (( "IK220Drv: Bus %2d Slot %2d chip select 0 at 0x%08lX\n", Interface.BusNumber, Slot,  PortAddress[index].LowPart ));
		  				index++;
		  				PortAddress[index].LowPart  = PciCommonConfig.u.type0.BaseAddresses[3] & 0xFFFFFFF0;
						kdPrint (( "IK220Drv: Bus %2d Slot %2d chip select 1 at 0x%08lX\n", Interface.BusNumber, Slot, PortAddress[index].LowPart ));
				  		index++;
					}
			} 
		}
		Interface.BusNumber++;
	} while ( (bytes!=0) && (index<(MAX_CARDS*2)) );


	NumberOfCards=0;
	for (index=0; index<(MAX_CARDS*2); index++) 
		if (PortAddress[index].LowPart != 0) NumberOfCards++;

	if (NumberOfCards==0)
	{
		kdPrint (( "IK220Drv: No card found!\n" ));
		IK220WriteErrorLog (deviceObject, 10, 0);
		RtlInitUnicodeString ( &linkString, linkText );
		IoDeleteSymbolicLink ( &linkString );
		IoDeleteDevice (DriverObject->DeviceObject);
		return STATUS_DEVICE_CONFIGURATION_ERROR;
	}




	// Report resource usage to system
	//////////////////////////////////

	kdPrint (( "IK220Drv: Report resource usage\n" ));


	SizeOfResourceList = 0;

	for ( index = 0; index<(MAX_CARDS*2); index=index+2 )
		if (PortAddress[index].LowPart != 0)
		{
			SizeOfResourceList = SizeOfResourceList + 3 * sizeof(CM_RESOURCE_LIST);
		}


	ResourceList = ExAllocatePool ( PagedPool, SizeOfResourceList);

	if (ResourceList == NULL)
	{		
		kdPrint (( "IK220Drv: Can't allocate memory for resource list\n" ));
		IK220WriteErrorLog (deviceObject, 2 , 0);

	 	RtlInitUnicodeString ( &linkString, linkText );
	 	IoDeleteSymbolicLink ( &linkString );
	 	IoDeleteDevice (DriverObject->DeviceObject);
	 	return STATUS_UNSUCCESSFUL;
	}


	RtlZeroMemory ( ResourceList, SizeOfResourceList);

	ResourceList->Count = 0;


	for ( index = 0; index<MAX_CARDS; index++ )
		if ( (PortAddress[index<<1].LowPart != 0) && (PortAddress[(index<<1)+1].LowPart != 0) )
		{
			kdPrint (( "IK220Drv: Claiming resources for card %3d\n", index ));

			// Axis 1
			kdPrint (( "IK220Drv: Reporting card resource %3d at 0x%08lX\n", index<<1, PortAddress[index<<1] ));
			ResourceList->Count++;
			PFullResDesc = &ResourceList->List[index*3];
			PPartResDesc = &(PFullResDesc->PartialResourceList.PartialDescriptors[0]);
		
			PFullResDesc->InterfaceType					= (INTERFACE_TYPE)Interface.BusType;
			PFullResDesc->BusNumber						= deviceExtension->BusNumber[index];
			PFullResDesc->PartialResourceList.Count		= 1;
			PFullResDesc->PartialResourceList.Version	= 0;
			PFullResDesc->PartialResourceList.Revision	= 0;

			PPartResDesc->Type				= CmResourceTypeMemory;
			PPartResDesc->ShareDisposition	= CmResourceShareDriverExclusive;
			PPartResDesc->Flags				= CM_RESOURCE_MEMORY_READ_WRITE;
			PPartResDesc->u.Memory.Start	= PortAddress[index<<1];
			PPartResDesc->u.Memory.Length	= MemorySize;
			

			// Axis 2
			kdPrint (( "IK220Drv: Reporting card resource %3d at 0x%08lX\n", (index<<1)+1, PortAddress[(index<<1)+1] ));
			ResourceList->Count++;
			PFullResDesc = &ResourceList->List[index*3+1];
			PPartResDesc = &(PFullResDesc->PartialResourceList.PartialDescriptors[0]);
			
			PFullResDesc->InterfaceType					= (INTERFACE_TYPE)Interface.BusType;
			PFullResDesc->BusNumber						= deviceExtension->BusNumber[index];
			PFullResDesc->PartialResourceList.Count		= 1;
			PFullResDesc->PartialResourceList.Version	= 0;
			PFullResDesc->PartialResourceList.Revision	= 0;
			
			PPartResDesc->Type				= CmResourceTypeMemory;
			PPartResDesc->ShareDisposition	= CmResourceShareDriverExclusive;
			PPartResDesc->Flags				= CM_RESOURCE_MEMORY_READ_WRITE;
			PPartResDesc->u.Memory.Start	= PortAddress[(index<<1)+1];
			PPartResDesc->u.Memory.Length	= MemorySize;


			// Configuration register
			kdPrint (( "IK220Drv: Reporting conf resource %3d at 0x%08lX\n", index, ConfAddress[index] ));
			ResourceList->Count++;
			PFullResDesc = &ResourceList->List[index*3+2];
			PPartResDesc = &(PFullResDesc->PartialResourceList.PartialDescriptors[0]);
			
			PFullResDesc->InterfaceType					= (INTERFACE_TYPE)Interface.BusType;
			PFullResDesc->BusNumber						= deviceExtension->BusNumber[index];
			PFullResDesc->PartialResourceList.Count		= 1;
			PFullResDesc->PartialResourceList.Version	= 0;
			PFullResDesc->PartialResourceList.Revision	= 0;

			PPartResDesc->Type				= CmResourceTypeMemory;
			PPartResDesc->ShareDisposition	= CmResourceShareDriverExclusive;
			PPartResDesc->Flags				= CM_RESOURCE_MEMORY_READ_WRITE;
			PPartResDesc->u.Memory.Start	= ConfAddress[index];
			PPartResDesc->u.Memory.Length	= ConfSize;

		}		





		ntStatus = IoReportResourceUsage ( NULL,
										   DriverObject,
										   ResourceList,
										   SizeOfResourceList,
										   NULL,
										   NULL,
										   0,
										   FALSE,
										   &ResourceConflict );



		if (!NT_SUCCESS (ntStatus))	
		{
			kdPrint (( "IK220Drv: Resource conflict: Status = 0x%08lX\n", ntStatus));
			IK220WriteErrorLog (deviceObject, 4 , ntStatus);
			ExFreePool (ResourceList);
			RtlInitUnicodeString ( &linkString, linkText );
			IoDeleteSymbolicLink ( &linkString );
			IoDeleteDevice (DriverObject->DeviceObject);
			return ntStatus;
		}




		ExFreePool (ResourceList);















	kdPrint (( "IK220Drv: Get hardware access\n" ));


	// Translate physical address to virtual address
	////////////////////////////////////////////////

	for ( index=0; index<(MAX_CARDS*2); index++ ) 
		if (PortAddress[index].LowPart != 0)
		{
			Translated = HalTranslateBusAddress ( Interface.BusType,
												  deviceExtension->BusNumber[index>>1],
												  PortAddress[index],
												  &MemType,
												  &MappedAddress );

			if (!Translated)
			{
				kdPrint (( "IK220Drv: Hardware access failed Axis=%3d\n", index ));
				IK220WriteErrorLog (deviceObject, 5, ntStatus);

				ntStatus = IoReportResourceUsage ( NULL,
												   DriverObject,
												   &EmptyResourceList,
												   sizeof(EmptyResourceList),
												   NULL,
												   NULL,
												   0,
												   FALSE,
												   &ResourceConflict );



				RtlInitUnicodeString ( &linkString, linkText );
				IoDeleteSymbolicLink ( &linkString );
				IoDeleteDevice (DriverObject->DeviceObject);
	 			return STATUS_UNSUCCESSFUL;
			}
			deviceExtension->PortBase[index] = (void*)MmMapIoSpace ( MappedAddress,
																	 MemorySize,
																	 FALSE );
			kdPrint (( "IK220Drv: Axis %3d mapped to 0x%08lX\n", index,  deviceExtension->PortBase[index] ));
		}
		else deviceExtension->PortBase[index] = NULL;



	for ( index=0; index<MAX_CARDS; index++ ) 
		if (ConfAddress[index].LowPart != 0)
		{
			Translated = HalTranslateBusAddress ( Interface.BusType,
												  deviceExtension->BusNumber[index],
												  ConfAddress[index],
												  &MemType,
												  &MappedAddress );

			if (!Translated)
			{
				kdPrint (( "IK220Drv: Hardware access failed Conf=%3d\n", index ));
				IK220WriteErrorLog (deviceObject, 5, ntStatus);

				ntStatus = IoReportResourceUsage ( NULL,
												   DriverObject,
												   &EmptyResourceList,
												   sizeof(EmptyResourceList),
												   NULL,
												   NULL,
												   0,
												   FALSE,
												   &ResourceConflict );



				RtlInitUnicodeString ( &linkString, linkText );
				IoDeleteSymbolicLink ( &linkString );
				IoDeleteDevice (DriverObject->DeviceObject);
	 			return STATUS_UNSUCCESSFUL;
			}
			deviceExtension->ConfBase[index] = (void*)MmMapIoSpace ( MappedAddress,
																	 MemorySize,
																	 FALSE );
			kdPrint (( "IK220Drv: Configuration register %3d mapped to 0x%08lX\n", index,  deviceExtension->ConfBase[index] ));


		}
		else deviceExtension->ConfBase[index] = NULL;


	kdPrint (( "IK220Drv: DriverEntry ends\n\n" ));
	return STATUS_SUCCESS;
}





VOID IK220DrvUnload ( IN PDRIVER_OBJECT	DriverObject )

{
	WCHAR linkText[] = L"\\DosDevices\\IK220Drv";
	UNICODE_STRING	linkString;								// Device name

	CM_RESOURCE_LIST	EmptyResourceList;					// Empty resource usage list
	BOOLEAN				ResourceConflict;					// Resource already in use


	
	kdPrint (( "IK220Drv: Driver unload\n" ));


	// Unload driver: remove resource usage, delete symbolic link, delete device
	////////////////////////////////////////////////////////////////////////////

	RtlZeroMemory (&EmptyResourceList, sizeof(EmptyResourceList) );

	IoReportResourceUsage ( NULL,
							DriverObject,
							&EmptyResourceList,
							sizeof(EmptyResourceList),
							NULL,
							NULL,
							0,
							FALSE,
							&ResourceConflict );


	RtlInitUnicodeString ( &linkString, linkText );
	IoDeleteSymbolicLink ( &linkString );

	IoDeleteDevice (DriverObject->DeviceObject);
}



NTSTATUS IK220DrvDispatch ( IN PDEVICE_OBJECT	deviceObject,
							IN PIRP				Irp )
{

#ifdef _DEBUG
	const char DrvVers[20] = "DRV: V"_VERSION"(D)";
#else
	const char DrvVers[20] = "DRV: V"_VERSION"(R)";
#endif
		
	
	NTSTATUS			ntStatus = STATUS_SUCCESS;				// NT-Status
	PIO_STACK_LOCATION	irpStack;
	ULONG				ioControlCode;
	ULONG				wordsToRead, wordsToWrite;				// Number of words for i/o
	PUCHAR				irpBufferB;								// Byte  buffer pointer for i/o
	PUSHORT				irpBufferS;								// Short buffer pointer for i/o
	PULONG				irpBufferL;								// Long  buffer pointer for i/o
	ULONG				i;										// Loop counter
	USHORT				cardNum, cardReg, RamAdr;				// Card number / card register / RAM address
	USHORT*				pSource;								// Source pointer 
	USHORT*				pDestination;							// Destination pointer
	ULONG*				pDestinationL;							// Destination pointer long
    USHORT				Status;									// Status from IK 220
	USHORT				TimeOut;									// Time out counter
	PDEVICE_EXTENSION	deviceExtension = deviceObject->DeviceExtension;

	irpStack = IoGetCurrentIrpStackLocation(Irp);
	switch (irpStack->MajorFunction)
	{
	case IRP_MJ_CREATE:
		kdPrint (( "IK220Drv: Dispatched to create\n" ));
		Irp->IoStatus.Information = 0;
		break;

	case IRP_MJ_CLOSE:
		kdPrint (( "IK220Drv: Dispatched to close\n" ));
		deviceExtension->DevStat  = 0;
		Irp->IoStatus.Information = 0;
		break;

	case IRP_MJ_READ:
		kdPrint (( "IK220Drv: Dispatched to read\n" ));
		Irp->IoStatus.Information = 0;
		break;

	case IRP_MJ_WRITE:
		kdPrint (( "IK220Drv: Dispatched to write\n" ));
		Irp->IoStatus.Information = 0;
		break;

	case IRP_MJ_DEVICE_CONTROL:
		kdPrint (( "IK220Drv: DeviceControl - " ));

		ioControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;
		switch (ioControlCode)
		{


		//
		//

		case IOCTL_INPUT:

			wordsToRead = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
			wordsToRead = wordsToRead /2;
			irpBufferS = Irp->AssociatedIrp.SystemBuffer;
			cardNum = *irpBufferS;
			cardReg = *(irpBufferS+1);

			pSource = (USHORT*) (deviceExtension->PortBase[cardNum]);

			kdPrint (( "Reading %2d word(s) card %3d at 0x%08lX register 0x%02X:", wordsToRead, cardNum, pSource, cardReg ));

			if (pSource)
			{
				pSource = pSource+cardReg;

				if (wordsToRead<=15)
				{
					for (i=0; i<wordsToRead; i++)
					{
						kdPrint (( " 0x%04X",*pSource ));
						*irpBufferS++ = *pSource++;
					}
					kdPrint (( "\n" ));
					Irp->IoStatus.Information = wordsToRead*2;
				}
				else
				{
					kdPrint (( "Illegal word count!\n" ));
					if ((deviceExtension->DevStat & 0x1) == 0)
					{
						IK220WriteErrorLog (deviceObject, 6, wordsToRead);
						deviceExtension->DevStat |= 0x1;
					}
					Irp->IoStatus.Information = 0;
				}
			}
			else
			{
				kdPrint (( "Card not found!\n" ));
				if ((deviceExtension->DevStat & 0x2) == 0)
				{
					IK220WriteErrorLog (deviceObject, 7, cardNum);
					deviceExtension->DevStat |= 0x2;
				}
				Irp->IoStatus.Information = 0;
			}
		break;



		case IOCTL_OUTPUT:

			wordsToWrite = irpStack->Parameters.DeviceIoControl.InputBufferLength;
			wordsToWrite = (wordsToWrite/2)-2;
			irpBufferS = Irp->AssociatedIrp.SystemBuffer;
			cardNum = *irpBufferS;
			cardReg = *(irpBufferS+1);
			pSource = irpBufferS+2;

			pDestination = (USHORT*) (deviceExtension->PortBase[cardNum]);

			kdPrint (( "Writing %2d word(s) card %3d at 0x%08lX register 0x%02X:", wordsToWrite, cardNum, pDestination, cardReg ));


			if (pDestination)
			{
				pDestination = pDestination+cardReg;

				if (wordsToWrite<=15)
				{
					for (i=0; i<(wordsToWrite); i++)
					{
						kdPrint (( " 0x%04X",*pSource ));
						*pDestination++ = *pSource++;
					}
					kdPrint (( "\n" ));
					Irp->IoStatus.Information = wordsToWrite*2;
				}
				else
				{
					kdPrint (( "Illegal word count!\n" ));
					if ((deviceExtension->DevStat & 0x1) == 0)
					{
						IK220WriteErrorLog (deviceObject, 6, wordsToWrite);
						deviceExtension->DevStat |= 0x1;
					}
					Irp->IoStatus.Information = 0;
				}
			}
			else
			{
				kdPrint (( "\nCard not found!\n" ));
				if ((deviceExtension->DevStat & 0x2) == 0)
				{
					IK220WriteErrorLog (deviceObject, 7, cardNum);
					deviceExtension->DevStat |= 0x2;
				}
				Irp->IoStatus.Information = 0;
			}
			break;




		case IOCTL_STATUS:
			wordsToRead = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
			irpBufferL = Irp->AssociatedIrp.SystemBuffer;

			kdPrint (( "IK-Card's at:\n" ));

			for (cardNum=0; cardNum<(MAX_CARDS*2); cardNum++)
			{
				if (deviceExtension->PortBase[cardNum]!=0)
				{
					pSource = (USHORT*) (deviceExtension->PortBase[cardNum]);
					Status = *(pSource+CodeReg);	
					
					if ( Status == 0x0007 )
					{
						*(irpBufferL+cardNum) = (ULONG)deviceExtension->PortBase[cardNum];
					  	kdPrint (( "(%3d)=0x%08lX\n", cardNum, *(irpBufferL+cardNum) ));
					}
					else
					{
						kdPrint (( "(%3d)=0x%08lX : Status failed (0x%04X)\n", cardNum, *(irpBufferL+cardNum), Status ));
						IK220WriteErrorLog (deviceObject, 7, cardNum);
						*(irpBufferL+cardNum) = 0L;
					}
				}
				else *(irpBufferL+cardNum) = 0L;
			}
			Irp->IoStatus.Information = (MAX_CARDS*2)*sizeof(ULONG);
			break;




		case IOCTL_DOWNLOAD:
			wordsToWrite = irpStack->Parameters.DeviceIoControl.InputBufferLength;
			wordsToWrite = (wordsToWrite/2)-2;					// Number of words to transfer
			irpBufferS = Irp->AssociatedIrp.SystemBuffer;
			cardNum = *irpBufferS;
			RamAdr  = *(irpBufferS+1);

			pSource = irpBufferS+2;
			pDestination = (USHORT*) (deviceExtension->PortBase[cardNum]);

			kdPrint (( "Loading %d word(s) card %3d at 0x%08lX start 0x%04X\n", wordsToWrite, cardNum, pDestination, RamAdr ));


			if (pDestination)
			{
					for (i=1; i<=wordsToWrite; i++)
					{
						*(pDestination + ControlReg) = BootMode;		// Set CPU to boot mode
						do 
						{
						} while ( !(*(pDestination + StatusReg) & PipeEmpty) );	// 

						*(pDestination + DatPort0  ) = RamAdr++;		// Write address to port 0
					    *(pDestination + DatPort1  ) = *pSource;		// Write data to port 1
						*(pDestination + ControlReg) = WriteRamMode;	// Transfer data to address in RAM
						pSource++;

						do 
						{
						} while ( !(*(pDestination + StatusReg) & PipeEmpty) );
					}
					Irp->IoStatus.Information = wordsToWrite*2;
			}
			else
			{
				kdPrint (( "\nCard not found!\n" ));
				if ((deviceExtension->DevStat & 0x2) == 0)
				{
					IK220WriteErrorLog (deviceObject, 7, cardNum);
					deviceExtension->DevStat |= 0x2;
				}
				Irp->IoStatus.Information = 0;
			}
			break;




		case IOCTL_BURSTRAM:
			wordsToRead = irpStack->Parameters.DeviceIoControl.OutputBufferLength;  // Max. number of values to transfer
			wordsToRead = ((wordsToRead/2)-1)/3;
			irpBufferS  = Irp->AssociatedIrp.SystemBuffer;

			cardNum		 = *irpBufferS;
			pDestination =  irpBufferS+1;
			pSource      = (USHORT*) (deviceExtension->PortBase[cardNum]);

			kdPrint (( "Burstread %d values card 0x%08lX\n", wordsToRead, pSource ));

			if (pSource)
			{
				i=0;
				Status=0;
				do
				{
					*(pSource+CmdPort) = cmdGetRam;									// Command:  to IK 220
					TimeOut=0;
					do
					{
						KeStallExecutionProcessor (1);								// Wait 1µs
						TimeOut++;													// Increment TimeOut
					} while ( !(*(pSource+Flag1Reg) & G28Sem0) & (TimeOut<1000) );	// Wait until finished or TimeOut

					if ( *(pSource+DatPort0)!=cmdGetRam )
					{

						Status = 0x8001;	break;									// Error from IK 220
					}
					if ( TimeOut>=1000)
					{
						Status = 0x8002;	break;									// TimeOut from IK 220
					}

					Status = Status | *(pSource+DatPort6);
					if (Status & statBufferNoVal)
					{
						break;														// RAM Buffer empty
					}

					*pDestination++ = *(pSource+DatPort1);							// Transfer counter value
					*pDestination++ = *(pSource+DatPort2);
					*pDestination++ = *(pSource+DatPort3);

					i++;
				} while ( (i<wordsToRead) && !(Status & statBufferEmpty) );


				*irpBufferS = Status;
				if ( !(Status & 0x8000) ) 
				{
					Irp->IoStatus.Information = (i*3+1)*2;
				}
				else
				{
					Irp->IoStatus.Information = (1*2);
				}
			}
			else
			{
				kdPrint (( "\nCard not found!\n" ));
				if ((deviceExtension->DevStat & 0x2) == 0)
				{
					IK220WriteErrorLog (deviceObject, 7, cardNum);
					deviceExtension->DevStat |= 0x2;
				}
				Irp->IoStatus.Information = 0;
			}
			break;






		case IOCTL_VERSION:

			wordsToRead = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
			wordsToRead = wordsToRead /2;
			irpBufferB = Irp->AssociatedIrp.SystemBuffer;
			irpBufferS = Irp->AssociatedIrp.SystemBuffer;
			cardNum = *irpBufferS;

			pSource = (USHORT*) (deviceExtension->PortBase[cardNum]);

			kdPrint (( "Reading version of card %3d\n", cardNum ));

			if (pSource)
			{
					*(pSource+CmdPort) = cmdGetVersion;					// Command:  to IK 220
					do {} while (!(*(pSource+Flag1Reg) & G28Sem0));		// Wait until finished

					if ( *(pSource+DatPort0)==cmdGetVersion )			// Error from IK 220
					{
						 *irpBufferB++ = *(pSource+DatPort1) >> 8;
						 *irpBufferB++ = *(pSource+DatPort1) &  0x00FF;

						 *irpBufferB++ = *(pSource+DatPort2) >> 8;
						 *irpBufferB++ = *(pSource+DatPort2) &  0x00FF;
				  
						 *irpBufferB++ = *(pSource+DatPort3) >> 8;
						 *irpBufferB++ = *(pSource+DatPort3) &  0x00FF;
					 
						 *irpBufferB++ = *(pSource+DatPort4) >> 8;
						 *irpBufferB++ = *(pSource+DatPort4) &  0x00FF;
					 
						 *irpBufferB++ = *(pSource+DatPort5) >> 8;
						 *irpBufferB++ = *(pSource+DatPort5) &  0x00FF;
					 
						 *irpBufferB++ = *(pSource+DatPort6) >> 8;
						 *irpBufferB++ = *(pSource+DatPort6) &  0x00FF;
					 
						 *irpBufferB++ = 0;
				  
						 for (i=0; i<sizeof(DrvVers); i++)
						 *irpBufferB++ = DrvVers[i];

						Irp->IoStatus.Information = 6*2+1+sizeof(DrvVers);
					}
					else
						Irp->IoStatus.Information = 0;
			}
			else
			{
				kdPrint (( "Card not found!\n" ));
				if ((deviceExtension->DevStat & 0x2) == 0)
				{
					IK220WriteErrorLog (deviceObject, 7, cardNum);
					deviceExtension->DevStat |= 0x2;
				}
				Irp->IoStatus.Information = 0;
			}
		break;



		case IOCTL_LATCHI:
			irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
			cardNum		= *irpBufferS;
			pDestinationL = (ULONG*) (deviceExtension->ConfBase[cardNum]);

			if (pDestinationL)
			{
				pDestinationL = pDestinationL + CONFREG_CNTRL;

				kdPrint (( "Internal latch card %3d at 0x%08lX\n", cardNum, pDestinationL ));

				*pDestinationL = *pDestinationL & 0xFFFFFEFF;	// Latch Int low
				KeStallExecutionProcessor (2);					// Wait 2µs
				*pDestinationL = *pDestinationL | 0x00000100;	// Latch Int high
				
				Irp->IoStatus.Information = 1;
			}
			else
			{
				kdPrint (( "Internal latch card %3d not found!\n", cardNum ));
				Irp->IoStatus.Information = 0;
			}
		break;
		



		case IOCTL_LATCHE:
			irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
			cardNum		= *irpBufferS;
			pDestinationL = (ULONG*) (deviceExtension->ConfBase[cardNum]);

			if (pDestinationL)
			{
				pDestinationL = pDestinationL + CONFREG_CNTRL;

				kdPrint (( "External latch card %3d at 0x%08lX\n", cardNum, pDestinationL ));

				*pDestinationL = *pDestinationL & 0xFFFFF7FF;	// Latch Ext low
				KeStallExecutionProcessor (2);					// Wait 2µs
				*pDestinationL = *pDestinationL | 0x00000800;	// Latch Ext high
				
				Irp->IoStatus.Information = 1;
			}
			else
			{
				kdPrint (( "External latch card %3d not found!\n", cardNum ));
				Irp->IoStatus.Information = 0;
			}
		break;


		case IOCTL_SYSLED:
			irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
			cardNum		= *irpBufferS;
			pDestinationL = (ULONG*) (deviceExtension->ConfBase[cardNum]);

			if (pDestinationL)
			{
				pDestinationL = pDestinationL + CONFREG_CNTRL;

				kdPrint (( "System LED card %3d at 0x%08lX mode=%d\n", cardNum, pDestinationL, *(irpBufferS+1) ));

				if ( *(irpBufferS+1)==0 ) *pDestinationL = *pDestinationL | 0x00000004;		// System LED off
				else *pDestinationL = *pDestinationL & 0xFFFFFFFB;							// System LED on

				
				Irp->IoStatus.Information = 1;
			}
			else
			{
				kdPrint (( "System LED card %3d not found!\n", cardNum ));
				Irp->IoStatus.Information = 0;
			}
		break;


        case IOCTL_SETI:
            irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
            cardNum     = *irpBufferS;
            pDestinationL = (ULONG*) (deviceExtension->ConfBase[cardNum]);

            if (pDestinationL)
            {
                pDestinationL = pDestinationL + CONFREG_CNTRL;

                kdPrint (( "Set internal latch card %3d at 0x%08lX to %s\n", cardNum, pDestinationL, *(irpBufferS+1)==0 ? "low" : "high"));

                if ( *(irpBufferS+1)==0 ) *pDestinationL = *pDestinationL & 0xFFFFFEFF;   // Latch Int low
                else                      *pDestinationL = *pDestinationL | 0x00000100;   // Latch Int high
                
                Irp->IoStatus.Information = 1;
            }
            else
            {
                kdPrint (( "Internal latch card %3d not found!\n", cardNum ));
                Irp->IoStatus.Information = 0;
            }
            break;


        case IOCTL_SETE:
            irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
            cardNum     = *irpBufferS;
            pDestinationL = (ULONG*) (deviceExtension->ConfBase[cardNum]);

            if (pDestinationL)
            {
                pDestinationL = pDestinationL + CONFREG_CNTRL;

                kdPrint (( "Set external latch card %3d at 0x%08lX to %s\n", cardNum, pDestinationL, *(irpBufferS+1)==0 ? "low" : "high"));

                if ( *(irpBufferS+1)==0 ) *pDestinationL = *pDestinationL & 0xFFFFF7FF;   // Latch Ext low
                else                      *pDestinationL = *pDestinationL | 0x00000800;   // Latch Ext high
                
                Irp->IoStatus.Information = 1;
            }
            else
            {
                kdPrint (( "External latch card %3d not found!\n", cardNum ));
                Irp->IoStatus.Information = 0;
            }
            break;



		default:
			if ((deviceExtension->DevStat & 0x4) == 0)
			{
				IK220WriteErrorLog (deviceObject, 8 ,ioControlCode);
				deviceExtension->DevStat |= 0x4;
			}	
			kdPrint (( "unknown funtion\n" ));
			ntStatus = STATUS_NOT_IMPLEMENTED;
			Irp->IoStatus.Information = 0;
			break;
		}

		break;




	default:
		if ((deviceExtension->DevStat & 0x4) == 0)
		{
			IK220WriteErrorLog (deviceObject, 8 ,0);
			deviceExtension->DevStat |= 0x4;
		}	
		kdPrint (( "IK220Drv: Dispatched to unknown MajorFunction\n" ));
		ntStatus = STATUS_NOT_IMPLEMENTED;
		Irp->IoStatus.Information = 0;
		break;
	}

	Irp->IoStatus.Status = ntStatus;
	IoCompleteRequest (Irp,IO_NO_INCREMENT);
	return ntStatus;
}




void IK220WriteErrorLog ( IN PDEVICE_OBJECT DeviceObject, 
				 	      IN ULONG EventID, 
					      IN ULONG ErrorValue  ) 
 
{ 
    PIO_ERROR_LOG_PACKET errorLogEntry; 
    UCHAR		 EntrySize; 
    PUCHAR		 StringLoc; 
    ULONG		 TempError; 
    static WCHAR ErrorBuffer[11]   = L"00000000"; 
    static WCHAR ErrorText[11][30] = { L"Can't create device ",			// EventID  0:
									   L"Can't read registry values ",	// EventID  1:
									   L"Can't allocate memory ",		// EventID  2:
									   L"Can't create symbolic link ", 	// EventID  3:
									   L"Resource conflict ",		 	// EventID  4:
									   L"Hardware access failed ",		// EventID  5:
									   L"Illegal word count ",			// EventID  6:
									   L"Card not found ",				// EventID  7:
									   L"Unknown function ",			// EventID  8:
									   L"PCI bus doesn't exist ",		// EventID  9:
									   L"No card found "				// EventID 10:
									 };
    short i; 
 
    EntrySize = sizeof(IO_ERROR_LOG_PACKET) + sizeof(ErrorText[EventID]); 

	if (ErrorValue!=0)
	{
		// Convert the error value into a string
		TempError = ErrorValue; 
		for (i=9; i>=0; i--)
		{ 
			ErrorBuffer[i] = (WCHAR)((TempError % 10) + L'0'); 
	        TempError /= 10; 
		} 
 
		EntrySize += sizeof(ErrorBuffer);
	}


    errorLogEntry = (PIO_ERROR_LOG_PACKET)IoAllocateErrorLogEntry( DeviceObject, EntrySize );
 
    if (errorLogEntry != NULL)
	{ 
        errorLogEntry->MajorFunctionCode = (UCHAR)-1; 
        errorLogEntry->RetryCount = (UCHAR)-1; 
        errorLogEntry->DumpDataSize = 0;

		if (ErrorValue!=0) errorLogEntry->NumberOfStrings = 2; 
        else errorLogEntry->NumberOfStrings = 1; 
		
		errorLogEntry->StringOffset = sizeof(IO_ERROR_LOG_PACKET);
        errorLogEntry->EventCategory = 0; 
        errorLogEntry->ErrorCode =  EventID;
        errorLogEntry->UniqueErrorValue = ErrorValue; 
        errorLogEntry->FinalStatus = 0;
        errorLogEntry->SequenceNumber = (ULONG)-1; 								
        errorLogEntry->IoControlCode = 0;
		errorLogEntry->DeviceOffset.QuadPart = 0;

        StringLoc = ((PUCHAR)errorLogEntry) + errorLogEntry->StringOffset; 
        RtlCopyMemory (StringLoc, ErrorText[EventID], sizeof(ErrorText[EventID])); 


		if (ErrorValue!=0)
		{
			while (*StringLoc) StringLoc+=sizeof(WCHAR);
			StringLoc+=sizeof(WCHAR);
			RtlCopyMemory (StringLoc, ErrorBuffer, sizeof(ErrorBuffer)); 
		}

        IoWriteErrorLogEntry(errorLogEntry); 
    } 
 
}
 
