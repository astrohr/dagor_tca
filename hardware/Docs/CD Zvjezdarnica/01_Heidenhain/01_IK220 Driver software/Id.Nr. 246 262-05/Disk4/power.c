
#include <ntddk.h>
#include <IK220DRV.h>
#include <ik220.h>


#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGESRP0, Ik220GotoPowerState)
#pragma alloc_text(PAGESRP0, Ik220PowerDispatch)
#pragma alloc_text(PAGESRP0, Ik220SetPowerD0)
#pragma alloc_text(PAGESRP0, Ik220SetPowerD3)
#pragma alloc_text(PAGESRP0, Ik220SaveDeviceState)
#pragma alloc_text(PAGESRP0, Ik220RestoreDeviceState)
#pragma alloc_text(PAGESRP0, Ik220SendWaitWake)
#endif // ALLOC_PRAGMA


NTSTATUS
Ik220SystemPowerCompletion(IN PDEVICE_OBJECT PDevObj, UCHAR MinorFunction,
                           IN POWER_STATE PowerState, IN PVOID Context,
                           PIO_STATUS_BLOCK IoStatus)
/*++

Routine Description:

    This routine is the completion routine for PoRequestPowerIrp calls
    in this module.

Arguments:

    PDevObj - Pointer to the device object the irp is completing for

    MinorFunction - IRP_MN_XXXX value requested

    PowerState - Power state request was made of

    Context - Event to set or NULL if no setting required

    IoStatus - Status block from request

Return Value:

    VOID


--*/
{
   if (Context != NULL) {
      KeSetEvent((PKEVENT)Context, IO_NO_INCREMENT, 0);
   }

   return STATUS_SUCCESS;
}


VOID
Ik220SaveDeviceState(IN PIK220_DEVICE_EXTENSION PDevExt)
/*++

Routine Description:

    This routine saves the device state of the UART

Arguments:

    PDevExt - Pointer to the device extension for the devobj to save the state
              for.

Return Value:

    VOID


--*/
{
   PIK220_DEVICE_STATE pDevState = &PDevExt->DeviceState;

   PAGED_CODE();

   Ik220Dump(SERTRACECALLS, ("IK220: Entering Ik220SaveDeviceState\n"));

   //
   // Read necessary registers direct
   //

//   pDevState->IER = READ_INTERRUPT_ENABLE(PDevExt->Controller);
//   pDevState->MCR = READ_MODEM_CONTROL(PDevExt->Controller);
//   pDevState->LCR = READ_LINE_CONTROL(PDevExt->Controller);


   Ik220Dump(SERTRACECALLS, ("IK220: Leaving Ik220SaveDeviceState\n"));
}

VOID
Ik220RestoreDeviceState(IN PIK220_DEVICE_EXTENSION PDevExt)
/*++

Routine Description:

    This routine restores the device state of the UART

Arguments:

    PDevExt - Pointer to the device extension for the devobj to restore the
    state for.

Return Value:

    VOID


--*/
{
   PIK220_DEVICE_STATE pDevState = &PDevExt->DeviceState;
//   SHORT divisor;
//   IK220_IOCTL_SYNC S;

   PAGED_CODE();

   Ik220Dump(SERTRACECALLS, ("IK220: Enter Ik220RestoreDeviceState\n"));
   Ik220Dump(SERTRACECALLS, ("------  PDevExt: %x\n", PDevExt));

   //
   // Disable interrupts both via OUT2 and IER
   //

   DISABLE_ALL_INTERRUPTS(PDevExt->Controller);


   //
   // In case we are dealing with a bitmasked multiportcard,
   // that has the mask register enabled, enable the
   // interrupts.
   //

//   if (PDevExt->InterruptStatus) {
//      WRITE_PORT_UCHAR(PDevExt->InterruptStatus,
//                       (UCHAR)(1 << (PDevExt->PortIndex - 1)));
//   }

   //
   // Restore a couple more registers
   //

//   WRITE_INTERRUPT_ENABLE(PDevExt->Controller, pDevState->IER);
//   WRITE_LINE_CONTROL(PDevExt->Controller, pDevState->LCR);

   //
   // Clear out any stale interrupts
   //

//   READ_INTERRUPT_ID_REG(PDevExt->Controller);
//   READ_LINE_STATUS(PDevExt->Controller);
//   READ_MODEM_STATUS(PDevExt->Controller);


   if (PDevExt->DeviceState.Reopen == TRUE) {
      Ik220Dump(SERPNPPOWER, ("IK220: Reopening device\n"));

      PDevExt->DeviceIsOpened = TRUE;
      PDevExt->DeviceState.Reopen = FALSE;

      //
      // This enables interrupts on the device!
      //

//      WRITE_MODEM_CONTROL(PDevExt->Controller,
//                          (UCHAR)(pDevState->MCR | SERIAL_MCR_OUT2));

      //
      // Refire the state machine
      //

      DISABLE_ALL_INTERRUPTS(PDevExt->Controller);
      ENABLE_ALL_INTERRUPTS(PDevExt->Controller);
   }

}

NTSTATUS
Ik220PowerDispatch(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp)
/*++

Routine Description:

    This is a dispatch routine for the IRPs that come to the driver with the
    IRP_MJ_POWER major code (power IRPs).

Arguments:

    PDevObj - Pointer to the device object for this device

    PIrp - Pointer to the IRP for the current request

Return Value:

    The function value is the final status of the call


--*/
{

   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
   PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(PIrp);
   NTSTATUS status;
   PDEVICE_OBJECT pLowerDevObj = pDevExt->LowerDeviceObject;
   PDEVICE_OBJECT pPdo = pDevExt->Pdo;
//   BOOLEAN acceptingIRPs;

   PAGED_CODE();

   if ((status = Ik220IRPPrologue(PIrp, pDevExt)) != STATUS_SUCCESS) {
      PoStartNextPowerIrp(PIrp);
      Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
      return status;
   }

   status = STATUS_SUCCESS;

   switch (pIrpStack->MinorFunction) {

   case IRP_MN_WAIT_WAKE:
      Ik220Dump(SERPNPPOWER, ("IK220: Got IRP_MN_WAIT_WAKE Irp\n"));
      break;


   case IRP_MN_POWER_SEQUENCE:
      Ik220Dump(SERPNPPOWER, ("IK220: Got IRP_MN_POWER_SEQUENCE Irp\n"));
      break;


   case IRP_MN_SET_POWER:
      Ik220Dump(SERPNPPOWER, ("IK220: Got IRP_MN_SET_POWER Irp\n"));

      //
      // Perform different ops if it was system or device
      //

      switch (pIrpStack->Parameters.Power.Type) {
      case SystemPowerState: {
            POWER_STATE powerState;

            //
            // They asked for a system power state change
            //

            Ik220Dump(SERPNPPOWER, ("------: SystemPowerState\n"));

            //
            // We will only service this if we are policy owner -- we
            // don't need to lock on this value since we only service
            // one power request at a time.
            //

            if (pDevExt->OwnsPowerPolicy != TRUE) {
               status = STATUS_SUCCESS;
               goto PowerExit;
            }


            switch (pIrpStack->Parameters.Power.State.SystemState) {
            case PowerSystemUnspecified:
               powerState.DeviceState = PowerDeviceUnspecified;
               break;

            case PowerSystemWorking:
               powerState.DeviceState = PowerDeviceD0;
               break;

            case PowerSystemSleeping1:
            case PowerSystemSleeping2:
            case PowerSystemSleeping3:
            case PowerSystemHibernate:
            case PowerSystemShutdown:
            case PowerSystemMaximum:
               powerState.DeviceState
                  = pDevExt->DeviceStateMap[pIrpStack->
                                            Parameters.Power.State.SystemState];
               break;

            default:
               status = STATUS_SUCCESS;
               goto PowerExit;
               break;
            }


            status = STATUS_SUCCESS;

            PoSetPowerState(PDevObj, pIrpStack->Parameters.Power.Type,
                            pIrpStack->Parameters.Power.State);

            //
            // Send IRP to change device state if we should change
            //

            //
            // We only power up the stack if the device is open.  This is based
            // on our policy of keeping the device powered down unless it is
            // open.
            //

            if (((powerState.DeviceState < pDevExt->PowerState)
                 && pDevExt->OpenCount)) {
               PoRequestPowerIrp(pPdo, IRP_MN_SET_POWER, powerState, NULL, NULL,
                                 NULL);
            }else {
               //
               // If powering down, we can't go past wake state
               // if wait-wake pending
               //

               if (powerState.DeviceState >= pDevExt->PowerState) {

                  //
                  // Power down -- ensure there is no wake-wait pending OR
                  // we can do down to that level and still wake the machine
                  //

                  if ((pDevExt->PendingWakeIrp == NULL && !pDevExt->SendWaitWake)
                      || powerState.DeviceState <= pDevExt->DeviceWake) {
                     PoRequestPowerIrp(pPdo, IRP_MN_SET_POWER, powerState, NULL,
                                       NULL, NULL);
                  } else {
                     //
                     // Fail the request
                     //

                     status = STATUS_INVALID_DEVICE_STATE;
                  }
               }
            }

            goto PowerExit;
         }

      case DevicePowerState:
         Ik220Dump(SERPNPPOWER, ("------: DevicePowerState\n"));
         break;

      default:
         Ik220Dump(SERPNPPOWER, ("------: UNKNOWN PowerState\n"));
         status = STATUS_SUCCESS;
         goto PowerExit;
      }


      //
      // If we are already in the requested state, just pass the IRP down
      //

      if (pDevExt->PowerState
          == pIrpStack->Parameters.Power.State.DeviceState) {
         Ik220Dump(SERPNPPOWER, ("IK220: Already in requested power state\n")
                    );
         status = STATUS_SUCCESS;
         break;
      }


      switch (pIrpStack->Parameters.Power.State.DeviceState) {

      case PowerDeviceD0:
         Ik220Dump(SERPNPPOWER, ("IK220: Going to power state D0\n"));
         return Ik220SetPowerD0(PDevObj, PIrp);

      case PowerDeviceD1:
      case PowerDeviceD2:
      case PowerDeviceD3:
         Ik220Dump(SERPNPPOWER, ("IK220: Going to power state D3\n"));
         return Ik220SetPowerD3(PDevObj, PIrp);

      default:
         break;
      }
      break;



   case IRP_MN_QUERY_POWER:

      Ik220Dump (SERPNPPOWER, ("IK220: Got IRP_MN_QUERY_POWER Irp\n"));

      //
      // Check if we have a wait-wake pending and if so,
      // ensure we don't power down too far.
      //


      if (pDevExt->PendingWakeIrp != NULL || pDevExt->SendWaitWake) {
         if (pIrpStack->Parameters.Power.Type == DevicePowerState
             && pIrpStack->Parameters.Power.State.DeviceState
             > pDevExt->DeviceWake) {
            status = PIrp->IoStatus.Status = STATUS_INVALID_DEVICE_STATE;
            PoStartNextPowerIrp(PIrp);
            Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
            return status;
         }
      }

      //
      // If no wait-wake, always successful
      //

      PIrp->IoStatus.Status = STATUS_SUCCESS;
      status = STATUS_SUCCESS;
      PoStartNextPowerIrp(PIrp);
      IoSkipCurrentIrpStackLocation(PIrp);
      return Ik220PoCallDriver(pDevExt, pLowerDevObj, PIrp);

   }   // switch (pIrpStack->MinorFunction)


   PowerExit:;

   PoStartNextPowerIrp(PIrp);


   //
   // Pass to the lower driver
   //
   IoSkipCurrentIrpStackLocation(PIrp);
   status = Ik220PoCallDriver(pDevExt, pLowerDevObj, PIrp);

   return status;
}


NTSTATUS
Ik220SetPowerD0(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp)
/*++

Routine Description:

    This routine Decides if we need to pass the power Irp down the stack
    or not.  It then either sets up a completion handler to finish the
    initialization or calls the completion handler directly.

Arguments:

    PDevObj - Pointer to the devobj we are changing power state on

    PIrp - Pointer to the IRP for the current request

Return Value:

    Return status of either PoCallDriver of the call to the initialization
    routine.


--*/
{
   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
   PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(PIrp);
   NTSTATUS status;

   PAGED_CODE();

   Ik220Dump(SERTRACECALLS, ("IK220: In Ik220SetPowerD0\n"));
   Ik220Dump(SERPNPPOWER, ("IK220: SetPowerD0 has IRP %x\n", PIrp));

   ASSERT(pDevExt->LowerDeviceObject);

   //
   // Set up completion to init device when it is on
   //

   KeClearEvent(&pDevExt->PowerD0Event);


   IoCopyCurrentIrpStackLocationToNext(PIrp);
   IoSetCompletionRoutine(PIrp, Ik220SyncCompletion, &pDevExt->PowerD0Event,
                          TRUE, TRUE, TRUE);

   Ik220Dump(SERPNPPOWER, ("IK220: Calling next driver\n"));

   status = PoCallDriver(pDevExt->LowerDeviceObject, PIrp);

   if (status == STATUS_PENDING) {
      Ik220Dump(SERPNPPOWER, ("IK220: Waiting for next driver\n"));
      KeWaitForSingleObject (&pDevExt->PowerD0Event, Executive, KernelMode,
                             FALSE, NULL);
   } else {
      if (!NT_SUCCESS(status)) {
         PIrp->IoStatus.Status = status;
         PoStartNextPowerIrp(PIrp);
         Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
         return status;
      }
   }

   if (!NT_SUCCESS(PIrp->IoStatus.Status)) {
      status = PIrp->IoStatus.Status;
      PoStartNextPowerIrp(PIrp);
      Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
      return status;
   }

   //
   // Restore the device
   //

   pDevExt->PowerState = PowerDeviceD0;

   //
   // Theoretically we could change states in the middle of processing
   // the restore which would result in a bad PKINTERRUPT being used
   // in Ik220RestoreDeviceState().
   //

   if (pDevExt->PNPState == IK220_PNP_STARTED) {
      Ik220RestoreDeviceState(pDevExt);
   }

   //
   // Now that we are powered up, call PoSetPowerState
   //

   PoSetPowerState(PDevObj, pIrpStack->Parameters.Power.Type,
                   pIrpStack->Parameters.Power.State);

   PoStartNextPowerIrp(PIrp);
   Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);


   Ik220Dump(SERTRACECALLS, ("IK220: Leaving Ik220SetPowerD0\n"));
   return status;
}


NTSTATUS
Ik220GotoPowerState(IN PDEVICE_OBJECT PDevObj,
                     IN PIK220_DEVICE_EXTENSION PDevExt,
                     IN DEVICE_POWER_STATE DevPowerState)
/*++

Routine Description:

    This routine causes the driver to request the stack go to a particular
    power state.

Arguments:

    PDevObj - Pointer to the device object for this device

    PDevExt - Pointer to the device extension we are working from

    DevPowerState - the power state we wish to go to

Return Value:

    The function value is the final status of the call


--*/
{
   KEVENT gotoPowEvent;
   NTSTATUS status;
   POWER_STATE powerState;

   PAGED_CODE();

   Ik220Dump(SERTRACECALLS, ("IK220: In Ik220GotoPowerState\n"));

   powerState.DeviceState = DevPowerState;

   KeInitializeEvent(&gotoPowEvent, SynchronizationEvent, FALSE);

   status = PoRequestPowerIrp(PDevObj, IRP_MN_SET_POWER, powerState,
                              Ik220SystemPowerCompletion, &gotoPowEvent,
                              NULL);

   if (status == STATUS_PENDING) {
      KeWaitForSingleObject(&gotoPowEvent, Executive, KernelMode, FALSE, NULL);
      status = STATUS_SUCCESS;
   }

#if DBG
   if (!NT_SUCCESS(status)) {
      Ik220Dump(SERPNPPOWER, ("IK220: Ik220GotoPowerState FAILED\n"));
   }
#endif

   Ik220Dump(SERTRACECALLS, ("IK220: Leaving Ik220GotoPowerState\n"));

   return status;
}



NTSTATUS
Ik220SetPowerD3(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp)
/*++

Routine Description:

    This routine handles the SET_POWER minor function.

Arguments:

    PDevObj - Pointer to the device object for this device

    PIrp - Pointer to the IRP for the current request

Return Value:

    The function value is the final status of the call


--*/
{
   NTSTATUS status = STATUS_SUCCESS;
   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
   PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(PIrp);

   PAGED_CODE();

   Ik220Dump(SERDIAG3, ("IK220: In Ik220SetPowerD3\n"));

   //
   // Send the wait wake now, just in time
   //


   if (pDevExt->SendWaitWake) {
      Ik220SendWaitWake(pDevExt);
   }
   //
   // Before we power down, call PoSetPowerState
   //

   PoSetPowerState(PDevObj, pIrpStack->Parameters.Power.Type,
                   pIrpStack->Parameters.Power.State);

   //
   // If the device is not closed, disable interrupts and allow the fifo's
   // to flush.
   //

   if (pDevExt->DeviceIsOpened == TRUE) {
//    LARGE_INTEGER charTime;

      pDevExt->DeviceIsOpened = FALSE;
      pDevExt->DeviceState.Reopen = TRUE;

      //
      // Shut down the chip
      //

//      SerialDisableUART(pDevExt);

      //
      // Drain the device
      //

//      SerialDrainUART(pDevExt, &charTime);

      //
      // Save the device state
      //

      Ik220SaveDeviceState(pDevExt);
   }

   //
   // If the device is not open, we don't need to save the state;
   // we can just reset the device on power-up
   //


   PIrp->IoStatus.Status = STATUS_SUCCESS;

   pDevExt->PowerState = PowerDeviceD3;

   //
   // For what we are doing, we don't need a completion routine
   // since we don't race on the power requests.
   //

   PIrp->IoStatus.Status = STATUS_SUCCESS;

   PoStartNextPowerIrp(PIrp);
   IoSkipCurrentIrpStackLocation(PIrp);

   return Ik220PoCallDriver(pDevExt, pDevExt->LowerDeviceObject, PIrp);
}


NTSTATUS
Ik220SendWaitWake(PIK220_DEVICE_EXTENSION PDevExt)
/*++

Routine Description:

    This routine causes a waitwake IRP to be sent

Arguments:

    PDevExt - Pointer to the device extension for this device

Return Value:

    STATUS_INVALID_DEVICE_STATE if one is already pending, else result
    of call to PoRequestPowerIrp.


--*/
{
   NTSTATUS status;
   PIRP pIrp;
   POWER_STATE powerState;

   PAGED_CODE();

   //
   // Make sure one isn't pending already -- Ik220 will only handle one at
   // a time.
   //

   if (PDevExt->PendingWakeIrp != NULL) {
      return STATUS_INVALID_DEVICE_STATE;
   }

   //
   // Make sure we are capable of waking the machine
   //

   if (PDevExt->SystemWake <= PowerSystemWorking) {
      return STATUS_INVALID_DEVICE_STATE;
   }

   if (PDevExt->DeviceWake == PowerDeviceUnspecified) {
      return STATUS_INVALID_DEVICE_STATE;
   }

   //
   // Send IRP to request wait wake and add a pending irp flag
   //
   //

   InterlockedIncrement(&PDevExt->PendingIRPCnt);

   powerState.SystemState = PDevExt->SystemWake;

   status = PoRequestPowerIrp(PDevExt->Pdo, IRP_MN_WAIT_WAKE,
                              powerState, Ik220WakeCompletion, PDevExt, &pIrp);

   if (status == STATUS_PENDING) {
      status = STATUS_SUCCESS;
      PDevExt->PendingWakeIrp = pIrp;
   } else if (!NT_SUCCESS(status)) {
      Ik220IRPEpilogue(PDevExt);
   }

   return status;
}

NTSTATUS
Ik220WakeCompletion(IN PDEVICE_OBJECT PDevObj, IN UCHAR MinorFunction,
                    IN POWER_STATE PowerState, IN PVOID Context,
                    IN PIO_STATUS_BLOCK IoStatus)
/*++

Routine Description:

    This routine handles completion of the waitwake IRP.

Arguments:

    PDevObj - Pointer to the device object for this device

    MinorFunction - Minor function previously supplied to PoRequestPowerIrp

    PowerState - PowerState previously supplied to PoRequestPowerIrp

    Context - a pointer to the device extension

    IoStatus - current/final status of the waitwake IRP

Return Value:

    The function value is the final status of attempting to process the
    waitwake.


--*/
{
   NTSTATUS status;
   PIK220_DEVICE_EXTENSION pDevExt = (PIK220_DEVICE_EXTENSION)Context;
   POWER_STATE powerState;

   status = IoStatus->Status;

   if (NT_SUCCESS(status)) {
      //
      // A wakeup has occurred -- powerup our stack
      //

      powerState.DeviceState = PowerDeviceD0;

      PoRequestPowerIrp(pDevExt->Pdo, IRP_MN_SET_POWER, powerState, NULL,
                        NULL, NULL);

   }

   pDevExt->PendingWakeIrp = NULL;
   Ik220IRPEpilogue(pDevExt);

   return status;
}


