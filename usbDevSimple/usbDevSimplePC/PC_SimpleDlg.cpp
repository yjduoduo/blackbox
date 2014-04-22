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
** 函数名称: CString IntToASCII(unsigned char input)
** 功能描述: 将两位的无符号整数转换为字符串
** 输　  入:   unsigned char input : 16进制数
** 输    出:   input  的字符串形式  

  ** 例    如： 0xFF - > "FF" 
************************************************************/
CString IntToASCII(unsigned char input)
{
	char temp[2];
	char exchange;
	CString output = "";		
	itoa(input,temp,16);		//结果高位放于temp[0],低位放于temp[1]
	
	if (temp[1] == 0)	        //因为如果input为0x0F 时，结果为 'F','0';	
	{						    //所以要交换一下temp[1]与temp[0]
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
 
    下面为 zyUSB USB 设备控制器 应用程序部分

*********************************************************************/

// 第一步: 加入zyUSBDev.dll动态库头文件
#include "zyUSBDev.h"

void CPC_SimpleDlg::OnSend() 
{
	unsigned char sendbuf[10] = {0x00,0x01,0x02,0x03,0x04,
								 0x05,0x06,0x07,0x08,0x09};	
											//发送的数据
	unsigned char recbuf[10];				//接收缓冲区

	for (int i = 0; i < 10; i++)		
		recbuf[i] = 0;						//清空接收缓冲区

//第二步:  调用发送数据函数发送10个字节
	int ret = zyUSB_WriteData(1,sendbuf,10,1000);	//通过逻辑端点1的OUT端口发送10字节数据
	if (ret != 10)							//发送10个字节
	{
		MessageBox("发送数据错误!");	
		return;
	}

//第三步:  调用接收数据函数接收10个字节
	ret = zyUSB_ReadData(0,recbuf,10,1000);	//通过逻辑端点1的IN端口接收10字节数据
	if (ret != 10)
	{
		MessageBox("接收数据错误!");
		return;
	}

	/*接收框控件对应的变量为字符串型*/
	m_ReceiveByte = "";						//显示收到的字节
	for (i = 0; i < 10; i++)
		m_ReceiveByte += IntToASCII(recbuf[i]) + " ";

	UpdateData(FALSE);		//更新控件中的数据
	
}
