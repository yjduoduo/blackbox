// PC_SimpleDlg.h : header file
//

#if !defined(AFX_PC_SIMPLEDLG_H__90FE2CC9_C6A0_4D14_A774_2E1261776E70__INCLUDED_)
#define AFX_PC_SIMPLEDLG_H__90FE2CC9_C6A0_4D14_A774_2E1261776E70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPC_SimpleDlg dialog

class CPC_SimpleDlg : public CDialog
{
// Construction
public:
	CPC_SimpleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPC_SimpleDlg)
	enum { IDD = IDD_PC_SIMPLE_DIALOG };
	CString	m_ReceiveByte;
	CString	m_SendByte;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPC_SimpleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPC_SimpleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PC_SIMPLEDLG_H__90FE2CC9_C6A0_4D14_A774_2E1261776E70__INCLUDED_)
