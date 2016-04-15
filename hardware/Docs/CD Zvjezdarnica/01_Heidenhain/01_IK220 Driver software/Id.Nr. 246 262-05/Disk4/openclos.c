
#include <ntddk.h>
#include <IK220DRV.h>
#include <ik220.h>



BOOLEAN
Ik220MarkOpen(
    IN PVOID Context
    );

BOOLEAN
Ik220CheckOpen(
    IN PVOID Context
    );

NTSTATUS
Ik220CreateOpen(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );


#ifdef ALLOC_PRAGMA

//
// Paged for open and PnP transactions
//

#pragma alloc_text(PAGEIK, Ik220Cleanup)
#pragma alloc_text(PAGEIK, Ik220Close)
#pragma alloc_text(PAGEIK, Ik220CheckOpen)
#pragma alloc_text(PAGEIK, Ik220MarkOpen)

//
// Always paged
//

#pragma alloc_text(PAGESRP0,Ik220CreateOpen)
#endif // ALLOC_PRAGMA

typedef struct _IK220_CHECK_OPEN {
    PIK220_DEVICE_EXTENSION Extension;
    NTSTATUS *StatusOfOpen;
    } IK220_CHECK_OPEN,*PIK220_CHECK_OPEN;


NTSTATUS
Ik220CreateOpen(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
/*++

Routine Description:

    We connect up to the interrupt for the create/open and initialize
    the structures needed to maintain an open for a device.

Arguments:

    DeviceObject - Pointer to the device object for this device

    Irp - Pointer to the IRP for the current request

Return Value:

    The function value is the final status of the call

--*/
{

    PIK220_DEVICE_EXTENSION extension = DeviceObject->DeviceExtension;
    IK220_CHECK_OPEN checkOpen;
    NTSTATUS localStatus;

    PAGED_CODE();

    if (extension->PNPState != IK220_PNP_STARTED) {
       Irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
       IoCompleteRequest(Irp, IO_NO_INCREMENT);
       return STATUS_INSUFFICIENT_RESOURCES;
    }

    //
    // Lock out changes to PnP state until we have our open state decided
    //

    ExAcquireFastMutex(&extension->OpenMutex);

    if ((localStatus = Ik220IRPPrologue(Irp, extension)) != STATUS_SUCCESS) {
       ExReleaseFastMutex(&extension->OpenMutex);
       Ik220CompleteRequest(extension, Irp, IO_NO_INCREMENT);
       return localStatus;
    }

    if (InterlockedIncrement(&extension->OpenCount) != 1) {
       ExReleaseFastMutex(&extension->OpenMutex);
       InterlockedDecrement(&extension->OpenCount);
       Irp->IoStatus.Status = STATUS_ACCESS_DENIED;
       Ik220CompleteRequest(extension, Irp, IO_NO_INCREMENT);
       return STATUS_ACCESS_DENIED;
    }

    Ik220Dump(
        SERIRPPATH,
        ("IK220: Dispatch entry for: %x\n",Irp)
        );
    Ik220Dump(
        SERDIAG3,
        ("IK220: In Ik220CreateOpen\n")
        );

    //
    // Before we do anything, let's make sure they aren't trying
    // to create a directory.  This is a silly, but what's a driver to do!?
    //

    if (IoGetCurrentIrpStackLocation(Irp)->Parameters.Create.Options &
        FILE_DIRECTORY_FILE) {
        ExReleaseFastMutex(&extension->OpenMutex);

        Irp->IoStatus.Status = STATUS_NOT_A_DIRECTORY;
        Irp->IoStatus.Information = 0;

        Ik220Dump(
            SERIRPPATH,
            ("IK220: Complete Irp: %x\n",Irp)
            );



        InterlockedDecrement(&extension->OpenCount);
        Ik220CompleteRequest(extension, Irp, IO_NO_INCREMENT);

        return STATUS_NOT_A_DIRECTORY;

    }

    //
    // Create a buffer for the RX data when no reads are outstanding.
    //

    extension->BufferSize = 0;

    switch (MmQuerySystemSize()) {

        case MmLargeSystem: {

            extension->BufferSize = 4096;

        }

        case MmMediumSystem: {

            extension->BufferSize = 1024;

        }

        case MmSmallSystem: {

            extension->BufferSize = 128;

        }

    }

    //
    // Ok, it looks like we really are going to open.  Lock down the
    // driver.
    //
    Ik220LockPagableSectionByHandle(Ik220Globals.PAGEIK_Handle);

    //
    // Power up the stack
    //

    (void)Ik220GotoPowerState(DeviceObject, extension, PowerDeviceD0);

    //
    // Not currently waiting for wake up
    //

    extension->SendWaitWake = FALSE;

    //
    // On a new open we "flush" the read queue by initializing the
    // count of characters.
    //

//    //
//    // Mark the device as busy for WMI
//    //
//
//    extension->WmiCommData.IsBusy = TRUE;

    extension->IrpMaskLocation = NULL;
    extension->HistoryMask = 0;
    extension->IsrWaitMask = 0;

#if !DBG
    //
    // Clear out the statistics.
    //

//    Ik220ClearStats(extension);
#endif


    checkOpen.Extension = extension;
    checkOpen.StatusOfOpen = &Irp->IoStatus.Status;

    Ik220CheckOpen(&checkOpen);


    //
    // We have been marked open, so now the PnP state can change
    //

    ExReleaseFastMutex(&extension->OpenMutex);

    localStatus = Irp->IoStatus.Status;
    Irp->IoStatus.Information=0L;

    Ik220Dump(
        SERIRPPATH,
        ("IK220: Complete Irp: %x\n",Irp)
        );

    if (!NT_SUCCESS(localStatus)) {
       InterlockedDecrement(&extension->OpenCount);
    }

    Ik220CompleteRequest(extension, Irp, IO_NO_INCREMENT);

    return localStatus;

}


NTSTATUS
Ik220Close(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
/*++

Routine Description:

    We simply disconnect the interrupt for now.

Arguments:

    DeviceObject - Pointer to the device object for this device

    Irp - Pointer to the IRP for the current request

Return Value:

    The function value is the final status of the call

--*/
{

    //
    // This "timer value" is used to wait 10 character times
    // after the hardware is empty before we actually "run down"
    // all of the flow control/break junk.
    //
//    LARGE_INTEGER tenCharDelay;

    //
    // Holds a character time.
    //
//    LARGE_INTEGER charTime;

    //
    // Just what it says.  This is the Ik220 specific device
    // extension of the device object create for the Ik220 driver.
    //
    PIK220_DEVICE_EXTENSION extension = DeviceObject->DeviceExtension;

    NTSTATUS status;

    //
    // Number of opens still active
    //

    LONG openCount;

    //
    // Number of DPC's still pending
    //

    ULONG pendingDPCs;

    //
    // Grab a mutex
    //

    ExAcquireFastMutex(&extension->CloseMutex);


    //
    // We succeed a close on a removing device
    //

    if ((status = Ik220IRPPrologue(Irp, extension)) != STATUS_SUCCESS) {
       Ik220Dump(SERERRORS, ("IK220: Close prologue failed for: %x\n",Irp));
       if (status == STATUS_DELETE_PENDING) {
             extension->BufferSize = 0;
             status = Irp->IoStatus.Status = STATUS_SUCCESS;
       }

       Ik220CompleteRequest(extension, Irp, IO_NO_INCREMENT);
       openCount = InterlockedDecrement(&extension->OpenCount);
       ASSERT(openCount == 0);
       ExReleaseFastMutex(&extension->CloseMutex);
       return status;
    }

    ASSERT(extension->OpenCount == 1);

    if (extension->OpenCount != 1) {
       Ik220Dump(SERERRORS, ("IK220: Close open count bad for: 0x%x\n",Irp));
       Ik220Dump(SERERRORS, ("------: Count: %x  Addr: 0x%x\n",
                              extension->OpenCount, &extension->OpenCount));
       ExReleaseFastMutex(&extension->CloseMutex);
       Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
       Ik220CompleteRequest(extension, Irp, IO_NO_INCREMENT);
       return STATUS_INVALID_DEVICE_REQUEST;
    }

    Ik220Dump(
        SERIRPPATH,
        ("IK220: Dispatch entry for: %x\n",Irp)
        );
    Ik220Dump(
        SERDIAG3,
        ("IK220: In Ik220Close\n")
        );

//    charTime.QuadPart = -SerialGetCharTime(extension).QuadPart;

    //
    // Do this now so that if the isr gets called it won't do anything
    // to cause more chars to get sent.  We want to run down the hardware.
    //

    extension->DeviceIsOpened = FALSE;

    //
    // Synchronize with the ISR to let it know that interrupts are
    // no longer important.
    //

    Ik220MarkClose(extension);


    //
    // The hardware is empty.  Delay 10 character times before
    // shut down all the flow control.
    //

//    tenCharDelay.QuadPart = charTime.QuadPart * 10;
//
//    KeDelayExecutionThread(
//        KernelMode,
//        TRUE,
//        &tenCharDelay
//        );
//
//
//    //
//    // Mark device as not busy for WMI
//    //
//
//    extension->WmiCommData.IsBusy = FALSE;

    //
    // All is done.  The port has been disabled from interrupting
    // so there is no point in keeping the memory around.
    //

    extension->BufferSize = 0;

    //
    // Stop waiting for wakeup
    //

    extension->SendWaitWake = FALSE;

    if (extension->PendingWakeIrp != NULL) {
       IoCancelIrp(extension->PendingWakeIrp);
    }

    //
    // Power down our device stack
    //

    (void)Ik220GotoPowerState(DeviceObject, extension, PowerDeviceD3);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information=0L;

    Ik220Dump(
        SERIRPPATH,
        ("IK220: Complete Irp: %x\n",Irp)
        );
    Ik220CompleteRequest(extension, Irp, IO_NO_INCREMENT);

    //
    // Unlock the pages.  If this is the last reference to the section
    // then the driver code will be flushed out.
    //

    //
    // First, we have to let the DPC's drain.  No more should be queued
    // since we aren't taking interrupts now....
    //

    pendingDPCs = InterlockedDecrement(&extension->DpcCount);
    LOGENTRY(LOG_CNT, 'DpD7', 0, extension->DpcCount, 0);

    if (pendingDPCs) {
       Ik220Dump(SERDIAG1, ("IK220: Drainging DPC's: %x\n",Irp));
       KeWaitForSingleObject(&extension->PendingDpcEvent, Executive,
                             KernelMode, FALSE, NULL);
    }


    Ik220Dump(SERDIAG1, ("IK220: DPC's drained: %x\n",Irp));



    //
    // Pages must be locked to release the mutex, so don't unlock
    // them until after we release the mutex
    //

    ExReleaseFastMutex(&extension->CloseMutex);

    //
    // Reset for next open
    //

    InterlockedIncrement(&extension->DpcCount);
    LOGENTRY(LOG_CNT, 'DpI6', 0, extension->DpcCount, 0);

    openCount = InterlockedDecrement(&extension->OpenCount);

    ASSERT(openCount == 0);
    Ik220UnlockPagableImageSection(Ik220Globals.PAGEIK_Handle);

    return STATUS_SUCCESS;

}


BOOLEAN
Ik220CheckOpen(
    IN PVOID Context
    )
/*++

Routine Description:

    This routine will traverse the circular doubly linked list
    of devices that are using the same interrupt object.  It will look
    for other devices that are open.  If it doesn't find any
    it will indicate that it is ok to open this device.

    If it finds another device open we have two cases:

        1) The device we are trying to open is on a multiport card.

           If the already open device is part of a multiport device
           this code will indicate it is ok to open.  We do this on the
           theory that the multiport devices are daisy chained
           and the cards can correctly arbitrate the interrupt
           line.  Note this assumption could be wrong.  Somebody
           could put two non-daisychained multiports on the
           same interrupt.  However, only a total clod would do
           such a thing, and in my opinion deserves everthing they
           get.

        2) The device we are trying to open is not on a multiport card.

            We indicate that it is not ok to open.

Arguments:

    Context - This is a structure that contains a pointer to the
              extension of the device we are trying to open, and
              a pointer to an NTSTATUS that will indicate whether
              the device was opened or not.

Return Value:

    This routine always returns FALSE.

--*/
{

    PIK220_DEVICE_EXTENSION extensionToOpen =
        ((PIK220_CHECK_OPEN)Context)->Extension;
    NTSTATUS *status = ((PIK220_CHECK_OPEN)Context)->StatusOfOpen;

    *status = STATUS_SUCCESS;
    Ik220MarkOpen(extensionToOpen);

    return FALSE;

}
BOOLEAN
Ik220MarkOpen(
    IN PVOID Context
    )
/*++

Routine Description:

    This routine merely sets a boolean to true to mark the fact that
    somebody opened the device and its worthwhile to pay attention
    to interrupts.

Arguments:

    Context - Really a pointer to the device extension.

Return Value:

    This routine always returns FALSE.

--*/
{

    PIK220_DEVICE_EXTENSION extension = Context;

    Ik220Reset(extension);

    //
    // Prepare for the opening by re-enabling interrupts.
    //
    // We do this my modifying the OUT2 line in the modem control.
    // In PC's this bit is "anded" with the interrupt line.
    //
    // For the Jensen, we will ALWAYS leave the line high.  That's
    // the way the hardware engineers want it.
    //

//    WRITE_MODEM_CONTROL(
//        extension->Controller,
//        (UCHAR)(READ_MODEM_CONTROL(extension->Controller) | SERIAL_MCR_OUT2)
//        );

    extension->DeviceIsOpened = TRUE;
//    extension->ErrorWord = 0;

    return FALSE;

}


BOOLEAN
Ik220MarkClose(
    IN PVOID Context
    )
/*++

Routine Description:

    This routine merely sets a boolean to false to mark the fact that
    somebody closed the device and it's no longer worthwhile to pay attention
    to interrupts.  It also disables the UART.

Arguments:

    Context - Really a pointer to the device extension.

Return Value:

    This routine always returns FALSE.

--*/
{

    PIK220_DEVICE_EXTENSION extension = Context;

//    SerialDisableUART(Context);
    extension->DeviceIsOpened = FALSE;

    return FALSE;

}


NTSTATUS
Ik220Cleanup(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
/*++

Routine Description:

    This function is used to kill all longstanding IO operations.

Arguments:

    DeviceObject - Pointer to the device object for this device

    Irp - Pointer to the IRP for the current request

Return Value:

    The function value is the final status of the call

--*/
{

    PIK220_DEVICE_EXTENSION extension = DeviceObject->DeviceExtension;
    NTSTATUS status;


    PAGED_CODE();

    //
    // We succeed a cleanup on a removing device
    //

    if ((status = Ik220IRPPrologue(Irp, extension)) != STATUS_SUCCESS) {
       if (status == STATUS_DELETE_PENDING) {
          status = Irp->IoStatus.Status = STATUS_SUCCESS;
       }
       Ik220CompleteRequest(extension, Irp, IO_NO_INCREMENT);
       return status;
    }

    Ik220Dump(
        SERIRPPATH,
        ("IK220: Dispatch entry for: %x\n",Irp)
        );

    Ik220KillPendingIrps(DeviceObject);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information=0L;

    Ik220Dump(
        SERIRPPATH,
        ("IK220: Complete Irp: %x\n",Irp)
        );
    Ik220CompleteRequest(extension, Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;

}
