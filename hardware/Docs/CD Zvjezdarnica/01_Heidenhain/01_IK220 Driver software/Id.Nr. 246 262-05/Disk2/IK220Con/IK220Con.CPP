#include <stdio.h>
#include <conio.h>
#include "IK220Con.h"
#include "..\include\IK220.h"


ULONG	IKCard[(MAX_CARDS*2)];									// Port address of IK card(s)
USHORT	Card, Ax;												// Axis number
USHORT	LedStat = 0;

char	VersCard[20], VersDrv[20], VersDll[20];		// Version text of card, driver and DLL


double	CntVal;										// Counter value
ULONG	DllStatus, DllInfo;

void main (void)

{


	if (!IK220Find256 (IKCard))		// Look for IK 220
	{
		printf ("Error: IK220Find\n");									
		if (!IK220DllStatus (&DllStatus, &DllInfo))  printf ("Error: IK220DllStatus\n");		// Read DLL-Status
		printf ("DLL-Status: 0x%08lX     DLL-Info: 0x%08lX", DllStatus, DllInfo);
		_getch();
		return;
	}

	for (Ax=0; Ax<(MAX_CARDS*2); Ax++)
		if (IKCard[Ax])
		{
		if (!IK220Init (Ax))  printf ("Error: IK220Init axis %3d\n", Ax);					// Initialize IK 220
			else 
			{
				printf ("Axis %3d initialized  -  ", Ax);

				if (!IK220Version (Ax, &VersCard[0], &VersDrv[0], &VersDll[0])) printf ("Error: IKVersion\n");				// Read port address of IK card(s)
				else 
					printf ("Card: %s  %s  %s\n", VersCard, VersDrv, VersDll);   


			}
		}

	for (Ax=0; Ax<(MAX_CARDS*2); Ax++)
		if (IKCard[Ax])
		{
			printf ("IK 220 (%3d) at address: 0x%08lX\n", Ax, IKCard[Ax]);
			IK220Led (Ax, 0);
		}

	printf ("\n");

	for (Card=0; Card<MAX_CARDS; Card++)
		IK220SysLed (Card, LedStat);



	while (!_kbhit())
	{
		for (Card=0; Card<MAX_CARDS; Card++)
		{
			for (Ax=(Card*2); Ax<(Card*2+2); Ax++)
			{
				if (IKCard[Ax])
				{
					if (!IK220Read48 (Ax, 0, &CntVal))  printf ("Error: IK220Read48 card %3d\r", Ax);	// Read counter value 
					else printf ("Axis %3d: %12.4f %d     ", Ax, CntVal, LedStat);
				}
			}
			if (IKCard[Card*2])
			{
				IK220SysLed (Card, LedStat);
				Sleep (1000);
				printf ("\r");
			}
		if (_kbhit()) break; 
		}
		LedStat=(LedStat ^ 0x01);
	}
	_getch();


 printf ("\n\n");
}
