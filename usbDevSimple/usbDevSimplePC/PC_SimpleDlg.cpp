// PC_SimpleDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "PC_Simple.h"
#include "PC_SimpleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CPC_SimpleDlg dialog

CPC_SimpleDlg::CPC_SimpleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPC_SimpleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPC_SimpleDlg)
	m_ReceiveByte = _T("");
	m_SendByte = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPC_SimpleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPC_SimpleDlg)
	DDX_Text(pDX, IDC_RECEIVEBYTE, m_ReceiveByte);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPC_SimpleDlg, CDialog)
	//{{AFX_MSG_MAP(CPC_SimpleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPC_SimpleDlg message handlers

BOOL CPC_SimpleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPC_SimpleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPC_SimpleDlg::OnPaint() 
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
		int x = (rect.Width() - cxIcon + 1) / 2;
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
HCURSOR CPC_SimpleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/***********************************************************
** ��������: CString IntToASCII(unsigned char input)
** ��������: ����λ���޷�������ת��Ϊ�ַ���
** �䡡  ��:   unsigned char input : 16������
** ��    ��:   input  ���ַ�����ʽ  

  ** ��    �磺 0xFF - > "FF" 
************************************************************/
CString IntToASCII(unsigned char input)
{
	char temp[2];
	char exchange;
	CString output = "";		
	itoa(input,temp,16);		//�����λ����temp[0],��λ����temp[1]
	
	if (temp[1] == 0)	        //��Ϊ���inputΪ0x0F ʱ�����Ϊ 'F','0';	
	{						    //����Ҫ����һ��temp[1]��temp[0]
		exchange = temp[1];
		temp[1]  = temp[0];
		temp[0]  = exchange;
	}	
	
	if (temp[0] == 0)  output += "0";
	   else	           output += temp[0];
	   
	   if (temp[1] == 0)  output += "0";
	   else            output += temp[1];
	   
	   return output;
}

/*******************************************************************
 
    ����Ϊ zyUSB USB �豸������ Ӧ�ó��򲿷�

*********************************************************************/

// ��һ��: ����zyUSBDev.dll��̬��ͷ�ļ�
#include "zyUSBDev.h"

void CPC_SimpleDlg::OnSend() 
{
	unsigned char sendbuf[10] = {0x00,0x01,0x02,0x03,0x04,
								 0x05,0x06,0x07,0x08,0x09};	
											//���͵�����
	unsigned char recbuf[10];				//���ջ�����

	for (int i = 0; i < 10; i++)		
		recbuf[i] = 0;						//��ս��ջ�����

//�ڶ���:  ���÷������ݺ�������10���ֽ�
	int ret = zyUSB_WriteData(1,sendbuf,10,1000);	//ͨ���߼��˵�1��OUT�˿ڷ���10�ֽ�����
	if (ret != 10)							//����10���ֽ�
	{
		MessageBox("�������ݴ���!");	
		return;
	}

//������:  ���ý������ݺ�������10���ֽ�
	ret = zyUSB_ReadData(0,recbuf,10,1000);	//ͨ���߼��˵�1��IN�˿ڽ���10�ֽ�����
	if (ret != 10)
	{
		MessageBox("�������ݴ���!");
		return;
	}

	/*���տ�ؼ���Ӧ�ı���Ϊ�ַ�����*/
	m_ReceiveByte = "";						//��ʾ�յ����ֽ�
	for (i = 0; i < 10; i++)
		m_ReceiveByte += IntToASCII(recbuf[i]) + " ";

	UpdateData(FALSE);		//���¿ؼ��е�����
	
}
