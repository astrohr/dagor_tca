// INITMSG.cpp : implementation file
//

#include "stdafx.h"
#include "IK220app.h"
#include "INITMSG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CINITMSG dialog


CINITMSG::CINITMSG(CWnd* pParent /*=NULL*/)
	: CDialog(CINITMSG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CINITMSG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CINITMSG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CINITMSG)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CINITMSG, CDialog)
	//{{AFX_MSG_MAP(CINITMSG)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CINITMSG message handlers
