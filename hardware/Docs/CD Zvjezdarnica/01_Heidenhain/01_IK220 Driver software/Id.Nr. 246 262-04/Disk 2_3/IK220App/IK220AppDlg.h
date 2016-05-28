// IK220TSTDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIK220TSTDlg dialog

#include "INITMSG.H"


class CIK220TSTDlg : public CDialog
{
// Construction
public:
	CINITMSG* m_pInitMsg;
	CIK220TSTDlg(CWnd* pParent = NULL);	// standard constructor

	ULONG	OldSta[8];
	double	m_SignalPeriod[8];
	USHORT  m_SignalType[8];
	USHORT  m_EncoderType[8];

// Dialog Data
	//{{AFX_DATA(CIK220TSTDlg)
	enum { IDD = IDD_IK220TST_DIALOG };
	CString	m_Axis0;
	CString	m_Axis1;
	CString	m_Axis2;
	CString	m_Axis3;
	CString	m_Axis4;
	CString	m_Axis5;
	CString	m_Axis6;									
	CString	m_Axis7;
	BOOL	m_Check0;
	BOOL	m_Check1;
	BOOL	m_Check2;
	BOOL	m_Check3;
	BOOL	m_Check4;
	BOOL	m_Check5;
	BOOL	m_Check6;
	BOOL	m_Check7;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIK220TSTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON	m_hIcon;

	BOOL	m_Active[16];
	UINT	TimID;
	ULONG	IKCard[16];

	// Generated message map functions
	//{{AFX_MSG(CIK220TSTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnResetREF();
	afx_msg void OnStartREF();
	afx_msg void OnStopREF();
	afx_msg void OnReset();
	afx_msg void OnStart();
	afx_msg void OnStop();
	virtual void OnCancel();
	afx_msg void OnCheck0();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnSetup();
	afx_msg void OnStatus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
