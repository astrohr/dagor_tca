//
//	Header file for IK 220 internal data structures
//
//	Version 0.1:	14.10.2002
//
//	eg,  Heidenhain Numerik AG
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef _IK220I_H_
#define _IK220I_H_

#include <wmilib.h>
#include <wmidata.h>



#if DBG
  #define kdPrint(arg)  DbgPrint arg
  #define kdBreak       DbgBreakPoint
#else
  #define kdPrint(arg)
  #define kdBreak
#endif


#define countof(x) (sizeof(x)/sizeof(x[0]))


#define SERDIAG1              ((ULONG)0x00000001)
#define SERDIAG2              ((ULONG)0x00000002)
#define SERDIAG3              ((ULONG)0x00000004)
#define SERDIAG4              ((ULONG)0x00000008)
#define SERDIAG5              ((ULONG)0x00000010)
#define SERIRPPATH            ((ULONG)0x00000020)
#define SERINITCODE           ((ULONG)0x00000040)
#define SERTRACECALLS         ((ULONG)0x00000040)
#define SERPNPPOWER           ((ULONG)0x00000100)
#define SERFLOW               ((ULONG)0x20000000)
#define SERERRORS             ((ULONG)0x40000000)
#define SERBUGCHECK           ((ULONG)0x80000000)
#define SERDBGALL             ((ULONG)0xFFFFFFFF)

#define SER_DBG_DEFAULT       SERDBGALL

#if DBG
extern ULONG Ik220DebugLevel;
#define Ik220Dump(LEVEL,STRING) \
        do { \
            ULONG _level = (LEVEL); \
            if (Ik220DebugLevel & _level) { \
                DbgPrint STRING; \
            } \
            if (_level == SERBUGCHECK) { \
                ASSERT(FALSE); \
            } \
        } while (0)
#else
#define Ik220Dump(LEVEL,STRING) do {NOTHING;} while (0)
#endif


//
// Some default driver values.  We will check the registry for
// them first.
//
#define IK220_UNINITIALIZED_DEFAULT    1234567


//
// This define gives the default Object directory
// that we should use to insert the symbolic links
// between the NT device name and namespace used by
// that object directory.
#define DEFAULT_DIRECTORY L"DosDevices"


// 2003-05-05 E.S.
// Number of cards increased from 8 to 128 --> 256 axes
///////////////////////////////////////////////////////
#define MAX_CARDS 128


typedef struct _IK220_DEVICE_STATE {
   //
   // TRUE if we need to set the state to open
   // on a powerup
   //

   BOOLEAN Reopen;

} IK220_DEVICE_STATE, *PIK220_DEVICE_STATE;




typedef struct _DEVICE_EXTENSION
{
//    PDEVICE_OBJECT  DeviceObject;
//    ULONG           DeviceType;
//    ULONG           BusNumber[MAX_CARDS];
    PUSHORT         PortBase[2];
//    PUSHORT         TrPortBase[2*MAX_CARDS];
    PULONG          ConfBase;
//    PULONG          TrConfBase[MAX_CARDS];
    ULONG           DevStat;
    ULONG           instance;



//    //
//    // This holds the isr that should be called from our own
//    // dispatching isr for "cards" that are trying to share the
//    // same interrupt.
//    //
//    PKSERVICE_ROUTINE TopLevelOurIsr;
//
//    //
//    // This holds the context that should be used when we
//    // call the above service routine.
//    //
//    PVOID TopLevelOurIsrContext;
//
//    //
//    // This links together all of the different "cards" that are
//    // trying to share the same interrupt of a non-mca machine.
//    //
//    LIST_ENTRY TopLevelSharers;
//
//    //
//    // This circular doubly linked list links together all
//    // devices that are using the same interrupt object.
//    // NOTE: This does not mean that they are using the
//    // same interrupt "dispatching" routine.
//    //
//    LIST_ENTRY CommonInterruptObject;
//
//    //
//    // This is to link together ports on the same multiport card
//    //
//
//    LIST_ENTRY MultiportSiblings;
//
    //
    // This links together all devobjs that this driver owns.
    // It is needed to search when starting a new device.
    //
    LIST_ENTRY AllDevObjs;

    //
    // For reporting resource usage, we keep around the physical
    // address we got from the registry.
    //
    PHYSICAL_ADDRESS OriginalController;
    PHYSICAL_ADDRESS OriginalPortBase[2];
    PHYSICAL_ADDRESS OriginalConfBase;

//    //
//    // For reporting resource usage, we keep around the physical
//    // address we got from the registry.
//    //
//    PHYSICAL_ADDRESS OriginalInterruptStatus;
//
//    //
//    // This value is set by the read code to hold the time value
//    // used for read interval timing.  We keep it in the extension
//    // so that the interval timer dpc routine determine if the
//    // interval time has passed for the IO.
//    //
//    LARGE_INTEGER IntervalTime;
//
//    //
//    // These two values hold the "constant" time that we should use
//    // to delay for the read interval time.
//    //
//    LARGE_INTEGER ShortIntervalAmount;
//    LARGE_INTEGER LongIntervalAmount;
//
//    //
//    // This holds the value that we use to determine if we should use
//    // the long interval delay or the short interval delay.
//    //
//    LARGE_INTEGER CutOverAmount;
//
//    //
//    // This holds the system time when we last time we had
//    // checked that we had actually read characters.  Used
//    // for interval timing.
//    //
//    LARGE_INTEGER LastReadTime;
//
    //
    // We keep a pointer around to our device name for dumps
    // and for creating "external" symbolic links to this
    // device.
    //
    UNICODE_STRING DeviceName;

//    //
//    // This points to the object directory that we will place
//    // a symbolic link to our device name.
//    //
//    UNICODE_STRING ObjectDirectory;
//
    //
    // This points to the device name for this device
    // sans device prefix.
    //
    UNICODE_STRING NtNameForPort;

    //
    // This points to the symbolic link name that will be
    // linked to the actual nt device name.
    //
    UNICODE_STRING SymbolicLinkName;

    //
    // This points to the pure "COMx" name
    //
    UNICODE_STRING DosName;

//    //
//    // This points the the delta time that we should use to
//    // delay for interval timing.
//    //
//    PLARGE_INTEGER IntervalTimeToUse;
//
    //
    // Points to the device object that contains
    // this device extension.
    //
    PDEVICE_OBJECT DeviceObject;

//    //
//    // After initialization of the driver is complete, this
//    // will either be NULL or point to the routine that the
//    // kernel will call when an interrupt occurs.
//    //
//    // If the pointer is null then this is part of a list
//    // of ports that are sharing an interrupt and this isn't
//    // the first port that we configured for this interrupt.
//    //
//    // If the pointer is non-null then this routine has some
//    // kind of structure that will "eventually" get us into
//    // the real ik220 isr with a pointer to this device extension.
//    //
//    // NOTE: On an MCA bus (except for multiport cards) this
//    // is always a pointer to the "real" ik220 isr.
//    PKSERVICE_ROUTINE OurIsr;
//
//    //
//    // This will generally point right to this device extension.
//    //
//    // However, when the port that this device extension is
//    // "managing" was the first port initialized on a chain
//    // of ports that were trying to share an interrupt, this
//    // will point to a structure that will enable dispatching
//    // to any port on the chain of sharers of this interrupt.
//    //
//    PVOID OurIsrContext;
//
    //
    // The base address for the set of device registers
    // of the ik220 port.
    //
    PUCHAR Controller;

//    //
//    // The base address for interrupt status register.
//    // This is only defined in the root extension.
//    //
//    PUCHAR InterruptStatus;
//
    //
    // Points to the interrupt object for used by this device.
    //
    PKINTERRUPT Interrupt;

//    //
//    // This list head is used to contain the time ordered list
//    // of read requests.  Access to this list is protected by
//    // the global cancel spinlock.
//    //
//    LIST_ENTRY ReadQueue;
//
//    //
//    // This list head is used to contain the time ordered list
//    // of write requests.  Access to this list is protected by
//    // the global cancel spinlock.
//    //
//    LIST_ENTRY WriteQueue;
//
//    //
//    // This list head is used to contain the time ordered list
//    // of set and wait mask requests.  Access to this list is protected by
//    // the global cancel spinlock.
//    //
//    LIST_ENTRY MaskQueue;
//
//    //
//    // Holds the serialized list of purge requests.
//    //
//    LIST_ENTRY PurgeQueue;
//
//    //
//    // This points to the irp that is currently being processed
//    // for the read queue.  This field is initialized by the open to
//    // NULL.
//    //
//    // This value is only set at dispatch level.  It may be
//    // read at interrupt level.
//    //
//    PIRP CurrentReadIrp;
//
//    //
//    // This points to the irp that is currently being processed
//    // for the write queue.
//    //
//    // This value is only set at dispatch level.  It may be
//    // read at interrupt level.
//    //
//    PIRP CurrentWriteIrp;
//
//    //
//    // Points to the irp that is currently being processed to
//    // affect the wait mask operations.
//    //
//    PIRP CurrentMaskIrp;
//
//    //
//    // Points to the irp that is currently being processed to
//    // purge the read/write queues and buffers.
//    //
//    PIRP CurrentPurgeIrp;
//
//    //
//    // Points to the current irp that is waiting on a comm event.
//    //
//    PIRP CurrentWaitIrp;
//
//    //
//    // Points to the irp that is being used to send an immediate
//    // character.
//    //
//    PIRP CurrentImmediateIrp;
//
//    //
//    // Points to the irp that is being used to count the number
//    // of characters received after an xoff (as currently defined
//    // by the IOCTL_SERIAL_XOFF_COUNTER ioctl) is sent.
//    //
//    PIRP CurrentXoffIrp;
//
//    //
//    // Holds the number of bytes remaining in the current write
//    // irp.
//    //
//    // This location is only accessed while at interrupt level.
//    //
//    ULONG WriteLength;
//
//    //
//    // Holds a pointer to the current character to be sent in
//    // the current write.
//    //
//    // This location is only accessed while at interrupt level.
//    //
//    PUCHAR WriteCurrentChar;
//
//    //
//    // This is a pointer to the first character of the buffer into
//    // which the interrupt service routine is copying characters.
//    //
//    PUCHAR ReadBufferBase;
//
//    //
//    // This is a count of the number of characters in the interrupt
//    // buffer.  This value is set and read at interrupt level.  Note
//    // that this value is only *incremented* at interrupt level so
//    // it is safe to read it at any level.  When characters are
//    // copied out of the read buffer, this count is decremented by
//    // a routine that synchronizes with the ISR.
//    //
//    ULONG CharsInInterruptBuffer;
//
//    //
//    // Points to the first available position for a newly received
//    // character.  This variable is only accessed at interrupt level and
//    // buffer initialization code.
//    //
//    PUCHAR CurrentCharSlot;
//
//    //
//    // This variable is used to contain the last available position
//    // in the read buffer.  It is updated at open and at interrupt
//    // level when switching between the users buffer and the interrupt
//    // buffer.
//    //
//    PUCHAR LastCharSlot;
//
//    //
//    // This marks the first character that is available to satisfy
//    // a read request.  Note that while this always points to valid
//    // memory, it may not point to a character that can be sent to
//    // the user.  This can occur when the buffer is empty.
//    //
//    PUCHAR FirstReadableChar;
//
//    //
//    // Pointer to the lock variable returned for this extension when
//    // locking down the driver
//    //
//    PVOID LockPtr;
//
//
    //
    // This variable holds the size of whatever buffer we are currently
    // using.
    //
    ULONG BufferSize;

//    //
//    // This variable holds .8 of BufferSize. We don't want to recalculate
//    // this real often - It's needed when so that an application can be
//    // "notified" that the buffer is getting full.
//    //
//    ULONG BufferSizePt8;
//
//    //
//    // This value holds the number of characters desired for a
//    // particular read.  It is initially set by read length in the
//    // IRP.  It is decremented each time more characters are placed
//    // into the "users" buffer buy the code that reads characters
//    // out of the typeahead buffer into the users buffer.  If the
//    // typeahead buffer is exhausted by the read, and the reads buffer
//    // is given to the isr to fill, this value is becomes meaningless.
//    //
//    ULONG NumberNeededForRead;
//
    //
    // This mask will hold the bitmask sent down via the set mask
    // ioctl.  It is used by the interrupt service routine to determine
    // if the occurence of "events" (in the ik220 drivers understanding
    // of the concept of an event) should be noted.
    //
    ULONG IsrWaitMask;

    //
    // This mask will always be a subset of the IsrWaitMask.  While
    // at device level, if an event occurs that is "marked" as interesting
    // in the IsrWaitMask, the driver will turn on that bit in this
    // history mask.  The driver will then look to see if there is a
    // request waiting for an event to occur.  If there is one, it
    // will copy the value of the history mask into the wait irp, zero
    // the history mask, and complete the wait irp.  If there is no
    // waiting request, the driver will be satisfied with just recording
    // that the event occured.  If a wait request should be queued,
    // the driver will look to see if the history mask is non-zero.  If
    // it is non-zero, the driver will copy the history mask into the
    // irp, zero the history mask, and then complete the irp.
    //
    ULONG HistoryMask;

    //
    // This is a pointer to the where the history mask should be
    // placed when completing a wait.  It is only accessed at
    // device level.
    //
    // We have a pointer here to assist us to synchronize completing a wait.
    // If this is non-zero, then we have wait outstanding, and the isr still
    // knows about it.  We make this pointer null so that the isr won't
    // attempt to complete the wait.
    //
    // We still keep a pointer around to the wait irp, since the actual
    // pointer to the wait irp will be used for the "common" irp completion
    // path.
    //
    ULONG *IrpMaskLocation;

//    //
//    // This mask holds all of the reason that transmission
//    // is not proceeding.  Normal transmission can not occur
//    // if this is non-zero.
//    //
//    // This is only written from interrupt level.
//    // This could be (but is not) read at any level.
//    //
//    ULONG TXHolding;
//
//    //
//    // This mask holds all of the reason that reception
//    // is not proceeding.  Normal reception can not occur
//    // if this is non-zero.
//    //
//    // This is only written from interrupt level.
//    // This could be (but is not) read at any level.
//    //
//    ULONG RXHolding;
//
//    //
//    // This holds the reasons that the driver thinks it is in
//    // an error state.
//    //
//    // This is only written from interrupt level.
//    // This could be (but is not) read at any level.
//    //
//    ULONG ErrorWord;
//
//    //
//    // This keeps a total of the number of characters that
//    // are in all of the "write" irps that the driver knows
//    // about.  It is only accessed with the cancel spinlock
//    // held.
//    //
//    ULONG TotalCharsQueued;
//
//    //
//    // This holds a count of the number of characters read
//    // the last time the interval timer dpc fired.  It
//    // is a long (rather than a ulong) since the other read
//    // completion routines use negative values to indicate
//    // to the interval timer that it should complete the read
//    // if the interval timer DPC was lurking in some DPC queue when
//    // some other way to complete occurs.
//    //
//    LONG CountOnLastRead;
//
//    //
//    // This is a count of the number of characters read by the
//    // isr routine.  It is *ONLY* written at isr level.  We can
//    // read it at dispatch level.
//    //
//    ULONG ReadByIsr;
//
//    //
//    // This holds the current baud rate for the device.
//    //
//    ULONG CurrentBaud;
//
//    //
//    // This is the number of characters read since the XoffCounter
//    // was started.  This variable is only accessed at device level.
//    // If it is greater than zero, it implies that there is an
//    // XoffCounter ioctl in the queue.
//    //
//    LONG CountSinceXoff;
//
//    //
//    // This ulong is incremented each time something trys to start
//    // the execution path that tries to lower the RTS line when
//    // doing transmit toggling.  If it "bumps" into another path
//    // (indicated by a false return value from queueing a dpc
//    // and a TRUE return value tring to start a timer) it will
//    // decrement the count.  These increments and decrements
//    // are all done at device level.  Note that in the case
//    // of a bump while trying to start the timer, we have to
//    // go up to device level to do the decrement.
//    //
//    ULONG CountOfTryingToLowerRTS;
//
//    //
//    // This ULONG is used to keep track of the "named" (in ntddser.h)
//    // baud rates that this particular device supports.
//    //
//    ULONG SupportedBauds;
//
    //
    // This value holds the span (in units of bytes) of the register
    // set controlling this port.  This is constant over the life
    // of the port.
    //
    ULONG SpanOfController;
    ULONG SpanOfPortBase[2];
    ULONG SpanOfConfBase;

//    //
//    // This value holds the span (in units of bytes) of the interrupt
//    // status register associated with this port.  This is constant
//    // over the life of the port.
//    //
//    ULONG SpanOfInterruptStatus;
//
//    //
//    // Hold the clock rate input to the ik220 part.
//    //
//    ULONG ClockRate;
//
//    //
//    // The number of characters to push out if a fifo is present.
//    //
//    ULONG TxFifoAmount;
//
//    //
//    // Set to indicate that it is ok to share interrupts within the device.
//    //
//    ULONG PermitShare;
//
//    //
//    // Holds the timeout controls for the device.  This value
//    // is set by the Ioctl processing.
//    //
//    // It should only be accessed under protection of the control
//    // lock since more than one request can be in the control dispatch
//    // routine at one time.
//    //
//    SERIAL_TIMEOUTS Timeouts;
//
//    //
//    // This holds the various characters that are used
//    // for replacement on errors and also for flow control.
//    //
//    // They are only set at interrupt level.
//    //
//    SERIAL_CHARS SpecialChars;
//
//    //
//    // This structure holds the handshake and control flow
//    // settings for the ik220 driver.
//    //
//    // It is only set at interrupt level.  It can be
//    // be read at any level with the control lock held.
//    //
//    SERIAL_HANDFLOW HandFlow;
//
//
//    //
//    // Holds performance statistics that applications can query.
//    // Reset on each open.  Only set at device level.
//    //
//    SERIALPERF_STATS PerfStats;
//
//    //
//    // This holds what we beleive to be the current value of
//    // the line control register.
//    //
//    // It should only be accessed under protection of the control
//    // lock since more than one request can be in the control dispatch
//    // routine at one time.
//    //
//    UCHAR LineControl;
//
    //
    // We keep track of whether the somebody has the device currently
    // opened with a simple boolean.  We need to know this so that
    // spurious interrupts from the device (especially during initialization)
    // will be ignored.  This value is only accessed in the ISR and
    // is only set via synchronization routines.  We may be able
    // to get rid of this boolean when the code is more fleshed out.
    //
    BOOLEAN DeviceIsOpened;

    //
    // Set at intialization to indicate that on the current
    // architecture we need to unmap the base register address
    // when we unload the driver.
    //
    BOOLEAN UnMapRegisters;
    BOOLEAN UnMapPortBase[2];
    BOOLEAN UnMapConfBase;

    //
    // Set at intialization to indicate that on the current
    // architecture we need to unmap the interrupt status address
    // when we unload the driver.
    //
    BOOLEAN UnMapStatus;

//    //
//    // This is only accessed at interrupt level.  It keeps track
//    // of whether the holding register is empty.
//    //
//    BOOLEAN HoldingEmpty;
//
//    //
//    // This variable is only accessed at interrupt level.  It
//    // indicates that we want to transmit a character immediately.
//    // That is - in front of any characters that could be transmitting
//    // from a normal write.
//    //
//    BOOLEAN TransmitImmediate;
//
//    //
//    // This variable is only accessed at interrupt level.  Whenever
//    // a wait is initiated this variable is set to false.
//    // Whenever any kind of character is written it is set to true.
//    // Whenever the write queue is found to be empty the code that
//    // is processing that completing irp will synchonize with the interrupt.
//    // If this synchronization code finds that the variable is true and that
//    // there is a wait on the transmit queue being empty then it is
//    // certain that the queue was emptied and that it has happened since
//    // the wait was initiated.
//    //
//    BOOLEAN EmptiedTransmit;
//
//    //
//    // We keep the following values around so that we can connect
//    // to the interrupt and report resources after the configuration
//    // record is gone.
//    //
//
    //
    // Translated vector
    //

    ULONG Vector;

    //
    // Translated Irql
    //

    KIRQL Irql;


    //
    // Untranslated vector
    //

    ULONG OriginalVector;


    //
    // Untranslated irql
    //

    ULONG OriginalIrql;


    //
    // Address space
    //

    ULONG AddressSpace;
    ULONG PortBaseAddressSpace[2];
    ULONG ConfBaseAddressSpace;


    //
    // Bus number
    //

    ULONG BusNumber;
    ULONG PortBaseBusNumber[2];
    ULONG ConfBaseBusNumber;


    //
    // Interface type
    //

    INTERFACE_TYPE InterfaceType;


    //
    // Port index no for multiport devices
    //

    ULONG PortIndex;


//    //
//    // Indexed flag for multiport devices
//    //
//
//    BOOLEAN Indexed;
//
//    //
//    // Mask inverted mask for multiport devices
//    //
//
//    ULONG MaskInverted;
//
//    //
//    // Needed to add new devices to multiport boards
//    //
//
//    ULONG NewPortIndex;
//    ULONG NewMaskInverted;
    PVOID NewExtension;

//    //
//    // We hold the character that should be transmitted immediately.
//    //
//    // Note that we can't use this to determine whether there is
//    // a character to send because the character to send could be
//    // zero.
//    //
//    UCHAR ImmediateChar;
//
//    //
//    // This holds the mask that will be used to mask off unwanted
//    // data bits of the received data (valid data bits can be 5,6,7,8)
//    // The mask will normally be 0xff.  This is set while the control
//    // lock is held since it wouldn't have adverse effects on the
//    // isr if it is changed in the middle of reading characters.
//    // (What it would do to the app is another question - but then
//    // the app asked the driver to do it.)
//    //
//    UCHAR ValidDataMask;
//
//    //
//    // The application can turn on a mode,via the
//    // IOCTL_SERIAL_LSRMST_INSERT ioctl, that will cause the
//    // ik220 driver to insert the line status or the modem
//    // status into the RX stream.  The parameter with the ioctl
//    // is a pointer to a UCHAR.  If the value of the UCHAR is
//    // zero, then no insertion will ever take place.  If the
//    // value of the UCHAR is non-zero (and not equal to the
//    // xon/xoff characters), then the ik220 driver will insert.
//    //
//    UCHAR EscapeChar;
//
//    //
//    // These two booleans are used to indicate to the isr transmit
//    // code that it should send the xon or xoff character.  They are
//    // only accessed at open and at interrupt level.
//    //
//    BOOLEAN SendXonChar;
//    BOOLEAN SendXoffChar;
//
//    //
//    // This boolean will be true if a 16550 is present *and* enabled.
//    //
//    BOOLEAN FifoPresent;
//
//    //
//    // This denotes that this particular port is an on the motherboard
//    // port for the Jensen hardware.  On these ports the OUT2 bit
//    // which is used to enable/disable interrupts is always hight.
//    //
//    BOOLEAN Jensen;
//
//    //
//    // This is the water mark that the rxfifo should be
//    // set to when the fifo is turned on.  This is not the actual
//    // value, but the encoded value that goes into the register.
//    //
//    UCHAR RxFifoTrigger;
//
//    //
//    // Says whether this device can share interrupts with devices
//    // other than ik220 devices.
//    //
//    BOOLEAN InterruptShareable;
//
    //
    // Records whether we actually created the symbolic link name
    // at driver load time.  If we didn't create it, we won't try
    // to destroy it when we unload.
    //
    BOOLEAN CreatedSymbolicLink;

    //
    // Records whether we actually created an entry in IK220COMM
    // at driver load time.  If we didn't create it, we won't try
    // to destroy it when the device is removed.
    //
    BOOLEAN CreatedIk220CommEntry;

//    //
//    // We place all of the kernel and Io subsystem "opaque" structures
//    // at the end of the extension.  We don't care about their contents.
//    //
//
    //
    // This lock will be used to protect various fields in
    // the extension that are set (& read) in the extension
    // by the io controls.
    //
    KSPIN_LOCK ControlLock;

    //
    // This lock will be used to protect the accept / reject state
    // transitions and flags of the driver  It must be acquired
    // before a cancel lock
    //

    KSPIN_LOCK FlagsLock;

//    //
//    // This points to a DPC used to complete read requests.
//    //
//    KDPC CompleteWriteDpc;
//
//    //
//    // This points to a DPC used to complete read requests.
//    //
//    KDPC CompleteReadDpc;
//
//    //
//    // This dpc is fired off if the timer for the total timeout
//    // for the read expires.  It will execute a dpc routine that
//    // will cause the current read to complete.
//    //
//    //
//    KDPC TotalReadTimeoutDpc;
//
//    //
//    // This dpc is fired off if the timer for the interval timeout
//    // expires.  If no more characters have been read then the
//    // dpc routine will cause the read to complete.  However, if
//    // more characters have been read then the dpc routine will
//    // resubmit the timer.
//    //
//    KDPC IntervalReadTimeoutDpc;
//
//    //
//    // This dpc is fired off if the timer for the total timeout
//    // for the write expires.  It will execute a dpc routine that
//    // will cause the current write to complete.
//    //
//    //
//    KDPC TotalWriteTimeoutDpc;
//
//    //
//    // This dpc is fired off if a comm error occurs.  It will
//    // execute a dpc routine that will cancel all pending reads
//    // and writes.
//    //
//    KDPC CommErrorDpc;
//
//    //
//    // This dpc is fired off if an event occurs and there was
//    // a irp waiting on that event.  A dpc routine will execute
//    // that completes the irp.
//    //
//    KDPC CommWaitDpc;
//
//    //
//    // This dpc is fired off when the transmit immediate char
//    // character is given to the hardware.  It will simply complete
//    // the irp.
//    //
//    KDPC CompleteImmediateDpc;
//
//    //
//    // This dpc is fired off if the transmit immediate char
//    // character times out.  The dpc routine will "grab" the
//    // irp from the isr and time it out.
//    //
//    KDPC TotalImmediateTimeoutDpc;
//
//    //
//    // This dpc is fired off if the timer used to "timeout" counting
//    // the number of characters received after the Xoff ioctl is started
//    // expired.
//    //
//    KDPC XoffCountTimeoutDpc;
//
//    //
//    // This dpc is fired off if the xoff counter actually runs down
//    // to zero.
//    //
//    KDPC XoffCountCompleteDpc;
//
//    //
//    // This dpc is fired off only from device level to start off
//    // a timer that will queue a dpc to check if the RTS line
//    // should be lowered when we are doing transmit toggling.
//    //
//    KDPC StartTimerLowerRTSDpc;
//
//    //
//    // This dpc is fired off when a timer expires (after one
//    // character time), so that code can be invoked that will
//    // check to see if we should lower the RTS line when
//    // doing transmit toggling.
//    //
//    KDPC PerhapsLowerRTSDpc;
//
//    //
//    // This DPC is fired to set an event stating that all other
//    // DPC's have been finish for this device extension so that
//    // paged code may be unlocked.
//    //
//
//    KDPC IsrUnlockPagesDpc;
//
//    //
//    // This is the kernal timer structure used to handle
//    // total read request timing.
//    //
//    KTIMER ReadRequestTotalTimer;
//
//    //
//    // This is the kernal timer structure used to handle
//    // interval read request timing.
//    //
//    KTIMER ReadRequestIntervalTimer;
//
//    //
//    // This is the kernal timer structure used to handle
//    // total time request timing.
//    //
//    KTIMER WriteRequestTotalTimer;
//
//    //
//    // This is the kernal timer structure used to handle
//    // total time request timing.
//    //
//    KTIMER ImmediateTotalTimer;
//
//    //
//    // This timer is used to timeout the xoff counter
//    // io.
//    //
//    KTIMER XoffCountTimer;
//
//    //
//    // This timer is used to invoke a dpc one character time
//    // after the timer is set.  That dpc will be used to check
//    // whether we should lower the RTS line if we are doing
//    // transmit toggling.
//    //
//    KTIMER LowerRTSTimer;
//
    //
    // This is a pointer to the next lower device in the IRP stack.
    //

    PDEVICE_OBJECT LowerDeviceObject;

    //
    // This is where keep track of the power state the device is in.
    //

    DEVICE_POWER_STATE PowerState;

    //
    // Pointer to the driver object
    //

    PDRIVER_OBJECT DriverObject;

//
//    //
//    // Event used to do some synchronization with the devices underneath me
//    // (namely ACPI)
//    //
//
//    KEVENT Ik220SyncEvent;
//
//
    //
    // String where we keep the symbolic link that is returned to us when we
    // register our device under the COMM class with the Plug and Play manager.
    //

    UNICODE_STRING DeviceClassSymbolicName;

//
//    //
//    // Ik220 ISR switch structure
//    //
//
//    PSERIAL_CISR_SW CIsrSw;
//
    //
    // Count of pending IRP's
    //

    ULONG PendingIRPCnt;

    //
    // Accepting requests?
    //

    ULONG DevicePNPAccept;

    //
    // No IRP's pending event
    //

    KEVENT PendingIRPEvent;

    //
    // PNP State
    //

    ULONG PNPState;

    //
    // Misc Flags
    //

    ULONG Flags;

    //
    // Open count
    //

    LONG OpenCount;

    //
    // Start sync event
    //

    KEVENT Ik220StartEvent;

    //
    // Current state during powerdown
    //

    IK220_DEVICE_STATE DeviceState;

    //
    // Device stack capabilites
    //

    DEVICE_POWER_STATE DeviceStateMap[PowerSystemMaximum];

    //
    // Event to signal transition to D0 completion
    //

    KEVENT PowerD0Event;

    //
    // List of stalled IRP's
    //

    LIST_ENTRY StalledIrpQueue;

    //
    // Mutex on open status
    //

    FAST_MUTEX OpenMutex;

    //
    // Mutex on close
    //

    FAST_MUTEX CloseMutex;

    //
    // TRUE if we own power policy
    //

    BOOLEAN OwnsPowerPolicy;

    //
    // SystemWake from devcaps
    //

    SYSTEM_POWER_STATE SystemWake;

    //
    // DeviceWake from devcaps
    //

    DEVICE_POWER_STATE DeviceWake;

    //
    // Our PDO
    //

    PDEVICE_OBJECT Pdo;

    //
    // Should we enable wakeup
    //

    BOOLEAN SendWaitWake;

    //
    // Pending wait wake IRP
    //

    PIRP PendingWakeIrp;

    //
    // WMI Information
    //

    WMILIB_CONTEXT WmiLibInfo;

    //
    // Name to use as WMI identifier
    //

    UNICODE_STRING WmiIdentifier;
//
//    //
//    // WMI Comm Data
//    //
//
//    SERIAL_WMI_COMM_DATA WmiCommData;
//
//    //
//    // WMI HW Data
//    //
//
//    SERIAL_WMI_HW_DATA WmiHwData;
//
//    //
//    // WMI Performance Data
//    //
//
//    SERIAL_WMI_PERF_DATA WmiPerfData;
//
    //
    // Pending DPC count
    //

    ULONG DpcCount;

    //
    // Pending DPC event
    //

    KEVENT PendingDpcEvent;

    //
    // Should we expose external interfaces?
    //

    ULONG SkipNaming;

} IK220_DEVICE_EXTENSION, *PIK220_DEVICE_EXTENSION;


#define IK220_PNPACCEPT_OK                 0x0L
#define IK220_PNPACCEPT_REMOVING           0x1L
#define IK220_PNPACCEPT_STOPPING           0x2L
#define IK220_PNPACCEPT_STOPPED            0x4L
#define IK220_PNPACCEPT_SURPRISE_REMOVING  0x8L

#define IK220_PNP_ADDED                    0x0L
#define IK220_PNP_STARTED                  0x1L
#define IK220_PNP_QSTOP                    0x2L
#define IK220_PNP_STOPPING                 0x3L
#define IK220_PNP_QREMOVE                  0x4L
#define IK220_PNP_REMOVING                 0x5L
#define IK220_PNP_RESTARTING               0x6L

#define IK220_FLAGS_CLEAR                  0x0L
#define IK220_FLAGS_STARTED                0x1L
#define IK220_FLAGS_STOPPED                0x2L
#define IK220_FLAGS_BROKENHW               0x4L


typedef struct _BUS_TAG
{   ULONG BusType;
    ULONG BusNumber;
} BUS, *PBUS;


typedef struct _CONFIG_DATA {
    PHYSICAL_ADDRESS    Controller;
    PHYSICAL_ADDRESS    TrController;
    PHYSICAL_ADDRESS    PortBase[2];
    PHYSICAL_ADDRESS    TrPortBase[2];
    PHYSICAL_ADDRESS    ConfBase;
    PHYSICAL_ADDRESS    TrConfBase;
//    PHYSICAL_ADDRESS    InterruptStatus;
//    PHYSICAL_ADDRESS    TrInterruptStatus;
    ULONG               SpanOfController;
    ULONG               SpanOfPortBase[2];
    ULONG               SpanOfConfBase;
//    ULONG               SpanOfInterruptStatus;
    ULONG               PortIndex;
//    ULONG               ClockRate;
    ULONG               BusNumber;
    ULONG               PortBaseBusNumber[2];
    ULONG               ConfBaseBusNumber;
    ULONG               AddressSpace;
    ULONG               PortBaseAddressSpace[2];
    ULONG               ConfBaseAddressSpace;
//    ULONG               DisablePort;
//    ULONG               ForceFifoEnable;
//    ULONG               RxFIFO;
//    ULONG               TxFIFO;
//    ULONG               PermitShare;
//    ULONG               PermitSystemWideShare;
//    ULONG               LogFifo;
//    ULONG               MaskInverted;
    KINTERRUPT_MODE     InterruptMode;
    INTERFACE_TYPE      InterfaceType;
    ULONG               OriginalVector;
    ULONG               OriginalIrql;
    ULONG               TrVector;
    ULONG               TrIrql;
    ULONG               Affinity;
//    ULONG               Indexed;
    } CONFIG_DATA,*PCONFIG_DATA;



//
// This structure contains configuration data, much of which
// is read from the registry.
//
typedef struct _IK220_FIRMWARE_DATA {
    PDRIVER_OBJECT  DriverObject;
    ULONG           ControllersFound;
    ULONG           DebugLevel;
    ULONG           ShouldBreakOnEntry;
    UNICODE_STRING  Directory;
    UNICODE_STRING  NtNameSuffix;
    UNICODE_STRING  DirectorySymbolicName;
    LIST_ENTRY      ConfigList;
} IK220_FIRMWARE_DATA,*PIK220_FIRMWARE_DATA;


#if DBG
#define Ik220LockPagableSectionByHandle(_secHandle) \
{ \
    MmLockPagableSectionByHandle((_secHandle)); \
    InterlockedIncrement(&Ik220Globals.PAGEIK_Count); \
}

#define Ik220UnlockPagableImageSection(_secHandle) \
{ \
   InterlockedDecrement(&Ik220Globals.PAGEIK_Count); \
   MmUnlockPagableImageSection(_secHandle); \
}


#define IK220_LOCKED_PAGED_CODE() \
    if ((KeGetCurrentIrql() > APC_LEVEL)  \
    && (Ik220Globals.PAGEIK_Count == 0)) { \
    KdPrint(("IK220: Pageable code called at IRQL %d without lock \n", \
             KeGetCurrentIrql())); \
        ASSERT(FALSE); \
        }

#else
#define Ik220LockPagableSectionByHandle(_secHandle) \
{ \
    MmLockPagableSectionByHandle((_secHandle)); \
}

#define Ik220UnlockPagableImageSection(_secHandle) \
{ \
   MmUnlockPagableImageSection(_secHandle); \
}

#define IK220_LOCKED_PAGED_CODE()
#endif // DBG


typedef enum _IK220_MEM_COMPARES {
    AddressesAreEqual,
    AddressesOverlap,
    AddressesAreDisjoint
    } IK220_MEM_COMPARES,*PIK220_MEM_COMPARES;


typedef struct _IK220_GLOBALS {
   LIST_ENTRY AllDevObjs;
   PVOID PAGEIK_Handle;
   UNICODE_STRING RegistryPath;
   PUSHORT         GloPortBase[2*MAX_CARDS];
   PULONG          GloConfBase[MAX_CARDS];
   ULONG           instances;
   BOOLEAN         CreatedSymbolicLink;
#if DBG
   ULONG PAGEIK_Count;
#endif // DBG
} IK220_GLOBALS, *PIK220_GLOBALS;

extern IK220_GLOBALS Ik220Globals;

typedef struct _IK220_USER_DATA {
   PHYSICAL_ADDRESS UserPort;
//   PHYSICAL_ADDRESS UserInterruptStatus;
   ULONG UserVector;
   UNICODE_STRING UserSymbolicLink;
   ULONG UserPortIndex;
   ULONG UserBusNumber;
   ULONG UserInterfaceType;
//   ULONG UserClockRate;
//   ULONG UserIndexed;
   ULONG UserInterruptMode;
   ULONG UserAddressSpace;
   ULONG UserLevel;
//   ULONG DefaultPermitSystemWideShare;
//   ULONG DisablePort;
//   ULONG PermitShareDefault;
//   ULONG MaskInverted;
} IK220_USER_DATA, *PIK220_USER_DATA;


#define DEVICE_OBJECT_NAME_LENGTH       128
#define SYMBOLIC_NAME_LENGTH            128
#define IK220_PNP_ID_STR                L"PCI\\VEN_10b5&DEV_9050&SUBSYS_117210b5"
#define IK220_DEVICE_MAP                L"IK220COMM"


#endif
