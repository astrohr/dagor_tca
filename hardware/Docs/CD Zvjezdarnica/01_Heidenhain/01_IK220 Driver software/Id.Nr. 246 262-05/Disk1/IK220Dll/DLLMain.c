/*++

	Copyright (c) 1997,  Dr. Johannes Heidenhain GmbH

	Module Name:	DllMain.c
	

	Abstract:		Windows DLL for IK 220

	Notes:			This DLL provides IK 220 card functions for IK 220.
					IK220 hardware is accsessed through IK220Drv.Sys/IK220Vxd.Sys.


	History:		- Initialize "Exist[2*MAX_CARDS]"							ES	1999-06-17
					- Initialize  PerfFreq & TIM_1000 for time out routines		ES	1999-06-17
					- Add function IK220Find256									ES	2003-05-05


--*/


#include <windows.h>
#include "dllglob.h"
#include "exports.h"
#include "TimeOut.h"
#include "..\..\Disk2\include\IK220.h"

#ifndef _Windows95
#include <winioctl.h>
#include <winsvc.h>
#endif

//
//  FUNCTION: DLLMain(HINSTANCE, DWORD, LPVOID)
//
//  PURPOSE:  Called when DLL is loaded by a process, and when new
//			  threads are created by a process that has already loaded the
//			  DLL.  Also called when threads of a process that has loaded the
//			  DLL exit cleanly and when the process itself unloads the DLL.
//
//  PARAMETERS:
//    hDLLInst    - Instance handle of the DLL
//    fdwReason   - Process attach/detach or thread attach/detach
//    lpvReserved - Reserved and not used
//
//  RETURN VALUE:  (Used only when fdwReason == DLL_PROCESS_ATTACH)
//    TRUE  -  Used to signify that the DLL should remain loaded.
//    FALSE -  Used to signify that the DLL should be immediately unloaded.
//
//  COMMENTS:
//
//    When fdwReason == DLL_PROCESS_ATTACH, the return value is used to
//    determine if the DLL should remain loaded, or should be immediately
//    unloaded depending upon whether the DLL could be initialized properly.
//    For all other values of fdwReason, the return value is ignored.
//


#undef StartService			// Service not started


CRITICAL_SECTION	CritSect[(2*MAX_CARDS)+1];


BOOL WINAPI DllMain(HINSTANCE hDLLInst, DWORD fdwReason, LPVOID lpvReserved)
{


int		i;
extern ULONG	Exists[2*MAX_CARDS];
extern __int64	PerfFreq;
extern __int64	TIM_1us, TIM_1ms, TIM_2s;
extern ULONG	DLLInfo;							// Info   of DLL functions


#ifdef _Windows95
	extern BOOL InitPci ();
	extern BOOL UnMapIK220 ();
#else

  #ifdef _StartService
	SC_HANDLE		hSCManager, hService;
	unsigned char	szService[] = "IK220Drv";
	BOOL			success;	
	SERVICE_STATUS	serviceStatus;
  #endif
#endif


    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:


		#ifdef _Windows95

			if (!InitPci()) return FALSE;
			if (!IKFind (Exists)) DLLInfo=DLLInfo | DLLInfo_NoIK220;
			
		#else		// _Windows95

		
		  #ifdef _StartService
			// Start device driver IK220Drv.SYS
			///////////////////////////////////

			hSCManager = OpenSCManager ( NULL,						// Local Computer
										 NULL,						// ServiceActive Database
										 SC_MANAGER_CONNECT );

			if (hSCManager == NULL) return FALSE;

			hService = OpenService ( hSCManager,
									 (LPCTSTR) szService,
									 SERVICE_START | SERVICE_STOP );

			if (hService == NULL)
			{
				CloseServiceHandle (hSCManager);
				return FALSE;
			}


			success = StartService ( hService,
									 0,
									 NULL );

			if ( (!success) && (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING) )
			{
				success = ControlService ( hService,
										   SERVICE_CONTROL_STOP,
										   &serviceStatus );

				if (success) success = StartService ( hService,
													  0,
													  NULL );
				else
				{
					CloseServiceHandle (hService);
					CloseServiceHandle (hSCManager);
					return FALSE;
				}
			}

			CloseServiceHandle (hService);
			CloseServiceHandle (hSCManager);

		  #endif		// _StartService


			// Open device driver IK220Drv.SYS
			//////////////////////////////////

			m_hIK220Drv = CreateFile ( "\\\\.\\IK220Drv",
									   GENERIC_READ | GENERIC_WRITE,
									   0,
									   NULL,
									   OPEN_EXISTING,
									   FILE_ATTRIBUTE_NORMAL,
									   NULL );

			if ( m_hIK220Drv == INVALID_HANDLE_VALUE ) DLLInfo=DLLInfo | DLLInfo_NoIK220;
		#endif		// _Windows95


			for (i=0; i<(2*MAX_CARDS)+1; i++)
				InitializeCriticalSection (&CritSect[i]);

//			if (!IKFind (Exists)) return FALSE;
			if (!IKFind (Exists)) DLLInfo=DLLInfo | DLLInfo_NoIK220;


			if (!QueryPerformanceFrequency((LARGE_INTEGER*)&PerfFreq))
			{
				PerfFreq = 0;
				
				TIM_1us  = 0;		// No time out active
				TIM_1ms  = 0;
				TIM_2s   = 0;


			}
			else
			{
				TIM_1us  = (__int64)((double)PerfFreq / 1000000.0);					// Timer value for 1µs
				TIM_1ms  = (__int64)((double)PerfFreq /    1000.0);					// Timer value for 1ms
				TIM_2s   = (__int64)((double)PerfFreq *       2.0);					// Timer value for 2s


				if ( TIM_1us==0 )  TIM_1us = 1;
				if ( TIM_1ms==0 )  TIM_1ms = 1;
				if ( TIM_2s ==0 )  TIM_2s  = 1;
			}

            break;


        case DLL_PROCESS_DETACH:

	#ifdef _Windows95

		UnMapIK220 ();

	#else		// ifdef _Windows95


			CloseHandle (m_hIK220Drv);								// Stop device driver IK220Drv.SYS

		  #ifdef _StartService
 			hSCManager = OpenSCManager ( NULL,						// Local Computer
										 NULL,						// ServiceActive Database
										 SC_MANAGER_CONNECT );		// Connect Service Manager

			hService = OpenService ( hSCManager,					// Open service for stop
									 (LPCTSTR) szService,
									 SERVICE_STOP );

			ControlService ( hService,								// Stop service
							 SERVICE_CONTROL_STOP,
							 &serviceStatus );

			CloseServiceHandle (hService);
			CloseServiceHandle (hSCManager);
		  #endif
		  

	#endif		// ifdef _Windows95

			for (i=0; i<(2*MAX_CARDS+1); i++)
				DeleteCriticalSection (&CritSect[i]);

            break;


        case DLL_THREAD_ATTACH:
            // A thread is being created in a process that has already loaded
            // this DLL.  Perform any per-thread initialization here.  The
            // return value is ignored.
            break;

        case DLL_THREAD_DETACH:
            // A thread is exiting cleanly in a process that has already
            // loaded this DLL.  Perform any per-thread clean up here.  The
            // return value is ignored.
            break;
    }
    return TRUE;
}
