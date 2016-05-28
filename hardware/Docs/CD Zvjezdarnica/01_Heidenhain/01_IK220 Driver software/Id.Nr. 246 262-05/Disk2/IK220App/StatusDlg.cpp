// StatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IK220app.h"
#include "IK220AppDlg.h"
#include "StatusDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CWinApp*	m_pMain;
extern BOOL		m_StatusOn;
extern BOOL		m_IK220found;

/////////////////////////////////////////////////////////////////////////////
// CStatusDlg dialog


CStatusDlg::CStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatusDlg)
	m_STEXT1 = _T("");
	m_STEXT2 = _T("");
	m_STEXT3 = _T("");
	m_STEXT4 = _T("");
	m_STEXT5 = _T("");
	m_STEXT6 = _T("");
	m_STEXT7 = _T("");
	m_STEXT8 = _T("");
	//}}AFX_DATA_INIT

	ASSERT (m_pParent != NULL);
	m_pParent = pParent;
	m_nID     = CStatusDlg::IDD;
}


void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusDlg)
	DDX_Text(pDX, IDC_STEXT1, m_STEXT1);
	DDX_Text(pDX, IDC_STEXT2, m_STEXT2);
	DDX_Text(pDX, IDC_STEXT3, m_STEXT3);
	DDX_Text(pDX, IDC_STEXT4, m_STEXT4);
	DDX_Text(pDX, IDC_STEXT5, m_STEXT5);
	DDX_Text(pDX, IDC_STEXT6, m_STEXT6);
	DDX_Text(pDX, IDC_STEXT7, m_STEXT7);
	DDX_Text(pDX, IDC_STEXT8, m_STEXT8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CStatusDlg message handlers

BOOL CStatusDlg::Create() 
{
	return CDialog::Create(m_nID, (CIK220TSTDlg*)m_pMain);
}


void CStatusDlg::OnOK() 
{
	m_StatusOn  = FALSE;
	ShowWindow (SW_HIDE);
}


