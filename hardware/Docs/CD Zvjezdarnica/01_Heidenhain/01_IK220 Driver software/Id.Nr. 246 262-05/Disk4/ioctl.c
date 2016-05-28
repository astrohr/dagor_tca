
#include <ntddk.h>
#include <IK220DRV.h>
#include <ik220.h>


#ifdef ALLOC_PRAGMA
//
// Locked during PnP operations and while open
//

#pragma alloc_text(PAGEIK, Ik220IoControl)
#pragma alloc_text(PAGEIK, Ik220InternalIoControl)
#endif


void IK220WriteErrorLog ( IN PDEVICE_OBJECT DeviceObject, 
                          IN ULONG EventID, 
                          IN ULONG ErrorValue  );


NTSTATUS
Ik220IoControl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
/*++

Routine Description:

    This routine provides the initial processing for all of the
    Ioctrls for the serial device.

Arguments:

    DeviceObject - Pointer to the device object for this device

    Irp - Pointer to the IRP for the current request

Return Value:

    The function value is the final status of the call

--*/
{
    //
    // The status that gets returned to the caller and
    // set in the Irp.
    //
    NTSTATUS ntStatus = STATUS_SUCCESS;

    //
    // The current stack location.  This contains all of the
    // information we need to process this particular request.
    //
    PIO_STACK_LOCATION irpStack;

    //
    // Just what it says.  This is the Ik220 specific device
    // extension of the device object create for the Ik220 driver.
    //
    PIK220_DEVICE_EXTENSION deviceExtension = DeviceObject->DeviceExtension;

    //
    // A temporary to hold the old IRQL so that it can be
    // restored once we complete/validate this request.
    //
//    KIRQL OldIrql;

    NTSTATUS prologueStatus;

#if DBG
    const char DrvVers[20] = "DRV: V"_VERSION"(D)";
#else
    const char DrvVers[20] = "DRV: V"_VERSION"(R)";
#endif
        
    
    ULONG               ioControlCode;
    ULONG               wordsToRead, wordsToWrite;              // Number of words for i/o
    PUCHAR              irpBufferB;                             // Byte  buffer pointer for i/o
    PUSHORT             irpBufferS;                             // Short buffer pointer for i/o
    PULONG              irpBufferL;                             // Long  buffer pointer for i/o
    ULONG               i;                                      // Loop counter
    USHORT              cardNum, cardReg, RamAdr;               // Card number / card register / RAM address
    volatile USHORT*    pSource;                                // Source pointer 
    volatile USHORT*    pDestination;                           // Destination pointer
    volatile ULONG*     pDestinationL;                          // Destination pointer long
    USHORT              Status;                                 // Status from IK 220
    USHORT              TimeOut;                                // Time out counter


    IK220_LOCKED_PAGED_CODE();

    //
    // We expect to be open so all our pages are locked down.  This is, after
    // all, an IO operation, so the device should be open first.
    //

    if (deviceExtension->DeviceIsOpened != TRUE) {
       Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
       IoCompleteRequest(Irp, IO_NO_INCREMENT);
       return STATUS_INVALID_DEVICE_REQUEST;
    }


    if ((prologueStatus = Ik220IRPPrologue(Irp, deviceExtension))
        != STATUS_SUCCESS) {
       Irp->IoStatus.Status = prologueStatus;
       Ik220CompleteRequest(deviceExtension, Irp, IO_NO_INCREMENT);
       return prologueStatus;
    }

    Ik220Dump(
        SERIRPPATH,
        ("IK220: Dispatch entry for: %x\n",Irp)
        );

    irpStack = IoGetCurrentIrpStackLocation(Irp);
    Irp->IoStatus.Information = 0L;
    Status = STATUS_SUCCESS;
		ioControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;

    switch (ioControlCode) {

        case IOCTL_INPUT:
            wordsToRead = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
            wordsToRead = wordsToRead /2;
            irpBufferS = Irp->AssociatedIrp.SystemBuffer;
            cardNum = *irpBufferS;
            cardReg = *(irpBufferS+1);

            pSource = (USHORT*) (Ik220Globals.GloPortBase[cardNum]);

            kdPrint (( "Reading %2d words card 0x%08lX register 0x%02X: ", wordsToRead, pSource, cardReg ));

            if (pSource)
            {
                pSource = pSource+cardReg;

                if (wordsToRead<=15)
                {
                    kdPrint (( "0x%04X\n",*pSource ));
                    for (i=0; i<wordsToRead; i++)
                        *irpBufferS++ = *pSource++;
                    Irp->IoStatus.Information = wordsToRead*2;
                }
                else
                {
                    kdPrint (( "Illegal word count!\n" ));
                    if ((deviceExtension->DevStat & 0x1) == 0)
                    {
                        IK220WriteErrorLog (DeviceObject, 6, wordsToRead);
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
                    IK220WriteErrorLog (DeviceObject, 7, cardNum);
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

            pDestination = (USHORT*) (Ik220Globals.GloPortBase[cardNum]);

            kdPrint (( "Writing %d words 0x%04X card 0x%X register 0x%02X: \n", wordsToWrite, *pSource, Ik220Globals.GloPortBase[cardNum], cardReg ));


            if (pDestination)
            {
                pDestination = pDestination+cardReg;

                if (wordsToWrite<=15)
                {
                    for (i=0; i<(wordsToWrite); i++)
                        *pDestination++ = *pSource++;
                    Irp->IoStatus.Information = wordsToWrite*2;
                }
                else
                {
                    kdPrint (( "Illegal word count!\n" ));
                    if ((deviceExtension->DevStat & 0x1) == 0)
                    {
                        IK220WriteErrorLog (DeviceObject, 6, wordsToWrite);
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
                    IK220WriteErrorLog (DeviceObject, 7, cardNum);
                    deviceExtension->DevStat |= 0x2;
                }
                Irp->IoStatus.Information = 0;
            }
            break;


        case IOCTL_STATUS:
            wordsToRead = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
            irpBufferL = Irp->AssociatedIrp.SystemBuffer;

            kdPrint (( "IK-Card's at:" ));

            for (cardNum=0; cardNum<2*MAX_CARDS; cardNum++)
            {
                if (Ik220Globals.GloPortBase[cardNum]!=0)
                {
                    pSource = (USHORT*) (Ik220Globals.GloPortBase[cardNum]);
                    Status = *(pSource+CodeReg);    
                    
                    if ( Status == 0x0007 )
                    {
                        *(irpBufferL+cardNum) = (ULONG)Ik220Globals.GloPortBase[cardNum];
                        kdPrint (( "(%2d)=0x%08lX\n", cardNum, *(irpBufferL+cardNum) ));
                    }
                    else
                    {
                        IK220WriteErrorLog (DeviceObject, 7, cardNum);
                        *(irpBufferL+cardNum) = 0L;
                    }
                }
                else *(irpBufferL+cardNum) = 0L;
            }
            Irp->IoStatus.Information = 2*MAX_CARDS*sizeof(ULONG);
            break;


        case IOCTL_DOWNLOAD:
            wordsToWrite = irpStack->Parameters.DeviceIoControl.InputBufferLength;
            wordsToWrite = (wordsToWrite/2)-2;                  // Number of words to transfer
            irpBufferS = Irp->AssociatedIrp.SystemBuffer;
            cardNum = *irpBufferS;
            RamAdr  = *(irpBufferS+1);

            pSource = irpBufferS+2;
            pDestination = (USHORT*) (Ik220Globals.GloPortBase[cardNum]);

            kdPrint (( "Loading %d words card 0x%08lX start 0x%04X: \n", wordsToWrite, pDestination, RamAdr ));


            if (pDestination)
            {
                    for (i=1; i<=wordsToWrite; i++)
                    {
                        *(pDestination + ControlReg) = BootMode;        // Set CPU to boot mode
                        do 
                        {
                        } while ( !(*(pDestination + StatusReg) & PipeEmpty) ); // 

                        *(pDestination + DatPort0  ) = RamAdr++;        // Write address to port 0
                        *(pDestination + DatPort1  ) = *pSource;        // Write data to port 1
                        *(pDestination + ControlReg) = WriteRamMode;    // Transfer data to address in RAM
                        pSource++;

                        do 
                        {
                        } while ( !(*(pDestination + StatusReg) & PipeEmpty) );
      			
                        if ((i & 0xff) == 1) {
                          kdPrint (("."));
                        }
                    }
                    kdPrint (("\n"));
                    Irp->IoStatus.Information = wordsToWrite*2;
            }
            else
            {
                kdPrint (( "\nCard not found!\n" ));
                if ((deviceExtension->DevStat & 0x2) == 0)
                {
                    IK220WriteErrorLog (DeviceObject, 7, cardNum);
                    deviceExtension->DevStat |= 0x2;
                }
                Irp->IoStatus.Information = 0;
            }
            break;


        case IOCTL_BURSTRAM:
            wordsToRead = irpStack->Parameters.DeviceIoControl.OutputBufferLength;  // Max. number of values to transfer
            wordsToRead = ((wordsToRead/2)-1)/3;
            irpBufferS  = Irp->AssociatedIrp.SystemBuffer;

            cardNum      = *irpBufferS;
            pDestination =  irpBufferS+1;
            pSource      = (USHORT*) (Ik220Globals.GloPortBase[cardNum]);

            kdPrint (( "Burstread %d values card 0x%08lX\n", wordsToRead, pSource ));

            if (pSource)
            {
                i=0;
                Status=0;
                do
                {
                    *(pSource+CmdPort) = cmdGetRam;                                 // Command:  to IK 220
                    TimeOut=0;
                    do
                    {
                        KeStallExecutionProcessor (1);                              // Wait 1µs
                        TimeOut++;                                                  // Increment TimeOut
                    } while ( !(*(pSource+Flag1Reg) & G28Sem0) & (TimeOut<1000) );  // Wait until finished or TimeOut

                    if ( *(pSource+DatPort0)!=cmdGetRam )
                    {

                        Status = 0x8001;    break;                                  // Error from IK 220
                    }
                    if ( TimeOut>=1000)
                    {
                        Status = 0x8002;    break;                                  // TimeOut from IK 220
                    }

                    Status = Status | *(pSource+DatPort6);
                    if (Status & statBufferNoVal)
                    {
                        break;                                                      // RAM Buffer empty
                    }

                    *pDestination++ = *(pSource+DatPort1);                          // Transfer counter value
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
                    IK220WriteErrorLog (DeviceObject, 7, cardNum);
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

            pSource = (USHORT*) (Ik220Globals.GloPortBase[cardNum]);

            kdPrint (( "Reading version card 0x%08lX: ", pSource ));

            if (pSource)
            {
                    *(pSource+CmdPort) = cmdGetVersion;                 // Command:  to IK 220
                    do {} while (!(*(pSource+Flag1Reg) & G28Sem0));     // Wait until finished

                    if ( *(pSource+DatPort0)==cmdGetVersion )           // Error from IK 220
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
                    IK220WriteErrorLog (DeviceObject, 7, cardNum);
                    deviceExtension->DevStat |= 0x2;
                }
                Irp->IoStatus.Information = 0;
            }
            break;


        case IOCTL_LATCHI:
            irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
            cardNum     = *irpBufferS;
            pDestinationL = (ULONG*) (Ik220Globals.GloConfBase[cardNum]);

            if (pDestinationL)
            {
                pDestinationL = pDestinationL + CONFREG_CNTRL;

                kdPrint (( "Internal latch card %d at 0x%08lX\n", cardNum, pDestinationL ));

                *pDestinationL = *pDestinationL & 0xFFFFFEFF;   // Latch Int low
                KeStallExecutionProcessor (2);                  // Wait 2µs
                *pDestinationL = *pDestinationL | 0x00000100;   // Latch Int high
                
                Irp->IoStatus.Information = 1;
            }
            else
            {
                kdPrint (( "Internal latch card %d not found!\n", cardNum ));
                Irp->IoStatus.Information = 0;
            }
            break;


        case IOCTL_LATCHE:
            irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
            cardNum     = *irpBufferS;
            pDestinationL = (ULONG*) (Ik220Globals.GloConfBase[cardNum]);

            if (pDestinationL)
            {
                pDestinationL = pDestinationL + CONFREG_CNTRL;

                kdPrint (( "External latch card %d at 0x%08lX\n", cardNum, pDestinationL ));

                *pDestinationL = *pDestinationL & 0xFFFFF7FF;   // Latch Ext low
                KeStallExecutionProcessor (2);                  // Wait 2µs
                *pDestinationL = *pDestinationL | 0x00000800;   // Latch Ext high
                
                Irp->IoStatus.Information = 1;
            }
            else
            {
                kdPrint (( "External latch card %d not found!\n", cardNum ));
                Irp->IoStatus.Information = 0;
            }
            break;


        case IOCTL_SYSLED:
            irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
            cardNum     = *irpBufferS;
            pDestinationL = (ULONG*) (Ik220Globals.GloConfBase[cardNum]);

            if (pDestinationL)
            {
                pDestinationL = pDestinationL + CONFREG_CNTRL;

                kdPrint (( "System LED card %d at 0x%08lX mode=%d\n", cardNum, pDestinationL, *(irpBufferS+1) ));

                if ( *(irpBufferS+1)==0 ) *pDestinationL = *pDestinationL | 0x00000004;     // System LED off
                else *pDestinationL = *pDestinationL & 0xFFFFFFFB;                          // System LED on

                
                Irp->IoStatus.Information = 1;
            }
            else
            {
                kdPrint (( "System LED card %d not found!\n", cardNum ));
                Irp->IoStatus.Information = 0;
            }
            break;


        case IOCTL_SETI:
            irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
            cardNum     = *irpBufferS;
            pDestinationL = (ULONG*) (Ik220Globals.GloConfBase[cardNum]);

            if (pDestinationL)
            {
                pDestinationL = pDestinationL + CONFREG_CNTRL;

                kdPrint (( "Set internal latch card %d at 0x%08lX to %s\n", cardNum, pDestinationL, *(irpBufferS+1)==0 ? "low" : "high"));

                if ( *(irpBufferS+1)==0 ) *pDestinationL = *pDestinationL & 0xFFFFFEFF;   // Latch Int low
                else                      *pDestinationL = *pDestinationL | 0x00000100;   // Latch Int high
                
                Irp->IoStatus.Information = 1;
            }
            else
            {
                kdPrint (( "Internal latch card %d not found!\n", cardNum ));
                Irp->IoStatus.Information = 0;
            }
            break;


        case IOCTL_SETE:
            irpBufferS  = Irp->AssociatedIrp.SystemBuffer;
            cardNum     = *irpBufferS;
            pDestinationL = (ULONG*) (Ik220Globals.GloConfBase[cardNum]);

            if (pDestinationL)
            {
                pDestinationL = pDestinationL + CONFREG_CNTRL;

                kdPrint (( "Set external latch card %d at 0x%08lX to %s\n", cardNum, pDestinationL, *(irpBufferS+1)==0 ? "low" : "high"));

                if ( *(irpBufferS+1)==0 ) *pDestinationL = *pDestinationL & 0xFFFFF7FF;   // Latch Ext low
                else                      *pDestinationL = *pDestinationL | 0x00000800;   // Latch Ext high
                
                Irp->IoStatus.Information = 1;
            }
            else
            {
                kdPrint (( "External latch card %d not found!\n", cardNum ));
                Irp->IoStatus.Information = 0;
            }
            break;


        default:
            if ((deviceExtension->DevStat & 0x4) == 0)
            {
                IK220WriteErrorLog (DeviceObject, 8 ,ioControlCode);
                deviceExtension->DevStat |= 0x4;
            }   
            kdPrint (( "unknown funtion\n" ));
            ntStatus = STATUS_NOT_IMPLEMENTED;
            Irp->IoStatus.Information = 0;
            break;
    }


//DoneWithIoctl:;

    Irp->IoStatus.Status = Status;

    Ik220Dump(
        SERIRPPATH,
        ("IK220: Complete Irp: %x\n",Irp)
        );

    Ik220CompleteRequest(deviceExtension, Irp, 0);

    return ntStatus;

}


NTSTATUS
Ik220InternalIoControl(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp)
/*++

Routine Description:

    This routine provides the initial processing for all of the
    internal Ioctrls for the serial device.

Arguments:

    PDevObj - Pointer to the device object for this device

    PIrp - Pointer to the IRP for the current request

Return Value:

    The function value is the final status of the call

--*/
{
    //
    // The status that gets returned to the caller and
    // set in the Irp.
    //
    NTSTATUS status;

    //
    // The current stack location.  This contains all of the
    // information we need to process this particular request.
    //
    PIO_STACK_LOCATION pIrpStack;

    //
    // Just what it says.  This is the Ik220 specific device
    // extension of the device object create for the Ik220 driver.
    //
    PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;

    //
    // A temporary to hold the old IRQL so that it can be
    // restored once we complete/validate this request.
    //
//    KIRQL OldIrql;

    NTSTATUS prologueStatus;

//    SYSTEM_POWER_STATE cap;

    IK220_LOCKED_PAGED_CODE();


    if ((prologueStatus = Ik220IRPPrologue(PIrp, pDevExt))
        != STATUS_SUCCESS) {
       Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
       return prologueStatus;
    }

    Ik220Dump(SERIRPPATH, ("IK220: Dispatch entry for: %x\n", PIrp));

    pIrpStack = IoGetCurrentIrpStackLocation(PIrp);
    PIrp->IoStatus.Information = 0L;
    status = STATUS_SUCCESS;

    switch (pIrpStack->Parameters.DeviceIoControl.IoControlCode) {

      //
      // Send a wait-wake IRP
      //
#if 0
      case IOCTL_IK220_INTERNAL_DO_WAIT_WAKE:
         //
         // Make sure we can do wait-wake based on what the device reported
         //

         for (cap = PowerSystemSleeping1; cap < PowerSystemMaximum; cap++) {
            if ((pDevExt->DeviceStateMap[cap] >= PowerDeviceD0)
                && (pDevExt->DeviceStateMap[cap] <= pDevExt->DeviceWake)) {
               break;
            }
         }

         if (cap < PowerSystemMaximum) {
            pDevExt->SendWaitWake = TRUE;
            status = STATUS_SUCCESS;
         } else {
            status = STATUS_NOT_SUPPORTED;
         }
         break;

      case IOCTL_IK220_INTERNAL_CANCEL_WAIT_WAKE:
         pDevExt->SendWaitWake = FALSE;

         if (pDevExt->PendingWakeIrp != NULL) {
            IoCancelIrp(pDevExt->PendingWakeIrp);
         }

         status = STATUS_SUCCESS;
         break;

#endif
      default:
         status = STATUS_INVALID_PARAMETER;
         break;

    }

    PIrp->IoStatus.Status = status;

    Ik220Dump(SERIRPPATH, ("IK220: Complete Irp: %x\n", PIrp));
    Ik220CompleteRequest(pDevExt, PIrp, 0);

    return status;
}







void IK220WriteErrorLog ( IN PDEVICE_OBJECT DeviceObject, 
                          IN ULONG EventID, 
                          IN ULONG ErrorValue  ) 
 
{ 
    PIO_ERROR_LOG_PACKET errorLogEntry; 
    UCHAR        EntrySize; 
    PUCHAR       StringLoc; 
    ULONG        TempError; 
    static WCHAR ErrorBuffer[11]   = L"00000000"; 
    static WCHAR ErrorText[11][30] = { L"Can't create device ",         // EventID  0:
                                       L"Can't read registry values ",  // EventID  1:
                                       L"Can't allocate memory ",       // EventID  2:
                                       L"Can't create symbolic link ",  // EventID  3:
                                       L"Resource conflict ",           // EventID  4:
                                       L"Hardware access failed ",      // EventID  5:
                                       L"Illegal word count ",          // EventID  6:
                                       L"Card not found ",              // EventID  7:
                                       L"Unknown function ",            // EventID  8:
                                       L"PCI bus doesn't exist ",       // EventID  9:
                                       L"No card found "                // EventID 10:
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
 
