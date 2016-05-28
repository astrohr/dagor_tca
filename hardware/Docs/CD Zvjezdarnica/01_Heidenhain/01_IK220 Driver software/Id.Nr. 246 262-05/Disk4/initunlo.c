
#include <ntddk.h>
#include <IK220DRV.h>
#include <ik220.h>



//
// This is the actual definition of Ik220DebugLevel.
// Note that it is only defined if this is a "debug"
// build.
//
#if DBG
extern ULONG Ik220DebugLevel = 0;
#endif

//
// All our global variables except DebugLevel stashed in one
// little package
//
IK220_GLOBALS Ik220Globals;

static const PHYSICAL_ADDRESS Ik220PhysicalZero = {0};

//
// We use this to query into the registry as to whether we
// should break at driver entry.
//

IK220_FIRMWARE_DATA    driverDefaults;

//
// This is exported from the kernel.  It is used to point
// to the address that the kernel debugger is using.
//
extern PUCHAR *KdComPortInUse;

//
// INIT - only needed during init and then can be disposed
// PAGESRP0 - always paged / never locked
// PAGEIK - must be locked when a device is open, else paged
//
//
// INIT is used for DriverEntry() specific code
//
// PAGESRP0 is used for code that is not often called and has nothing
// to do with I/O performance.  An example, IRP_MJ_PNP/IRP_MN_START_DEVICE
// support functions
//
// PAGEIK is used for code that needs to be locked after an open for both
// performance and IRQL reasons.
//

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)

#pragma alloc_text(PAGESRP0, Ik220InitController)
#pragma alloc_text(PAGESRP0, Ik220FindInitController)
#pragma alloc_text(PAGESRP0, Ik220GetMappedAddress)
#pragma alloc_text(PAGESRP0, Ik220RemoveDevObj)
#pragma alloc_text(PAGESRP0, Ik220ReleaseResources)
#pragma alloc_text(PAGESRP0, Ik220Unload)
#pragma alloc_text(PAGESRP0, Ik220MemCompare)


//
// PAGEIK handled is keyed off of Ik220Reset, so Ik220Reset
// must remain in PAGEIK for things to work properly
//

#pragma alloc_text(PAGEIK, Ik220Reset)
#endif


NTSTATUS
DriverEntry(
           IN PDRIVER_OBJECT DriverObject,
           IN PUNICODE_STRING RegistryPath
           )

/*++

Routine Description:

    The entry point that the system point calls to initialize
    any driver.

    This routine will gather the configuration information,
    report resource usage, attempt to initialize all ik220
    devices, connect to interrupts for ports.  If the above
    goes reasonably well it will fill in the dispatch points,
    reset the ik220 devices and then return to the system.

Arguments:

    DriverObject - Just what it says,  really of little use
    to the driver itself, it is something that the IO system
    cares more about.

    PathToRegistry - points to the entry for this driver
    in the current control set of the registry.

Return Value:

    Always STATUS_SUCCESS

--*/
{

   //
   // Lock the paged code in their frames
   //

   PVOID lockPtr = MmLockPagableCodeSection(Ik220Reset);

   PAGED_CODE();





#if DBG
// DbgBreakPoint();
#endif





   ASSERT(Ik220Globals.PAGEIK_Handle == NULL);
#if DBG
   Ik220Globals.PAGEIK_Count = 0;
   Ik220LogInit();
#endif
   Ik220Globals.PAGEIK_Handle = lockPtr;
   Ik220Globals.instances = 0;
   Ik220Globals.CreatedSymbolicLink = FALSE;

   Ik220Globals.RegistryPath.MaximumLength = RegistryPath->MaximumLength;
   Ik220Globals.RegistryPath.Length = RegistryPath->Length;
   Ik220Globals.RegistryPath.Buffer = ExAllocatePool(PagedPool, Ik220Globals.RegistryPath.MaximumLength);

   if (Ik220Globals.RegistryPath.Buffer == NULL) {
      MmUnlockPagableImageSection(lockPtr);
      return STATUS_INSUFFICIENT_RESOURCES;
   }

   RtlZeroMemory(Ik220Globals.RegistryPath.Buffer,
                 Ik220Globals.RegistryPath.MaximumLength);
   RtlMoveMemory(Ik220Globals.RegistryPath.Buffer,
                 RegistryPath->Buffer, RegistryPath->Length);

   //
   // Initialize all our globals
   //

   InitializeListHead(&Ik220Globals.AllDevObjs);

   //
   // Call to find out default values to use for all the devices that the
   // driver controls, including whether or not to break on entry.
   //

   Ik220GetConfigDefaults(&driverDefaults, RegistryPath);

#if DBG
   //
   // Set global debug output level
   //
   Ik220DebugLevel = driverDefaults.DebugLevel;
#endif

   //
   // Break on entry if requested via registry
   //

   if (driverDefaults.ShouldBreakOnEntry) {
//      DbgBreakPoint();
   }


   //
   // Just dump out how big the extension is.
   //

   Ik220Dump(SERDIAG1, ("IK220: The number of bytes in the extension is: %d"
                         "\n", sizeof(IK220_DEVICE_EXTENSION)));


    //
    // Initialize the Driver Object with driver's entry points
    //

    DriverObject->DriverUnload                                  = Ik220Unload;
    DriverObject->DriverExtension->AddDevice                    = Ik220AddDevice;

    DriverObject->MajorFunction[IRP_MJ_FLUSH_BUFFERS]           = 0; //Ik220Flush;
    DriverObject->MajorFunction[IRP_MJ_WRITE]                   = 0; //Ik220Write;
    DriverObject->MajorFunction[IRP_MJ_READ]                    = 0; //Ik220Read;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]          = Ik220IoControl;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = Ik220InternalIoControl;
    DriverObject->MajorFunction[IRP_MJ_CREATE]                  = Ik220CreateOpen;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]                   = Ik220Close;
    DriverObject->MajorFunction[IRP_MJ_CLEANUP]                 = Ik220Cleanup;
    DriverObject->MajorFunction[IRP_MJ_PNP]                     = Ik220PnpDispatch;
    DriverObject->MajorFunction[IRP_MJ_POWER]                   = Ik220PowerDispatch;
    DriverObject->MajorFunction[IRP_MJ_QUERY_INFORMATION]       = Ik220QueryInformationFile;
    DriverObject->MajorFunction[IRP_MJ_SET_INFORMATION]         = Ik220SetInformationFile;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL]          = Ik220SystemControlDispatch;


    //
    // Unlock pageable text
    //
    MmUnlockPagableImageSection(lockPtr);

    return STATUS_SUCCESS;
}


BOOLEAN
Ik220CleanLists(IN PVOID Context)
/*++

Routine Description:

    Removes a device object from any of the ik220 linked lists it may
    appear on.

Arguments:

    Context - Actually a PIK220_DEVICE_EXTENSION (for the devobj being
              removed).

Return Value:

    Always TRUE

--*/
{
   PIK220_DEVICE_EXTENSION pDevExt = (PIK220_DEVICE_EXTENSION)Context;


   //
   // AllDevObjs should never be empty since we have a sentinal
   //

   ASSERT(!IsListEmpty(&pDevExt->AllDevObjs));

   RemoveEntryList(&pDevExt->AllDevObjs);
   InitializeListHead(&pDevExt->AllDevObjs);

   return TRUE;
}


VOID
Ik220ReleaseResources(IN PIK220_DEVICE_EXTENSION PDevExt)
/*++

Routine Description:

    Releases resources (not pool) stored in the device extension.

Arguments:

    PDevExt - Pointer to the device extension to release resources from.

Return Value:

    VOID

--*/
{
   int ii;

   PAGED_CODE();

   Ik220Dump(SERTRACECALLS,("IK220: Enter Ik220ReleaseResources\n"));

   //
   // Remove us from any lists we may be on
   //

   Ik220CleanLists(PDevExt);

   //
   // If necessary, unmap the device registers.
   //

   if (PDevExt->UnMapRegisters) {
      MmUnmapIoSpace(PDevExt->Controller, PDevExt->SpanOfController);
   }

   for (ii = 0; ii < 2; ++ii) {
      if (PDevExt->UnMapPortBase[ii]) {
         MmUnmapIoSpace(PDevExt->PortBase[ii], PDevExt->SpanOfPortBase[ii]);
         Ik220Globals.GloPortBase[PDevExt->instance*2+ii] = 0;
      }
   }

   if (PDevExt->UnMapConfBase) {
      MmUnmapIoSpace(PDevExt->ConfBase, PDevExt->SpanOfConfBase);
      Ik220Globals.GloConfBase[PDevExt->instance] = 0;
   }

   if (PDevExt->UnMapStatus) {
//      MmUnmapIoSpace(PDevExt->InterruptStatus,
//                     PDevExt->SpanOfInterruptStatus);
   }

   Ik220Dump(SERTRACECALLS,("IK220: Leave Ik220ReleaseResources\n"));
}


VOID
Ik220DisableInterfacesResources(IN PDEVICE_OBJECT PDevObj)
{
   PIK220_DEVICE_EXTENSION pDevExt
      = (PIK220_DEVICE_EXTENSION)PDevObj->DeviceExtension;

   PAGED_CODE();

   Ik220Dump(SERTRACECALLS,("IK220: Enter Ik220DisableInterfaces\n"));

   //
   // Only do these many things if the device has started and still
   // has resources allocated
   //

   if (pDevExt->Flags & IK220_FLAGS_STARTED) {
       if (!(pDevExt->Flags & IK220_FLAGS_STOPPED)) {

          Ik220ReleaseResources(pDevExt);
       }

      //
      // Remove us from WMI consideration
      //

      IoWMIRegistrationControl(PDevObj, WMIREG_ACTION_DEREGISTER);
   }

   //
   // Undo external names
   //

   Ik220UndoExternalNaming(pDevExt);

   Ik220Dump(SERTRACECALLS,("IK220: Exit Ik220DisableInterfaces\n"));
}


NTSTATUS
Ik220RemoveDevObj(IN PDEVICE_OBJECT PDevObj)
/*++

Routine Description:

    Removes a ik220 device object from the system.

Arguments:

    PDevObj - A pointer to the Device Object we want removed.

Return Value:

    Always TRUE

--*/
{
   PIK220_DEVICE_EXTENSION pDevExt
      = (PIK220_DEVICE_EXTENSION)PDevObj->DeviceExtension;

   PAGED_CODE();

   Ik220Dump(SERTRACECALLS,("IK220: Enter Ik220RemoveDevObj\n"));

   if (!(pDevExt->DevicePNPAccept & IK220_PNPACCEPT_SURPRISE_REMOVING)) {
      //
      // Disable all external interfaces and release resources
      //

      Ik220DisableInterfacesResources(PDevObj);
   }

   IoDetachDevice(pDevExt->LowerDeviceObject);

   //
   // Free memory allocated in the extension
   //

   if (pDevExt->NtNameForPort.Buffer != NULL) {
      ExFreePool(pDevExt->NtNameForPort.Buffer);
   }

   if (pDevExt->DeviceName.Buffer != NULL) {
      ExFreePool(pDevExt->DeviceName.Buffer);
   }

   if (pDevExt->SymbolicLinkName.Buffer != NULL) {
      ExFreePool(pDevExt->SymbolicLinkName.Buffer);
   }

   if (pDevExt->DosName.Buffer != NULL) {
      ExFreePool(pDevExt->DosName.Buffer);
   }

   //
   // Delete the devobj
   //

   IoDeleteDevice(PDevObj);


   Ik220Dump(SERTRACECALLS,("IK220: Leave Ik220RemoveDevObj\n"));

   return STATUS_SUCCESS;
}


VOID
Ik220KillPendingIrps(PDEVICE_OBJECT PDevObj)
/*++

Routine Description:

   This routine kills any irps pending for the passed device object.

Arguments:

    PDevObj - Pointer to the device object whose irps must die.

Return Value:

    VOID

--*/
{
    PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
//    KIRQL oldIrql;

   Ik220Dump (SERTRACECALLS,("IK220: Enter Ik220KillPendingIrps\n"));
#if 0
    //
    // First kill all the reads and writes.
    //

    SerialKillAllReadsOrWrites(PDevObj, &pDevExt->WriteQueue,
                               &pDevExt->CurrentWriteIrp);

    SerialKillAllReadsOrWrites(PDevObj, &pDevExt->ReadQueue,
                               &pDevExt->CurrentReadIrp);

    //
    // Next get rid of purges.
    //

    SerialKillAllReadsOrWrites(PDevObj, &pDevExt->PurgeQueue,
                               &pDevExt->CurrentPurgeIrp);

    //
    // Get rid of any mask operations.
    //

    SerialKillAllReadsOrWrites(PDevObj, &pDevExt->MaskQueue,
                               &pDevExt->CurrentMaskIrp);
#endif
    //
    // Now get rid a pending wait mask irp.
    //
#if 0
    IoAcquireCancelSpinLock(&oldIrql);

    if (pDevExt->CurrentWaitIrp) {

        PDRIVER_CANCEL cancelRoutine;

        cancelRoutine = pDevExt->CurrentWaitIrp->CancelRoutine;
        pDevExt->CurrentWaitIrp->Cancel = TRUE;

        if (cancelRoutine) {

            pDevExt->CurrentWaitIrp->CancelIrql = oldIrql;
            pDevExt->CurrentWaitIrp->CancelRoutine = NULL;

            cancelRoutine(PDevObj, pDevExt->CurrentWaitIrp);

        }

    } else {

        IoReleaseCancelSpinLock(oldIrql);

    }
#endif
    //
    // Cancel any pending wait-wake irps
    //

    if (pDevExt->PendingWakeIrp != NULL) {
       IoCancelIrp(pDevExt->PendingWakeIrp);
       pDevExt->PendingWakeIrp = NULL;
    }

    //
    // Finally, dump any stalled IRPS
    //

//    SerialKillAllStalled(PDevObj);


    Ik220Dump (SERTRACECALLS,("IK220: Leave Ik220KillPendingIrps\n"));
}


NTSTATUS
Ik220InitController(IN PDEVICE_OBJECT PDevObj, IN PCONFIG_DATA PConfigData)
/*++

Routine Description:

    Really too many things to mention here.  In general initializes
    kernel synchronization structures, allocates the typeahead buffer,
    sets up defaults, etc.

Arguments:

    PDevObj       - Device object for the device to be started

    PConfigData   - Pointer to a record for a single port.

Return Value:

    STATUS_SUCCCESS if everything went ok.  A !NT_SUCCESS status
    otherwise.

--*/
{

   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;

   //
   // This will hold the string that we need to use to describe
   // the name of the device to the IO system.
   //

//   UNICODE_STRING uniNameString;

   //
   // Holds the NT Status that is returned from each call to the
   // kernel and executive.
   //

   NTSTATUS status = STATUS_SUCCESS;

   //
   // Indicates that a conflict was detected for resources
   // used by this device.
   //

   int ii;
   int jj;
   BOOLEAN conflictDetected = FALSE;

   PAGED_CODE();


   Ik220Dump(
             SERDIAG1,
             ("IK220: Initializing for configuration record of %wZ\n",
              &pDevExt->DeviceName)
             );


   //
   // Initialize the spinlock associated with fields read (& set)
   // by IO Control functions and the flags spinlock.
   //

   KeInitializeSpinLock(&pDevExt->ControlLock);
   KeInitializeSpinLock(&pDevExt->FlagsLock);

#if 0
   //
   // Initialize the timers used to timeout operations.
   //

   KeInitializeTimer(&pDevExt->ReadRequestTotalTimer);
   KeInitializeTimer(&pDevExt->ReadRequestIntervalTimer);
   KeInitializeTimer(&pDevExt->WriteRequestTotalTimer);
   KeInitializeTimer(&pDevExt->ImmediateTotalTimer);
   KeInitializeTimer(&pDevExt->XoffCountTimer);
   KeInitializeTimer(&pDevExt->LowerRTSTimer);


   //
   // Intialialize the dpcs that will be used to complete
   // or timeout various IO operations.
   //

   KeInitializeDpc(&pDevExt->CompleteWriteDpc, SerialCompleteWrite, pDevExt);
   KeInitializeDpc(&pDevExt->CompleteReadDpc, SerialCompleteRead, pDevExt);
   KeInitializeDpc(&pDevExt->TotalReadTimeoutDpc, SerialReadTimeout, pDevExt);
   KeInitializeDpc(&pDevExt->IntervalReadTimeoutDpc, SerialIntervalReadTimeout,
                   pDevExt);
   KeInitializeDpc(&pDevExt->TotalWriteTimeoutDpc, SerialWriteTimeout, pDevExt);
   KeInitializeDpc(&pDevExt->CommErrorDpc, SerialCommError, pDevExt);
   KeInitializeDpc(&pDevExt->CompleteImmediateDpc, SerialCompleteImmediate,
                   pDevExt);
   KeInitializeDpc(&pDevExt->TotalImmediateTimeoutDpc, SerialTimeoutImmediate,
                   pDevExt);
   KeInitializeDpc(&pDevExt->CommWaitDpc, SerialCompleteWait, pDevExt);
   KeInitializeDpc(&pDevExt->XoffCountTimeoutDpc, SerialTimeoutXoff, pDevExt);
   KeInitializeDpc(&pDevExt->XoffCountCompleteDpc, SerialCompleteXoff, pDevExt);
   KeInitializeDpc(&pDevExt->StartTimerLowerRTSDpc, SerialStartTimerLowerRTS,
                   pDevExt);
   KeInitializeDpc(&pDevExt->PerhapsLowerRTSDpc, SerialInvokePerhapsLowerRTS,
                   pDevExt);
   KeInitializeDpc(&pDevExt->IsrUnlockPagesDpc, SerialUnlockPages, pDevExt);
#endif

#if 0 // DBG
   //
   // Init debug stuff
   //

   pDevExt->DpcQueued[0].Dpc = &pDevExt->CompleteWriteDpc;
   pDevExt->DpcQueued[1].Dpc = &pDevExt->CompleteReadDpc;
   pDevExt->DpcQueued[2].Dpc = &pDevExt->TotalReadTimeoutDpc;
   pDevExt->DpcQueued[3].Dpc = &pDevExt->IntervalReadTimeoutDpc;
   pDevExt->DpcQueued[4].Dpc = &pDevExt->TotalWriteTimeoutDpc;
   pDevExt->DpcQueued[5].Dpc = &pDevExt->CommErrorDpc;
   pDevExt->DpcQueued[6].Dpc = &pDevExt->CompleteImmediateDpc;
   pDevExt->DpcQueued[7].Dpc = &pDevExt->TotalImmediateTimeoutDpc;
   pDevExt->DpcQueued[8].Dpc = &pDevExt->CommWaitDpc;
   pDevExt->DpcQueued[9].Dpc = &pDevExt->XoffCountTimeoutDpc;
   pDevExt->DpcQueued[10].Dpc = &pDevExt->XoffCountCompleteDpc;
   pDevExt->DpcQueued[11].Dpc = &pDevExt->StartTimerLowerRTSDpc;
   pDevExt->DpcQueued[12].Dpc = &pDevExt->PerhapsLowerRTSDpc;
   pDevExt->DpcQueued[13].Dpc = &pDevExt->IsrUnlockPagesDpc;

#endif



   //
   // Map the memory for the control registers for the ik220 device
   // into virtual memory.
   //
   pDevExt->Controller =
      Ik220GetMappedAddress(PConfigData->InterfaceType,
                             PConfigData->BusNumber,
                             PConfigData->TrController,
                             PConfigData->SpanOfController,
                             (BOOLEAN)PConfigData->AddressSpace,
                             &pDevExt->UnMapRegisters);

   if (!pDevExt->Controller) {

      Ik220LogError(
                    PDevObj->DriverObject,
                    pDevExt->DeviceObject,
                    PConfigData->TrController,
                    Ik220PhysicalZero,
                    0,
                    0,
                    0,
                    7,
                    STATUS_SUCCESS,
                    IK220_REGISTERS_NOT_MAPPED,
                    pDevExt->DeviceName.Length+sizeof(WCHAR),
                    pDevExt->DeviceName.Buffer,
                    0,
                    NULL
                    );

      Ik220Dump(
                SERERRORS,
                ("IK220: Could not map memory for device registers for %wZ\n",
                 &pDevExt->DeviceName)
                );

      pDevExt->UnMapRegisters = FALSE;
      status = STATUS_NONE_MAPPED;
      goto ExtensionCleanup;

   }

   pDevExt->AddressSpace          = PConfigData->AddressSpace;
   pDevExt->OriginalController    = PConfigData->Controller;
   pDevExt->SpanOfController      = PConfigData->SpanOfController;



   //
   // Map the memory for the port base for the ik220 device
   // into virtual memory.
   //

   for (ii = 0; ii < 2; ++ii) {
      pDevExt->PortBase[ii] =
         Ik220GetMappedAddress(PConfigData->InterfaceType,
                                PConfigData->PortBaseBusNumber[ii],
                                PConfigData->TrPortBase[ii],
                                PConfigData->SpanOfPortBase[ii],
                                (BOOLEAN)PConfigData->PortBaseAddressSpace[ii],
                                &pDevExt->UnMapPortBase[ii]);

      Ik220Globals.GloPortBase[pDevExt->instance*2+ii] = pDevExt->PortBase[ii];

      if (!pDevExt->PortBase[ii]) {

         Ik220LogError(
                       PDevObj->DriverObject,
                       pDevExt->DeviceObject,
                       PConfigData->TrPortBase[ii],
                       Ik220PhysicalZero,
                       0,
                       0,
                       0,
                       7,
                       STATUS_SUCCESS,
                       IK220_REGISTERS_NOT_MAPPED,
                       pDevExt->DeviceName.Length+sizeof(WCHAR),
                       pDevExt->DeviceName.Buffer,
                       0,
                       NULL
                       );

         Ik220Dump(
                   SERERRORS,
                   ("IK220: Could not map memory for device registers for %wZ\n",
                    &pDevExt->DeviceName)
                   );

         //
         // Manually unmap the other register here if necessary
         //

         if (pDevExt->UnMapRegisters) {
            MmUnmapIoSpace((PVOID)PConfigData->TrController.QuadPart,
                           PConfigData->SpanOfController);
         }

         pDevExt->UnMapRegisters = FALSE;

         for (jj = 0; jj < ii; ++ii) {
            if (pDevExt->UnMapPortBase[jj] ) {
               MmUnmapIoSpace((PVOID)PConfigData->TrPortBase[jj].QuadPart,
                              PConfigData->SpanOfPortBase[jj]);
               pDevExt->UnMapPortBase[jj] = FALSE;
            }
         }
         pDevExt->UnMapPortBase[ii] = FALSE;
         status = STATUS_NONE_MAPPED;
         goto ExtensionCleanup;

      }

      pDevExt->PortBaseAddressSpace[ii] = PConfigData->PortBaseAddressSpace[ii];
      pDevExt->OriginalPortBase[ii]     = PConfigData->PortBase[ii];
      pDevExt->SpanOfPortBase[ii]       = PConfigData->SpanOfPortBase[ii];
   }


   pDevExt->ConfBase =
      Ik220GetMappedAddress(PConfigData->InterfaceType,
                             PConfigData->ConfBaseBusNumber,
                             PConfigData->TrConfBase,
                             PConfigData->SpanOfConfBase,
                             (BOOLEAN)PConfigData->ConfBaseAddressSpace,
                             &pDevExt->UnMapConfBase);

   Ik220Globals.GloConfBase[pDevExt->instance] = pDevExt->ConfBase;

   if (!pDevExt->ConfBase) {

      Ik220LogError(
                    PDevObj->DriverObject,
                    pDevExt->DeviceObject,
                    PConfigData->TrConfBase,
                    Ik220PhysicalZero,
                    0,
                    0,
                    0,
                    7,
                    STATUS_SUCCESS,
                    IK220_REGISTERS_NOT_MAPPED,
                    pDevExt->DeviceName.Length+sizeof(WCHAR),
                    pDevExt->DeviceName.Buffer,
                    0,
                    NULL
                    );

      Ik220Dump(
                SERERRORS,
                ("IK220: Could not map memory for device registers for %wZ\n",
                 &pDevExt->DeviceName)
                );

      //
      // Manually unmap the other register here if necessary
      //

      if (pDevExt->UnMapRegisters) {
         MmUnmapIoSpace((PVOID)PConfigData->TrController.QuadPart,
                        PConfigData->SpanOfController);
      }

      pDevExt->UnMapRegisters = FALSE;

      for (jj = 0; jj < 2; ++jj) {
         if (pDevExt->UnMapPortBase[jj] ) {
            MmUnmapIoSpace((PVOID)PConfigData->TrPortBase[jj].QuadPart,
                           PConfigData->SpanOfPortBase[jj]);
            pDevExt->UnMapPortBase[jj] = FALSE;
         }
      }
      pDevExt->UnMapConfBase = FALSE;
      status = STATUS_NONE_MAPPED;
      goto ExtensionCleanup;

   }

   pDevExt->ConfBaseAddressSpace = PConfigData->ConfBaseAddressSpace;
   pDevExt->OriginalConfBase     = PConfigData->ConfBase;
   pDevExt->SpanOfConfBase       = PConfigData->SpanOfPortBase[0];


   //
   // if we have an interrupt status then map it.
   //

//   pDevExt->InterruptStatus =
//      (PUCHAR)PConfigData->TrInterruptStatus.QuadPart;
//
//   if (pDevExt->InterruptStatus) {
//
//      pDevExt->InterruptStatus
//         = Ik220GetMappedAddress(PConfigData->InterfaceType,
//                                  PConfigData->BusNumber,
//                                  PConfigData->TrInterruptStatus,
//                                  PConfigData->SpanOfInterruptStatus,
//                                  (BOOLEAN)PConfigData->AddressSpace,
//                                  &pDevExt->UnMapStatus);
//
//
//      if (!pDevExt->InterruptStatus) {
//
//         Ik220LogError(
//                       PDevObj->DriverObject,
//                       PDevObj,
//                       PConfigData->TrController,
//                       Ik220PhysicalZero,
//                       0,
//                       0,
//                       0,
//                       8,
//                       STATUS_SUCCESS,
//                       IK220_REGISTERS_NOT_MAPPED,
//                       pDevExt->DeviceName.Length+sizeof(WCHAR),
//                       pDevExt->DeviceName.Buffer,
//                       0,
//                       NULL
//                       );
//
//         Ik220Dump(SERERRORS, ("IK220: Could not map memory for interrupt "
//                                "status for %wZ\n",
//                  &pDevExt->DeviceName));
//
//         //
//         // Manually unmap the other register here if necessary
//         //
//
//         if (pDevExt->UnMapRegisters) {
//            MmUnmapIoSpace((PVOID)PConfigData->TrController.QuadPart,
//                           PConfigData->SpanOfController);
//         }
//
//         pDevExt->UnMapRegisters = FALSE;
//         pDevExt->UnMapStatus = FALSE;
//         status = STATUS_NONE_MAPPED;
//         goto ExtensionCleanup;
//
//      }
//
//      pDevExt->OriginalInterruptStatus = PConfigData->InterruptStatus;
//      pDevExt->SpanOfInterruptStatus = PConfigData->SpanOfInterruptStatus;
//
//
//   }

#if 0
   //
   // Shareable interrupt?
   //

   if ((BOOLEAN)PConfigData->PermitSystemWideShare) {
      pDevExt->InterruptShareable = TRUE;
   }
#endif

   //
   // Save off the interface type and the bus number.
   //

   pDevExt->InterfaceType = PConfigData->InterfaceType;
   pDevExt->BusNumber     = PConfigData->BusNumber;
   for (ii = 0; ii < 2; ++ii) {
      pDevExt->PortBaseBusNumber[ii] = PConfigData->PortBaseBusNumber[ii];
   }
   pDevExt->ConfBaseBusNumber = PConfigData->ConfBaseBusNumber;

   pDevExt->PortIndex = PConfigData->PortIndex;
//   pDevExt->Indexed = (BOOLEAN)PConfigData->Indexed;
//   pDevExt->MaskInverted = PConfigData->MaskInverted;

   //
   // Get the translated interrupt vector, level, and affinity
   //

   pDevExt->OriginalIrql      = PConfigData->OriginalIrql;
   pDevExt->OriginalVector    = PConfigData->OriginalVector;


   //
   // PnP uses the passed translated values rather than calling
   // HalGetInterruptVector()
   //

   pDevExt->Vector = PConfigData->TrVector;
   pDevExt->Irql = (UCHAR)PConfigData->TrIrql;


   //
   // Mark this device as not being opened by anyone.  We keep a
   // variable around so that spurious interrupts are easily
   // dismissed by the ISR.
   //

   pDevExt->DeviceIsOpened = FALSE;

   //
   // Store values into the extension for interval timing.
   //

   //
   // If the interval timer is less than a second then come
   // in with a short "polling" loop.
   //
   // For large (> then 2 seconds) use a 1 second poller.
   //

//   pDevExt->ShortIntervalAmount.QuadPart  = -1;
//   pDevExt->LongIntervalAmount.QuadPart   = -10000000;
//   pDevExt->CutOverAmount.QuadPart        = 200000000;


   //
   // Common error path cleanup.  If the status is
   // bad, get rid of the device extension, device object
   // and any memory associated with it.
   //

ExtensionCleanup: ;
   if (!NT_SUCCESS(status)) {

      if (pDevExt->UnMapRegisters) {
         MmUnmapIoSpace(pDevExt->Controller, pDevExt->SpanOfController);
      }

      for (ii = 0; ii < 2; ++ii) {
         if (pDevExt->UnMapPortBase[ii]) {
            MmUnmapIoSpace(pDevExt->PortBase[ii], pDevExt->SpanOfPortBase[ii]);
            Ik220Globals.GloPortBase[pDevExt->instance*2+ii] = 0;
         }
      }

      if (pDevExt->UnMapConfBase) {
         MmUnmapIoSpace(pDevExt->ConfBase, pDevExt->SpanOfConfBase);
         Ik220Globals.GloConfBase[pDevExt->instance] = 0;
      }

      if (pDevExt->UnMapStatus) {
//         MmUnmapIoSpace(pDevExt->InterruptStatus,
//                        pDevExt->SpanOfInterruptStatus);
      }

   }

   return status;

}


NTSTATUS
Ik220InitOneController(IN PDEVICE_OBJECT PDevObj, IN PCONFIG_DATA PConfigData)
/*++

Routine Description:

    This routine will call the real port initialization code.  It sets
    up the ISR and Context correctly for a one port device.

Arguments:

    All args are simply passed along.

Return Value:

    Status returned from the controller initialization routine.

--*/
{

   NTSTATUS status;
   PIK220_DEVICE_EXTENSION pDevExt;

   PAGED_CODE();

   status = Ik220InitController(PDevObj, PConfigData);

   if (NT_SUCCESS(status)) {

      pDevExt = PDevObj->DeviceExtension;

      //
      // We successfully initialized the single controller.

   }

   return status;

}


BOOLEAN
Ik220Reset(
           IN PVOID Context
           )
/*++

Routine Description:

    This places the hardware in a standard configuration.

    NOTE: This assumes that it is called at interrupt level.


Arguments:

    Context - The device extension for ik220 device
    being managed.

Return Value:

    Always FALSE.

--*/
{

   PIK220_DEVICE_EXTENSION extension = Context;
   IK220_LOCKED_PAGED_CODE();


   //
   // Enable which interrupts we want to receive.
   //
   // NOTE NOTE: This does not actually let interrupts
   // occur.  We must still raise the OUT2 bit in the
   // modem control register.  We will do that on open.
   //

   ENABLE_ALL_INTERRUPTS(extension->Controller);



   return FALSE;
}


VOID
Ik220Unload(
            IN PDRIVER_OBJECT DriverObject
            )
/*++

Routine Description:

    This routine is defunct since all device objects are removed before
    the driver is unloaded.

Arguments:

    DriverObject - Pointer to the driver object controling all of the
                   devices.

Return Value:

    None.

--*/
{
   PVOID lockPtr;

   PAGED_CODE();

   lockPtr = MmLockPagableCodeSection(Ik220Unload);

   //
   // Unnecessary since our BSS is going away, but do it anyhow to be safe
   //

   Ik220Globals.PAGEIK_Handle = NULL;

   if (Ik220Globals.RegistryPath.Buffer != NULL) {
      ExFreePool(Ik220Globals.RegistryPath.Buffer);
      Ik220Globals.RegistryPath.Buffer = NULL;
   }

#if DBG
   Ik220LogFree();
#endif

   Ik220Dump(
             SERDIAG3,
             ("IK220: In Ik220Unload\n")
             );

   MmUnlockPagableImageSection(lockPtr);

}


PVOID
Ik220GetMappedAddress(
                      IN INTERFACE_TYPE BusType,
                      IN ULONG BusNumber,
                      PHYSICAL_ADDRESS IoAddress,
                      ULONG NumberOfBytes,
                      ULONG AddressSpace,
                      PBOOLEAN MappedAddress
                      )
/*++

Routine Description:

    This routine maps an IO address to system address space.

Arguments:

    BusType - what type of bus - eisa, mca, isa
    IoBusNumber - which IO bus (for machines with multiple buses).
    IoAddress - base device address to be mapped.
    NumberOfBytes - number of bytes for which address is valid.
    AddressSpace - Denotes whether the address is in io space or memory.
    MappedAddress - indicates whether the address was mapped.
                    This only has meaning if the address returned
                    is non-null.

Return Value:

    Mapped address

--*/
{
//   PHYSICAL_ADDRESS cardAddress;
   PVOID address;

   PAGED_CODE();

   //
   // Map the device base address into the virtual address space
   // if the address is in memory space.
   //

   if (!AddressSpace) {

      address = MmMapIoSpace(
                            IoAddress,
                            NumberOfBytes,
                            FALSE
                            );

      *MappedAddress = (BOOLEAN)((address)?(TRUE):(FALSE));


   } else {

      address = (PVOID)IoAddress.LowPart;
      *MappedAddress = FALSE;

   }

   return address;

}


IK220_MEM_COMPARES
Ik220MemCompare(
                IN PHYSICAL_ADDRESS A,
                IN ULONG SpanOfA,
                IN PHYSICAL_ADDRESS B,
                IN ULONG SpanOfB
                )
/*++

Routine Description:

    Compare two phsical address.

Arguments:

    A - One half of the comparison.

    SpanOfA - In units of bytes, the span of A.

    B - One half of the comparison.

    SpanOfB - In units of bytes, the span of B.


Return Value:

    The result of the comparison.

--*/
{

   LARGE_INTEGER a;
   LARGE_INTEGER b;

   LARGE_INTEGER lower;
   ULONG lowerSpan;
   LARGE_INTEGER higher;

   PAGED_CODE();

   a = A;
   b = B;

   if (a.QuadPart == b.QuadPart) {

      return AddressesAreEqual;

   }

   if (a.QuadPart > b.QuadPart) {

      higher = a;
      lower = b;
      lowerSpan = SpanOfB;

   } else {

      higher = b;
      lower = a;
      lowerSpan = SpanOfA;

   }

   if ((higher.QuadPart - lower.QuadPart) >= lowerSpan) {

      return AddressesAreDisjoint;

   }

   return AddressesOverlap;

}


NTSTATUS
Ik220FindInitController(IN PDEVICE_OBJECT PDevObj, IN PCONFIG_DATA PConfig)
/*++

Routine Description:

    This function discovers what type of controller is responsible for
    the given port and initializes the controller and port.

Arguments:

    PDevObj - Pointer to the devobj for the port we are about to init.

    PConfig - Pointer to configuration data for the port we are about to init.

Return Value:

    STATUS_SUCCESS on success, appropriate error value on failure.

--*/
{

   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
//   PDEVICE_OBJECT pDeviceObject;
   PIK220_DEVICE_EXTENSION pExtension;
//   PHYSICAL_ADDRESS serialPhysicalMax;
//   IK220_LIST_DATA listAddition;
   BOOLEAN didInit = FALSE;
   PLIST_ENTRY pCurDevObj;
   NTSTATUS status;

   PAGED_CODE();


   Ik220Dump(SERTRACECALLS, ("IK220: entering Ik220FindInitController\n"));


   //
   // Loop through all of the driver's device objects making
   // sure that this new record doesn't overlap with any of them.
   //

   if (!IsListEmpty(&Ik220Globals.AllDevObjs)) {
      pCurDevObj = Ik220Globals.AllDevObjs.Flink;
      pExtension = CONTAINING_RECORD(pCurDevObj, IK220_DEVICE_EXTENSION,
                                     AllDevObjs);
   } else {
      pCurDevObj = NULL;
      pExtension = NULL;
   }


   while (pCurDevObj != NULL
          && pCurDevObj != &Ik220Globals.AllDevObjs) {
      //
      // We only care about this list if the elements are on the
      // same bus as this new entry.
      //

      if ((pExtension->InterfaceType  == PConfig->InterfaceType) &&
          (pExtension->AddressSpace   == PConfig->AddressSpace)  &&
          (pExtension->BusNumber      == PConfig->BusNumber)) {

         Ik220Dump(
                   SERDIAG1,
                   ("IK220: Comparing it to %wZ\n"
                    "------- already in the device list\n"
                    "------- PortAddress is %x\n"
//                    "------- Interrupt Status is %x\n"
                    "------- BusNumber is %d\n"
                    "------- BusType is %d\n"
                    "------- AddressSpace is %d\n",
                    &pExtension->DeviceName,
                    pExtension->OriginalController.LowPart,
//                    pExtension->OriginalInterruptStatus.LowPart,
                    pExtension->BusNumber,
                    pExtension->InterfaceType,
                    pExtension->AddressSpace
                   )
                   );

         //
         // Check to see if the controller addresses are not equal.
         //

         if (Ik220MemCompare(
                             PConfig->Controller,
                             PConfig->SpanOfController,
                             pExtension->OriginalController,
                             pExtension->SpanOfController
                             ) != AddressesAreDisjoint) {

            //
            // We don't want to log an error if the addresses
            // are the same and the name is the same and
            // the new item is from the firmware.
            //

            Ik220Dump(
                      SERERRORS,
                      ("IK220: Error in config record for %wZ\n"
                       "------- Register address overlaps with\n"
                       "------- previous ik220 device\n",
                       &pDevExt->DeviceName)
                      );

            return STATUS_NO_SUCH_DEVICE;
         }

      }

      pCurDevObj = pCurDevObj->Flink;

      if (pCurDevObj != NULL) {
         pExtension = CONTAINING_RECORD(pCurDevObj, IK220_DEVICE_EXTENSION,
                                        AllDevObjs);
      }
   }   // while (pCurDevObj != NULL && pCurDevObj != &Ik220Globals.AllDevObjs)



   //
   // Now, we will check if this is a port on a multiport card.
   // The conditions are same ISR set and same IRQL/Vector
   //

   //
   // Loop through all previously attached devices
   //

   if (!IsListEmpty(&Ik220Globals.AllDevObjs)) {
      pCurDevObj = Ik220Globals.AllDevObjs.Flink;
      pExtension = CONTAINING_RECORD(pCurDevObj, IK220_DEVICE_EXTENSION,
                                     AllDevObjs);
   } else {
      pCurDevObj = NULL;
      pExtension = NULL;
   }



   Ik220Dump(SERDIAG1, ("IK220: Aha! It is a standalone node or first multi"
                         "\n"));
   status = Ik220InitOneController(PDevObj, PConfig);

   if (!NT_SUCCESS(status)) {
      return status;
   }

   //
   // The device is initialized.  Now we need to check if
   // this device shares an interrupt with anyone.
   //


   //
   // Loop through all previously attached devices
   //

   if (!IsListEmpty(&Ik220Globals.AllDevObjs)) {
      pCurDevObj = Ik220Globals.AllDevObjs.Flink;
      pExtension = CONTAINING_RECORD(pCurDevObj, IK220_DEVICE_EXTENSION,
                                     AllDevObjs);
   } else {
      pCurDevObj = NULL;
      pExtension = NULL;
   }

   //
   // Go through the list again looking for previous devices
   // with the same interrupt.  The first one found will either be a root
   // or standalone.  Order of insertion is important here!
   //

   if (pCurDevObj != NULL) {
      do {

         //
         // We only care about interrupts that are on
         // the same bus.
         //

         if ((pExtension->Irql == PConfig->TrIrql) &&
             (pExtension->Vector == PConfig->TrVector)) {
            pExtension->NewExtension = pDevExt;

            //
            // We will share another's CIsrSw so we can free the one
            // allocated for us during init
            //


//            ExFreePool(pDevExt->CIsrSw);

            Ik220Dump(SERDIAG1, ("Becoming sharer: %08X %08X\n",
                                  pExtension, pExtension->OriginalIrql));


            return STATUS_SUCCESS;

         }


         //
         // No match, check some more
         //

         pCurDevObj = pCurDevObj->Flink;

         if (pCurDevObj != NULL) {
            pExtension = CONTAINING_RECORD(pCurDevObj, IK220_DEVICE_EXTENSION,
                                           AllDevObjs);
         }
      } while (pCurDevObj != NULL
               && pCurDevObj != &Ik220Globals.AllDevObjs);
   }

   return STATUS_SUCCESS;
}

