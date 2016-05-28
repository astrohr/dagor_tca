// IK220TST.cpp : Defines the class behaviors for the application.
//

#include <winsvc.h>
#include "stdafx.h"
#include "IK220App.h"
#include "IK220AppDlg.h"
#include "StatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIK220TSTApp

BEGIN_MESSAGE_MAP(CIK220TSTApp, CWinApp)
	//{{AFX_MSG_MAP(CIK220TSTApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIK220TSTApp construction

CIK220TSTApp::CIK220TSTApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIK220TSTApp object

CIK220TSTApp theApp;
CStatusDlg* m_pModeless;
CWinApp*		m_pMain;
BOOL	m_StatusOn;
BOOL	m_IK220found;

/////////////////////////////////////////////////////////////////////////////
// CIK220TSTApp initialization

BOOL CIK220TSTApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	CIK220TSTDlg dlg;
	m_pMainWnd = &dlg;

	m_pModeless = new CStatusDlg (NULL);
	m_pModeless->Create();


	dlg.DoModal();				// Show dialog


	m_pModeless->DestroyWindow();
	delete (m_pModeless);


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}




