
#include <ntddk.h>
#include <IK220DRV.h>
#include <ik220.h>


static const PHYSICAL_ADDRESS Ik220PhysicalZero = {0};


#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGESRP0, Ik220CreateDevObj)
#pragma alloc_text(PAGESRP0, Ik220AddDevice)
#pragma alloc_text(PAGESRP0, Ik220PnpDispatch)
#pragma alloc_text(PAGESRP0, Ik220StartDevice)
#pragma alloc_text(PAGESRP0, Ik220FinishStartDevice)
#pragma alloc_text(PAGESRP0, Ik220GetPortInfo)
#pragma alloc_text(PAGESRP0, Ik220DoExternalNaming)
#pragma alloc_text(PAGESRP0, Ik220ItemCallBack)
#endif // ALLOC_PRAGMA

//
// Instantiate the GUID
//

#include <initguid.h>

// {20B8602B-20B8602B-44dc-AE0A-484BCBDD49B5}
DEFINE_GUID(GUID_CLASS_IK,
            0x20b8602b, 0x593f, 0x44dc, 0xae, 0xa, 0x48, 0x4b, 0xcb, 0xdd, 0x49, 0xb5);


#if DBG

UCHAR *IkSystemCapString[] = {
   "PowerSystemUnspecified",
   "PowerSystemWorking",
   "PowerSystemSleeping1",
   "PowerSystemSleeping2",
   "PowerSystemSleeping3",
   "PowerSystemHibernate",
   "PowerSystemShutdown",
   "PowerSystemMaximum"
};

UCHAR *IkDeviceCapString[] = {
   "PowerDeviceUnspecified",
   "PowerDeviceD0",
   "PowerDeviceD1",
   "PowerDeviceD2",
   "PowerDeviceD3",
   "PowerDeviceMaximum"
};

#endif // DBG


NTSTATUS
Ik220SyncCompletion(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp,
                    IN PKEVENT Ik220SyncEvent)
{
   KeSetEvent(Ik220SyncEvent, IO_NO_INCREMENT, FALSE);
   return STATUS_MORE_PROCESSING_REQUIRED;
}


NTSTATUS
Ik220CreateDevObj(IN PDRIVER_OBJECT DriverObject,
                  OUT PDEVICE_OBJECT *NewDeviceObject)

/*++

Routine Description:

    This routine will create and initialize a functional device object to
    be attached to a Ik220 controller PDO.

Arguments:

    DriverObject - a pointer to the driver object this is created under
    NewDeviceObject - a location to store the pointer to the new device object

Return Value:

    STATUS_SUCCESS if everything was successful
    reason for failure otherwise

--*/
{
   UNICODE_STRING deviceObjName;
   PDEVICE_OBJECT deviceObject = NULL;
   PIK220_DEVICE_EXTENSION pDevExt;
   NTSTATUS status = STATUS_SUCCESS;
   UNICODE_STRING instanceStr;
   WCHAR instanceNumberBuffer[20];


   PAGED_CODE();

   Ik220Dump (SERTRACECALLS,("IK220: Enter Ik220CreateDevObj\n"));

   //
   // Zero out allocated memory pointers so we know if they must be freed
   //

   RtlZeroMemory(&deviceObjName, sizeof(UNICODE_STRING));

   deviceObjName.MaximumLength = DEVICE_OBJECT_NAME_LENGTH * sizeof(WCHAR);
   deviceObjName.Buffer = ExAllocatePool(PagedPool, deviceObjName.MaximumLength
                                     + sizeof(WCHAR));


   if (deviceObjName.Buffer == NULL) {
      Ik220LogError(DriverObject, NULL, Ik220PhysicalZero, Ik220PhysicalZero,
                     0, 0, 0, 19, STATUS_SUCCESS, IK220_INSUFFICIENT_RESOURCES,
                     0, NULL, 0, NULL);
      Ik220Dump(SERERRORS, ("IK220: Couldn't allocate memory for device name"
                             "\n"));

      return STATUS_INSUFFICIENT_RESOURCES;

   }

   RtlZeroMemory(deviceObjName.Buffer, deviceObjName.MaximumLength
                 + sizeof(WCHAR));


   RtlAppendUnicodeToString(&deviceObjName, L"\\Device\\Ik220Drv");

   RtlInitUnicodeString(&instanceStr, NULL);

   instanceStr.MaximumLength = sizeof(instanceNumberBuffer);
   instanceStr.Buffer = instanceNumberBuffer;

   RtlIntegerToUnicodeString(Ik220Globals.instances++, 10, &instanceStr);

   RtlAppendUnicodeStringToString(&deviceObjName, &instanceStr);


   //
   // Create the device object
   //

   status = IoCreateDevice(DriverObject, sizeof(IK220_DEVICE_EXTENSION),
                           &deviceObjName, FILE_DEVICE_IK220,
                           FILE_DEVICE_SECURE_OPEN, TRUE, &deviceObject);


   if (!NT_SUCCESS(status)) {
      Ik220Dump(SERERRORS, ("Ik220AddDevice: Create device failed - %x \n",
                             status));
      goto Ik220CreateDevObjError;
   }

   ASSERT(deviceObject != NULL);


   //
   // The device object has a pointer to an area of non-paged
   // pool allocated for this device.  This will be the device
   // extension. Zero it out.
   //

   pDevExt = deviceObject->DeviceExtension;
   RtlZeroMemory(pDevExt, sizeof(IK220_DEVICE_EXTENSION));

   pDevExt->instance = Ik220Globals.instances-1;

   //
   // Initialize the count of IRP's pending
   //

   pDevExt->PendingIRPCnt = 1;


   //
   // Initialize the count of DPC's pending
   //

   pDevExt->DpcCount = 1;

   //
   // Allocate Pool and save the nt device name in the device extension.
   //

   pDevExt->DeviceName.Buffer =
      ExAllocatePool(PagedPool, deviceObjName.Length + sizeof(WCHAR));

   if (!pDevExt->DeviceName.Buffer) {

      Ik220LogError(
                    DriverObject,
                    NULL,
                    Ik220PhysicalZero,
                    Ik220PhysicalZero,
                    0,
                    0,
                    0,
                    19,
                    STATUS_SUCCESS,
                    IK220_INSUFFICIENT_RESOURCES,
                    0,
                    NULL,
                    0,
                    NULL
                    );
      Ik220Dump(
                SERERRORS,
                ("IK220: Couldn't allocate memory for DeviceName\n"));

      status = STATUS_INSUFFICIENT_RESOURCES;
      goto Ik220CreateDevObjError;
   }

   pDevExt->DeviceName.MaximumLength = deviceObjName.Length
      + sizeof(WCHAR);

   //
   // Zero fill it.
   //

   RtlZeroMemory(pDevExt->DeviceName.Buffer,
                 pDevExt->DeviceName.MaximumLength);

   RtlAppendUnicodeStringToString(&pDevExt->DeviceName, &deviceObjName);

   pDevExt->NtNameForPort.Buffer = ExAllocatePool(PagedPool,
                                                  deviceObjName.MaximumLength);

   if (pDevExt->NtNameForPort.Buffer == NULL) {
      Ik220Dump(SERERRORS, ("Ik220AddDevice: Cannot allocate memory for "
                             "NtName\n"));
      status = STATUS_INSUFFICIENT_RESOURCES;
      goto Ik220CreateDevObjError;
   }

   pDevExt->NtNameForPort.MaximumLength = deviceObjName.MaximumLength;
   RtlAppendUnicodeStringToString(&pDevExt->NtNameForPort,
                                  &deviceObjName);



   //
   // Set up the device extension.
   //

   pDevExt->DeviceIsOpened = FALSE;
   pDevExt->DeviceObject   = deviceObject;
   pDevExt->DriverObject   = DriverObject;
   pDevExt->DeviceObject   = deviceObject;
   pDevExt->PowerState     = PowerDeviceD0;

   pDevExt->CreatedSymbolicLink    = FALSE;
   pDevExt->OwnsPowerPolicy = TRUE;

   InitializeListHead(&pDevExt->AllDevObjs);
//   InitializeListHead(&pDevExt->ReadQueue);
//   InitializeListHead(&pDevExt->WriteQueue);
//   InitializeListHead(&pDevExt->MaskQueue);
//   InitializeListHead(&pDevExt->PurgeQueue);
   InitializeListHead(&pDevExt->StalledIrpQueue);

   ExInitializeFastMutex(&pDevExt->OpenMutex);
   ExInitializeFastMutex(&pDevExt->CloseMutex);

   KeInitializeEvent(&pDevExt->PendingIRPEvent, SynchronizationEvent, FALSE);
   KeInitializeEvent(&pDevExt->PendingDpcEvent, SynchronizationEvent, FALSE);
   KeInitializeEvent(&pDevExt->PowerD0Event, SynchronizationEvent, FALSE);


   deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

   *NewDeviceObject = deviceObject;

   ExFreePool(deviceObjName.Buffer);

   Ik220Dump (SERTRACECALLS,("IK220: Leave Ik220CreateDevObj\n") );
   return STATUS_SUCCESS;


   Ik220CreateDevObjError:

   Ik220Dump (SERERRORS,("IK220: Ik220CreateDevObj Error, Cleaning up\n") );

   //
   // Free the allocated strings for the NT and symbolic names if they exist.
   //

   if (deviceObjName.Buffer != NULL) {
      ExFreePool(deviceObjName.Buffer);
   }

   if (deviceObject) {

      if (pDevExt->NtNameForPort.Buffer != NULL) {
         ExFreePool(pDevExt->NtNameForPort.Buffer);
      }

      if (pDevExt->DeviceName.Buffer != NULL) {
         ExFreePool(pDevExt->DeviceName.Buffer);
      }

      IoDeleteDevice(deviceObject);
   }

   *NewDeviceObject = NULL;

   Ik220Dump (SERTRACECALLS,("IK220: Leave Ik220CreateDevObj\n") );
   return status;
}



NTSTATUS
Ik220AddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PPdo)
/*++

Routine Description:

    This routine creates a functional device object for com ports in the
    system and attaches them to the physical device objects for the ports


Arguments:

    DriverObject - a pointer to the object for this driver

    PPdo - a pointer to the PDO in the stack we need to attach to

Return Value:

    status from device creation and initialization

--*/
{
   PDEVICE_OBJECT pNewDevObj = NULL;
   PDEVICE_OBJECT pLowerDevObj = NULL;
   NTSTATUS status;
   PIK220_DEVICE_EXTENSION pDevExt;

   PAGED_CODE();

   Ik220Dump(SERTRACECALLS, ("IK220: Enter Ik220AddDevice with PPdo "
                              "0x%x\n", PPdo));
   if (PPdo == NULL) {
      //
      // Return no more devices
      //

      Ik220Dump(SERERRORS, ("Ik220AddDevice: Enumeration request, returning"
                             " NO_MORE_ENTRIES\n"));

      return (STATUS_NO_MORE_ENTRIES);
   }



   //
   // create and initialize the new device object
   //

   status = Ik220CreateDevObj(DriverObject, &pNewDevObj);

   if (!NT_SUCCESS(status)) {

      Ik220Dump(SERERRORS,
                 ("Ik220AddDevice - error creating new devobj [%#08lx]\n",
                  status));
      return status;
   }


   //
   // Layer our DO on top of the lower device object
   // The return value is a pointer to the device object to which the
   // DO is actually attached.
   //

   pLowerDevObj = IoAttachDeviceToDeviceStack(pNewDevObj, PPdo);


   //
   // No status. Do the best we can.
   //
   ASSERT(pLowerDevObj != NULL);


   pDevExt = pNewDevObj->DeviceExtension;
   pDevExt->LowerDeviceObject = pLowerDevObj;
   pDevExt->Pdo = PPdo;



   //
   // Specify that this driver only supports buffered IO.  This basically
   // means that the IO system copies the users data to and from
   // system supplied buffers.
   //
   // Also specify that we are power pagable.
   //

   pNewDevObj->Flags |= DO_BUFFERED_IO | DO_POWER_PAGABLE;

   Ik220Dump(SERTRACECALLS, ("IK220: Leave Ik220AddDevice\n"));

   return status;
}


NTSTATUS
Ik220PnpDispatch(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp)
/*++

Routine Description:

    This is a dispatch routine for the IRPs that come to the driver with the
    IRP_MJ_PNP major code (plug-and-play IRPs).

Arguments:

    PDevObj - Pointer to the device object for this device

    PIrp - Pointer to the IRP for the current request

Return Value:

    The function value is the final status of the call


--*/
{
   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
   PDEVICE_OBJECT pLowerDevObj = pDevExt->LowerDeviceObject;
   PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(PIrp);
   NTSTATUS status;
   PDEVICE_CAPABILITIES pDevCaps;

   PAGED_CODE();

   if ((status = Ik220IRPPrologue(PIrp, pDevExt)) != STATUS_SUCCESS) {
      Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
      return status;
   }

   switch (pIrpStack->MinorFunction) {
   case IRP_MN_QUERY_CAPABILITIES: {
      PKEVENT pQueryCapsEvent;
      SYSTEM_POWER_STATE cap;

      Ik220Dump(SERPNPPOWER, ("IK220: Got IRP_MN_QUERY_DEVICE_CAPABILITIES "
                               "IRP\n"));

      pQueryCapsEvent = ExAllocatePool(NonPagedPool, sizeof(KEVENT));

      if (pQueryCapsEvent == NULL) {
         PIrp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
         Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
         return STATUS_INSUFFICIENT_RESOURCES;
      }

      KeInitializeEvent(pQueryCapsEvent, SynchronizationEvent, FALSE);

      IoCopyCurrentIrpStackLocationToNext(PIrp);
      IoSetCompletionRoutine(PIrp, Ik220SyncCompletion, pQueryCapsEvent,
                             TRUE, TRUE, TRUE);

      status = IoCallDriver(pLowerDevObj, PIrp);


      //
      // Wait for lower drivers to be done with the Irp
      //

      if (status == STATUS_PENDING) {
         KeWaitForSingleObject(pQueryCapsEvent, Executive, KernelMode, FALSE,
                               NULL);
      }

      ExFreePool(pQueryCapsEvent);

      status = PIrp->IoStatus.Status;

      if (pIrpStack->Parameters.DeviceCapabilities.Capabilities == NULL) {
         goto errQueryCaps;
      }

      //
      // Save off their power capabilities
      //

      Ik220Dump(SERPNPPOWER, ("IK220: Mapping power capabilities\n"));

      pIrpStack = IoGetCurrentIrpStackLocation(PIrp);

      pDevCaps = pIrpStack->Parameters.DeviceCapabilities.Capabilities;

      for (cap = PowerSystemSleeping1; cap < PowerSystemMaximum;
           cap++) {
         Ik220Dump(SERPNPPOWER, ("  IK220: %d: %s <--> %s\n",
                                  cap, IkSystemCapString[cap],
                                  IkDeviceCapString[pDevCaps->DeviceState[cap]]
                                  ));

         pDevExt->DeviceStateMap[cap] = pDevCaps->DeviceState[cap];
      }

      pDevExt->DeviceStateMap[PowerSystemUnspecified]
         = PowerDeviceUnspecified;

      pDevExt->DeviceStateMap[PowerSystemWorking]
        = PowerDeviceD0;

      pDevExt->SystemWake = pDevCaps->SystemWake;
      pDevExt->DeviceWake = pDevCaps->DeviceWake;

      errQueryCaps:;

      Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
      return status;
   }

   case IRP_MN_QUERY_DEVICE_RELATIONS:
      //
      // We just pass this down -- serenum enumerates our bus for us.
      //

      Ik220Dump (SERPNPPOWER, ("IK220: Got IRP_MN_QUERY_DEVICE_RELATIONS "
                                "Irp\n"));

      switch (pIrpStack->Parameters.QueryDeviceRelations.Type) {
      case BusRelations:
         Ik220Dump(SERPNPPOWER, ("------- BusRelations Query\n"));
         break;

      case EjectionRelations:
         Ik220Dump(SERPNPPOWER, ("------- EjectionRelations Query\n"));
         break;

      case PowerRelations:
         Ik220Dump(SERPNPPOWER, ("------- PowerRelations Query\n"));
         break;

      case RemovalRelations:
         Ik220Dump(SERPNPPOWER, ("------- RemovalRelations Query\n"));
         break;

      case TargetDeviceRelation:
         Ik220Dump(SERPNPPOWER, ("------- TargetDeviceRelation Query\n"));
         break;

      default:
         Ik220Dump(SERPNPPOWER, ("------- Unknown Query\n"));
         break;
      }

      IoSkipCurrentIrpStackLocation(PIrp);
      status = Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
      return status;


   case IRP_MN_QUERY_INTERFACE:
      Ik220Dump (SERPNPPOWER, ("IK220: Got IRP_MN_QUERY_INTERFACE Irp\n"));
      break;


   case IRP_MN_QUERY_RESOURCE_REQUIREMENTS:
      Ik220Dump (SERPNPPOWER, ("IK220: Got "
                                "IRP_MN_QUERY_RESOURCE_REQUIREMENTS Irp\n"));
      break;


   case IRP_MN_START_DEVICE: {
//    PVOID startLockPtr;

      Ik220Dump (SERPNPPOWER, ("IK220: Got IRP_MN_START_DEVICE Irp\n"));

      //
      // Ik220StartDevice will pass this Irp to the next driver,
      // and process it as completion so just complete it here.
      //

      Ik220LockPagableSectionByHandle(Ik220Globals.PAGEIK_Handle);

      //
      // We used to make sure the stack was powered up, but now it
      // is supposed to be done implicitly by start_device.
      // If that wasn't the case we would just make this call:
      //
      //   status = Ik220GotoPowerState(PDevObj, pDevExt, PowerDeviceD0);
      //

      pDevExt->PowerState = PowerDeviceD0;

      status = Ik220StartDevice(PDevObj, PIrp);

      (void)Ik220GotoPowerState(PDevObj, pDevExt, PowerDeviceD3);

      Ik220UnlockPagableImageSection(Ik220Globals.PAGEIK_Handle);


      PIrp->IoStatus.Status = status;

      Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
      return status;
   }


   case IRP_MN_READ_CONFIG:
      Ik220Dump (SERPNPPOWER, ("IK220: Got IRP_MN_READ_CONFIG Irp\n"));
      break;


   case IRP_MN_WRITE_CONFIG:
      Ik220Dump (SERPNPPOWER, ("IK220: Got IRP_MN_WRITE_CONFIG Irp\n"));
      break;


   case IRP_MN_EJECT:
      Ik220Dump (SERPNPPOWER, ("IK220: Got IRP_MN_EJECT Irp\n"));
      break;


   case IRP_MN_SET_LOCK:
      Ik220Dump (SERPNPPOWER, ("IK220: Got IRP_MN_SET_LOCK Irp\n"));
      break;


   case IRP_MN_QUERY_ID: {
         UNICODE_STRING pIdBuf;
         PWCHAR pPnpIdStr;
         ULONG pnpIdStrLen;

         Ik220Dump(SERPNPPOWER, ("IK220: Got IRP_MN_QUERY_ID Irp\n"));

         if (pIrpStack->Parameters.QueryId.IdType != BusQueryHardwareIDs
             && pIrpStack->Parameters.QueryId.IdType != BusQueryCompatibleIDs) {
            IoSkipCurrentIrpStackLocation(PIrp);
            return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
         }

         if (pIrpStack->Parameters.QueryId.IdType == BusQueryCompatibleIDs) {
            PIrp->IoStatus.Status = STATUS_SUCCESS;
            IoSkipCurrentIrpStackLocation(PIrp);
            return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
         }

         pPnpIdStr = IK220_PNP_ID_STR;
         pnpIdStrLen = sizeof(IK220_PNP_ID_STR);

         if (PIrp->IoStatus.Information != 0) {
            ULONG curStrLen;
            ULONG allocLen = 0;
            PWSTR curStr = (PWSTR)PIrp->IoStatus.Information;

            //
            // We have to walk the strings to count the amount of space to
            // reallocate
            //

            while ((curStrLen = wcslen(curStr)) != 0) {
               allocLen += curStrLen * sizeof(WCHAR) + sizeof(UNICODE_NULL);
               curStr += curStrLen + 1;
            }

            allocLen += sizeof(UNICODE_NULL);

            pIdBuf.Buffer = ExAllocatePool(PagedPool, allocLen
                                           + pnpIdStrLen
                                           + sizeof(WCHAR));

            if (pIdBuf.Buffer == NULL) {
               //
               // Clean up after other drivers since we are
               // sending the irp back up.
               //

               ExFreePool((PWSTR)PIrp->IoStatus.Information);


               PIrp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
               PIrp->IoStatus.Information = 0;
               Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
               return STATUS_INSUFFICIENT_RESOURCES;
            }

            pIdBuf.MaximumLength = (USHORT)(allocLen + pnpIdStrLen);
            pIdBuf.Length = (USHORT)allocLen - sizeof(UNICODE_NULL);

            RtlZeroMemory(pIdBuf.Buffer, pIdBuf.MaximumLength + sizeof(WCHAR));
            RtlCopyMemory(pIdBuf.Buffer, (PWSTR)PIrp->IoStatus.Information,
                          allocLen);
            RtlAppendUnicodeToString(&pIdBuf, pPnpIdStr);

            //
            // Free what the previous driver allocated
            //

            ExFreePool((PWSTR)PIrp->IoStatus.Information);


         } else {

            Ik220Dump(SERPNPPOWER, ("IK220: ID is sole ID\n"));

            pIdBuf.Buffer = ExAllocatePool(PagedPool, pnpIdStrLen
                                           + sizeof(WCHAR) * 2);

            if (pIdBuf.Buffer == NULL) {
               PIrp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
               PIrp->IoStatus.Information = 0;
               Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
               return STATUS_INSUFFICIENT_RESOURCES;
            }

            pIdBuf.MaximumLength  = (USHORT)pnpIdStrLen;
            pIdBuf.Length = 0;

            RtlZeroMemory(pIdBuf.Buffer, pIdBuf.MaximumLength + sizeof(WCHAR)
                          * 2);

            RtlAppendUnicodeToString(&pIdBuf, pPnpIdStr);
         }

         PIrp->IoStatus.Information = (ULONG_PTR)pIdBuf.Buffer;
         PIrp->IoStatus.Status = STATUS_SUCCESS;

         IoCopyCurrentIrpStackLocationToNext(PIrp);
         return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
      }

      case IRP_MN_FILTER_RESOURCE_REQUIREMENTS: {
         PKEVENT pResFiltEvent;
         PIO_RESOURCE_REQUIREMENTS_LIST pReqList;
         PIO_RESOURCE_LIST pResList;
         PIO_RESOURCE_DESCRIPTOR pResDesc;
         ULONG j;
//       ULONG reqCnt;
         ULONG gotISR;
         ULONG gotInt;
         ULONG gotMem;
         ULONG listNum;

         Ik220Dump(SERPNPPOWER, ("IK220: Got "
                                  "IRP_MN_FILTER_RESOURCE_REQUIREMENTS Irp\n"));
         Ik220Dump(SERPNPPOWER, ("------- for device %x\n", pLowerDevObj));


         pResFiltEvent = ExAllocatePool(NonPagedPool, sizeof(KEVENT));

         if (pResFiltEvent == NULL) {
            PIrp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
            Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
            return STATUS_INSUFFICIENT_RESOURCES;
         }

         KeInitializeEvent(pResFiltEvent, SynchronizationEvent, FALSE);

         IoCopyCurrentIrpStackLocationToNext(PIrp);
         IoSetCompletionRoutine(PIrp, Ik220SyncCompletion, pResFiltEvent,
                                TRUE, TRUE, TRUE);

         status = IoCallDriver(pLowerDevObj, PIrp);


         //
         // Wait for lower drivers to be done with the Irp
         //

         if (status == STATUS_PENDING) {
            KeWaitForSingleObject (pResFiltEvent, Executive, KernelMode, FALSE,
                                   NULL);
         }

         ExFreePool(pResFiltEvent);

         if (PIrp->IoStatus.Information == 0) {
            if (pIrpStack->Parameters.FilterResourceRequirements
                .IoResourceRequirementList == 0) {
               Ik220Dump(SERPNPPOWER, ("------- Can't filter NULL resources!"
                                        "\n"));
               status = PIrp->IoStatus.Status;
               Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
               return status;
            }

            PIrp->IoStatus.Information = (ULONG_PTR)pIrpStack->Parameters
                                        .FilterResourceRequirements
                                        .IoResourceRequirementList;

         }


         //
         // Force ISR ports in IO_RES_REQ_LIST to shared status
         // Force interrupts to shared status
         //

         //
         // We will only process the first list -- multiport boards
         // should not have alternative resources
         //

         pReqList = (PIO_RESOURCE_REQUIREMENTS_LIST)PIrp->IoStatus.Information;
         pResList = &pReqList->List[0];

         Ik220Dump(SERPNPPOWER, ("------- List has %x lists "
                                  "(including alternatives)\n",
                  pReqList->AlternativeLists));

         for (listNum = 0; listNum < (pReqList->AlternativeLists);
              listNum++) {
            gotISR = 0;
            gotInt = 0;
            gotMem = 0;

            Ik220Dump(SERPNPPOWER, ("------- List has %x resources in it\n",
                     pResList->Count));

            for (j = 0; (j < pResList->Count); j++) {
               pResDesc = &pResList->Descriptors[j];

               kdPrint(("------- List item %d: Type %x\n",
                        j, pResDesc->Type));

               switch (pResDesc->Type) {
               case CmResourceTypePort:
                  kdPrint(("------- Port:%x,Flag:%x\n",
                           pResDesc->u.Port.MinimumAddress.u.LowPart, pResDesc->Flags));
                  break;

               case CmResourceTypeInterrupt:
                  kdPrint(("------- Irq:Vector:%x\n",
                           pResDesc->u.Interrupt.MinimumVector));
                  if (!gotInt) {
                     gotInt = 1;
                     if (pResDesc->ShareDisposition != CmResourceShareShared) {
                        pResDesc->ShareDisposition = CmResourceShareShared;
                        Ik220Dump(SERPNPPOWER, ("------- Sharing interrupt "
                                                 "for device %x\n",
                                                 pLowerDevObj));
                     } else {
//                        pDevExt->InterruptShareable = TRUE;
                        Ik220Dump(SERPNPPOWER, ("------- Globally sharing "
                                                 " interrupt for device %x\n",
                                                 pLowerDevObj));
                     }
                  }
                  break;

               case CmResourceTypeMemory:
                  kdPrint(("------- Mem:%x,Len:%x\n",
                           pResDesc->u.Memory.MinimumAddress.u.LowPart,
                           pResDesc->u.Memory.Length));
                  break;
         
               default:
                  break;
               }

               //
               // If we found what we need, we can break out of the loop
               //

               if (gotInt) {
                  break;
               }
            }

            pResList = (PIO_RESOURCE_LIST)((PUCHAR)pResList
                                           + sizeof(IO_RESOURCE_LIST)
                                           + sizeof(IO_RESOURCE_DESCRIPTOR)
                                           * (pResList->Count - 1));
         }



         PIrp->IoStatus.Status = STATUS_SUCCESS;
         Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
         return STATUS_SUCCESS;
      }

   case IRP_MN_QUERY_PNP_DEVICE_STATE:
      {
         if (pDevExt->Flags & IK220_FLAGS_BROKENHW) {
            (PNP_DEVICE_STATE)PIrp->IoStatus.Information |= PNP_DEVICE_FAILED;

            PIrp->IoStatus.Status = STATUS_SUCCESS;
         }

         IoCopyCurrentIrpStackLocationToNext(PIrp);
         return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
      }

   case IRP_MN_STOP_DEVICE:
      {
         ULONG pendingIRPs;
//         KIRQL oldIrql;

         Ik220Dump(SERPNPPOWER, ("IK220: Got IRP_MN_STOP_DEVICE Irp\n"));
         Ik220Dump(SERPNPPOWER, ("------- for device %x\n", pLowerDevObj));



         Ik220SetFlags(pDevExt, IK220_FLAGS_STOPPED);
         Ik220SetAccept(pDevExt,IK220_PNPACCEPT_STOPPED);
         Ik220ClearAccept(pDevExt, IK220_PNPACCEPT_STOPPING);

         pDevExt->PNPState = IK220_PNP_STOPPING;

         //
         // From this point on all non-PNP IRP's will be queued
         //

         //
         // Decrement for entry here
         //

         InterlockedDecrement(&pDevExt->PendingIRPCnt);

         //
         // Decrement for stopping
         //

         pendingIRPs = InterlockedDecrement(&pDevExt->PendingIRPCnt);

         if (pendingIRPs) {
            KeWaitForSingleObject(&pDevExt->PendingIRPEvent, Executive,
                                  KernelMode, FALSE, NULL);
         }

         //
         // Re-increment the count for later
         //

         InterlockedIncrement(&pDevExt->PendingIRPCnt);

         //
         // We need to free resources...basically this is a remove
         // without the detach from the stack.
         //

         if (pDevExt->Flags & IK220_FLAGS_STARTED) {
            Ik220ReleaseResources(pDevExt);
         }

         //
         // Pass the irp down
         //

         PIrp->IoStatus.Status = STATUS_SUCCESS;
         IoSkipCurrentIrpStackLocation(PIrp);

         return IoCallDriver(pLowerDevObj, PIrp);
      }

   case IRP_MN_QUERY_STOP_DEVICE:
      {
//         KIRQL oldIrql;

         Ik220Dump(SERPNPPOWER, ("IK220: Got IRP_MN_QUERY_STOP_DEVICE Irp\n")
                    );
         Ik220Dump(SERPNPPOWER, ("------- for device %x\n", pLowerDevObj));

         //
         // See if we should succeed a stop query
         //


         //
         // If the device hasn't started yet, we ignore this request
         // and just pass it down.
         //

         if (pDevExt->PNPState != IK220_PNP_STARTED) {
            IoSkipCurrentIrpStackLocation(PIrp);
            return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
         }

         //
         // Lock around the open status
         //

         ExAcquireFastMutex(&pDevExt->OpenMutex);

         if (pDevExt->DeviceIsOpened) {
            ExReleaseFastMutex(&pDevExt->OpenMutex);
            PIrp->IoStatus.Status = STATUS_DEVICE_BUSY;
            Ik220Dump(SERPNPPOWER, ("------- failing; device open\n"));
            Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
            return STATUS_DEVICE_BUSY;
         }

         pDevExt->PNPState = IK220_PNP_QSTOP;

         Ik220SetAccept(pDevExt, IK220_PNPACCEPT_STOPPING);
         //
         // Unlock around the open status
         //

         ExReleaseFastMutex(&pDevExt->OpenMutex);

         PIrp->IoStatus.Status = STATUS_SUCCESS;
         IoCopyCurrentIrpStackLocationToNext(PIrp);
         return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
      }

   case IRP_MN_CANCEL_STOP_DEVICE:
      Ik220Dump(SERPNPPOWER, ("IK220: Got "
                               "IRP_MN_CANCEL_STOP_DEVICE Irp\n"));
      Ik220Dump(SERPNPPOWER, ("------- for device %x\n", pLowerDevObj));

      if (pDevExt->PNPState == IK220_PNP_QSTOP) {
         //
         // Restore the device state
         //

         pDevExt->PNPState = IK220_PNP_STARTED;
         Ik220ClearAccept(pDevExt, IK220_PNPACCEPT_STOPPING);
      }

      PIrp->IoStatus.Status = STATUS_SUCCESS;
      IoCopyCurrentIrpStackLocationToNext(PIrp);
      return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);

   case IRP_MN_CANCEL_REMOVE_DEVICE:

      Ik220Dump(SERPNPPOWER, ("IK220: Got "
                               "IRP_MN_CANCEL_REMOVE_DEVICE Irp\n"));
      Ik220Dump(SERPNPPOWER, ("------- for device %x\n", pLowerDevObj));

      //
      // Restore the device state
      //

      pDevExt->PNPState = IK220_PNP_STARTED;
      Ik220ClearAccept(pDevExt, IK220_PNPACCEPT_REMOVING);

      PIrp->IoStatus.Status = STATUS_SUCCESS;
      IoCopyCurrentIrpStackLocationToNext(PIrp);
      return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);

   case IRP_MN_QUERY_REMOVE_DEVICE:
      {
//         KIRQL oldIrql;
         Ik220Dump(SERPNPPOWER, ("IK220: Got "
                                  "IRP_MN_QUERY_REMOVE_DEVICE Irp\n"));
         Ik220Dump(SERPNPPOWER, ("------- for device %x\n", pLowerDevObj));

         ExAcquireFastMutex(&pDevExt->OpenMutex);

         //
         // See if we should succeed a remove query
         //

         if (pDevExt->DeviceIsOpened) {
            ExReleaseFastMutex(&pDevExt->OpenMutex);
            PIrp->IoStatus.Status = STATUS_DEVICE_BUSY;
            Ik220Dump(SERPNPPOWER, ("------- failing; device open\n"));
            Ik220CompleteRequest(pDevExt, PIrp, IO_NO_INCREMENT);
            return STATUS_DEVICE_BUSY;
         }

         pDevExt->PNPState = IK220_PNP_QREMOVE;
         Ik220SetAccept(pDevExt, IK220_PNPACCEPT_REMOVING);
         ExReleaseFastMutex(&pDevExt->OpenMutex);

         PIrp->IoStatus.Status = STATUS_SUCCESS;
         IoCopyCurrentIrpStackLocationToNext(PIrp);
         return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
      }

   case IRP_MN_SURPRISE_REMOVAL:
      {
         ULONG pendingIRPs;
//         KIRQL oldIrql;

         Ik220Dump(SERPNPPOWER, ("IK220: Got IRP_MN_SURPRISE_REMOVAL Irp\n"));
         Ik220Dump(SERPNPPOWER, ("------- for device %x\n", pLowerDevObj));

         //
         // Prevent any new I/O to the device
         //

         Ik220SetAccept(pDevExt, IK220_PNPACCEPT_SURPRISE_REMOVING);

         //
         // Dismiss all pending requests
         //

         Ik220KillPendingIrps(PDevObj);

         //
         // Wait for any pending requests we raced on.
         //

         //
         // Decrement once for ourselves
         //

         InterlockedDecrement(&pDevExt->PendingIRPCnt);

         //
         // Decrement for the remove
         //

         pendingIRPs = InterlockedDecrement(&pDevExt->PendingIRPCnt);

         if (pendingIRPs) {
            KeWaitForSingleObject(&pDevExt->PendingIRPEvent, Executive,
                                  KernelMode, FALSE, NULL);
         }

         //
         // Reset for subsequent remove
         //

         InterlockedIncrement(&pDevExt->PendingIRPCnt);

         //
         // Remove any external interfaces and release resources
         //

         Ik220DisableInterfacesResources(PDevObj);

         PIrp->IoStatus.Status = STATUS_SUCCESS;
         IoSkipCurrentIrpStackLocation(PIrp);

         return Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
      }

   case IRP_MN_REMOVE_DEVICE:

      {
         ULONG pendingIRPs;
//         KIRQL oldIrql;

         Ik220Dump(SERPNPPOWER, ("IK220: Got IRP_MN_REMOVE_DEVICE Irp\n"));
         Ik220Dump(SERPNPPOWER, ("------- for device %x\n", pLowerDevObj));

         //
         // If we get this, we have to remove
         //

         //
         // Mark as not accepting requests
         //

         Ik220SetAccept(pDevExt, IK220_PNPACCEPT_REMOVING);

         //
         // Complete all pending requests
         //

         Ik220KillPendingIrps(PDevObj);

         //
         // Decrement for this Irp itself
         //

         InterlockedDecrement(&pDevExt->PendingIRPCnt);

         //
         // Wait for any pending requests we raced on -- this decrement
         // is for our "placeholder".
         //

         pendingIRPs = InterlockedDecrement(&pDevExt->PendingIRPCnt);

         if (pendingIRPs) {
            KeWaitForSingleObject(&pDevExt->PendingIRPEvent, Executive,
                                  KernelMode, FALSE, NULL);
         }

         //
         // Remove us
         //

         Ik220RemoveDevObj(PDevObj);


         //
         // Pass the irp down
         //

         PIrp->IoStatus.Status = STATUS_SUCCESS;

         IoCopyCurrentIrpStackLocationToNext(PIrp);

         //
         // We do decrement here because we incremented on entry here.
         //

         IoCallDriver(pLowerDevObj, PIrp);


         return STATUS_SUCCESS;
      }

   default:
      break;



   }   // switch (pIrpStack->MinorFunction)

   //
   // Pass to driver beneath us
   //

   IoSkipCurrentIrpStackLocation(PIrp);
   status = Ik220IoCallDriver(pDevExt, pLowerDevObj, PIrp);
   return status;
}


NTSTATUS
Ik220FinishStartDevice(IN PDEVICE_OBJECT PDevObj,
                        IN PCM_RESOURCE_LIST PResList,
                        IN PCM_RESOURCE_LIST PTrResList,
                        PIK220_USER_DATA PUserData)
/*++

Routine Description:

    This routine does serial-specific procedures to start a device.  It
    does this either for a legacy device detected by its registry entries,
    or for a PnP device after the start IRP has been sent down the stack.


Arguments:

   PDevObj    -  Pointer to the devobj that is starting

   PResList   -  Pointer to the untranslated resources needed by this device

   PTrResList -  Pointer to the translated resources needed by this device

   PUserData  -  Pointer to the user-specified resources/attributes


  Return Value:

    STATUS_SUCCESS on success, something else appropriate on failure


--*/
{
   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
   NTSTATUS status = STATUS_SUCCESS;
   PCONFIG_DATA pConfig;
   HANDLE pnpKey;
   ULONG one = 1;
   BOOLEAN allocedUserData = FALSE;
   int ii;

   PAGED_CODE();

   //
   // See if this is a restart, and if so don't reallocate the world
   //

   if ((pDevExt->Flags & IK220_FLAGS_STOPPED)
       && (pDevExt->Flags & IK220_FLAGS_STARTED)) {
      Ik220ClearFlags(pDevExt, IK220_FLAGS_STOPPED);

      pDevExt->PNPState = IK220_PNP_RESTARTING;

      //
      // Re-init resource-related things in the extension
      //

      pDevExt->OriginalController = Ik220PhysicalZero;
//      pDevExt->OriginalInterruptStatus = Ik220PhysicalZero;

      pDevExt->Controller = NULL;
//      pDevExt->InterruptStatus = NULL;
      pDevExt->Interrupt = NULL;
      for (ii = 0; ii < 2; ++ii) {
         pDevExt->OriginalPortBase[ii] = Ik220PhysicalZero;
         pDevExt->PortBase[ii] = NULL;
         pDevExt->SpanOfPortBase[ii] = 0;
         pDevExt->PortBaseAddressSpace[ii] = 0;
         pDevExt->PortBaseBusNumber[ii] = 0;
      }

      pDevExt->OriginalConfBase = Ik220PhysicalZero;
      pDevExt->ConfBase = NULL;
      pDevExt->SpanOfConfBase = 0;
      pDevExt->ConfBaseAddressSpace = 0;
      pDevExt->ConfBaseBusNumber = 0;

      pDevExt->SpanOfController = 0;
//      pDevExt->SpanOfInterruptStatus = 0;

      pDevExt->Vector = 0;
      pDevExt->Irql = 0;
      pDevExt->OriginalVector = 0;
      pDevExt->OriginalIrql = 0;
      pDevExt->AddressSpace = 0;
      pDevExt->BusNumber = 0;
      pDevExt->InterfaceType = 0;

//      pDevExt->CIsrSw = NULL;

      ASSERT(PUserData == NULL);


      PUserData = ExAllocatePool(PagedPool, sizeof(IK220_USER_DATA));

      if (PUserData == NULL) {
         return STATUS_INSUFFICIENT_RESOURCES;
      }

      allocedUserData = TRUE;

      RtlZeroMemory(PUserData, sizeof(IK220_USER_DATA));

//      PUserData->DisablePort = FALSE;

   } else {
      //
      // Mark as serenumerable -- toss status because we can
      // still start without this key.
      //

      status = IoOpenDeviceRegistryKey(pDevExt->Pdo,
                                       PLUGPLAY_REGKEY_DEVICE,
                                       STANDARD_RIGHTS_WRITE, &pnpKey);

      if (NT_SUCCESS(status)) {
         ULONG powerPolicy = 0;

         //
         // Find out if we own power policy
         //

         Ik220GetRegistryKeyValue(pnpKey, L"Ik220RelinquishPowerPolicy",
                                   sizeof(L"Ik220RelinquishPowerPolicy"),
                                   &powerPolicy, sizeof(ULONG));

         pDevExt->OwnsPowerPolicy = powerPolicy ? FALSE : TRUE;


         ZwClose(pnpKey);
      }
   }

   //
   // Allocate the config record.
   //

   pConfig = ExAllocatePool (PagedPool, sizeof(CONFIG_DATA));

   if (pConfig == NULL) {

      Ik220LogError(pDevExt->DriverObject, NULL, Ik220PhysicalZero,
                     Ik220PhysicalZero, 0, 0, 0, 37, STATUS_SUCCESS,
                     IK220_INSUFFICIENT_RESOURCES, 0, NULL, 0, NULL);

      Ik220Dump(SERERRORS, ("IK220: Couldn't allocate memory for the\n"
                             "------  user configuration record\n"));

      status = STATUS_INSUFFICIENT_RESOURCES;
      goto Ik220FinishStartDeviceError;
   }

   RtlZeroMemory(pConfig, sizeof(CONFIG_DATA));


   //
   // Get the configuration info for the device.
   //

   status = Ik220GetPortInfo(PDevObj, PResList, PTrResList, pConfig,
                              PUserData);

   if (!NT_SUCCESS(status)) {
      goto Ik220FinishStartDeviceError;
   }


   //
   // See if we are in the proper power state.
   //



   if (pDevExt->PowerState != PowerDeviceD0) {

      status = Ik220GotoPowerState(pDevExt->Pdo, pDevExt, PowerDeviceD0);

      if (!NT_SUCCESS(status)) {
         goto Ik220FinishStartDeviceError;
      }
   }

   //
   // Find and initialize the controller
   //

   status = Ik220FindInitController(PDevObj, pConfig);

   if (!NT_SUCCESS(status)) {
      goto Ik220FinishStartDeviceError;
   }


   //
   // Add the PDevObj to the master list
   //

   InsertTailList(&Ik220Globals.AllDevObjs, &pDevExt->AllDevObjs);


   //
   // Reset the device.
   //


   //
   // While the device isn't open, disable all interrupts.
   //

   DISABLE_ALL_INTERRUPTS (pDevExt->Controller);


   //
   // This should set up everything as it should be when
   // a device is to be opened.  We do need to lower the
   // modem lines, and disable the recalcitrant fifo
   // so that it will show up if the user boots to dos.
   //

   Ik220Reset(pDevExt);

   Ik220MarkClose(pDevExt);

   if (pDevExt->PNPState == IK220_PNP_ADDED ) {
      //
      // Do the external naming now that the device is accessible.
      //

      status = Ik220DoExternalNaming(pDevExt, pDevExt->DeviceObject->
                                      DriverObject);


      if (!NT_SUCCESS(status)) {
         Ik220Dump (SERERRORS,("IK220: External Naming Failed - Status %x\n",
                                status));

         //
         // Allow the device to start anyhow
         //

         status = STATUS_SUCCESS;
      }
   } else {
      Ik220Dump(SERPNPPOWER, ("Not doing external naming -- state is %x\n",
                               pDevExt->PNPState));
   }

Ik220FinishStartDeviceError:;

   if (!NT_SUCCESS (status)) {

      Ik220Dump(SERDIAG1,("IK220: Cleaning up failed start\n"));

      //
      // Resources created by this routine will be cleaned up by the remove
      //

      if (pDevExt->PNPState == IK220_PNP_RESTARTING) {
         //
         // Kill all that lives and breathes -- we'll clean up the
         // rest on the impending remove
         //

         Ik220KillPendingIrps(PDevObj);

         //
         // In fact, pretend we're removing so we don't take any
         // more irps
         //

         Ik220SetAccept(pDevExt, IK220_PNPACCEPT_REMOVING);
         Ik220ClearFlags(pDevExt, IK220_FLAGS_STARTED);
      }
   } else { // SUCCESS

//      //
//      // Fill in WMI hardware data
//      //
//
//      pDevExt->WmiHwData.IrqNumber = pDevExt->Irql;
//      pDevExt->WmiHwData.IrqLevel = pDevExt->Irql;
//      pDevExt->WmiHwData.IrqVector = pDevExt->Vector;
//      pDevExt->WmiHwData.IrqAffinityMask = pConfig->Affinity;
//      pDevExt->WmiHwData.InterruptType = pConfig->InterruptMode == Latched
//         ? IK220_WMI_INTTYPE_LATCHED : IK220_WMI_INTTYPE_LEVEL;
//      pDevExt->WmiHwData.BaseIOAddress = (ULONG_PTR)pDevExt->Controller;
//
//      //
//      // Fill in WMI device state data (as defaults)
//      //
//
//      pDevExt->WmiCommData.BaudRate = pDevExt->CurrentBaud;
//      pDevExt->WmiCommData.BitsPerByte = (pDevExt->LineControl & 0x03) + 5;
//      pDevExt->WmiCommData.ParityCheckEnable = (pDevExt->LineControl & 0x08)
//         ? TRUE : FALSE;
//
//      switch (pDevExt->LineControl & IK220_PARITY_MASK) {
//      case IK220_NONE_PARITY:
//         pDevExt->WmiCommData.Parity = IK220_WMI_PARITY_NONE;
//         break;
//
//      case IK220_ODD_PARITY:
//         pDevExt->WmiCommData.Parity = IK220_WMI_PARITY_ODD;
//         break;
//
//      case IK220_EVEN_PARITY:
//         pDevExt->WmiCommData.Parity = IK220_WMI_PARITY_EVEN;
//         break;
//
//      case IK220_MARK_PARITY:
//         pDevExt->WmiCommData.Parity = IK220_WMI_PARITY_MARK;
//         break;
//
//      case IK220_SPACE_PARITY:
//         pDevExt->WmiCommData.Parity = IK220_WMI_PARITY_SPACE;
//         break;
//
//      default:
//         ASSERTMSG(0, "IK220: Illegal Parity setting for WMI");
//         pDevExt->WmiCommData.Parity = IK220_WMI_PARITY_NONE;
//         break;
//      }
//
//      pDevExt->WmiCommData.StopBits = pDevExt->LineControl & IK220_STOP_MASK
//         ? (pDevExt->WmiCommData.BitsPerByte == 5 ? IK220_WMI_STOP_1_5
//            : IK220_WMI_STOP_2) : IK220_WMI_STOP_1;
//      pDevExt->WmiCommData.XoffCharacter = pDevExt->SpecialChars.XoffChar;
//      pDevExt->WmiCommData.XoffXmitThreshold = pDevExt->HandFlow.XoffLimit;
//      pDevExt->WmiCommData.XonCharacter = pDevExt->SpecialChars.XonChar;
//      pDevExt->WmiCommData.XonXmitThreshold = pDevExt->HandFlow.XonLimit;
//      pDevExt->WmiCommData.MaximumBaudRate
//         = Ik220ReportMaxBaudRate(pDevExt->SupportedBauds);
//      pDevExt->WmiCommData.MaximumOutputBufferSize = (UINT32)((ULONG)-1);
//      pDevExt->WmiCommData.MaximumInputBufferSize = (UINT32)((ULONG)-1);
//      pDevExt->WmiCommData.Support16BitMode = FALSE;
//      pDevExt->WmiCommData.SupportDTRDSR = TRUE;
//      pDevExt->WmiCommData.SupportIntervalTimeouts = TRUE;
//      pDevExt->WmiCommData.SupportParityCheck = TRUE;
//      pDevExt->WmiCommData.SupportRTSCTS = TRUE;
//      pDevExt->WmiCommData.SupportXonXoff = TRUE;
//      pDevExt->WmiCommData.SettableBaudRate = TRUE;
//      pDevExt->WmiCommData.SettableDataBits = TRUE;
//      pDevExt->WmiCommData.SettableFlowControl = TRUE;
//      pDevExt->WmiCommData.SettableParity = TRUE;
//      pDevExt->WmiCommData.SettableParityCheck = TRUE;
//      pDevExt->WmiCommData.SettableStopBits = TRUE;
//      pDevExt->WmiCommData.IsBusy = FALSE;
//
//      //
//      // Fill in wmi perf data (all zero's)
//      //
//
//      RtlZeroMemory(&pDevExt->WmiPerfData, sizeof(pDevExt->WmiPerfData));
//
//
//      if (pDevExt->PNPState == IK220_PNP_ADDED) {
//         PULONG countSoFar = &IoGetConfigurationInformation()->Ik220Count;
//         (*countSoFar)++;
//
//         //
//         // Register for WMI
//         //
//
//         pDevExt->WmiLibInfo.GuidCount = sizeof(Ik220WmiGuidList) /
//                                              sizeof(WMIGUIDREGINFO);
//         pDevExt->WmiLibInfo.GuidList = Ik220WmiGuidList;
//         ASSERT (pDevExt->WmiLibInfo.GuidCount == IK220_WMI_GUID_LIST_SIZE);
//
//         pDevExt->WmiLibInfo.QueryWmiRegInfo = Ik220QueryWmiRegInfo;
//         pDevExt->WmiLibInfo.QueryWmiDataBlock = Ik220QueryWmiDataBlock;
//         pDevExt->WmiLibInfo.SetWmiDataBlock = Ik220SetWmiDataBlock;
//         pDevExt->WmiLibInfo.SetWmiDataItem = Ik220SetWmiDataItem;
//         pDevExt->WmiLibInfo.ExecuteWmiMethod = NULL;
//         pDevExt->WmiLibInfo.WmiFunctionControl = NULL;
//
//         IoWMIRegistrationControl(PDevObj, WMIREG_ACTION_REGISTER);
//
//      }
//
//      if (pDevExt->PNPState == IK220_PNP_RESTARTING) {
//         //
//         // Release the stalled IRP's
//         //
//
//         Ik220UnstallIrps(pDevExt);
//      }

      pDevExt->PNPState = IK220_PNP_STARTED;
      Ik220ClearAccept(pDevExt, ~IK220_PNPACCEPT_OK);
      Ik220SetFlags(pDevExt, IK220_FLAGS_STARTED);

   }

   if (pConfig) {
      ExFreePool (pConfig);
   }

   if ((PUserData != NULL) && allocedUserData) {
      ExFreePool(PUserData);
   }

   Ik220Dump (SERTRACECALLS, ("IK220: leaving Ik220FinishStartDevice\n"));

   return status;
}


NTSTATUS
Ik220StartDevice(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp)
/*++

Routine Description:

    This routine first passes the start device Irp down the stack then
    it picks up the resources for the device, ititializes, puts it on any
    appropriate lists (i.e shared interrupt or interrupt status) and
    connects the interrupt.

Arguments:

    PDevObj - Pointer to the device object for this device

    PIrp - Pointer to the IRP for the current request

Return Value:

    Return status


--*/
{
   PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(PIrp);
   NTSTATUS status = STATUS_NOT_IMPLEMENTED;
   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
   PDEVICE_OBJECT pLowerDevObj = pDevExt->LowerDeviceObject;

   PAGED_CODE();

   Ik220Dump (SERTRACECALLS, ("IK220: entering Ik220StartDevice\n"));


   //
   // Pass this down to the next device object
   //

   KeInitializeEvent(&pDevExt->Ik220StartEvent, SynchronizationEvent,
                     FALSE);

   IoCopyCurrentIrpStackLocationToNext(PIrp);
   IoSetCompletionRoutine(PIrp, Ik220SyncCompletion,
                          &pDevExt->Ik220StartEvent, TRUE, TRUE, TRUE);

   status = IoCallDriver(pLowerDevObj, PIrp);


   //
   // Wait for lower drivers to be done with the Irp
   //

   if (status == STATUS_PENDING) {
      KeWaitForSingleObject (&pDevExt->Ik220StartEvent, Executive, KernelMode,
                             FALSE, NULL);

      status = PIrp->IoStatus.Status;
   }

   if (!NT_SUCCESS(status)) {
      Ik220Dump (SERERRORS, ("IK220: error with IoCallDriver %x\n", status));
      return status;
   }


   //
   // Do the Ik220 specific items to start the device
   //

   status = Ik220FinishStartDevice(PDevObj, pIrpStack->Parameters.StartDevice
                                    .AllocatedResources,
                                    pIrpStack->Parameters.StartDevice
                                    .AllocatedResourcesTranslated, NULL);
   return status;
}


NTSTATUS
Ik220ItemCallBack(
                  IN PVOID Context,
                  IN PUNICODE_STRING PathName,
                  IN INTERFACE_TYPE BusType,
                  IN ULONG BusNumber,
                  IN PKEY_VALUE_FULL_INFORMATION *BusInformation,
                  IN CONFIGURATION_TYPE ControllerType,
                  IN ULONG ControllerNumber,
                  IN PKEY_VALUE_FULL_INFORMATION *ControllerInformation,
                  IN CONFIGURATION_TYPE PeripheralType,
                  IN ULONG PeripheralNumber,
                  IN PKEY_VALUE_FULL_INFORMATION *PeripheralInformation
                  )
/*++

Routine Description:

    This routine is called to check if a particular item
    is present in the registry.

Arguments:

    Context - Pointer to a boolean.

    PathName - unicode registry path.  Not Used.

    BusType - Internal, Isa, ...

    BusNumber - Which bus if we are on a multibus system.

    BusInformation - Configuration information about the bus. Not Used.

    ControllerType - Controller type.

    ControllerNumber - Which controller if there is more than one
                       controller in the system.

    ControllerInformation - Array of pointers to the three pieces of
                            registry information.

    PeripheralType - Should be a peripheral.

    PeripheralNumber - Which peripheral - not used..

    PeripheralInformation - Configuration information. Not Used.

Return Value:

    STATUS_SUCCESS

--*/
{
   PAGED_CODE();

   *((BOOLEAN *)Context) = TRUE;
   return STATUS_SUCCESS;
}


NTSTATUS
Ik220GetPortInfo(IN PDEVICE_OBJECT PDevObj, IN PCM_RESOURCE_LIST PResList,
                  IN PCM_RESOURCE_LIST PTrResList, OUT PCONFIG_DATA PConfig,
                  IN PIK220_USER_DATA PUserData)
/*++

Routine Description:

    This routine will get the configuration information and put
    it and the translated values into CONFIG_DATA structures.
    It first sets up with  defaults and then queries the registry
    to see if the user has overridden these defaults; if this is a legacy
    multiport card, it uses the info in PUserData instead of groping the
    registry again.

Arguments:

    PDevObj - Pointer to the device object.

    PResList - Pointer to the untranslated resources requested.

    PTrResList - Pointer to the translated resources requested.

    PConfig - Pointer to configuration info

    PUserData - Pointer to data discovered in the registry for
    legacy devices.

Return Value:

    STATUS_SUCCESS if consistant configuration was found - otherwise.
    returns STATUS_IK220_NO_DEVICE_INITED.

--*/
{
   PIK220_DEVICE_EXTENSION pDevExt = PDevObj->DeviceExtension;
   PDEVICE_OBJECT pLowerDevObj = pDevExt->LowerDeviceObject;
   NTSTATUS status = STATUS_NOT_IMPLEMENTED;
   CONFIGURATION_TYPE pointer = PointerPeripheral;
//   CONFIGURATION_TYPE controllerType  = Ik220Controller;

   HANDLE keyHandle;
   ULONG count;
   ULONG i;
   INTERFACE_TYPE interfaceType;

   PCM_PARTIAL_RESOURCE_LIST pPartialResourceList, pPartialTrResourceList;
   PCM_PARTIAL_RESOURCE_DESCRIPTOR pPartialResourceDesc, pPartialTrResourceDesc;

   PCM_FULL_RESOURCE_DESCRIPTOR pFullResourceDesc = NULL,
      pFullTrResourceDesc = NULL;

   ULONG defaultInterruptMode;
//   ULONG defaultAddressSpace;
   ULONG defaultInterfaceType;
   ULONG zero = 0;
   ULONG gotInt = 0;
   ULONG gotMem = 0;
   ULONG gotISR = 0;
   ULONG gotIO = 0;
   ULONG ioResIndex = 0;
   ULONG curIoIndex = 0;

   PAGED_CODE();

   Ik220Dump(SERTRACECALLS, ("IK220: entering Ik220GetPortInfo\n"));

   Ik220Dump(SERPNPPOWER, ("IK220: resource pointer is %x\n", PResList));
   Ik220Dump(SERPNPPOWER, ("IK220: TR resource pointer is %x\n", PTrResList));


   if ((PResList == NULL) || (PTrResList == NULL)) {
      //
      // This shouldn't happen in theory
      //

       ASSERT(PResList != NULL);
       ASSERT(PTrResList != NULL);

      //
      // This status is as appropriate as I can think of
      //
      return STATUS_INSUFFICIENT_RESOURCES;
   }

   //
   // Each resource list should have only one set of resources
   //

   ASSERT(PResList->Count == 1);
   ASSERT(PTrResList->Count == 1);

   //
   // See if this is a multiport device.  This way we allow other
   // pseudo-serial devices with extra resources to specify another range
   // of I/O ports.  If this is not a multiport, we only look at the first
   // range.  If it is a multiport, we look at the first two ranges.
   //

   status = IoOpenDeviceRegistryKey(pDevExt->Pdo, PLUGPLAY_REGKEY_DEVICE,
                                    STANDARD_RIGHTS_WRITE, &keyHandle);

   if (!NT_SUCCESS(status)) {
      return status;
   }

   status = Ik220GetRegistryKeyValue(keyHandle, L"Ik220IoResourcesIndex",
                                      sizeof(L"Ik220IoResourcesIndex"),
                                      &ioResIndex, sizeof(ULONG));

   if (!NT_SUCCESS(status)) {
      ioResIndex = 0;
   }

   ZwClose(keyHandle);


   pFullResourceDesc   = &PResList->List[0];
   pFullTrResourceDesc = &PTrResList->List[0];

   //
   // Ok, if we have a full resource descriptor.  Let's take it apart.
   //

   if (pFullResourceDesc) {
      pPartialResourceList    = &pFullResourceDesc->PartialResourceList;
      pPartialResourceDesc    = pPartialResourceList->PartialDescriptors;
      count                   = pPartialResourceList->Count;

      //
      // Pull out the stuff that is in the full descriptor.
      //

      PConfig->InterfaceType  = pFullResourceDesc->InterfaceType;
      PConfig->BusNumber      = pFullResourceDesc->BusNumber;

      //
      // Now run through the partial resource descriptors looking for the port,
      // interrupt, and clock rate.
      //

//      PConfig->ClockRate = 1843200;
//      PConfig->InterruptStatus = Ik220PhysicalZero;
//      PConfig->SpanOfInterruptStatus = IK220_STATUS_LENGTH;

      for (i = 0;     i < count;     i++, pPartialResourceDesc++) {

         switch (pPartialResourceDesc->Type) {
         case CmResourceTypePort: {

            if (gotIO == 0) { // This is the ik220 register set
               if (curIoIndex == ioResIndex) {
                  gotIO = 1;
                  PConfig->Controller       = pPartialResourceDesc->u.Port.Start;
                  PConfig->SpanOfController = pPartialResourceDesc->u.Port.Length;
                  PConfig->AddressSpace     = pPartialResourceDesc->Flags;
                  Ik220Dump(SERDIAG1, ("IK220: Port address: %x\n",
                                        PConfig->Controller.LowPart));
               } else {
                  curIoIndex++;
               }
            }
            break;
         }

         case CmResourceTypeInterrupt: {
            if (gotInt == 0) {
               gotInt = 1;
               PConfig->OriginalVector = pPartialResourceDesc->u.Interrupt.Vector;
               PConfig->OriginalIrql   = pPartialResourceDesc->u.Interrupt.Level;
               PConfig->Affinity       = pPartialResourceDesc->u.Interrupt.Affinity;

               if (pPartialResourceDesc->Flags
                   & CM_RESOURCE_INTERRUPT_LATCHED) {
                  PConfig->InterruptMode  = Latched;
               } else {
                  PConfig->InterruptMode  = LevelSensitive;
               }
               Ik220Dump(SERDIAG1, ("IK220: OriginalVector: %x\n",
                                     PConfig->OriginalVector));
               Ik220Dump(SERDIAG1, ("IK220: OriginalIrql: %x\n",
                                     PConfig->OriginalIrql));
            }
            break;
         }

         case CmResourceTypeMemory: {
            switch (gotMem) {
              case 0:
                PConfig->ConfBase             = pPartialResourceDesc->u.Memory.Start;
                PConfig->SpanOfConfBase       = pPartialResourceDesc->u.Memory.Length;
                PConfig->ConfBaseAddressSpace = pPartialResourceDesc->Flags;
                PConfig->ConfBaseBusNumber    = pFullResourceDesc->BusNumber;
                Ik220Dump(SERDIAG1, ("IK220: ConfBase: %x\n",
                                      PConfig->ConfBase));
                gotMem = 1;
                break;
              case 1:
                PConfig->PortBase[0]             = pPartialResourceDesc->u.Memory.Start;
                PConfig->SpanOfPortBase[0]       = pPartialResourceDesc->u.Memory.Length;
                PConfig->PortBaseAddressSpace[0] = pPartialResourceDesc->Flags;
                PConfig->PortBaseBusNumber[0]    = pFullResourceDesc->BusNumber;
                Ik220Dump(SERDIAG1, ("IK220: PortBase[0]: %x\n",
                                      PConfig->PortBase[0]));
                gotMem = 2;
                break;
              case 2:
                PConfig->PortBase[1]             = pPartialResourceDesc->u.Memory.Start;
                PConfig->SpanOfPortBase[1]       = pPartialResourceDesc->u.Memory.Length;
                PConfig->PortBaseAddressSpace[1] = pPartialResourceDesc->Flags;
                PConfig->PortBaseBusNumber[1]    = pFullResourceDesc->BusNumber;
                Ik220Dump(SERDIAG1, ("IK220: PortBase[1]: %x\n",
                                      PConfig->PortBase[1]));
                gotMem = 3;
                break;
              default:
                break;
            }
            break;
         }
         
         case CmResourceTypeDeviceSpecific: {
               break;
            }


         default: {
               break;
            }
         }   // switch (pPartialResourceDesc->Type)
      }       // for (i = 0;     i < count;     i++, pPartialResourceDesc++)
   }           // if (pFullResourceDesc)


   //
   // Do the same for the translated resources
   //

   gotInt = 0;
   gotMem = 0;
   gotISR = 0;
   gotIO = 0;
   curIoIndex = 0;

   if (pFullTrResourceDesc) {
      pPartialTrResourceList = &pFullTrResourceDesc->PartialResourceList;
      pPartialTrResourceDesc = pPartialTrResourceList->PartialDescriptors;
      count = pPartialTrResourceList->Count;

      //
      // Reload PConfig with the translated values for later use
      //

      PConfig->InterfaceType  = pFullTrResourceDesc->InterfaceType;
      PConfig->BusNumber      = pFullTrResourceDesc->BusNumber;

//      PConfig->TrInterruptStatus = Ik220PhysicalZero;

      for (i = 0;     i < count;     i++, pPartialTrResourceDesc++) {

         switch (pPartialTrResourceDesc->Type) {
         case CmResourceTypePort: {

            if (gotIO == 0) {
               if (curIoIndex == ioResIndex) {
                  gotIO = 1;
                  PConfig->TrController = pPartialTrResourceDesc->u.Port.Start;
                  PConfig->AddressSpace = pPartialTrResourceDesc->Flags;
                  Ik220Dump(SERDIAG1, ("IK220: Translated port address: %x\n",
                                        PConfig->TrController.LowPart));
               } else {
                  curIoIndex++;
               }
            }
            break;
         }

         case CmResourceTypeInterrupt: {
            if (gotInt == 0) {
               gotInt = 1;
               PConfig->TrVector = pPartialTrResourceDesc->u.Interrupt.Vector;
               PConfig->TrIrql   = pPartialTrResourceDesc->u.Interrupt.Level;
               PConfig->Affinity = pPartialTrResourceDesc->u.Interrupt.Affinity;
               Ik220Dump(SERDIAG1, ("IK220: Translated vector: %x\n",
                                     PConfig->TrVector));
               Ik220Dump(SERDIAG1, ("IK220: Translated irql: %x\n",
                                     PConfig->TrIrql));
            }
            break;
         }

         case CmResourceTypeMemory: {
            switch (gotMem) {
              case 0:
                PConfig->TrConfBase           = pPartialTrResourceDesc->u.Memory.Start;
                PConfig->ConfBaseAddressSpace = pPartialTrResourceDesc->Flags;
                Ik220Dump(SERDIAG1, ("IK220: Translated ConfBase: %x\n",
                                      PConfig->TrConfBase));
                gotMem = 1;
                break;
              case 1:
                PConfig->TrPortBase[0]           = pPartialTrResourceDesc->u.Memory.Start;
                PConfig->PortBaseAddressSpace[0] = pPartialTrResourceDesc->Flags;
                Ik220Dump(SERDIAG1, ("IK220: Translated PortBase[0]: %x\n",
                                      PConfig->TrPortBase[0]));
                gotMem = 2;
                break;
              case 2:
                PConfig->TrPortBase[1]           = pPartialTrResourceDesc->u.Memory.Start;
                PConfig->PortBaseAddressSpace[1] = pPartialTrResourceDesc->Flags;
                Ik220Dump(SERDIAG1, ("IK220: Translated PortBase[1]: %x\n",
                                      PConfig->TrPortBase[1]));
                gotMem = 3;
                break;
              default:
                break;
            }
            break;
         }

         default: {
               break;
         }
         }   // switch (pPartialTrResourceDesc->Type)
      }       // for (i = 0;     i < count;     i++, pPartialTrResourceDesc++)
   }           // if (pFullTrResourceDesc)


   //
   // Initialize a config data structure with default values for those that
   // may not already be initialized.
   //

   PConfig->PortIndex = 0;

   //
   // Query the registry to look for the first bus on
   // the system (that isn't the internal bus - we assume
   // that the firmware code knows about those ports).  We
   // will use that as the default bus if no bustype or bus
   // number is specified in the "user" configuration records.
   //

   defaultInterfaceType            = (ULONG)Isa;


   for (interfaceType = 0;
       interfaceType < MaximumInterfaceType;
       interfaceType++
       ) {

      ULONG   busZero     = 0;
      BOOLEAN foundOne    = FALSE;


      if (interfaceType != Internal) {

         IoQueryDeviceDescription(
                                 &interfaceType,
                                 &busZero,
                                 NULL,
                                 NULL,
                                 NULL,
                                 NULL,
                                 Ik220ItemCallBack,
                                 &foundOne
                                 );

         if (foundOne) {
            defaultInterfaceType = (ULONG)interfaceType;

            if (defaultInterfaceType == MicroChannel) {

               defaultInterruptMode = CM_RESOURCE_INTERRUPT_LEVEL_SENSITIVE;
            }
            break;
         }
      }   // if (interfaceType != Internal)
   }       // for (interfaceType = 0


   //
   // Get any user data associated with the port now and override the
   // values passed in if applicable.  If this a legacy device, this
   // is where we may actually get the parameters.
   //

   //
   // Open the "Device Parameters" section of registry for this device object.
   // If PUserData is NULL, this is PnP enumerated and we need to check,
   // otherwise we are doing a legacy device and have the info already.
   //


   if (PUserData == NULL) {
      status = IoOpenDeviceRegistryKey (pDevExt->Pdo,
                                        PLUGPLAY_REGKEY_DEVICE,
                                        STANDARD_RIGHTS_READ,
                                        &keyHandle);

      if (!NT_SUCCESS(status)) {

         Ik220Dump(SERERRORS, ("IK220: IoOpenDeviceRegistryKey failed - %x "
                                "\n", status));
         goto PortInfoCleanUp;

      } else {

         status = Ik220GetRegistryKeyValue (keyHandle,
                                             L"PortIndex",
                                             sizeof(L"PortIndex"),
                                             &PConfig->PortIndex,
                                             sizeof (ULONG));

         if (!NT_SUCCESS(status)) {
//            PConfig->ClockRate = defaultClockRate;
         }

         ZwClose (keyHandle);
      }
   } else {
      //
      // This was a legacy device, either use a driver default or copy over
      // the user-specified values.
      //
      ULONG badValue = (ULONG)-1;

      PConfig->PortIndex = PUserData->UserPortIndex;
   }

   //
   // Do some error checking on the configuration info we have.
   //
   // Make sure that the interrupt is non zero (which we defaulted
   // it to).
   //
   // Make sure that the portaddress is non zero (which we defaulted
   // it to).
   //
   // Make sure that the DosDevices is not NULL (which we defaulted
   // it to).
   //
   // We need to make sure that if an interrupt status
   // was specified, that a port index was also specfied,
   // and if so that the port index is <= maximum ports
   // on a board.
   //
   // We should also validate that the bus type and number
   // are correct.
   //
   // We will also validate that the interrupt mode makes
   // sense for the bus.
   //

   if (!PConfig->Controller.LowPart) {

      //
      // Ehhhh! Lose Game.
      //

      Ik220LogError(
                    PDevObj->DriverObject,
                    NULL,
                    PConfig->Controller,
                    Ik220PhysicalZero,
                    0,
                    0,
                    0,
                    58,
                    STATUS_SUCCESS,
                    IK220_INVALID_USER_CONFIG,
                    0,
                    NULL,
                    sizeof(L"PortAddress") + sizeof(WCHAR),
                    L"PortAddress"
                    );

      Ik220Dump (SERERRORS,
                  ("IK220: Bogus port address %x\n",
                   PConfig->Controller.LowPart));

      status = IK220_INVALID_USER_CONFIG;
      goto PortInfoCleanUp;
   }


   if (!PConfig->PortBase[0].LowPart) {

      //
      // Ehhhh! Lose Game.
      //

      Ik220LogError(
                    PDevObj->DriverObject,
                    NULL,
                    PConfig->PortBase[0],
                    Ik220PhysicalZero,
                    0,
                    0,
                    0,
                    58,
                    STATUS_SUCCESS,
                    IK220_INVALID_USER_CONFIG,
                    0,
                    NULL,
                    sizeof(L"PortAddress") + sizeof(WCHAR),
                    L"PortAddress"
                    );

      Ik220Dump (SERERRORS,
                  ("IK220: Bogus port address %x\n",
                   PConfig->PortBase[0].LowPart));

      status = IK220_INVALID_USER_CONFIG;
      goto PortInfoCleanUp;
   }


   if (!PConfig->ConfBase.LowPart) {

      //
      // Ehhhh! Lose Game.
      //

      Ik220LogError(
                    PDevObj->DriverObject,
                    NULL,
                    PConfig->ConfBase,
                    Ik220PhysicalZero,
                    0,
                    0,
                    0,
                    58,
                    STATUS_SUCCESS,
                    IK220_INVALID_USER_CONFIG,
                    0,
                    NULL,
                    sizeof(L"PortAddress") + sizeof(WCHAR),
                    L"PortAddress"
                    );

      Ik220Dump (SERERRORS,
                  ("IK220: Bogus port address %x\n",
                   PConfig->ConfBase.LowPart));

      status = IK220_INVALID_USER_CONFIG;
      goto PortInfoCleanUp;
   }


   if (!PConfig->OriginalVector) {

      //
      // Ehhhh! Lose Game.
      //

      Ik220LogError(
                    pDevExt->DriverObject,
                    NULL,
                    PConfig->Controller,
                    Ik220PhysicalZero,
                    0,
                    0,
                    0,
                    59,
                    STATUS_SUCCESS,
                    IK220_INVALID_USER_CONFIG,
                    pDevExt->DeviceName.Length,
                    pDevExt->DeviceName.Buffer,
                    sizeof (L"Interrupt"),
                    L"Interrupt"
                    );

      Ik220Dump (SERERRORS,("IK220: Bogus vector %x\n",
                             PConfig->OriginalVector));

      status = IK220_INVALID_USER_CONFIG;
      goto PortInfoCleanUp;
   }


//   if (PConfig->InterruptStatus.LowPart != 0) {
//
//      if (PConfig->PortIndex == MAXULONG) {
//
//         //
//         // Ehhhh! Lose Game.
//         //
//
//         Ik220LogError(
//                       pDevExt->DriverObject,
//                       NULL,
//                       PConfig->Controller,
//                       Ik220PhysicalZero,
//                       0,
//                       0,
//                       0,
//                       30,
//                       STATUS_SUCCESS,
//                       IK220_INVALID_PORT_INDEX,
//                       0,
//                       NULL,
//                       0,
//                       NULL
//                       );
//
//         Ik220Dump (SERERRORS,
//                     ("IK220: Bogus port index %x\n", PConfig->PortIndex));
//
//         status = IK220_INVALID_PORT_INDEX;
//         goto PortInfoCleanUp;
//
//      } else if (!PConfig->PortIndex) {
//
//         //
//         // So sorry, you must have a non-zero port index.
//         //
//
//         Ik220LogError(
//                       pDevExt->DriverObject,
//                       NULL,
//                       PConfig->Controller,
//                       Ik220PhysicalZero,
//                       0,
//                       0,
//                       0,
//                       31,
//                       STATUS_SUCCESS,
//                       IK220_INVALID_PORT_INDEX,
//                       0,
//                       NULL,
//                       0,
//                       NULL
//                       );
//
//         Ik220Dump(
//                   SERERRORS,
//                   ("IK220: Port index must be > 0 for any\n"
//                    "------- port on a multiport card: %x\n",
//                    PConfig->PortIndex)
//                   );
//
//         status = IK220_INVALID_PORT_INDEX;
//         goto PortInfoCleanUp;
//
//      } else {
//
//         if (PConfig->Indexed) {
//
//            if (PConfig->PortIndex > IK220_MAX_PORTS_INDEXED) {
//
//               Ik220LogError(
//                             pDevExt->DriverObject,
//                             NULL,
//                             PConfig->Controller,
//                             Ik220PhysicalZero,
//                             0,
//                             0,
//                             0,
//                             32,
//                             STATUS_SUCCESS,
//                             IK220_PORT_INDEX_TOO_HIGH,
//                             0,
//                             NULL,
//                             0,
//                             NULL
//                             );
//
//               Ik220Dump (SERERRORS,
//                           ("IK220: port index to large %x\n",
//                            PConfig->PortIndex));
//
//               status = IK220_PORT_INDEX_TOO_HIGH;
//               goto PortInfoCleanUp;
//            }
//
//         } else {
//
//            if (PConfig->PortIndex > IK220_MAX_PORTS_NONINDEXED) {
//
//               Ik220LogError(
//                             pDevExt->DriverObject,
//                             NULL,
//                             PConfig->Controller,
//                             Ik220PhysicalZero,
//                             0,
//                             0,
//                             0,
//                             33,
//                             STATUS_SUCCESS,
//                             IK220_PORT_INDEX_TOO_HIGH,
//                             0,
//                             NULL,
//                             0,
//                             NULL
//                             );
//
//               Ik220Dump (SERERRORS,
//                           ("IK220: port index to large %x\n",
//                            PConfig->PortIndex));
//
//               status = IK220_PORT_INDEX_TOO_HIGH;
//               goto PortInfoCleanUp;
//            }
//
//         }
//
//      }   // else  (if !PConfig->PortIndex)
//
//   }       // if (PConfig->InterruptStatus != 0)


   //
   // We don't want to cause the hal to have a bad day,
   // so let's check the interface type and bus number.
   //
   // We only need to check the registry if they aren't
   // equal to the defaults.
   //

   if (PConfig->BusNumber != 0) {

      BOOLEAN foundIt;

      if (PConfig->InterfaceType >= MaximumInterfaceType) {

         //
         // Ehhhh! Lose Game.
         //

         Ik220LogError(
                       pDevExt->DriverObject,
                       NULL,
                       PConfig->Controller,
                       Ik220PhysicalZero,
                       0,
                       0,
                       0,
                       34,
                       STATUS_SUCCESS,
                       IK220_UNKNOWN_BUS,
                       0,
                       NULL,
                       0,
                       NULL
                       );

         Ik220Dump (SERERRORS,
                     ("IK220: Invalid Bus type %x\n", PConfig->BusNumber));

         status = IK220_UNKNOWN_BUS;
         goto PortInfoCleanUp;
      }

      IoQueryDeviceDescription(
                              (INTERFACE_TYPE *)&PConfig->InterfaceType,
                              &zero,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              Ik220ItemCallBack,
                              &foundIt
                              );

      if (!foundIt) {

         Ik220LogError(
                       pDevExt->DriverObject,
                       NULL,
                       PConfig->Controller,
                       Ik220PhysicalZero,
                       0,
                       0,
                       0,
                       35,
                       STATUS_SUCCESS,
                       IK220_BUS_NOT_PRESENT,
                       0,
                       NULL,
                       0,
                       NULL
                       );
         Ik220Dump(
                   SERERRORS,
                   ("IK220: There aren't that many of those\n"
                    "------- busses on this system,%x\n",
                    PConfig->BusNumber)
                   );

         status = IK220_BUS_NOT_PRESENT;
         goto PortInfoCleanUp;

      }

   }   // if (PConfig->BusNumber != 0)


   if ((PConfig->InterfaceType == MicroChannel) &&
       (PConfig->InterruptMode == CM_RESOURCE_INTERRUPT_LATCHED)) {

      Ik220LogError(
                    pDevExt->DriverObject,
                    NULL,
                    PConfig->Controller,
                    Ik220PhysicalZero,
                    0,
                    0,
                    0,
                    36,
                    STATUS_SUCCESS,
                    IK220_BUS_INTERRUPT_CONFLICT,
                    0,
                    NULL,
                    0,
                    NULL
                    );
      Ik220Dump(
                SERERRORS,
                ("IK220: Latched interrupts and MicroChannel\n"
                 "------- busses don't mix\n")
                );

      status = IK220_BUS_INTERRUPT_CONFLICT;
      goto PortInfoCleanUp;
   }


   status = STATUS_SUCCESS;

   //
   // Dump out the port configuration.
   //

   Ik220Dump(SERDIAG1, ("IK220: Controller address: %x\n",
                          PConfig->Controller.LowPart));

   Ik220Dump(SERDIAG1, ("IK220: Conf base: %x\n",
                         PConfig->ConfBase.LowPart));

   Ik220Dump(SERDIAG1, ("IK220: Port base[0]: %x\n",
                         PConfig->PortBase[0].LowPart));

   Ik220Dump(SERDIAG1, ("IK220: Port base[1]: %x\n",
                         PConfig->PortBase[1].LowPart));

   Ik220Dump(SERDIAG1, ("IK220: Port Index: %x\n",
                         PConfig->PortIndex));

   Ik220Dump(SERDIAG1, ("IK220: Port BusNumber: %x\n",
                         PConfig->BusNumber));

   Ik220Dump(SERDIAG1, ("IK220: AddressSpace: %x\n",
                         PConfig->AddressSpace));

   Ik220Dump(SERDIAG1, ("IK220: ConfBaseAddressSpace: %x\n",
                         PConfig->ConfBaseAddressSpace));

   Ik220Dump(SERDIAG1, ("IK220: PortBaseAddressSpace[0]: %x\n",
                         PConfig->PortBaseAddressSpace[0]));

   Ik220Dump(SERDIAG1, ("IK220: PortBaseAddressSpace[1]: %x\n",
                         PConfig->PortBaseAddressSpace[1]));

   Ik220Dump(SERDIAG1, ("IK220: InterruptMode: %x\n",
                         PConfig->InterruptMode));

   Ik220Dump(SERDIAG1, ("IK220: InterfaceType: %x\n",
                         PConfig->InterfaceType));

   Ik220Dump(SERDIAG1, ("IK220: OriginalVector: %x\n",
                         PConfig->OriginalVector));

   Ik220Dump(SERDIAG1, ("IK220: OriginalIrql: %x\n",
                         PConfig->OriginalIrql));

   PortInfoCleanUp:;

   return status;
}


NTSTATUS
Ik220DoExternalNaming(IN PIK220_DEVICE_EXTENSION PDevExt,
                      IN PDRIVER_OBJECT PDrvObj)
/*++

Routine Description:

    This routine will be used to create a symbolic link
    to the driver name in the given object directory.

    It will also create an entry in the device map for
    this device - IF we could create the symbolic link.

Arguments:

    Extension - Pointer to the device extension.

Return Value:

    None.

--*/
{
   NTSTATUS status = STATUS_SUCCESS;
   HANDLE keyHandle;
   WCHAR *pRegName = NULL;
   UNICODE_STRING linkName;
   PDEVICE_OBJECT pLowerDevObj, pDevObj;
   ULONG bufLen;


   PAGED_CODE();

   pDevObj = PDevExt->DeviceObject;
   pLowerDevObj = PDevExt->LowerDeviceObject;

   status = IoOpenDeviceRegistryKey(PDevExt->Pdo, PLUGPLAY_REGKEY_DEVICE,
                                    STANDARD_RIGHTS_READ, &keyHandle);

   //
   // Check to see if we are allowed to do external naming; if not,
   // then we just return success
   //


   if (status != STATUS_SUCCESS) {
      return status;
   }


   Ik220GetRegistryKeyValue(keyHandle, L"Ik220SkipExternalNaming",
                             sizeof(L"Ik220SkipExternalNaming"),
                             &PDevExt->SkipNaming, sizeof(ULONG));

   if (PDevExt->SkipNaming) {
      ZwClose(keyHandle);
      return STATUS_SUCCESS;
   }

   RtlZeroMemory(&linkName, sizeof(UNICODE_STRING));

   linkName.MaximumLength = SYMBOLIC_NAME_LENGTH*sizeof(WCHAR);
   linkName.Buffer = ExAllocatePool(PagedPool, linkName.MaximumLength
                                    + sizeof(WCHAR));

   if (linkName.Buffer == NULL) {
      Ik220LogError(PDrvObj, pDevObj, Ik220PhysicalZero, Ik220PhysicalZero,
                     0, 0, 0, 19, STATUS_SUCCESS, IK220_INSUFFICIENT_RESOURCES,
                     0, NULL, 0, NULL);
      Ik220Dump(SERERRORS, ("IK220: Couldn't allocate memory for device name"
                             "\n"));

      status = STATUS_INSUFFICIENT_RESOURCES;
      ZwClose(keyHandle);
      goto Ik220DoExternalNamingError;

   }

   RtlZeroMemory(linkName.Buffer, linkName.MaximumLength + sizeof(WCHAR));


   pRegName = ExAllocatePool(PagedPool, SYMBOLIC_NAME_LENGTH * sizeof(WCHAR) + sizeof(WCHAR));

   if (pRegName == NULL) {
      Ik220LogError(PDrvObj, pDevObj, Ik220PhysicalZero, Ik220PhysicalZero,
                     0, 0, 0, 19, STATUS_SUCCESS, IK220_INSUFFICIENT_RESOURCES,
                     0, NULL, 0, NULL);
      Ik220Dump(SERERRORS, ("IK220: Couldn't allocate memory for buffer"
                             "\n"));

      status = STATUS_INSUFFICIENT_RESOURCES;
      ZwClose(keyHandle);
      goto Ik220DoExternalNamingError;

   }

   //
   // Fetch PortName which contains the suggested REG_SZ symbolic name.
   //

   status = Ik220GetRegistryKeyValue(keyHandle, L"PortName",
                                      sizeof(L"PortName"), pRegName,
                                      SYMBOLIC_NAME_LENGTH * sizeof(WCHAR));

   if (!NT_SUCCESS(status)) {

      //
      // This is for PCMCIA which currently puts the name under Identifier.
      //

      status = Ik220GetRegistryKeyValue(keyHandle, L"Identifier",
                                         sizeof(L"Identifier"),
                                         pRegName, SYMBOLIC_NAME_LENGTH
                                         * sizeof(WCHAR));

      if (!NT_SUCCESS(status)) {

         //
         // Hmm.  Either we have to pick a name or bail...
         //
         // ...we pick a name.
         //

         Ik220Dump(SERERRORS, ("IK220: Getting PortName/Identifier failed - "
                                "%x\n", status));
//         ZwClose (keyHandle);
//         goto Ik220DoExternalNamingError;

         wcscpy(pRegName, L"Ik220Drv");
      }

   }

   ZwClose (keyHandle);

   bufLen = wcslen(pRegName) * sizeof(WCHAR) + sizeof(UNICODE_NULL);

   PDevExt->WmiIdentifier.Buffer = ExAllocatePool(PagedPool, bufLen);

   if (PDevExt->WmiIdentifier.Buffer == NULL) {
      Ik220LogError(PDrvObj, pDevObj, Ik220PhysicalZero, Ik220PhysicalZero,
                    0, 0, 0, 19, STATUS_SUCCESS, IK220_INSUFFICIENT_RESOURCES,
                    0, NULL, 0, NULL);
      Ik220Dump(SERERRORS,
                 ("IK220: Couldn't allocate memory for WMI name\n"));

      status = STATUS_INSUFFICIENT_RESOURCES;
      goto Ik220DoExternalNamingError;
   }

   RtlZeroMemory(PDevExt->WmiIdentifier.Buffer, bufLen);

   PDevExt->WmiIdentifier.Length = 0;
   PDevExt->WmiIdentifier.MaximumLength = (USHORT)bufLen - 1;
   RtlAppendUnicodeToString(&PDevExt->WmiIdentifier, pRegName);


   //
   // Create the "\\DosDevices\\<symbolicName>" string
   //
   RtlAppendUnicodeToString(&linkName, L"\\");
   RtlAppendUnicodeToString(&linkName, DEFAULT_DIRECTORY);
   RtlAppendUnicodeToString(&linkName, L"\\");
   RtlAppendUnicodeToString(&linkName, pRegName);

   //
   // Allocate Pool and save the symbolic link name in the device extension.
   //
   PDevExt->SymbolicLinkName.MaximumLength = linkName.Length + sizeof(WCHAR);
   PDevExt->SymbolicLinkName.Buffer
      = ExAllocatePool(PagedPool, PDevExt->SymbolicLinkName.MaximumLength);

   if (!PDevExt->SymbolicLinkName.Buffer) {

      Ik220LogError(PDrvObj, pDevObj, Ik220PhysicalZero, Ik220PhysicalZero,
                    0, 0, 0, 19, STATUS_SUCCESS, IK220_INSUFFICIENT_RESOURCES,
                    0, NULL, 0, NULL);
      Ik220Dump(SERERRORS,
                 ("IK220: Couldn't allocate memory for symbolic link name\n"));

      status = STATUS_INSUFFICIENT_RESOURCES;
      goto Ik220DoExternalNamingError;
   }



   //
   // Zero fill it.
   //

   RtlZeroMemory(PDevExt->SymbolicLinkName.Buffer,
                 PDevExt->SymbolicLinkName.MaximumLength);

   RtlAppendUnicodeStringToString(&PDevExt->SymbolicLinkName,
                                  &linkName);

   PDevExt->DosName.Buffer = ExAllocatePool(PagedPool, 64 + sizeof(WCHAR));

   if (!PDevExt->DosName.Buffer) {

      Ik220LogError(PDrvObj, pDevObj, Ik220PhysicalZero, Ik220PhysicalZero,
                    0, 0, 0, 19, STATUS_SUCCESS, IK220_INSUFFICIENT_RESOURCES,
                    0, NULL, 0, NULL);
      Ik220Dump(SERERRORS,
                 ("IK220: Couldn't allocate memory for Dos name\n"));

      status =  STATUS_INSUFFICIENT_RESOURCES;
      goto Ik220DoExternalNamingError;
   }


   PDevExt->DosName.MaximumLength = 64 + sizeof(WCHAR);

   //
   // Zero fill it.
   //

   PDevExt->DosName.Length = 0;

   RtlZeroMemory(PDevExt->DosName.Buffer,
                 PDevExt->DosName.MaximumLength);

   RtlAppendUnicodeToString(&PDevExt->DosName, pRegName);
   RtlZeroMemory(((PUCHAR)(&PDevExt->DosName.Buffer[0]))
                 + PDevExt->DosName.Length, sizeof(WCHAR));

   Ik220Dump(SERDIAG1, ("IK220: DosName is %wZ\n",
                         &PDevExt->DosName));

   //

   if (!Ik220Globals.CreatedSymbolicLink)
   {
      status = IoCreateSymbolicLink (&PDevExt->SymbolicLinkName,
                                     &PDevExt->DeviceName);

      if (!NT_SUCCESS(status)) {

         //
         // Oh well, couldn't create the symbolic link.  No point
         // in trying to create the device map entry.
         //

         Ik220LogError(PDrvObj, pDevObj, Ik220PhysicalZero, Ik220PhysicalZero,
                        0, 0, 0, 52, status, IK220_NO_SYMLINK_CREATED,
                        PDevExt->DeviceName.Length + sizeof(WCHAR),
                        PDevExt->DeviceName.Buffer, 0, NULL);

         Ik220Dump(
                   SERERRORS,
                   ("IK220: Couldn't create the symbolic link\n"
                    "------- for port %wZ\n",
                    &PDevExt->DeviceName)
                   );

         goto Ik220DoExternalNamingError;

      }
 
      Ik220Globals.CreatedSymbolicLink = TRUE;
      PDevExt->CreatedSymbolicLink = TRUE;


      status = RtlWriteRegistryValue(RTL_REGISTRY_DEVICEMAP, L"IK220COMM",
                                      PDevExt->DeviceName.Buffer, REG_SZ,
                                      PDevExt->DosName.Buffer,
                                      PDevExt->DosName.Length + sizeof(WCHAR));

      if (!NT_SUCCESS(status)) {

         Ik220LogError(PDrvObj, pDevObj, Ik220PhysicalZero, Ik220PhysicalZero,
                        0, 0, 0, 53, status, IK220_NO_DEVICE_MAP_CREATED,
                        PDevExt->DeviceName.Length + sizeof(WCHAR),
                        PDevExt->DeviceName.Buffer, 0, NULL);

         Ik220Dump(SERERRORS, ("IK220: Couldn't create the device map entry\n"
                                "------- for port %wZ\n", &PDevExt->DeviceName));

         goto Ik220DoExternalNamingError;
      }

      PDevExt->CreatedIk220CommEntry = TRUE;
   }

   //
   // Make the device visible via a device association as well.
   // The reference string is the eight digit device index
   //

   status = IoRegisterDeviceInterface(PDevExt->Pdo, (LPGUID)&GUID_CLASS_IK,
                                      NULL, &PDevExt->DeviceClassSymbolicName);

   if (!NT_SUCCESS(status)) {
      Ik220Dump(SERERRORS, ("IK220: Couldn't register class association\n"
                             "------- for port %wZ\n",
               &PDevExt->DeviceName));

      PDevExt->DeviceClassSymbolicName.Buffer = NULL;
      goto Ik220DoExternalNamingError;
   }


   //
   // Now set the symbolic link for the association
   //

   status = IoSetDeviceInterfaceState(&PDevExt->DeviceClassSymbolicName,
                                         TRUE);

   if (!NT_SUCCESS(status)) {
      Ik220Dump(SERERRORS, ("IK220: Couldn't set class association\n"
                             "------- for port %wZ\n",
               &PDevExt->DeviceName));
   }

   Ik220DoExternalNamingError:;

   //
   // Clean up error conditions
   //

   if (!NT_SUCCESS(status)) {
      if (PDevExt->DosName.Buffer != NULL) {
         ExFreePool(PDevExt->DosName.Buffer);
         PDevExt->DosName.Buffer = NULL;
      }

      if (PDevExt->CreatedSymbolicLink ==  TRUE) {
         IoDeleteSymbolicLink(&PDevExt->SymbolicLinkName);
         PDevExt->CreatedSymbolicLink = FALSE;
         Ik220Globals.CreatedSymbolicLink = FALSE;
      }

      if (PDevExt->SymbolicLinkName.Buffer != NULL) {
         ExFreePool(PDevExt->SymbolicLinkName.Buffer);
         PDevExt->SymbolicLinkName.Buffer = NULL;
      }

      if (PDevExt->DeviceName.Buffer != NULL) {
         RtlDeleteRegistryValue(RTL_REGISTRY_DEVICEMAP, IK220_DEVICE_MAP,
                                PDevExt->DeviceName.Buffer);
      }

      if (PDevExt->DeviceClassSymbolicName.Buffer) {
         IoSetDeviceInterfaceState (&PDevExt->DeviceClassSymbolicName, FALSE);
      }

      if (PDevExt->WmiIdentifier.Buffer != NULL) {
         ExFreePool(PDevExt->WmiIdentifier.Buffer);
         PDevExt->WmiIdentifier.Buffer = NULL;
      }
   }

   //
   // Always clean up our temp buffers.
   //

   if (linkName.Buffer != NULL) {
      ExFreePool(linkName.Buffer);
   }

   if (pRegName != NULL) {
      ExFreePool(pRegName);
   }

   return status;
}


VOID
Ik220UndoExternalNaming(IN PIK220_DEVICE_EXTENSION Extension)
/*++

Routine Description:

    This routine will be used to delete a symbolic link
    to the driver name in the given object directory.

    It will also delete an entry in the device map for
    this device if the symbolic link had been created.

Arguments:

    Extension - Pointer to the device extension.

Return Value:

    None.

--*/
{

   NTSTATUS status;

   PAGED_CODE();

   Ik220Dump(
             SERDIAG3,
             ("IK220: In Ik220UndoExternalNaming for\n"
              "------- extension: %x of port %wZ\n",
              Extension,&Extension->DeviceName)
             );

   //
   // Maybe there is nothing for us to do
   //

   if (Extension->SkipNaming) {
      return;
   }

   //
   // We're cleaning up here.  One reason we're cleaning up
   // is that we couldn't allocate space for the directory
   // name or the symbolic link.
   //

   if (Extension->SymbolicLinkName.Buffer &&
       Extension->CreatedSymbolicLink) {

            if (Extension->DeviceClassSymbolicName.Buffer) {
               status = IoSetDeviceInterfaceState (&Extension->
                                                   DeviceClassSymbolicName,
                                                   FALSE);

               //
               // IoRegisterDeviceClassInterface() allocated this string for us,
               // and we no longer need it.
               //

               ExFreePool( Extension->DeviceClassSymbolicName.Buffer );
            }


      IoDeleteSymbolicLink (&Extension->SymbolicLinkName);

   }

   if (Extension->WmiIdentifier.Buffer) {
      ExFreePool(Extension->WmiIdentifier.Buffer);
      Extension->WmiIdentifier.MaximumLength
         = Extension->WmiIdentifier.Length = 0;
      Extension->WmiIdentifier.Buffer = NULL;
   }

   //
   // We're cleaning up here.  One reason we're cleaning up
   // is that we couldn't allocate space for the NtNameOfPort.
   //

   if ((Extension->DeviceName.Buffer != NULL)
        && Extension->CreatedIk220CommEntry) {

      status = RtlDeleteRegistryValue(RTL_REGISTRY_DEVICEMAP, IK220_DEVICE_MAP,
                                     Extension->DeviceName.Buffer);

      if (!NT_SUCCESS(status)) {

         Ik220LogError(
                       Extension->DeviceObject->DriverObject,
                       Extension->DeviceObject,
                       Extension->OriginalController,
                       Ik220PhysicalZero,
                       0,
                       0,
                       0,
                       55,
                       status,
                       IK220_NO_DEVICE_MAP_DELETED,
                       Extension->DeviceName.Length+sizeof(WCHAR),
                       Extension->DeviceName.Buffer,
                       0,
                       NULL
                       );
         Ik220Dump(
                   SERERRORS,
                   ("IK220: Couldn't delete value entry %wZ\n",
                    &Extension->DeviceName)
                   );

      }
   }
}


