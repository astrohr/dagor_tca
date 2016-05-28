// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IK220App.h"
#include "SetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog


CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupDlg)
	m_Period = 0.0;
	m_Encoder = -1;
	m_Signal = -1;
	//}}AFX_DATA_INIT
}


void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDlg)
	DDX_Text(pDX, IDC_PERIOD, m_Period);
	DDV_MinMaxDouble(pDX, m_Period, 1.e-003, 10000.);
	DDX_Radio(pDX, IDC_ENCODER, m_Encoder);
	DDX_Radio(pDX, IDC_SIGNAL, m_Signal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers
