
#include <ntddk.h>
#include <IK220DRV.h>
#include <ik220.h>
#include <wmistr.h>

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGESRP0, Ik220SystemControlDispatch)
#endif

NTSTATUS
Ik220SystemControlDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    SYSCTL_IRP_DISPOSITION disposition;
    NTSTATUS status;
    PIK220_DEVICE_EXTENSION pDevExt
      = (PIK220_DEVICE_EXTENSION)DeviceObject->DeviceExtension;

    PAGED_CODE();

    status = WmiSystemControl(   &pDevExt->WmiLibInfo,
                                 DeviceObject, 
                                 Irp,
                                 &disposition);
    switch(disposition)
    {
        case IrpProcessed:
        {
            //
            // This irp has been processed and may be completed or pending.
            break;
        }
        
        case IrpNotCompleted:
        {
            //
            // This irp has not been completed, but has been fully processed.
            // we will complete it now
            IoCompleteRequest(Irp, IO_NO_INCREMENT);                
            break;
        }
        
        case IrpForward:
        case IrpNotWmi:
        {
            //
            // This irp is either not a WMI irp or is a WMI irp targetted
            // at a device lower in the stack.
            IoSkipCurrentIrpStackLocation(Irp);
            status = IoCallDriver(pDevExt->LowerDeviceObject, Irp);
            break;
        }
                                    
        default:
        {
            //
            // We really should never get here, but if we do just forward....
            ASSERT(FALSE);
            IoSkipCurrentIrpStackLocation(Irp);
            status = IoCallDriver(pDevExt->LowerDeviceObject, Irp);
            break;
        }        
    }
    
    return(status);

}
