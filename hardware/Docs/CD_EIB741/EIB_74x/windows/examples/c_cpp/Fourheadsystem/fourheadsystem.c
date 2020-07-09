/*!------------------------------------------------
*
*  Demo programm for EIB7
*
*  \file    fourheadsystem.c
*  \author  DR.JOHANNES HEIDENHAIN GmbH
*  \date    15.05.2012
*  \version $Revision: 1.0 $
* 
*  \brief   example for incremental measurement 
*           systems with four reading heads
*  
*  Content:
*  Each of the four reading heads will be referenced
*  with distance coded reference marks. The final
*  position will be calculated out of the absolute
*  positions of the four reading heads.
*
-----------------------------------------------------*/


#include <eib7.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef Linux
#include <signal.h>
#endif

#ifdef _WIN32
#define POS_SPEC "Position (%s): %10I64d\r"
#else
#define POS_SPEC "Position (%s): %010lld\r"
#endif


/* definitions */
#define EIB_TCP_TIMEOUT             5000   /* timeout for TCP connection in ms                      */
#define NUM_OF_AXIS                 4      /* number of axes of the EIB                             */
#define NUM_OF_IO                   4      /* number of inputs and outputs                          */
#define MAX_SRT_DATA                200    /* maximum size of soft realtime data                    */
#define MAX_TEXT_LEN                200    /* maximum size of console input string                  */
#define TIMESTAMP_PERIOD            1000   /* Timestamp Period = 1 ms = 1000us                      */
#define TRIGGER_PERIOD              1000   /* Trigger Period = 1 ms = 1000 us                       */
#define SYSTEM_NOMINAL_INCREMENT    1000   /* nominal increment for distance coded reference marks  */
#define SYSTEM_LINE_COUNTS          28000  /* total number of signal periods                        */


/* struct for soft realtime mode data */
typedef struct EncData
{
   ENCODER_POSITION incr_position;           /* incremental position value                   */
   ENCODER_POSITION position;                /* absolute position value                      */
   unsigned short status;                    /* status word                                  */
   unsigned long Timestamp;                  /* timestamp                                    */
   ENCODER_POSITION ref_c;                   /* distance coded reference position values     */
} EncData_t;


/* function declarations */
void CheckError(EIB7_ERR error);


/* global variable for console handler to stop on user request */
static int stop = 0;

#ifdef _WIN32

/* handler for console to catch user inputs */
BOOL CtrlHandler( DWORD fdwCtrlType )
{
   if(fdwCtrlType == CTRL_C_EVENT)
   {
      stop = 1;
      return TRUE;
   }

   return FALSE;
}
#endif

#ifdef Linux

/* handler for console to catch user inputs */
void CtrlHandler(int sig)
{
   if(sig==SIGINT)
     stop = 1;
}
#endif



/* The EIB is configured for four incremental reading heads with 1Vpp 
   interface and distance coded reference marks. In a first step the 
   system will be referenced. For this each reading head must traverse 
   two consecutive reference marks. Afterwards the final position will
   be generated with the four absolute positions of the reading heads.
   */
int main(int argc, char **argv)
{
   EIB7_HANDLE eib;                       /* EIB handle                  */
   unsigned long ip;                      /* IP address of EIB           */
   unsigned long num;                     /* number of encoder axes      */
   unsigned long TimerTicks;              /* timer ticks per us          */
   unsigned long TimerPeriod;             /* timer trigger period        */
   unsigned long TimestampTicks;          /* ticks per us (timestamp)    */
   unsigned long TimestampPeriod;         /* period of timestamp counter */
   EIB7_AXIS axis[NUM_OF_AXIS];           /* axes array                  */
   EIB7_IO input[NUM_OF_IO];              /* IO array (input)            */
   EIB7_IO output[NUM_OF_IO];             /* IO array (output)           */
   EIB7_DataPacketSection packet[5];      /* Data packet configuration   */
   char fw_version[20];                   /* firmware version string     */
   char hostname[MAX_TEXT_LEN];           /* hostname string             */
   char TriggerConf[MAX_TEXT_LEN];        /* input string                */
   int ExtTrigger;                        /* activate external trigger   */
   unsigned char srt_data[MAX_SRT_DATA];  /* buffer for udp data packet  */
   unsigned long entries;                 /* entries read from FIFO      */
   void *field;                           /* pointer to data field       */
   unsigned long sz;                      /* size of data field          */
   EIB7_ERR error;                        /* error code                  */
   ENCODER_POSITION pos_value;            /* final position value        */
   EncData_t encpos1, encpos2;            /* encoder position and status */
   EncData_t encpos3, encpos4;            /* encoder position and status */
   unsigned int refdone;                  /* status for referencing      */
   unsigned int head1ref, head2ref;       /* status for referencing      */
   unsigned int head3ref, head4ref;       /* status for referencing      */
   unsigned int referror;                 /* status for referencing      */
   unsigned int sigerror;                 /* encoder signal status       */
   unsigned int i;


   /* register console handler for program termination on user request */
#ifdef _WIN32
   SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE );
#endif
#ifdef Linux
   signal(SIGINT, CtrlHandler);
   signal(SIGTERM, CtrlHandler);
#endif

   /* ask for parameters */
   printf("Please enter IP address or hostname: ");
   scanf("%s", hostname);
   printf("use external trigger (y/n)? ");
   scanf("%s",TriggerConf);
   ExtTrigger = 0;
   if(TriggerConf[0]=='y' || TriggerConf[0]=='Y')
   {
      ExtTrigger = 1;
   }

   /* open connection to EIB */
   CheckError(EIB7GetHostIP(hostname, &ip));
   CheckError(EIB7Open(ip, &eib, EIB_TCP_TIMEOUT, fw_version, sizeof(fw_version)));
   printf("\nEIB firmware version: %s\n\n", fw_version);

   /* read axes array */
   CheckError(EIB7GetAxis(eib, axis, NUM_OF_AXIS, &num));

   /* init axes: 1 Vpp */
   for(i=0; i<num; i++)
   {
      CheckError(EIB7InitAxis(axis[i],
                 EIB7_IT_Incremental,     /* incremental encoder                */
                 EIB7_EC_Rotary,          /* rotary measurement system          */
                 EIB7_RM_DistanceCoded,   /* distance coded reference marks     */
                 SYSTEM_LINE_COUNTS,      /* line counts                        */
                 SYSTEM_NOMINAL_INCREMENT,/* nominal increment                  */
                 EIB7_HS_None,            /* not used                           */
                 EIB7_LS_None,            /* not used                           */
                 EIB7_CS_CompActive,      /* signal compensation on             */
                 EIB7_BW_High,            /* signal bandwidth: high             */
                 EIB7_CLK_Default,        /* not used for incremental interface */
                 EIB7_RT_Long,            /* not used for incremental interface */
                 EIB7_CT_Long             /* not used for incremental interface */
               ));

      CheckError(EIB7SetTimestamp(axis[i], EIB7_MD_Enable));
      CheckError(EIB7ClearEncoderErrors(axis[i]));
      CheckError(EIB7ClearLostTriggerError(axis[i]));
   }

   /* timestamp configuration */
   TimestampPeriod = TIMESTAMP_PERIOD;
   CheckError(EIB7GetTimestampTicks(eib, &TimestampTicks));
   TimestampPeriod *= TimestampTicks;
   CheckError(EIB7SetTimestampPeriod(eib, TimestampPeriod));
   
	/* data packet configuration */
   CheckError(EIB7AddDataPacketSection(packet, 0, EIB7_DR_Global, EIB7_PDF_TriggerCounter));
   CheckError(EIB7AddDataPacketSection(packet, 1, EIB7_DR_Encoder1, EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_DistCodedRef));
   CheckError(EIB7AddDataPacketSection(packet, 2, EIB7_DR_Encoder2, EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_DistCodedRef));
   CheckError(EIB7AddDataPacketSection(packet, 3, EIB7_DR_Encoder3, EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_DistCodedRef));
   CheckError(EIB7AddDataPacketSection(packet, 4, EIB7_DR_Encoder4, EIB7_PDF_StatusWord | EIB7_PDF_PositionData | EIB7_PDF_Timestamp | EIB7_PDF_DistCodedRef));
   CheckError(EIB7ConfigDataPacket(eib, packet, 5));

   /* trigger configuration */
   if(ExtTrigger)
   {
      /* external trigger */
      unsigned long ilen, olen;

      printf("using external trigger\n");
      CheckError(EIB7GetIO(eib, input, NUM_OF_IO, &ilen, output, NUM_OF_IO, &olen));
      /* trigger input enabled, termination resistor disabled */
      CheckError(EIB7InitInput(input[0], EIB7_IOM_Trigger, EIB7_MD_Disable));

      for(i=0; i<num; i++)
         CheckError(EIB7AxisTriggerSource(axis[i], EIB7_AT_TrgInput1));
      CheckError(EIB7MasterTriggerSource(eib, EIB7_AT_TrgInput1));
   }
   else
   {
      /* internal trigger */
      printf("using internal timer trigger\n");
      CheckError(EIB7GetTimerTriggerTicks(eib, &TimerTicks));
      TimerPeriod = TRIGGER_PERIOD;
      TimerPeriod *= TimerTicks;
      CheckError(EIB7SetTimerTriggerPeriod(eib, TimerPeriod));

      for(i=0; i<num; i++)
         CheckError(EIB7AxisTriggerSource(axis[i], EIB7_AT_TrgTimer));
      CheckError(EIB7MasterTriggerSource(eib, EIB7_AT_TrgTimer));
   }

   /* operation mode SoftRealtime */
   CheckError(EIB7SelectMode(eib, EIB7_OM_SoftRealtime));
   if(ExtTrigger)
   {
		CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, EIB7_TS_TrgInput1));
	 }
   else
   {
      CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Enable, EIB7_TS_TrgTimer));
   }


   printf("\n\npress Ctrl-C to stop\n\n");

   /**********************************************************
   * finding reference positions
   ***********************************************************/

   /* start referncing */
   for(i=0; i<num; i++)
   {
      CheckError(EIB7ClearRefStatus(axis[i]));
      CheckError(EIB7StartRef(axis[i], EIB7_RP_RefPos2));
   }
   printf("start referencing...\n\n");

   /* initialisation */
   refdone = 0;
   referror = 0;
   head1ref = 0;
   head2ref = 0;
   head3ref = 0;
   head4ref = 0;
   encpos1.ref_c = 0;
   encpos2.ref_c = 0;
   encpos3.ref_c = 0;
   encpos4.ref_c = 0;

   while((stop == 0) & (refdone == 0))
   {
      error = EIB7ReadFIFOData(eib, srt_data, 1, &entries, 100);
      if(error==EIB7_FIFOOverflow)
      {
         printf("\nFIFO Overflow error detected, clearing FIFO.\n");
         EIB7ClearFIFO(eib);
      }

      if(entries > 0)
      {
         /* reading head 1 */
         CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder1,
                    EIB7_PDF_StatusWord, &field, &sz));
         encpos1.status = *(unsigned short *)field;
         CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder1,
                    EIB7_PDF_PositionData, &field, &sz));
         encpos1.incr_position = *(ENCODER_POSITION *)field;

         if((encpos1.status & 0x0F00) == 0x0700)
         {
            /* reading head 1 referenced */
            if(head1ref == 0)
            {
               printf("Encoder head 1 referenced                \n");
               CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder1,
                       EIB7_PDF_DistCodedRef, &field, &sz));
               encpos1.ref_c = *(ENCODER_POSITION *)field;
            }
            head1ref = 1;
         }
         if((encpos1.status & 0x08BA) != 0)
         {
            referror++;
            refdone = 1;
         }

         /* print position of encoder head 1 for user feedback */
         printf(POS_SPEC, "incr", encpos1.incr_position);

         /* reading head 2 */
         CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder2,
                    EIB7_PDF_StatusWord, &field, &sz));
         encpos2.status = *(unsigned short *)field;

         if((encpos2.status & 0x0F00) == 0x0700)
         {
            /* reading head 2 referenced */
            if(head2ref == 0)
            {
               printf("Encoder head 2 referenced                \n");
               CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder2,
                       EIB7_PDF_DistCodedRef, &field, &sz));
               encpos2.ref_c = *(ENCODER_POSITION *)field;
            }
            head2ref = 1;
         }
         if((encpos2.status & 0x08BA) != 0)
         {
            referror++;
            refdone = 1;
         }

         /* reading head 3 */
         CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder3,
                    EIB7_PDF_StatusWord, &field, &sz));
         encpos3.status = *(unsigned short *)field;

         if((encpos3.status & 0x0F00) == 0x0700)
         {
            /* reading head 3 referenced */
            if(head3ref == 0)
            {
               printf("Encoder head 3 referenced                \n");
               CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder3,
                       EIB7_PDF_DistCodedRef, &field, &sz));
               encpos3.ref_c = *(ENCODER_POSITION *)field;
            }
            head3ref = 1;
         }
         if((encpos3.status & 0x08BA) != 0)
         {
            referror++;
            refdone = 1;
         }

         /* reading head 4 */
         CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder4,
                    EIB7_PDF_StatusWord, &field, &sz));
         encpos4.status = *(unsigned short *)field;

         if((encpos4.status & 0x0F00) == 0x0700)
         {
            /* reading head 4 referenced */
            if(head4ref == 0)
            {
               printf("Encoder head 4 referenced                \n");
               CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder4,
                       EIB7_PDF_DistCodedRef, &field, &sz));
               encpos4.ref_c = *(ENCODER_POSITION *)field;
            }
            head4ref = 1;
         }
         if((encpos4.status & 0x08BA) != 0)
         {
            referror++;
            refdone = 1;
         }

         /* check for end of referencing */
         if((head1ref > 0) & (head2ref > 0) & (head3ref > 0) & (head4ref > 0))
         {
            refdone = 1;
         }
      }
   } /* end of loop */


   /**********************************************************
   * normal operation
   ***********************************************************/

   if(referror > 0)
   {
      printf("\n...Error while referencing                \n\n");
   }
   else
   {
      /* no error */
      printf("\n...referencing finished                \n\n");

      while(stop == 0) 
      {
         /* run till the user press Ctrl-C */

         error = EIB7ReadFIFOData(eib, srt_data, 1, &entries, 100);
         if(error==EIB7_FIFOOverflow)
         {
            printf("\nFIFO Overflow error detected, clearing FIFO.\n");
            EIB7ClearFIFO(eib);
         }

         if(entries > 0)
         {
            /* initialisation */
            sigerror = 0;

            /* reading head 1 */
            CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder1,
                       EIB7_PDF_StatusWord, &field, &sz));
            encpos1.status = *(unsigned short *)field;
            CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder1,
                       EIB7_PDF_PositionData, &field, &sz));
            encpos1.incr_position = *(ENCODER_POSITION *)field;

            /* reading head 2 */
            CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder2,
                       EIB7_PDF_StatusWord, &field, &sz));
            encpos2.status = *(unsigned short *)field;
            CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder2,
                       EIB7_PDF_PositionData, &field, &sz));
            encpos2.incr_position = *(ENCODER_POSITION *)field;

            /* reading head 3 */
            CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder3,
                       EIB7_PDF_StatusWord, &field, &sz));
            encpos3.status = *(unsigned short *)field;
            CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder3,
                       EIB7_PDF_PositionData, &field, &sz));
            encpos3.incr_position = *(ENCODER_POSITION *)field;

            /* reading head 4 */
            CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder4,
                       EIB7_PDF_StatusWord, &field, &sz));
            encpos4.status = *(unsigned short *)field;
            CheckError(EIB7GetDataFieldPtr(eib, srt_data, EIB7_DR_Encoder4,
                       EIB7_PDF_PositionData, &field, &sz));
            encpos4.incr_position = *(ENCODER_POSITION *)field;

            /* caluclate absolute positions */
            encpos1.position = encpos1.incr_position - encpos1.ref_c;
            encpos2.position = encpos2.incr_position - encpos2.ref_c;
            encpos3.position = encpos3.incr_position - encpos3.ref_c;
            encpos4.position = encpos4.incr_position - encpos4.ref_c;

            /* check signal status */
            if((encpos1.status & 0x00BA) != 0)
               sigerror++;
            if((encpos2.status & 0x00BA) != 0)
               sigerror++;
            if((encpos3.status & 0x00BA) != 0)
               sigerror++;
            if((encpos4.status & 0x00BA) != 0)
               sigerror++;

            /* calculate final position value */
            pos_value = encpos1.position + encpos2.position + encpos3.position + encpos4.position;

            if(sigerror == 0)
               printf(POS_SPEC, "Ok", pos_value);
            else
               printf(POS_SPEC, "Error", pos_value);
         }
      }  /* end of loop */
   }

   /* disable trigger */
   CheckError(EIB7GlobalTriggerEnable(eib, EIB7_MD_Disable, EIB7_TS_All));
   /* disable SoftRealtime mode */
   CheckError(EIB7SelectMode(eib, EIB7_OM_Polling));

   /* close connection to EIB */
   EIB7Close(eib);

   printf("\nStopped on user request\n");

   exit(1);
}


/* check error code
   This function prints the error code and a brief description to the standard error console.
   The program will be terminated afterwards.
   */
void CheckError(EIB7_ERR error)
{
   if(error != EIB7_NoError)
   {
      char mnemonic[32];
      char message[256];

      EIB7GetErrorInfo(error, mnemonic, 32, message, 256);

      fprintf(stderr, "\nError %08x (%s): %s\n", error, mnemonic, message);
      exit(0);
   }
}
