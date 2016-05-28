// INITMSG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CINITMSG dialog

class CINITMSG : public CDialog
{
// Construction
public:
	CINITMSG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CINITMSG)
	enum { IDD = IDD_INITMSG };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CINITMSG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CINITMSG)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
