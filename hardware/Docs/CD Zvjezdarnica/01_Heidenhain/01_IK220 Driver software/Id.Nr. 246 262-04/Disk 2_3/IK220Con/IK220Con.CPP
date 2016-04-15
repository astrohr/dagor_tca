#include <stdio.h>
#include <conio.h>
#include "IK220Con.h"



ULONG	IKCard[16];									// Port address of IK card(s)
USHORT	Ax;											// Axis number

char	VersCard[20], VersDrv[20], VersDll[20];		// Version text of card, driver and DLL


double	CntVal;										// Counter value
ULONG	DllStatus, DllInfo;

void main (void)

{


	if (!IK220Find (IKCard))		// Look for IK 220
	{
		printf ("Error: IK220Find\n");									
		if (!IK220DllStatus (&DllStatus, &DllInfo))  printf ("Error: IK220DllStatus\n");		// Read DLL-Status
		printf ("DLL-Status: 0x%08lX     DLL-Info: 0x%08lX", DllStatus, DllInfo);
		_getch();
		return;
	}

	for (Ax=0; Ax<16; Ax++)
		if (IKCard[Ax])
		{
		if (!IK220Init (Ax))  printf ("Error: IK220Init axis %d\n", Ax);					// Initialize IK 220
			else 
			{
				printf ("Axis %d initialized  -  ", Ax);

				if (!IK220Version (Ax, &VersCard[0], &VersDrv[0], &VersDll[0])) printf ("Error: IKVersion\n");				// Read port address of IK card(s)
				else 
					printf ("Card: %s  %s  %s\n", VersCard, VersDrv, VersDll);   


			}
		}

	for (Ax=0; Ax<16; Ax++)
		if (IKCard[Ax]) printf ("IK 220 (%2d) at address: 0x%08lX\n", Ax, IKCard[Ax]);	
	printf ("\n");



	while (!_kbhit())
	{
		for (Ax=0; Ax<2; Ax++)
			if (IKCard[Ax])
			{
				if (!IK220Read48 (Ax, 0, &CntVal))  printf ("Error: IK220Read48 card %d\n", Ax);	// Read counter value 
				else printf ("Axis %d: %12.4f     ", Ax, CntVal);
			}
		printf ("\r");
		Sleep (200);
	}
	_getch();


 printf ("\n\n");
}
