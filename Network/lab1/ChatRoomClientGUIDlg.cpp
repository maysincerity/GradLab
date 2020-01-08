// ChatRoomClientGUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatRoomClientGUI.h"
#include "ChatRoomClientGUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CChatRoomClientGUIDlg �Ի���




CChatRoomClientGUIDlg::CChatRoomClientGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatRoomClientGUIDlg::IDD, pParent)
	, m_Port(8080)
	, addr("D:/")//Ĭ��D��
	, m_SendMsg("")
	, m_Socket(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Socket = new CSessionSocket(); //�����Ựsocket����
	this->m_thread == NULL;
	this->m_server = NULL;
}

CChatRoomClientGUIDlg::~CChatRoomClientGUIDlg(){
	if(this->m_Socket != NULL){
		this->m_Socket->Close();
		delete this->m_Socket;
	}
}

void CChatRoomClientGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPCtrl);
	DDX_Text(pDX, IDC_PORT, m_Port);
	DDX_Control(pDX, IDC_MSGLIST, m_List);
	DDX_Text(pDX, IDC_SENDMSG, m_SendMsg);
}

BEGIN_MESSAGE_MAP(CChatRoomClientGUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SEND, &CChatRoomClientGUIDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_CONN, &CChatRoomClientGUIDlg::OnBnClickedConn)
	ON_BN_CLICKED(IDC_QUIT, &CChatRoomClientGUIDlg::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatRoomClientGUIDlg::OnBnClickedClose_s)
END_MESSAGE_MAP()


// CChatRoomClientGUIDlg ��Ϣ�������

BOOL CChatRoomClientGUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// �����׽���
	this->m_Socket->Create(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChatRoomClientGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatRoomClientGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChatRoomClientGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// ������Ŀ¼
void CChatRoomClientGUIDlg::OnBnClickedSend()
{
	UpdateData(TRUE); //�õ��û�������༭����������ַ���

	this->addr = this->m_SendMsg;//����Ŀ¼

	//��ձ༭��
	this->m_SendMsg = "";
	UpdateData(FALSE);
}

//���ӷ�����
void CChatRoomClientGUIDlg::OnBnClickedConn()
{
	//�õ��û������IP��ַ
	BYTE f0, f1, f2, f3;
	CIPAddressCtrl *ipCtrl = (CIPAddressCtrl *)this->GetDlgItem(IDC_IPADDRESS);
	ipCtrl->GetAddress(f0, f1, f2, f3);
	CString ip;
	ip.Format(_T("%d.%d.%d.%d"), f0, f1, f2, f3);

	//�õ��û�����Ķ˿ں�
	UpdateData(TRUE);

	//����������
	string IP(ip);
	this->m_server = new Server(IP, this->m_Port,this->addr);
	HANDLE ser_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(Server_Startup), (LPVOID)(m_server), 0, 0);
	this->m_thread = ser_thread;
}

//����������
void Server_Startup(Server *server) {
	if (server->WinsockStartup() == -1)
		return;
	if (server->ServerStartup() == -1)
		return;
	if (server->ListenStartup() == -1)
		return;
	string msg;
	msg = "Server is open. IP:" + server->ip + ", Port:" + to_string(server->port) + ", Main Catagory:" + server->addr;
	CChatRoomClientGUIDlg *dlg = (CChatRoomClientGUIDlg *)(AfxGetApp()->GetMainWnd());
	dlg->m_List.AddString(CString(msg.c_str()));//������Ϣ
	if (server->Loop() == -1)
		return;
}


void CChatRoomClientGUIDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//����ʲô����������ֹ�û���Enter���رմ���
}

void CChatRoomClientGUIDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//����ʲô����������ֹ�û���Esc���رմ���

}

void CChatRoomClientGUIDlg::OnBnClickedQuit()
{
	this->EndDialog(TRUE);//�رնԻ���
}

void CChatRoomClientGUIDlg::OnBnClickedClose_s()
{
	if (this->m_thread)
	{
		TerminateThread(this->m_thread, 0);//��ֹ�߳�
		string msg("Server is closed.");
		CChatRoomClientGUIDlg *dlg = (CChatRoomClientGUIDlg *)(AfxGetApp()->GetMainWnd());
		dlg->m_List.AddString(CString(msg.c_str()));//������Ϣ
		this->m_thread = NULL;
		delete this->m_server;//ɾ��������
		this->m_server = NULL;
		return;
	}
}

//����PreTranslateMessage��ʹ�� ��Ϣ�༭���ܴ���س��������û��������ݰ��س����ͷ�����Ϣ
BOOL CChatRoomClientGUIDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_RETURN){
		HWND hWnd = ::GetFocus();
		if(IDC_SENDMSG == ::GetDlgCtrlID(hWnd)){
			this->OnBnClickedSend();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
