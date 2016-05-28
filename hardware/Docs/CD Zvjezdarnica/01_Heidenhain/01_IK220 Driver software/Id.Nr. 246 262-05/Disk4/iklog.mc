;/*++ BUILD Version: 0001    // Increment this if a change has global effects
;
;Copyright (c) 1992, 1993  Microsoft Corporation
;
;Module Name:
;
;    ntiologc.h
;
;Abstract:
;
;    Constant definitions for the I/O error code log values.
;
;Author:
;
;    Tony Ercolano (Tonye) 12-23-1992
;
;Revision History:
;
;--*/
;
;#ifndef _SERLOG_
;#define _SERLOG_
;
;//
;//  Status values are 32 bit values layed out as follows:
;//
;//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
;//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
;//  +---+-+-------------------------+-------------------------------+
;//  |Sev|C|       Facility          |               Code            |
;//  +---+-+-------------------------+-------------------------------+
;//
;//  where
;//
;//      Sev - is the severity code
;//
;//          00 - Success
;//          01 - Informational
;//          10 - Warning
;//          11 - Error
;//
;//      C - is the Customer code flag
;//
;//      Facility - is the facility code
;//
;//      Code - is the facility's status code
;//
;
MessageIdTypedef=NTSTATUS

SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
               Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
               Warning=0x2:STATUS_SEVERITY_WARNING
               Error=0x3:STATUS_SEVERITY_ERROR
              )

FacilityNames=(System=0x0
               RpcRuntime=0x2:FACILITY_RPC_RUNTIME
               RpcStubs=0x3:FACILITY_RPC_STUBS
               Io=0x4:FACILITY_IO_ERROR_CODE
               Ik220=0x6:FACILITY_IK220_ERROR_CODE
              )


MessageId=0x0001 Facility=Ik220 Severity=Informational SymbolicName=IK220_KERNEL_DEBUGGER_ACTIVE
Language=English
The kernel debugger is already using %2.
.

MessageId=0x0003 Facility=Ik220 Severity=Informational SymbolicName=IK220_USER_OVERRIDE
Language=English
User configuration data for parameter %2 overriding firmware configuration data.
.

MessageId=0x0004 Facility=Ik220 Severity=Warning SymbolicName=IK220_NO_SYMLINK_CREATED
Language=English
Unable to create the symbolic link for %2.
.

MessageId=0x0005 Facility=Ik220 Severity=Warning SymbolicName=IK220_NO_DEVICE_MAP_CREATED
Language=English
Unable to create the device map entry for %2.
.

MessageId=0x0006 Facility=Ik220 Severity=Warning SymbolicName=IK220_NO_DEVICE_MAP_DELETED
Language=English
Unable to delete the device map entry for %2.
.

MessageId=0x0007 Facility=Ik220 Severity=Error SymbolicName=IK220_UNREPORTED_IRQL_CONFLICT
Language=English
Another driver on the system, which did not report its resources, has already claimed the interrupt used by %2.
.

MessageId=0x0008 Facility=Ik220 Severity=Error SymbolicName=IK220_INSUFFICIENT_RESOURCES
Language=English
Not enough resources were available for the driver.
.

MessageId=0x000A Facility=Ik220 Severity=Error SymbolicName=IK220_REGISTERS_NOT_MAPPED
Language=English
The hardware locations for %2 could not be translated to something the memory management system could understand.
.

MessageId=0x000B Facility=Ik220 Severity=Error SymbolicName=IK220_RESOURCE_CONFLICT
Language=English
The hardware resources for %2 are already in use by another device.
.

MessageId=0x000C Facility=Ik220 Severity=Error SymbolicName=IK220_NO_BUFFER_ALLOCATED
Language=English
No memory could be allocated in which to place new data for %2.
.

MessageId=0x0011 Facility=Ik220 Severity=Error SymbolicName=IK220_NOT_ENOUGH_CONFIG_INFO
Language=English
Some firmware configuration information was incomplete.
.

MessageId=0x0012 Facility=Ik220 Severity=Error SymbolicName=IK220_NO_PARAMETERS_INFO
Language=English
No Parameters subkey was found for user defined data.  This is odd, and it also means no user configuration can be found.
.

MessageId=0x0013 Facility=Ik220 Severity=Error SymbolicName=IK220_UNABLE_TO_ACCESS_CONFIG
Language=English
Specific user configuration data is unretrievable.
.

MessageId=0x0016 Facility=Ik220 Severity=Error SymbolicName=IK220_UNKNOWN_BUS
Language=English
The bus type for %2 is not recognizable.
.

MessageId=0x0017 Facility=Ik220 Severity=Error SymbolicName=IK220_BUS_NOT_PRESENT
Language=English
The bus type for %2 is not available on this computer.
.

MessageId=0x0018 Facility=Ik220 Severity=Error SymbolicName=IK220_BUS_INTERRUPT_CONFLICT
Language=English
The bus specified for %2 does not support the specified method of interrupt.
.

MessageId=0x0019 Facility=Ik220 Severity=Error SymbolicName=IK220_INVALID_USER_CONFIG
Language=English
User configuration for parameter %2 must have %3.
.

MessageId=0x001A Facility=Ik220 Severity=Error SymbolicName=IK220_DEVICE_TOO_HIGH
Language=English
The user specified port for %2 is way too high in physical memory.
.

MessageId=0x001B Facility=Ik220 Severity=Error SymbolicName=IK220_STATUS_TOO_HIGH
Language=English
The status port for %2 is way too high in physical memory.
.

MessageId=0x0022 Facility=Ik220 Severity=Informational SymbolicName=IK220_DISABLED_PORT
Language=English
Disabling %2 as requested by the configuration data.
.

MessageId=0x0023 Facility=Ik220 Severity=Error SymbolicName=IK220_GARBLED_PARAMETER
Language=English
Parameter %2 data is unretrievable from the registry.
.

MessageId=0x0029 Facility=Ik220 Severity=Error SymbolicName=IK220_REGISTRY_WRITE_FAILED
Language=English
Error writing to the registry.
.

MessageId=0x002C Facility=Ik220 Severity=Error SymbolicName=IK220_NO_DEVICE_REPORT_RES
Language=English
Could not report device %2 to IO subsystem due to a resource conflict.
.

MessageId=0x002D Facility=Ik220 Severity=Error SymbolicName=IK220_HARDWARE_FAILURE
Language=English
The driver detected a hardware failure on device %2 and will disable this device.
.

;#endif /* _NTIOLOGC_ */

