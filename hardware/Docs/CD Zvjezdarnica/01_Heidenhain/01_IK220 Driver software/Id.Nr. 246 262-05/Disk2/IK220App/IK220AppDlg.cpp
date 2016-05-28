// IK220TSTDlg.cpp : implementation file
//

#include "stdafx.h"
#include <winioctl.h>
#include "IK220App.h"
#include "IK220AppDlg.h"
#include "SetupDlg.h"
#include "StatusDlg.h"
#include "..\include\IK220.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CStatusDlg* m_pModeless;
extern BOOL		   m_StatusOn;	
extern BOOL		   m_IK220found;	


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIK220TSTDlg dialog

CIK220TSTDlg::CIK220TSTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIK220TSTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIK220TSTDlg)
	m_Axis0 = _T("");
	m_Axis1 = _T("");
	m_Axis2 = _T("");
	m_Axis3 = _T("");
	m_Axis4 = _T("");
	m_Axis5 = _T("");
	m_Axis6 = _T("");
	m_Axis7 = _T("");
	m_Check0 = FALSE;
	m_Check1 = FALSE;
	m_Check2 = FALSE;
	m_Check3 = FALSE;
	m_Check4 = FALSE;
	m_Check5 = FALSE;
	m_Check6 = FALSE;
	m_Check7 = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIK220TSTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIK220TSTDlg)
	DDX_Text(pDX, IDC_Axis0, m_Axis0);
	DDX_Text(pDX, IDC_Axis1, m_Axis1);
	DDX_Text(pDX, IDC_Axis2, m_Axis2);
	DDX_Text(pDX, IDC_Axis3, m_Axis3);
	DDX_Text(pDX, IDC_Axis4, m_Axis4);
	DDX_Text(pDX, IDC_Axis5, m_Axis5);
	DDX_Text(pDX, IDC_Axis6, m_Axis6);
	DDX_Text(pDX, IDC_Axis7, m_Axis7);
	DDX_Check(pDX, IDC_CHECK0, m_Check0);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Check(pDX, IDC_CHECK3, m_Check3);
	DDX_Check(pDX, IDC_CHECK4, m_Check4);
	DDX_Check(pDX, IDC_CHECK5, m_Check5);
	DDX_Check(pDX, IDC_CHECK6, m_Check6);
	DDX_Check(pDX, IDC_CHECK7, m_Check7);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIK220TSTDlg, CDialog)
	//{{AFX_MSG_MAP(CIK220TSTDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RESET_RI, OnResetREF)
	ON_BN_CLICKED(IDC_START_RI, OnStartREF)
	ON_BN_CLICKED(IDC_STOP_RI, OnStopREF)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_CHECK0, OnCheck0)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_Setup, OnSetup)
	ON_BN_CLICKED(IDC_STATUS, OnStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIK220TSTDlg message handlers

BOOL CIK220TSTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_StatusOn   = FALSE;
	m_IK220found = TRUE;


	for (int Ax=0; Ax<8; Ax++) IKCard[Ax]=0;

	if (!IK220Find (IKCard))								// Get base address of cards
	{
		m_IK220found = FALSE;
		AfxMessageBox ("No IK 220 found", MB_ICONSTOP);

	}

/*	m_pInitMsg = new CINITMSG (NULL);
	m_pInitMsg->Create(IDD_INITMSG);

	m_pInitMsg->m_Progress.SetPos (0);
	m_pInitMsg->m_Progress.SetRange (0, 8);
*/



	for (Ax=0; Ax<8; Ax++)
	{
		m_Active[Ax]		= FALSE;
		OldSta[Ax]			= 0xFFFF;
		m_SignalPeriod[Ax]	= 0.020;						// Default signal period :  20µm
		m_EncoderType[Ax]	= 0;							// Default encoder type  :	incremental
		m_SignalType[Ax]	= 1;							// Default signal  type	 :  1Vss

	}


	
	for (Ax=0; Ax<8; Ax++)
	{
		if (IKCard[Ax]!=0)
		{
//			m_pInitMsg->m_Progress.SetPos (Ax+1);
			IK220Init (Ax);								// Initialize cards

			if ( !IK220WritePar (Ax, 1, m_EncoderType[Ax]) ) AfxMessageBox ("Error in function IK220WritePar", MB_ICONEXCLAMATION);
			if ( !IK220WritePar (Ax, 2, m_SignalType[Ax] ) ) AfxMessageBox ("Error in function IK220WritePar", MB_ICONEXCLAMATION);


			m_Active[Ax]=TRUE;
			switch (Ax)
			{
				case 0: m_Check0=TRUE; break;
				case 1: m_Check1=TRUE; break;
				case 2: m_Check2=TRUE; break;
				case 3: m_Check3=TRUE; break;
				case 4: m_Check4=TRUE; break;
				case 5: m_Check5=TRUE; break;
				case 6: m_Check6=TRUE; break;
				case 7: m_Check7=TRUE; break;
			}
		}
	}







//	m_pInitMsg->DestroyWindow();
//	delete (m_pInitMsg);

	TimID = SetTimer (1, 150, NULL);				// Start timer to display counter values

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CIK220TSTDlg::OnCancel() 
{
	KillTimer (TimID);
	CDialog::OnCancel();
}



void CIK220TSTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIK220TSTDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width()  - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIK220TSTDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


// Timer to display counter values.

void CIK220TSTDlg::OnTimer(UINT nIDEvent) 
{																 
	double	Wert;
	int		Ax;
	char	buffer[25];

	const char *format    = "%1.6f";
	const char *hexformat = "%04X";
	CString	Axis;
	ULONG	Stat;
	CString StatusTXT;

	USHORT EnStatus, EnAlarm;


	for (Ax=0; Ax<8; Ax++)
		if (IKCard[Ax]!=0) 
		{
  			if ( m_EncoderType[Ax]==0 ) 
			{
		   		if ( IK220Read48 (Ax, 0, &Wert) )					// Read incremental value
				{
					Wert = Wert*m_SignalPeriod[Ax];
					sprintf(buffer, format, Wert);
					Axis = buffer;
				if	(Wert>=0) Axis="+"+Axis;
				}																	  
				else Axis = "   Error  ";
			}
 

			if ( m_EncoderType[Ax]==1 ) 
			{
		   		if ( IK220ReadEn (Ax, &EnStatus, &Wert, &EnAlarm) )				// Read EnDat value
				{
					Wert = Wert*m_SignalPeriod[Ax];
					sprintf(buffer, format, Wert);
					Axis = buffer;
				if	(Wert>=0) Axis="+"+Axis;
				}																	  
				else Axis = "   Error  ";
			}



					
			switch (Ax)								// View value
			{
				case 0: m_Axis0 = Axis; break;
				case 1: m_Axis1 = Axis; break;
				case 2: m_Axis2 = Axis; break;
				case 3: m_Axis3 = Axis; break;
			    case 4: m_Axis4 = Axis; break;
			    case 5: m_Axis5 = Axis; break;
			    case 6: m_Axis6 = Axis; break;
				case 7: m_Axis7 = Axis; break;
			}
		}
		UpdateData(FALSE);			// Refresh display


		if (m_StatusOn && m_pModeless)
		{
			for (Ax=0; Ax<8; Ax++)
			{
				StatusTXT ="";
				if (IKCard[Ax]!=0) 
				{
		            if (IK220Status(Ax, &Stat))
					{
			            if (OldSta[Ax] != Stat)
						{
			                OldSta[Ax] = Stat;
                    
							sprintf(buffer, hexformat, Stat);
							StatusTXT=buffer;
							StatusTXT="0x"+StatusTXT+": ";

							if (Stat & 0x00000001) StatusTXT = StatusTXT + "Latch 0, ";
			                if (Stat & 0x00000002) StatusTXT = StatusTXT + "Latch 1, ";
			                if (Stat & 0x00000004) StatusTXT = StatusTXT + "Latch 2, ";
							if (Stat & 0x00000020) StatusTXT = StatusTXT + "started, "; else StatusTXT = StatusTXT + "stopped, ";
			                if (Stat & 0x00000040) StatusTXT = StatusTXT + "REF-Function active, ";
			                if (Stat & 0x00000080) StatusTXT = StatusTXT + "Frequency Error, ";
			                if (Stat & 0x00000100) StatusTXT = StatusTXT + "Amplitude Error, ";
                 
			                if ((Stat & 0x0000C000)==0x00000000) StatusTXT = StatusTXT + "No REF";
			                if ((Stat & 0x0000C000)==0x00004000) StatusTXT = StatusTXT + "Wait 1. REF";
			                if ((Stat & 0x0000C000)==0x00008000) StatusTXT = StatusTXT + "Wait 2. REF";
			                if ((Stat & 0x0000C000)==0x0000C000) StatusTXT = StatusTXT + "REF";
						}				
					}
					else
					{
						StatusTXT = "   --- Status Error ---";
					}
				}
				else
				{
					StatusTXT = "   --- No IK 220 installed ---";
				}

				if (StatusTXT!="")
				{
					switch (Ax)								// View value
					{
		    			case 0: m_pModeless->m_STEXT1 = StatusTXT; break;
					 	case 1: m_pModeless->m_STEXT2 = StatusTXT; break;
					 	case 2: m_pModeless->m_STEXT3 = StatusTXT; break;
					 	case 3: m_pModeless->m_STEXT4 = StatusTXT; break;
					    case 4: m_pModeless->m_STEXT5 = StatusTXT; break;
					    case 5: m_pModeless->m_STEXT6 = StatusTXT; break;
					    case 6: m_pModeless->m_STEXT7 = StatusTXT; break;
					 	case 7: m_pModeless->m_STEXT8 = StatusTXT; break;
					}
					m_pModeless->UpdateData(FALSE);
				}
			}
		}
	CDialog::OnTimer(nIDEvent);
}




void CIK220TSTDlg::OnReset() 
{
	for (int Ax=0; Ax<8; Ax++)
		if (IKCard[Ax]!=0) 
			if (m_Active[Ax]) IK220Reset (Ax);		// Reset counter
}

void CIK220TSTDlg::OnStart() 
{
	for (int Ax=0; Ax<8; Ax++)
		if (IKCard[Ax]!=0) 
			if (m_Active[Ax]) IK220Start (Ax);		// Start counter
}

void CIK220TSTDlg::OnStop() 
{
	for (int Ax=0; Ax<8; Ax++)
		if (IKCard[Ax]!=0) 
			if (m_Active[Ax]) IK220Stop (Ax);		// Stop counter
}

void CIK220TSTDlg::OnResetREF() 
{
	for (int Ax=0; Ax<8; Ax++)
		if (IKCard[Ax]!=0) 
			if (m_Active[Ax]) IK220ResetRef (Ax);		// Reset counter with reference mark
}


void CIK220TSTDlg::OnStartREF() 
{
	for (int Ax=0; Ax<8; Ax++)
		if (IKCard[Ax]!=0) 
			if (m_Active[Ax]) IK220StartRef (Ax);		// Start counter with reference mark
}



void CIK220TSTDlg::OnStopREF() 
{
	for (int Ax=0; Ax<8; Ax++)
		if (IKCard[Ax]!=0) 
			if (m_Active[Ax]) IK220StopRef (Ax);		// Stop counter with reference mark
}



void CIK220TSTDlg::OnCheck0() 
{
	if (IKCard[0]!=0)
	{
		m_Active[0] = !m_Active[0];
		m_Check0	= !m_Check0;
	}
	else m_Check0 = FALSE;
}

void CIK220TSTDlg::OnCheck1() 
{
	if (IKCard[1]!=0)
	{
		m_Active[1] = !m_Active[1];
		m_Check1	= !m_Check1;
	}
	else m_Check1 = FALSE;
}

void CIK220TSTDlg::OnCheck2() 
{
	if (IKCard[2]!=0)
	{
		m_Active[2] = !m_Active[2];
		m_Check2	= !m_Check2;
	}
	else m_Check2 = FALSE;
}		

void CIK220TSTDlg::OnCheck3() 
{
	if (IKCard[3]!=0)
	{
		m_Active[3] = !m_Active[3];
		m_Check3	= !m_Check3;
	}
	else m_Check3 = FALSE;
}

void CIK220TSTDlg::OnCheck4() 
{
	if (IKCard[4]!=0)
	{
		m_Active[4] = !m_Active;
		m_Check4	= !m_Check4;
	}
	else m_Check4 = FALSE;
}

void CIK220TSTDlg::OnCheck5() 
{
	if (IKCard[5]!=0)
	{
		m_Active[5] = !m_Active[5];
		m_Check5	= !m_Check5;
	}
	else m_Check5 = FALSE;
}

void CIK220TSTDlg::OnCheck6() 
{
	if (IKCard[6]!=0)
	{
		m_Active[6] = !m_Active[6];
		m_Check6	= !m_Check6;
	}
	else m_Check6 = FALSE;
}

void CIK220TSTDlg::OnCheck7() 
{
	if (IKCard[7]!=0)
	{
		m_Active[7] = !m_Active[7];
		m_Check7	= !m_Check7;
	}
	else m_Check7 = FALSE;
}



void CIK220TSTDlg::OnSetup() 
{
	int Ax;
	ULONG Para_1, Para_2;

	USHORT EnStatus, EnType, EnTurns, EnRefDist, EnCntDir;
	ULONG  EnPeriod, EnStep;


	CSetupDlg dlgSetup;



	if (!m_IK220found)
	{
		AfxMessageBox ("No IK 220 found", MB_ICONSTOP);
		return;
	}


	Ax=0;
	while ( ((IKCard[Ax]==0) || (!m_Active[Ax])) && (Ax<=8) ) Ax++;

	if (Ax<=8)
	{
		dlgSetup.m_Period = m_SignalPeriod[Ax];
		if (!IK220ReadPar (Ax, 1, &Para_1)) AfxMessageBox ("Error in function IK220ReadPar", MB_ICONEXCLAMATION);
		if (!IK220ReadPar (Ax, 2, &Para_2)) AfxMessageBox ("Error in function IK220ReadPar", MB_ICONEXCLAMATION);

		dlgSetup.m_Encoder = Para_1;
		dlgSetup.m_Signal  = Para_2;
		
		if (dlgSetup.DoModal() == IDOK)
		{
			Para_1 = dlgSetup.m_Encoder;
			Para_2 = dlgSetup.m_Signal;
			for (Ax=0; Ax<8; Ax++)
				if ( (IKCard[Ax]!=0) && (m_Active[Ax]) )
				{
					m_EncoderType[Ax]  = dlgSetup.m_Encoder;
					m_SignalType[Ax]   = dlgSetup.m_Signal;
					m_SignalPeriod[Ax] = dlgSetup.m_Period;

					if (!IK220WritePar (Ax, 1, Para_1)) AfxMessageBox ("Error in function IK220WritePar", MB_ICONEXCLAMATION);
					if (!IK220WritePar (Ax, 2, Para_2)) AfxMessageBox ("Error in function IK220WritePar", MB_ICONEXCLAMATION);

					if ( m_EncoderType[Ax]==1)
					{
						if ( !IK220ResetEn  (Ax, &EnStatus) ) AfxMessageBox ("Error in function IK220ResetEn", MB_ICONEXCLAMATION);
						if ( !IK220ConfigEn (Ax, &EnStatus, &EnType, &EnPeriod, &EnStep, &EnTurns, &EnRefDist, &EnCntDir) )
							AfxMessageBox ("Error in function IK220ConfigEn", MB_ICONEXCLAMATION);
					}
				}
		}
	}
	else
		AfxMessageBox ("No axis selected", MB_ICONSTOP);
}

void CIK220TSTDlg::OnStatus() 
{	
	for (int Ax=0; Ax<8; Ax++) OldSta[Ax]=0xFFFF; 
	m_pModeless->ShowWindow(SW_SHOWNORMAL);
	m_pModeless->BringWindowToTop();
	m_StatusOn = TRUE;
}



