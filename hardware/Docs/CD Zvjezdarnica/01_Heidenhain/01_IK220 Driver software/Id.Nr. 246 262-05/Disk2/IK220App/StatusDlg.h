// StatusDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatusDlg dialog

class CStatusDlg : public CDialog
{
// Construction
public:
	CStatusDlg(CWnd* pParent);   // standard constructor
	BOOL Create();

// Dialog Data
	//{{AFX_DATA(CStatusDlg)
	enum { IDD = IDD_STATUS };
	CString	m_STEXT1;
	CString	m_STEXT2;
	CString	m_STEXT3;
	CString	m_STEXT4;
	CString	m_STEXT5;
	CString	m_STEXT6;
	CString	m_STEXT7;
	CString	m_STEXT8;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	int	  m_nID;

	// Generated message map functions
	//{{AFX_MSG(CStatusDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
