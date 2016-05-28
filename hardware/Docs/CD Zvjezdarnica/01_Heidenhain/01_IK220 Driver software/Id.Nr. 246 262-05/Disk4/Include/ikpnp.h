//
//  Header file for IK 220 pnp functions
//
//  Version 0.1:  14.10.2002
//
//  eg,  Heidenhain Numerik AG
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////




#ifndef _IKPNP_H_
#define _IKPNP_H_

//#include "ik220i.h"


//
// This macro disables all interrupts on the hardware.
//
// Arguments:
//
// BaseAddress - A pointer to the address from which the hardware
//               device registers are located.
//
//
#define DISABLE_ALL_INTERRUPTS(BaseAddress)


//
// This macro enables all interrupts on the hardware.
//
// Arguments:
//
// BaseAddress - A pointer to the address from which the hardware
//               device registers are located.
//
//
#define ENABLE_ALL_INTERRUPTS(BaseAddress)



#define Ik220CompleteRequest(PDevExt, PIrp, PriBoost) \
   { \
      IoCompleteRequest((PIrp), (PriBoost)); \
      Ik220IRPEpilogue((PDevExt)); \
   }


//typedef
//NTSTATUS
//(*PSERIAL_START_ROUTINE) (
//    IN PSERIAL_DEVICE_EXTENSION
//    );
//
//typedef
//VOID
//(*PSERIAL_GET_NEXT_ROUTINE) (
//    IN PIRP *CurrentOpIrp,
//    IN PLIST_ENTRY QueueToProcess,
//    OUT PIRP *NewIrp,
//    IN BOOLEAN CompleteCurrent,
//    PSERIAL_DEVICE_EXTENSION Extension
//    );
//
NTSTATUS
Ik220Read(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

//NTSTATUS
//Ik220StartRead(
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//VOID
//Ik220CompleteRead(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//VOID
//Ik220ReadTimeout(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
NTSTATUS
DriverEntry(
           IN PDRIVER_OBJECT DriverObject,
           IN PUNICODE_STRING RegistryPath
           );

//VOID
//Ik220IntervalReadTimeout(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//NTSTATUS
//Ik220Flush(
//    IN PDEVICE_OBJECT DeviceObject,
//    IN PIRP Irp
//    );
//
//NTSTATUS
//Ik220Write(
//    IN PDEVICE_OBJECT DeviceObject,
//    IN PIRP Irp
//    );
//
//NTSTATUS
//Ik220StartWrite(
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//VOID
//Ik220GetNextWrite(
//    IN PIRP *CurrentOpIrp,
//    IN PLIST_ENTRY QueueToProcess,
//    IN PIRP *NewIrp,
//    IN BOOLEAN CompleteCurrent,
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//VOID
//Ik220CompleteWrite(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//BOOLEAN
//Ik220ProcessEmptyTransmit(
//    IN PVOID Context
//    );
//
//VOID
//Ik220WriteTimeout(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//VOID
//Ik220CommError(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
NTSTATUS
Ik220Cleanup(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
Ik220InitOneController(IN PDEVICE_OBJECT PDevObj, IN PCONFIG_DATA PConfigData);

NTSTATUS
Ik220CreateOpen(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
Ik220Close(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

//BOOLEAN
//Ik220SetDTR(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220ClrDTR(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220SetRTS(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220ClrRTS(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220SetChars(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220SetBaud(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220SetLineControl(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220SetupNewHandFlow(
//    IN PSERIAL_DEVICE_EXTENSION Extension,
//    IN PSERIAL_HANDFLOW NewHandFlow
//    );
//
//BOOLEAN
//Ik220SetHandFlow(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220TurnOnBreak(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220TurnOffBreak(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220PretendXoff(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220PretendXon(
//    IN PVOID Context
//    );
//
//VOID
//Ik220HandleReducedIntBuffer(
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//VOID
//Ik220ProdXonXoff(
//    IN PSERIAL_DEVICE_EXTENSION Extension,
//    IN BOOLEAN SendXon
//    );
//
NTSTATUS
Ik220IoControl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

//NTSTATUS
//Ik220StartMask(
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//VOID
//Ik220CancelWait(
//    IN PDEVICE_OBJECT DeviceObject,
//    IN PIRP Irp
//    );
//
//VOID
//Ik220CompleteWait(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//VOID
//Ik220StartImmediate(
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//VOID
//Ik220CompleteImmediate(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//VOID
//Ik220TimeoutImmediate(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//VOID
//Ik220TimeoutXoff(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//VOID
//Ik220CompleteXoff(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//NTSTATUS
//Ik220StartPurge(
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//BOOLEAN
//Ik220PurgeInterruptBuff(
//    IN PVOID Context
//    );
//
NTSTATUS
Ik220QueryInformationFile(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
Ik220SetInformationFile(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

//VOID
//Ik220KillAllReadsOrWrites(
//    IN PDEVICE_OBJECT DeviceObject,
//    IN PLIST_ENTRY QueueToClean,
//    IN PIRP *CurrentOpIrp
//    );
//
//VOID
//Ik220GetNextIrp(
//    IN PIRP *CurrentOpIrp,
//    IN PLIST_ENTRY QueueToProcess,
//    OUT PIRP *NextIrp,
//    IN BOOLEAN CompleteCurrent,
//    IN PSERIAL_DEVICE_EXTENSION extension
//    );
//
//VOID
//Ik220GetNextIrpLocked(IN PIRP *CurrentOpIrp, IN PLIST_ENTRY QueueToProcess,
//                       OUT PIRP *NextIrp, IN BOOLEAN CompleteCurrent,
//                       IN PSERIAL_DEVICE_EXTENSION extension, IN KIRQL OldIrql);
//
//VOID
//Ik220TryToCompleteCurrent(
//    IN PSERIAL_DEVICE_EXTENSION Extension,
//    IN PKSYNCHRONIZE_ROUTINE SynchRoutine OPTIONAL,
//    IN KIRQL IrqlForRelease,
//    IN NTSTATUS StatusToUse,
//    IN PIRP *CurrentOpIrp,
//    IN PLIST_ENTRY QueueToProcess,
//    IN PKTIMER IntervalTimer,
//    IN PKTIMER TotalTimer,
//    IN PSERIAL_START_ROUTINE Starter,
//    IN PSERIAL_GET_NEXT_ROUTINE GetNextIrp,
//    IN LONG RefType
//    );
//
//NTSTATUS
//Ik220StartOrQueue(
//    IN PSERIAL_DEVICE_EXTENSION Extension,
//    IN PIRP Irp,
//    IN PLIST_ENTRY QueueToExamine,
//    IN PIRP *CurrentOpIrp,
//    IN PSERIAL_START_ROUTINE Starter
//    );
//
//VOID
//Ik220CancelQueued(
//    PDEVICE_OBJECT DeviceObject,
//    PIRP Irp
//    );
//
NTSTATUS
Ik220CompleteIfError(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
    );

//ULONG
//Ik220HandleModemUpdate(
//    IN PSERIAL_DEVICE_EXTENSION Extension,
//    IN BOOLEAN DoingTX
//    );
//
//BOOLEAN
//Ik220ISR(
//    IN PKINTERRUPT InterruptObject,
//    IN PVOID Context
//    );
//
//NTSTATUS
//Ik220GetDivisorFromBaud(
//    IN ULONG ClockRate,
//    IN LONG DesiredBaud,
//    OUT PSHORT AppropriateDivisor
//    );
//
VOID
Ik220Unload(
    IN PDRIVER_OBJECT DriverObject
    );

BOOLEAN
Ik220Reset(
    IN PVOID Context
    );

//BOOLEAN
//Ik220PerhapsLowerRTS(
//    IN PVOID Context
//    );
//
//VOID
//Ik220StartTimerLowerRTS(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//VOID
//Ik220InvokePerhapsLowerRTS(
//    IN PKDPC Dpc,
//    IN PVOID DeferredContext,
//    IN PVOID SystemContext1,
//    IN PVOID SystemContext2
//    );
//
//VOID
//Ik220CleanupDevice(
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//UCHAR
//Ik220ProcessLSR(
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//LARGE_INTEGER
//Ik220GetCharTime(
//    IN PSERIAL_DEVICE_EXTENSION Extension
//    );
//
//BOOLEAN
//Ik220SharerIsr(
//    IN PKINTERRUPT InterruptObject,
//    IN PVOID Context
//    );
//
BOOLEAN
Ik220MarkClose(
    IN PVOID Context
    );

//BOOLEAN
//Ik220IndexedMultiportIsr(
//    IN PKINTERRUPT InterruptObject,
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220BitMappedMultiportIsr(
//    IN PKINTERRUPT InterruptObject,
//    IN PVOID Context
//    );
//
//VOID
//Ik220PutChar(
//    IN PSERIAL_DEVICE_EXTENSION Extension,
//    IN UCHAR CharToPut
//    );
//
//BOOLEAN
//Ik220GetStats(
//    IN PVOID Context
//    );
//
//BOOLEAN
//Ik220ClearStats(
//    IN PVOID Context
//    );
//
//NTSTATUS
//Ik220CloseComplete(
//    IN PDEVICE_OBJECT DeviceObject,
//    IN PIRP Irp,
//    IN PVOID Context
//    );
//
//NTSTATUS
//Ik220PnpDispatch(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp);
//
NTSTATUS
Ik220PowerDispatch(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
Ik220SetPowerD0(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

NTSTATUS
Ik220SetPowerD3(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

//NTSTATUS
//Ik220OpenClose(
//   IN PDEVICE_OBJECT DeviceObject,
//   IN PIRP Irp
//   );
//
NTSTATUS
Ik220GetConfigDefaults(
    IN PIK220_FIRMWARE_DATA DriverDefaultsPtr,
    IN PUNICODE_STRING RegistryPath
    );

//VOID
//Ik220GetProperties(
//    IN PSERIAL_DEVICE_EXTENSION Extension,
//    IN PSERIAL_COMMPROP Properties
//    );
//
//NTSTATUS
//Ik220EnumerateLegacy(IN PDRIVER_OBJECT DriverObject,
//                      IN PUNICODE_STRING RegistryPath,
//                      IN PSERIAL_FIRMWARE_DATA DriverDefaultsPtr);
//
//NTSTATUS
//Ik220MigrateLegacyRegistry(IN PDEVICE_OBJECT PPdo,
//                            IN PSERIAL_USER_DATA PUserData,
//                            IN BOOLEAN IsMulti);
//
//NTSTATUS
//Ik220BuildResourceList(OUT PCM_RESOURCE_LIST PResourceList,
//                        OUT PULONG PPartialCount,
//                        IN PSERIAL_USER_DATA PUserData);
//
//NTSTATUS
//Ik220TranslateResourceList(IN PDRIVER_OBJECT DriverObject,
//                            IN PKEY_BASIC_INFORMATION UserSubKey,
//                            OUT PCM_RESOURCE_LIST PTrResourceList,
//                            IN PCM_RESOURCE_LIST PResourceList,
//                            IN ULONG PartialCount,
//                            IN PSERIAL_USER_DATA PUserData);
//
//NTSTATUS
//Ik220BuildRequirementsList(OUT PIO_RESOURCE_REQUIREMENTS_LIST PRequiredList,
//                            IN ULONG PartialCount,
//                            IN PSERIAL_USER_DATA PUserData);
//
//BOOLEAN
//Ik220IsUserDataValid(IN PDRIVER_OBJECT DriverObject,
//                      IN PKEY_BASIC_INFORMATION UserSubKey,
//                      IN PRTL_QUERY_REGISTRY_TABLE Parameters,
//                      IN ULONG DefaultInterfaceType,
//                      IN PSERIAL_USER_DATA PUserData);
//
//NTSTATUS
//Ik220ControllerCallBack(
//                  IN PVOID Context,
//                  IN PUNICODE_STRING PathName,
//                  IN INTERFACE_TYPE BusType,
//                  IN ULONG BusNumber,
//                  IN PKEY_VALUE_FULL_INFORMATION *BusInformation,
//                  IN CONFIGURATION_TYPE ControllerType,
//                  IN ULONG ControllerNumber,
//                  IN PKEY_VALUE_FULL_INFORMATION *ControllerInformation,
//                  IN CONFIGURATION_TYPE PeripheralType,
//                  IN ULONG PeripheralNumber,
//                  IN PKEY_VALUE_FULL_INFORMATION *PeripheralInformation
//                  );
//
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
    );

NTSTATUS
Ik220AddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PLowerDevObj);

NTSTATUS
Ik220CreateDevObj(IN PDRIVER_OBJECT DriverObject,
                   OUT PDEVICE_OBJECT *NewDeviceObject);

NTSTATUS
Ik220StartDevice(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp);

NTSTATUS
Ik220GetRegistryKeyValue (IN HANDLE Handle, IN PWCHAR KeyNameString,
                          IN ULONG KeyNameStringLength, IN PVOID Data,
                          IN ULONG DataLength);

NTSTATUS
Ik220PnpDispatch(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp);

NTSTATUS
Ik220GetPortInfo(IN PDEVICE_OBJECT PDevObj, IN PCM_RESOURCE_LIST PResList,
                 IN PCM_RESOURCE_LIST PTrResList, OUT PCONFIG_DATA PConfig,
                 IN PIK220_USER_DATA PUserData);

NTSTATUS
Ik220FinishStartDevice(IN PDEVICE_OBJECT PDevObj,
                        IN PCM_RESOURCE_LIST PResList,
                        IN PCM_RESOURCE_LIST PTrResList,
                        IN PIK220_USER_DATA PUserData);

NTSTATUS
Ik220PutRegistryKeyValue (
                          IN HANDLE Handle,
                          IN PWCHAR KeyNameString,
                          IN ULONG KeyNameStringLength,
                          IN ULONG Dtype,
                          IN PVOID Data,
                          IN ULONG DataLength
                          );

NTSTATUS
Ik220InitController(IN PDEVICE_OBJECT PDevObj, IN PCONFIG_DATA PConfigData);

//NTSTATUS
//Ik220InitMultiPort(IN PSERIAL_DEVICE_EXTENSION PDevExt,
//                    IN PCONFIG_DATA PConfigData, IN PDEVICE_OBJECT PDevObj);
//
NTSTATUS
Ik220FindInitController(IN PDEVICE_OBJECT PDevObj, IN PCONFIG_DATA PConfig);


//BOOLEAN
//Ik220CIsrSw(IN PKINTERRUPT InterruptObject, IN PVOID Context);
//
NTSTATUS
Ik220DoExternalNaming(IN PIK220_DEVICE_EXTENSION PDevExt,
                      IN PDRIVER_OBJECT PDrvObj);

PVOID
Ik220GetMappedAddress(
    IN INTERFACE_TYPE BusType,
    IN ULONG BusNumber,
    PHYSICAL_ADDRESS IoAddress,
    ULONG NumberOfBytes,
    ULONG AddressSpace,
    PBOOLEAN MappedAddress
    );

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
    );

//BOOLEAN
//Ik220DoesPortExist(
//    IN PSERIAL_DEVICE_EXTENSION Extension,
//    PUNICODE_STRING InsertString,
//    IN ULONG ForceFifo,
//    IN ULONG LogFifo
//    );
//
IK220_MEM_COMPARES
Ik220MemCompare(
    IN PHYSICAL_ADDRESS A,
    IN ULONG SpanOfA,
    IN PHYSICAL_ADDRESS B,
    IN ULONG SpanOfB
    );

VOID
Ik220UndoExternalNaming(
    IN PIK220_DEVICE_EXTENSION Extension
    );


NTSTATUS
Ik220IRPPrologue(IN PIRP PIrp, IN PIK220_DEVICE_EXTENSION PDevExt);

VOID
Ik220IRPEpilogue(IN PIK220_DEVICE_EXTENSION PDevExt);

NTSTATUS
Ik220IoCallDriver(PIK220_DEVICE_EXTENSION PDevExt, PDEVICE_OBJECT PDevObj,
                  PIRP PIrp);

NTSTATUS
Ik220PoCallDriver(PIK220_DEVICE_EXTENSION PDevExt, PDEVICE_OBJECT PDevObj,
                   PIRP PIrp);

NTSTATUS
Ik220RemoveDevObj(IN PDEVICE_OBJECT PDevObj);

VOID
Ik220ReleaseResources(IN PIK220_DEVICE_EXTENSION PDevExt);

VOID
Ik220KillPendingIrps(PDEVICE_OBJECT DeviceObject);

//VOID
//Ik220DisableUART(IN PVOID Context);
//
//VOID
//Ik220DrainUART(IN PSERIAL_DEVICE_EXTENSION PDevExt,
//                IN PLARGE_INTEGER PDrainTime);
//
NTSTATUS
Ik220SystemControlDispatch(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp);

//NTSTATUS
//Ik220SetWmiDataItem(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp,
//                     IN ULONG GuidIndex, IN ULONG InstanceIndex,
//                     IN ULONG DataItemId,
//                     IN ULONG BufferSize, IN PUCHAR PBuffer);
//
//NTSTATUS
//Ik220SetWmiDataBlock(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp,
//                      IN ULONG GuidIndex, IN ULONG InstanceIndex,
//                      IN ULONG BufferSize,
//                      IN PUCHAR PBuffer);
//
//NTSTATUS
//Ik220QueryWmiDataBlock(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp,
//                        IN ULONG GuidIndex,
//                        IN ULONG InstanceIndex,
//                        IN ULONG InstanceCount,
//                        IN OUT PULONG InstanceLengthArray,
//                        IN ULONG OutBufferSize,
//                        OUT PUCHAR PBuffer);
//
//NTSTATUS
//Ik220QueryWmiRegInfo(IN PDEVICE_OBJECT PDevObj, OUT PULONG PRegFlags,
//                      OUT PUNICODE_STRING PInstanceName,
//                      OUT PUNICODE_STRING *PRegistryPath,
//                      OUT PUNICODE_STRING MofResourceName,
//                      OUT PDEVICE_OBJECT *Pdo);
//
VOID
Ik220RestoreDeviceState(IN PIK220_DEVICE_EXTENSION PDevExt);

VOID
Ik220SaveDeviceState(IN PIK220_DEVICE_EXTENSION PDevExt);

NTSTATUS
Ik220SyncCompletion(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp,
                    IN PKEVENT Ik220SyncEvent);

NTSTATUS
Ik220GotoPowerState(IN PDEVICE_OBJECT PDevObj,
                    IN PIK220_DEVICE_EXTENSION PDevExt,
                    IN DEVICE_POWER_STATE DevPowerState);

NTSTATUS
Ik220FilterIrps(IN PIRP PIrp, IN PIK220_DEVICE_EXTENSION PDevExt);

//VOID
//Ik220KillAllStalled(IN PDEVICE_OBJECT PDevObj);
//
//VOID
//Ik220UnstallIrps(IN PSERIAL_DEVICE_EXTENSION PDevExt);
//
NTSTATUS
Ik220InternalIoControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

NTSTATUS
Ik220SendWaitWake(PIK220_DEVICE_EXTENSION PDevExt);

NTSTATUS
Ik220WakeCompletion(IN PDEVICE_OBJECT PDevObj, IN UCHAR MinorFunction,
                     IN POWER_STATE PowerState, IN PVOID Context,
                     IN PIO_STATUS_BLOCK IoStatus);

//UINT32
//Ik220ReportMaxBaudRate(ULONG Bauds);
//
//BOOLEAN
//Ik220SetMCRContents(IN PVOID Context);
//
//BOOLEAN
//Ik220GetMCRContents(IN PVOID Context);
//
//BOOLEAN
//Ik220SetFCRContents(IN PVOID Context);
//
//NTSTATUS
//Ik220TossWMIRequest(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp,
//                     IN ULONG GuidIndex);
//
//VOID
//Ik220DpcEpilogue(IN PSERIAL_DEVICE_EXTENSION PDevExt, IN PKDPC PDpc);
//
//BOOLEAN
//Ik220InsertQueueDpc(IN PRKDPC Dpc, IN PVOID Sarg1, IN PVOID Sarg2,
//                     IN PSERIAL_DEVICE_EXTENSION PDevExt);
//
//BOOLEAN
//Ik220SetTimer(IN PKTIMER Timer, IN LARGE_INTEGER DueTime,
//               IN PKDPC Dpc OPTIONAL, IN PSERIAL_DEVICE_EXTENSION PDevExt);
//
//BOOLEAN
//Ik220CancelTimer(IN PKTIMER Timer, IN PSERIAL_DEVICE_EXTENSION PDevExt);
//
//VOID
//Ik220UnlockPages(IN PKDPC PDpc, IN PVOID PDeferredContext,
//                  IN PVOID PSysContext1, IN PVOID PSysContext2);
//
//VOID
//Ik220MarkHardwareBroken(IN PSERIAL_DEVICE_EXTENSION PDevExt);
//
VOID
Ik220DisableInterfacesResources(IN PDEVICE_OBJECT PDevObj);

VOID
Ik220SetDeviceFlags(IN PIK220_DEVICE_EXTENSION PDevExt, OUT PULONG PFlags,
                    IN ULONG Value, IN BOOLEAN Set);


//typedef struct _SERIAL_UPDATE_CHAR {
//    PSERIAL_DEVICE_EXTENSION Extension;
//    ULONG CharsCopied;
//    BOOLEAN Completed;
//    } SERIAL_UPDATE_CHAR,*PSERIAL_UPDATE_CHAR;
//
////
//// The following simple structure is used to send a pointer
//// the device extension and an ioctl specific pointer
//// to data.
////
//typedef struct _SERIAL_IOCTL_SYNC {
//    PSERIAL_DEVICE_EXTENSION Extension;
//    PVOID Data;
//    } SERIAL_IOCTL_SYNC,*PSERIAL_IOCTL_SYNC;
//
#define Ik220SetFlags(PDevExt, Value) \
   Ik220SetDeviceFlags((PDevExt), &(PDevExt)->Flags, (Value), TRUE)
#define Ik220ClearFlags(PDevExt, Value) \
   Ik220SetDeviceFlags((PDevExt), &(PDevExt)->Flags, (Value), FALSE)
#define Ik220SetAccept(PDevExt, Value) \
   Ik220SetDeviceFlags((PDevExt), &(PDevExt)->DevicePNPAccept, (Value), TRUE)
#define Ik220ClearAccept(PDevExt, Value) \
   Ik220SetDeviceFlags((PDevExt), &(PDevExt)->DevicePNPAccept, (Value), FALSE)



//
////
//// The following three macros are used to initialize, set
//// and clear references in IRPs that are used by
//// this driver.  The reference is stored in the fourth
//// argument of the irp, which is never used by any operation
//// accepted by this driver.
////
//
//#define SERIAL_REF_ISR         (0x00000001)
//#define SERIAL_REF_CANCEL      (0x00000002)
//#define SERIAL_REF_TOTAL_TIMER (0x00000004)
//#define SERIAL_REF_INT_TIMER   (0x00000008)
//#define SERIAL_REF_XOFF_REF    (0x00000010)
//
//
//#define SERIAL_INIT_REFERENCE(Irp) { \
//    ASSERT(sizeof(ULONG_PTR) <= sizeof(PVOID)); \
//    IoGetCurrentIrpStackLocation((Irp))->Parameters.Others.Argument4 = NULL; \
//    }
//
//#define SERIAL_SET_REFERENCE(Irp,RefType) \
//   do { \
//       LONG _refType = (RefType); \
//       PULONG_PTR _arg4 = (PVOID)&IoGetCurrentIrpStackLocation((Irp))->Parameters.Others.Argument4; \
//       ASSERT(!(*_arg4 & _refType)); \
//       *_arg4 |= _refType; \
//   } while (0)
//
//#define SERIAL_CLEAR_REFERENCE(Irp,RefType) \
//   do { \
//       LONG _refType = (RefType); \
//       PULONG_PTR _arg4 = (PVOID)&IoGetCurrentIrpStackLocation((Irp))->Parameters.Others.Argument4; \
//       ASSERT(*_arg4 & _refType); \
//       *_arg4 &= ~_refType; \
//   } while (0)
//
//#define SERIAL_REFERENCE_COUNT(Irp) \
//    ((ULONG_PTR)((IoGetCurrentIrpStackLocation((Irp))->Parameters.Others.Argument4)))
//
//
//
////////////////////////////////////////////////////////////////////////////
//
//
////
////NTSTATUS
////Ik220SyncCompletion(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp,
////                    IN PKEVENT Ik220SyncEvent);
////
////NTSTATUS
////Ik220AddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PLowerDevObj);
////
////NTSTATUS
////Ik220CreateDevObj(IN PDRIVER_OBJECT DriverObject,
////                   OUT PDEVICE_OBJECT *NewDeviceObject);
////
////NTSTATUS
////Ik220StartDevice(IN PDEVICE_OBJECT PDevObj, IN PIRP PIrp);
////
////NTSTATUS
////Ik220IRPPrologue(IN PIRP PIrp, IN PIK220_DEVICE_EXTENSION PDevExt);
////
////VOID
////Ik220IRPEpilogue(IN PIK220_DEVICE_EXTENSION PDevExt);
////
////NTSTATUS
////Ik220IoCallDriver(PIK220_DEVICE_EXTENSION PDevExt, PDEVICE_OBJECT PDevObj,
////                  PIRP PIrp);

#endif
