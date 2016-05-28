
#include <ntddk.h>
#include <IK220DRV.h>
#include <ik220.h>

#if DBG

KSPIN_LOCK LogSpinLock;

struct IK220_LOG_ENTRY {
    ULONG        le_sig;          // Identifying string
    ULONG_PTR    le_info1;        // entry specific info
    ULONG_PTR    le_info2;        // entry specific info
    ULONG_PTR    le_info3;        // entry specific info
}; // IK220_LOG_ENTRY


struct IK220_LOG_ENTRY *Ik220LStart = 0;    // No log yet
struct IK220_LOG_ENTRY *Ik220LPtr;
struct IK220_LOG_ENTRY *Ik220LEnd;

ULONG LogMask = 0x0;

VOID
Ik220DebugLogEntry(IN ULONG Mask, IN ULONG Sig, IN ULONG_PTR Info1,
                      IN ULONG_PTR Info2, IN ULONG_PTR Info3)
/*++

Routine Description:

    Adds an Entry to serial log.

Arguments:

Return Value:

    None.

--*/
{
    KIRQL irql;

typedef union _SIG {
    struct {
        UCHAR Byte0;
        UCHAR Byte1;
        UCHAR Byte2;
        UCHAR Byte3;
    } b;
    ULONG l;
} SIG, *PSIG;

    SIG sig, rsig;


    if (Ik220LStart == 0) {
        return;
    }

    if ((Mask & LogMask) == 0) {
        return;
    }

    irql = KeGetCurrentIrql();

    if (irql < DISPATCH_LEVEL) {
        KeAcquireSpinLock(&LogSpinLock, &irql);
    } else {
        KeAcquireSpinLockAtDpcLevel(&LogSpinLock);
    }

    if (Ik220LPtr > Ik220LStart) {
        Ik220LPtr -= 1;    // Decrement to next entry
    } else {
        Ik220LPtr = Ik220LEnd;
    }

    sig.l = Sig;
    rsig.b.Byte0 = sig.b.Byte3;
    rsig.b.Byte1 = sig.b.Byte2;
    rsig.b.Byte2 = sig.b.Byte1;
    rsig.b.Byte3 = sig.b.Byte0;

    Ik220LPtr->le_sig = rsig.l;
    Ik220LPtr->le_info1 = Info1;
    Ik220LPtr->le_info2 = Info2;
    Ik220LPtr->le_info3 = Info3;

    ASSERT(Ik220LPtr >= Ik220LStart);

    if (irql < DISPATCH_LEVEL) {
        KeReleaseSpinLock(&LogSpinLock, irql);
    } else {
        KeReleaseSpinLockFromDpcLevel(&LogSpinLock);
    }

    return;
}


VOID
Ik220LogInit()
/*++

Routine Description:

    Init the debug log - remember interesting information in a circular buffer

Arguments:

Return Value:

    None.

--*/
{
#ifdef MAX_DEBUG
    ULONG logSize = 4096*6;
#else
    ULONG logSize = 4096*3;
#endif


    KeInitializeSpinLock(&LogSpinLock);

    Ik220LStart = ExAllocatePoolWithTag(NonPagedPool, logSize, 'XMOC');

    if (Ik220LStart) {
        Ik220LPtr = Ik220LStart;

        // Point the end (and first entry) 1 entry from the end of the segment
        Ik220LEnd = Ik220LStart + (logSize / sizeof(struct IK220_LOG_ENTRY))
            - 1;
    } else {
#if DBG
//        DbgBreakPoint ();
#endif
    }

    return;
}

VOID
Ik220LogFree(
    )
/*++

Routine Description:

Arguments:

Return Value:

    None.

--*/
{
    if (Ik220LStart) {
        ExFreePool(Ik220LStart);
    }

    return;
}

#endif // DBG

