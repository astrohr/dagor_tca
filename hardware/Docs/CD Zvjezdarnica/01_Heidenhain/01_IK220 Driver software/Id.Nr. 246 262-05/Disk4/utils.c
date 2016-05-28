
#include <ntddk.h>
#include <IK220DRV.h>
#include <ik220.h>


#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGESRP0, Ik220LogError)
#endif

static const PHYSICAL_ADDRESS Ik220PhysicalZero = {0};


VOID
Ik220FilterCancelQueued(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp)
/*++

Routine Description:

    This routine will be used cancel irps on the stalled queue.

Arguments:

    PDevObj - Pointer to the device object.

    PIrp - Pointer to the Irp to cancel

Return Value:

    None.

--*/
{
   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
   PIO_STACK_LOCATION pIrpSp = IoGetCurrentIrpStackLocation(PIrp);

   PIrp->IoStatus.Status = STATUS_CANCELLED;
   PIrp->IoStatus.Information = 0;

   RemoveEntryList(&PIrp->Tail.Overlay.ListEntry);

   IoReleaseCancelSpinLock(PIrp->CancelIrql);
}


NTSTATUS
Ik220FilterIrps(IN PIRP PIrp, IN PIK220_DEVICE_EXTENSION PDevExt)
/*++

Routine Description:

    This routine will be used to approve irps for processing.
    If an irp is approved, success will be returned.  If not,
    the irp will be queued or rejected outright.  The IoStatus struct
    and return value will appropriately reflect the actions taken.

Arguments:

    PIrp - Pointer to the Irp to cancel

    PDevExt - Pointer to the device extension

Return Value:

    None.

--*/
{
   PIO_STACK_LOCATION pIrpStack;
   KIRQL oldIrqlFlags;

   pIrpStack = IoGetCurrentIrpStackLocation(PIrp);

   KeAcquireSpinLock(&PDevExt->FlagsLock, &oldIrqlFlags);

   if ((PDevExt->DevicePNPAccept == IK220_PNPACCEPT_OK)
       && ((PDevExt->Flags & IK220_FLAGS_BROKENHW) == 0)) {
      KeReleaseSpinLock(&PDevExt->FlagsLock, oldIrqlFlags);
      return STATUS_SUCCESS;
   }

   if ((PDevExt->DevicePNPAccept & IK220_PNPACCEPT_REMOVING)
       || (PDevExt->Flags & IK220_FLAGS_BROKENHW)
       || (PDevExt->DevicePNPAccept & IK220_PNPACCEPT_SURPRISE_REMOVING)) {

      KeReleaseSpinLock(&PDevExt->FlagsLock, oldIrqlFlags);

      //
      // Accept all PNP IRP's -- we assume PNP can synchronize itself
      //

      if (pIrpStack->MajorFunction == IRP_MJ_PNP) {
         return STATUS_SUCCESS;
      }

      PIrp->IoStatus.Status = STATUS_DELETE_PENDING;
      return STATUS_DELETE_PENDING;
   }

   if (PDevExt->DevicePNPAccept & IK220_PNPACCEPT_STOPPING) {
       KIRQL oldIrql;

       KeReleaseSpinLock(&PDevExt->FlagsLock, oldIrqlFlags);


      //
      // Accept all PNP IRP's -- we assume PNP can synchronize itself
      //

      if (pIrpStack->MajorFunction == IRP_MJ_PNP) {
         return STATUS_SUCCESS;
      }

      IoAcquireCancelSpinLock(&oldIrql);

      if (PIrp->Cancel) {
         IoReleaseCancelSpinLock(oldIrql);
         PIrp->IoStatus.Status = STATUS_CANCELLED;
         return STATUS_CANCELLED;
      } else {
         //
         // Mark the Irp as pending
         //

         PIrp->IoStatus.Status = STATUS_PENDING;
         IoMarkIrpPending(PIrp);

         //
         // Queue up the IRP
         //

         InsertTailList(&PDevExt->StalledIrpQueue,
                        &PIrp->Tail.Overlay.ListEntry);

         IoSetCancelRoutine(PIrp, Ik220FilterCancelQueued);
         IoReleaseCancelSpinLock(oldIrql);
         return STATUS_PENDING;
      }
   }

   KeReleaseSpinLock(&PDevExt->FlagsLock, oldIrqlFlags);

   return STATUS_SUCCESS;
}


NTSTATUS
Ik220IRPPrologue(IN PIRP PIrp, IN PIK220_DEVICE_EXTENSION PDevExt)
/*++

Routine Description:

   This function must be called at any IRP dispatch entry point.  It,
   with Ik220IRPEpilogue(), keeps track of all pending IRP's for the given
   PDevObj.

Arguments:

   PDevObj - Pointer to the device object we are tracking pending IRP's for.

Return Value:

    Tentative status of the Irp.

--*/
{
   InterlockedIncrement(&PDevExt->PendingIRPCnt);

   return Ik220FilterIrps(PIrp, PDevExt);
}


VOID
Ik220IRPEpilogue(IN PIK220_DEVICE_EXTENSION PDevExt)
/*++

Routine Description:

   This function must be called at any IRP dispatch entry point.  It,
   with Ik220IRPPrologue(), keeps track of all pending IRP's for the given
   PDevObj.

Arguments:

   PDevObj - Pointer to the device object we are tracking pending IRP's for.

Return Value:

   None.

--*/
{
   LONG pendingCnt;

   pendingCnt = InterlockedDecrement(&PDevExt->PendingIRPCnt);

   ASSERT(pendingCnt >= 0);

   if (pendingCnt == 0) {
      KeSetEvent(&PDevExt->PendingIRPEvent, IO_NO_INCREMENT, FALSE);
   }
}


NTSTATUS
Ik220IoCallDriver(PIK220_DEVICE_EXTENSION PDevExt, PDEVICE_OBJECT PDevObj,
                   PIRP PIrp)
/*++

Routine Description:

   This function must be called instead of IoCallDriver.  It automatically
   updates Irp tracking for PDevObj.

Arguments:
   PDevExt - Device extension attached to PDevObj

   PDevObj - Pointer to the device object we are tracking pending IRP's for.

   PIrp - Pointer to the Irp we are passing to the next driver.

Return Value:

   None.

--*/
{
   NTSTATUS status;

   status = IoCallDriver(PDevObj, PIrp);
   Ik220IRPEpilogue(PDevExt);
   return status;
}


NTSTATUS
Ik220PoCallDriver(PIK220_DEVICE_EXTENSION PDevExt, PDEVICE_OBJECT PDevObj,
                  PIRP PIrp)
/*++

Routine Description:

   This function must be called instead of PoCallDriver.  It automatically
   updates Irp tracking for PDevObj.

Arguments:
   PDevExt - Device extension attached to PDevObj

   PDevObj - Pointer to the device object we are tracking pending IRP's for.

   PIrp - Pointer to the Irp we are passing to the next driver.

Return Value:

   None.

--*/
{
   NTSTATUS status;

   status = PoCallDriver(PDevObj, PIrp);
   Ik220IRPEpilogue(PDevExt);
   return status;
}


VOID
Ik220LogError(
              IN PDRIVER_OBJECT DriverObject,
              IN PDEVICE_OBJECT DeviceObject OPTIONAL,
              IN PHYSICAL_ADDRESS P1,
              IN PHYSICAL_ADDRESS P2,
              IN ULONG SequenceNumber,
              IN UCHAR MajorFunctionCode,
              IN UCHAR RetryCount,
              IN ULONG UniqueErrorValue,
              IN NTSTATUS FinalStatus,
              IN NTSTATUS SpecificIOStatus,
              IN ULONG LengthOfInsert1,
              IN PWCHAR Insert1,
              IN ULONG LengthOfInsert2,
              IN PWCHAR Insert2
              )

/*++

Routine Description:

    This routine allocates an error log entry, copies the supplied data
    to it, and requests that it be written to the error log file.

Arguments:

    DriverObject - A pointer to the driver object for the device.

    DeviceObject - A pointer to the device object associated with the
    device that had the error, early in initialization, one may not
    yet exist.

    P1,P2 - If phyical addresses for the controller ports involved
    with the error are available, put them through as dump data.

    SequenceNumber - A ulong value that is unique to an IRP over the
    life of the irp in this driver - 0 generally means an error not
    associated with an irp.

    MajorFunctionCode - If there is an error associated with the irp,
    this is the major function code of that irp.

    RetryCount - The number of times a particular operation has been
    retried.

    UniqueErrorValue - A unique long word that identifies the particular
    call to this function.

    FinalStatus - The final status given to the irp that was associated
    with this error.  If this log entry is being made during one of
    the retries this value will be STATUS_SUCCESS.

    SpecificIOStatus - The IO status for a particular error.

    LengthOfInsert1 - The length in bytes (including the terminating NULL)
                      of the first insertion string.

    Insert1 - The first insertion string.

    LengthOfInsert2 - The length in bytes (including the terminating NULL)
                      of the second insertion string.  NOTE, there must
                      be a first insertion string for their to be
                      a second insertion string.

    Insert2 - The second insertion string.

Return Value:

    None.

--*/

{
   PIO_ERROR_LOG_PACKET errorLogEntry;

   PVOID objectToUse;
   SHORT dumpToAllocate = 0;
   PUCHAR ptrToFirstInsert;
   PUCHAR ptrToSecondInsert;

   PAGED_CODE();

   if (Insert1 == NULL) {
      LengthOfInsert1 = 0;
   }

   if (Insert2 == NULL) {
      LengthOfInsert2 = 0;
   }


   if (ARGUMENT_PRESENT(DeviceObject)) {

      objectToUse = DeviceObject;

   } else {

      objectToUse = DriverObject;

   }

   if (Ik220MemCompare(
                       P1,
                       (ULONG)1,
                       Ik220PhysicalZero,
                       (ULONG)1
                       ) != AddressesAreEqual) {

      dumpToAllocate = (SHORT)sizeof(PHYSICAL_ADDRESS);

   }

   if (Ik220MemCompare(
                       P2,
                       (ULONG)1,
                       Ik220PhysicalZero,
                       (ULONG)1
                       ) != AddressesAreEqual) {

      dumpToAllocate += (SHORT)sizeof(PHYSICAL_ADDRESS);

   }

   errorLogEntry = IoAllocateErrorLogEntry(
                                          objectToUse,
                                          (UCHAR)(sizeof(IO_ERROR_LOG_PACKET) +
                                                  dumpToAllocate
                                                  + LengthOfInsert1 +
                                                  LengthOfInsert2)
                                          );

   if ( errorLogEntry != NULL ) {

      errorLogEntry->ErrorCode = SpecificIOStatus;
      errorLogEntry->SequenceNumber = SequenceNumber;
      errorLogEntry->MajorFunctionCode = MajorFunctionCode;
      errorLogEntry->RetryCount = RetryCount;
      errorLogEntry->UniqueErrorValue = UniqueErrorValue;
      errorLogEntry->FinalStatus = FinalStatus;
      errorLogEntry->DumpDataSize = dumpToAllocate;

      if (dumpToAllocate) {

         RtlCopyMemory(
                      &errorLogEntry->DumpData[0],
                      &P1,
                      sizeof(PHYSICAL_ADDRESS)
                      );

         if (dumpToAllocate > sizeof(PHYSICAL_ADDRESS)) {

            RtlCopyMemory(
                         ((PUCHAR)&errorLogEntry->DumpData[0])
                         +sizeof(PHYSICAL_ADDRESS),
                         &P2,
                         sizeof(PHYSICAL_ADDRESS)
                         );

            ptrToFirstInsert =
            ((PUCHAR)&errorLogEntry->DumpData[0])+(2*sizeof(PHYSICAL_ADDRESS));

         } else {

            ptrToFirstInsert =
            ((PUCHAR)&errorLogEntry->DumpData[0])+sizeof(PHYSICAL_ADDRESS);


         }

      } else {

         ptrToFirstInsert = (PUCHAR)&errorLogEntry->DumpData[0];

      }

      ptrToSecondInsert = ptrToFirstInsert + LengthOfInsert1;

      if (LengthOfInsert1) {

         errorLogEntry->NumberOfStrings = 1;
         errorLogEntry->StringOffset = (USHORT)(ptrToFirstInsert -
                                                (PUCHAR)errorLogEntry);
         RtlCopyMemory(
                      ptrToFirstInsert,
                      Insert1,
                      LengthOfInsert1
                      );

         if (LengthOfInsert2) {

            errorLogEntry->NumberOfStrings = 2;
            RtlCopyMemory(
                         ptrToSecondInsert,
                         Insert2,
                         LengthOfInsert2
                         );

         }

      }

      IoWriteErrorLogEntry(errorLogEntry);

   }

}


VOID
Ik220SetDeviceFlags(IN PIK220_DEVICE_EXTENSION PDevExt, OUT PULONG PFlags,
                    IN ULONG Value, IN BOOLEAN Set)
/*++

Routine Description:

   Sets flags in a value protected by the flags spinlock.  This is used
   to set values that would stop IRP's from being accepted.

Arguments:
   PDevExt - Device extension attached to PDevObj

   PFlags - Pointer to the flags variable that needs changing

   Value - Value to modify flags variable with

   Set - TRUE if |= , FALSE if &=

Return Value:

   None.

--*/
{
   KIRQL oldIrql;

   KeAcquireSpinLock(&PDevExt->FlagsLock, &oldIrql);

   if (Set) {
      *PFlags |= Value;
   } else {
      *PFlags &= ~Value;
   }

   KeReleaseSpinLock(&PDevExt->FlagsLock, oldIrql);
}

