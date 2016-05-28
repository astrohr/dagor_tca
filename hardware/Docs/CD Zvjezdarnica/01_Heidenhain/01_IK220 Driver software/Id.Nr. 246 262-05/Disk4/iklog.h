/*++ BUILD Version: 0001    // Increment this if a change has global effects

Copyright (c) 1992, 1993  Microsoft Corporation

Module Name:

    ntiologc.h

Abstract:

    Constant definitions for the I/O error code log values.

Author:

    Tony Ercolano (Tonye) 12-23-1992

Revision History:

--*/

#ifndef _SERLOG_
#define _SERLOG_

//
//  Status values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-------------------------+-------------------------------+
//  |Sev|C|       Facility          |               Code            |
//  +---+-+-------------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//

//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_RPC_STUBS               0x3
#define FACILITY_RPC_RUNTIME             0x2
#define FACILITY_IO_ERROR_CODE           0x4
#define FACILITY_IK220_ERROR_CODE        0x6


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: IK220_KERNEL_DEBUGGER_ACTIVE
//
// MessageText:
//
//  The kernel debugger is already using %2.
//
#define IK220_KERNEL_DEBUGGER_ACTIVE     ((NTSTATUS)0x40060001L)

//
// MessageId: IK220_USER_OVERRIDE
//
// MessageText:
//
//  User configuration data for parameter %2 overriding firmware configuration data.
//
#define IK220_USER_OVERRIDE              ((NTSTATUS)0x40060003L)

//
// MessageId: IK220_NO_SYMLINK_CREATED
//
// MessageText:
//
//  Unable to create the symbolic link for %2.
//
#define IK220_NO_SYMLINK_CREATED         ((NTSTATUS)0x80060004L)

//
// MessageId: IK220_NO_DEVICE_MAP_CREATED
//
// MessageText:
//
//  Unable to create the device map entry for %2.
//
#define IK220_NO_DEVICE_MAP_CREATED      ((NTSTATUS)0x80060005L)

//
// MessageId: IK220_NO_DEVICE_MAP_DELETED
//
// MessageText:
//
//  Unable to delete the device map entry for %2.
//
#define IK220_NO_DEVICE_MAP_DELETED      ((NTSTATUS)0x80060006L)

//
// MessageId: IK220_UNREPORTED_IRQL_CONFLICT
//
// MessageText:
//
//  Another driver on the system, which did not report its resources, has already claimed the interrupt used by %2.
//
#define IK220_UNREPORTED_IRQL_CONFLICT   ((NTSTATUS)0xC0060007L)

//
// MessageId: IK220_INSUFFICIENT_RESOURCES
//
// MessageText:
//
//  Not enough resources were available for the driver.
//
#define IK220_INSUFFICIENT_RESOURCES     ((NTSTATUS)0xC0060008L)

//
// MessageId: IK220_REGISTERS_NOT_MAPPED
//
// MessageText:
//
//  The hardware locations for %2 could not be translated to something the memory management system could understand.
//
#define IK220_REGISTERS_NOT_MAPPED       ((NTSTATUS)0xC006000AL)

//
// MessageId: IK220_RESOURCE_CONFLICT
//
// MessageText:
//
//  The hardware resources for %2 are already in use by another device.
//
#define IK220_RESOURCE_CONFLICT          ((NTSTATUS)0xC006000BL)

//
// MessageId: IK220_NO_BUFFER_ALLOCATED
//
// MessageText:
//
//  No memory could be allocated in which to place new data for %2.
//
#define IK220_NO_BUFFER_ALLOCATED        ((NTSTATUS)0xC006000CL)

//
// MessageId: IK220_NOT_ENOUGH_CONFIG_INFO
//
// MessageText:
//
//  Some firmware configuration information was incomplete.
//
#define IK220_NOT_ENOUGH_CONFIG_INFO     ((NTSTATUS)0xC0060011L)

//
// MessageId: IK220_NO_PARAMETERS_INFO
//
// MessageText:
//
//  No Parameters subkey was found for user defined data.  This is odd, and it also means no user configuration can be found.
//
#define IK220_NO_PARAMETERS_INFO         ((NTSTATUS)0xC0060012L)

//
// MessageId: IK220_UNABLE_TO_ACCESS_CONFIG
//
// MessageText:
//
//  Specific user configuration data is unretrievable.
//
#define IK220_UNABLE_TO_ACCESS_CONFIG    ((NTSTATUS)0xC0060013L)

//
// MessageId: IK220_UNKNOWN_BUS
//
// MessageText:
//
//  The bus type for %2 is not recognizable.
//
#define IK220_UNKNOWN_BUS                ((NTSTATUS)0xC0060016L)

//
// MessageId: IK220_BUS_NOT_PRESENT
//
// MessageText:
//
//  The bus type for %2 is not available on this computer.
//
#define IK220_BUS_NOT_PRESENT            ((NTSTATUS)0xC0060017L)

//
// MessageId: IK220_BUS_INTERRUPT_CONFLICT
//
// MessageText:
//
//  The bus specified for %2 does not support the specified method of interrupt.
//
#define IK220_BUS_INTERRUPT_CONFLICT     ((NTSTATUS)0xC0060018L)

//
// MessageId: IK220_INVALID_USER_CONFIG
//
// MessageText:
//
//  User configuration for parameter %2 must have %3.
//
#define IK220_INVALID_USER_CONFIG        ((NTSTATUS)0xC0060019L)

//
// MessageId: IK220_DEVICE_TOO_HIGH
//
// MessageText:
//
//  The user specified port for %2 is way too high in physical memory.
//
#define IK220_DEVICE_TOO_HIGH            ((NTSTATUS)0xC006001AL)

//
// MessageId: IK220_STATUS_TOO_HIGH
//
// MessageText:
//
//  The status port for %2 is way too high in physical memory.
//
#define IK220_STATUS_TOO_HIGH            ((NTSTATUS)0xC006001BL)

//
// MessageId: IK220_DISABLED_PORT
//
// MessageText:
//
//  Disabling %2 as requested by the configuration data.
//
#define IK220_DISABLED_PORT              ((NTSTATUS)0x40060022L)

//
// MessageId: IK220_GARBLED_PARAMETER
//
// MessageText:
//
//  Parameter %2 data is unretrievable from the registry.
//
#define IK220_GARBLED_PARAMETER          ((NTSTATUS)0xC0060023L)

//
// MessageId: IK220_REGISTRY_WRITE_FAILED
//
// MessageText:
//
//  Error writing to the registry.
//
#define IK220_REGISTRY_WRITE_FAILED      ((NTSTATUS)0xC0060029L)

//
// MessageId: IK220_NO_DEVICE_REPORT_RES
//
// MessageText:
//
//  Could not report device %2 to IO subsystem due to a resource conflict.
//
#define IK220_NO_DEVICE_REPORT_RES       ((NTSTATUS)0xC006002CL)

//
// MessageId: IK220_HARDWARE_FAILURE
//
// MessageText:
//
//  The driver detected a hardware failure on device %2 and will disable this device.
//
#define IK220_HARDWARE_FAILURE           ((NTSTATUS)0xC006002DL)

#endif /* _NTIOLOGC_ */
