//
//	Header file for IK 220 internal data structures
//
//	Version 0.1:	14.10.2002
//
//	eg,  Heidenhain Numerik AG
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef   _LOG_H_
#define   _LOG_H_


#define LOG_MISC          0x00000001        //debug log entries
#define LOG_CNT           0x00000002

//
// Assert Macros
//

#if DBG

#define LOGENTRY(mask, sig, info1, info2, info3)     \
    Ik220DebugLogEntry(mask, sig, (ULONG_PTR)info1, \
                        (ULONG_PTR)info2,            \
                        (ULONG_PTR)info3)

VOID
Ik220DebugLogEntry(IN ULONG Mask, IN ULONG Sig, IN ULONG_PTR Info1,
                    IN ULONG_PTR Info2, IN ULONG_PTR Info3);

VOID
Ik220LogInit();

VOID
Ik220LogFree();

#else
#define LOGENTRY(mask, sig, info1, info2, info3)
#endif


#endif // _LOG_H_
