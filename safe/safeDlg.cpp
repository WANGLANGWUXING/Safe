
// safeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "safeDlg.h"
#include "afxdialogex.h"
#include "psapi.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsafeDlg �Ի���



CsafeDlg::CsafeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SAFE_DIALOG, pParent)
	, m_strCPU(_T(""))
	, m_strMemory(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsafeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabs);
	DDX_Text(pDX, IDC_STATIC_CPU, m_strCPU);
	DDX_Text(pDX, IDC_STATIC_MEMORY, m_strMemory);
	DDX_Control(pDX, IDC_STATIC_CPU, m_cpu);
	DDX_Control(pDX, IDC_STATIC_MEMORY, m_memory);
	DDX_Control(pDX, IDC_BUTTON1, m_changePrivilege);
}

BEGIN_MESSAGE_MAP(CsafeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MEMORYUP, &CsafeDlg::OnBnClickedBtnMemoryup)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CsafeDlg::OnBnClickedButton1)
	ON_WM_HOTKEY()
	ON_COMMAND(ID_32783, &CsafeDlg::OnShutdown)
	ON_COMMAND(ID_32784, &CsafeDlg::OnRestart)
	ON_COMMAND(ID_32785, &CsafeDlg::OnLockscreen)
	ON_COMMAND(ID_32786, &CsafeDlg::OnSleep1)
	ON_COMMAND(ID_32787, &CsafeDlg::OnLogout)
END_MESSAGE_MAP()

DWORD WINAPI MyHotKey(LPVOID lpThreadParameter)
{
	HWND nHwnd = (HWND)lpThreadParameter;
	while (true)
	{
		//F12
		if (GetKey(123))
		{
			//
			//IsWindowVisible���ָ�����ڵĿ���״̬
			ShowWindow(nHwnd, !IsWindowVisible(nHwnd));
			// SetForegroundWindow
			// ����������ָ�����ڵ��߳����õ�ǰ̨�����Ҽ���ô��ڡ�
			// ��������ת��ô��ڣ���Ϊ�û��ĸ��ֿ��ӵļǺš�
			// ϵͳ������ǰ̨���ڵ��̷߳����Ȩ���Ը��������߳�
			SetForegroundWindow(nHwnd);
			Sleep(300);
		}
		Sleep(10);
	}
}

// CsafeDlg ��Ϣ�������

BOOL CsafeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MyRtlAdjustPrivilege();
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	MyRtlAdjustPrivilege();
	m_menu.LoadMenu(IDR_MENU2);
	SetMenu(&m_menu);
	// ��Ȩ��ť��ʾ
	IsAdmin();
	m_tabs.MyInserItem(8, "�����б�", "������Ϣ","��������","ϵͳ����",
		"ɱ��","PE","VS����","�ļ���Ϣ");
	m_tabs.MyInsertChild(8, new CProcessDlg(), IDD_PROCESS_DIALOG,
		new CWinDlg(), IDD_WIN_DIALOG,
		new CCleanDlg(),IDD_CLEAN_DIALOG,
		new CServiceDlg(),IDD_SERVICE_DIALOG,
		new CKillVirusDlg(),IDD_KILLVIRUS_DIALOG,
		new CPEDlg(),IDD_PE_DIALOG,
		new CVSCleanDlg(),IDD_VSCLEAN_DIALOG,
		new CFileInfo(),IDD_FILEINFO_DIALOG
	);
	// ����״̬��
	UINT nSatusId[] = { STATUS_0};

	m_status.Create(this);
	m_status.SetIndicators(nSatusId, 1);
	m_status.SetIndicators(nSatusId, _countof(nSatusId));
	m_status.SetPaneInfo(0, STATUS_0, SBPS_NORMAL, 10000);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	m_status.SetPaneText(0, "�ϰ��:F12 | Ctrl+Shift+H");
	
	// �����ϰ��
	CreateThread(NULL, NULL, MyHotKey, (LPVOID)m_hWnd, NULL, NULL);
	RegisterHotKey(m_hWnd, m_HotKey, MOD_SHIFT | MOD_CONTROL, 'H');
	m_bClose = true;

	//�����߳�ˢ��CPUʹ���ʺ��ڴ�ռ����;
	hThread = AfxBeginThread(MyThreadFunction, this);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CsafeDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CsafeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT  CsafeDlg::MyThreadFunction(LPVOID lpParam)
{
	CsafeDlg*  TangDlg = (CsafeDlg*)lpParam;

	CString Buff1;
	CString Buff2;

	MEMORYSTATUS memStatus;
	memStatus.dwLength = sizeof(MEMORYSTATUS);

	while (TangDlg->m_bClose)
	{
		//CPU������
		Buff1.Format("������ %d %%", GetCpuUsage());
		TangDlg->m_cpu.SetWindowText(Buff1);

		//�ڴ�ռ����
		GlobalMemoryStatus(&memStatus);
		Buff2.Format("ռ���� %d %%", (int)memStatus.dwMemoryLoad);
		TangDlg->m_memory.SetWindowText(Buff2);
		Sleep(500);
	}

	return 0;
}

double CsafeDlg::FILETIME2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}
//����CPUռ����
int CsafeDlg::GetCpuUsage()
{
	//����ʱ��   �ں�ʱ��  �û�ʱ��
	_FILETIME idleTime, kernelTime, userTime;
	//��ȡʱ��
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//�ȴ�1000����ʹ���ں˶�������ȷ
	WaitForSingleObject(hEvent, 1000);
	//��ȡ�µ�ʱ��
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	//������ʱ��ת��
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);
	//�����ʹ����
	return int(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);
}


//�����ڴ�
void ClearMemory()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}
	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);
	int count = 0;
	//��ȡϵͳ�е�һ�����̵���Ϣ  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//���ý��̹�������С
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//�����ܶ�Ľ�ָ�����̵�ҳ��ӹ������Ƴ�
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//��ȡ��һ�����̵���Ϣ  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}


void CsafeDlg::OnBnClickedBtnMemoryup()
{
	//��ȡ����ǰ���ڴ���Ϣ
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//��ʼ�����ڴ�
	ClearMemory();
	//��ȡ�������ڴ���Ϣ
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//���������Ϣ
	CString m_szStatus;
	m_szStatus.Format(_T("�ڴ�������ϣ��������� : %.2f MB"), preUsedMem - afterUsedMem);

	MessageBox(m_szStatus, TEXT("��ʾ"), MB_ICONINFORMATION);

}



void CsafeDlg::OnClose()
{
	m_bClose = false;

	HANDLE MyProcess = GetCurrentProcess();
	TerminateProcess(MyProcess, 0);
	CDialogEx::OnClose();
}
// ��ʾС����
BOOL CsafeDlg::IsAdmin()
{
	// 1. ��ñ����̵�����
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return false;
	// 2. ��ȡ��������
	TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeDefault;
	BOOL                 bIsAdmin = false;
	DWORD                dwSize = 0;
	if (GetTokenInformation(hToken, TokenElevationType, &ElevationType,
		sizeof(TOKEN_ELEVATION_TYPE), &dwSize)) {
		// 2.1 ��������Ա��Ķ�ӦSID
		BYTE adminSID[SECURITY_MAX_SID_SIZE];
		dwSize = sizeof(adminSID);
		CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);
		// 2.2 �жϵ�ǰ���������û���ɫ�Ƿ�Ϊ����Ա
		if (ElevationType == TokenElevationTypeLimited) {
			// a. ��ȡ�������Ƶľ��
			HANDLE hUnfilteredToken = NULL;
			GetTokenInformation(hToken, TokenLinkedToken, (PVOID)&hUnfilteredToken,
				sizeof(HANDLE), &dwSize);
			// b. ������ԭʼ�������Ƿ��������Ա��SID
			if (!CheckTokenMembership(hUnfilteredToken, &adminSID, &bIsAdmin))
				return false;
			CloseHandle(hUnfilteredToken);
		}
		else {
			bIsAdmin = IsUserAnAdmin();
		}
		CloseHandle(hToken);
	}

	// 3. �жϾ����Ȩ��״��
	BOOL bFullToken = false;
	switch (ElevationType) {
	case TokenElevationTypeDefault: /* Ĭ�ϵ��û���UAC������ */
		if (IsUserAnAdmin())  bFullToken = true; // Ĭ���û��й���ԱȨ��
		else                  bFullToken = false;// Ĭ���û����ǹ���Ա��
		break;
	case TokenElevationTypeFull:    /* �Ѿ��ɹ���߽���Ȩ�� */
		if (IsUserAnAdmin())  bFullToken = true; //��ǰ�Թ���ԱȨ������
		else                  bFullToken = false;//��ǰδ�Թ���ԱȨ������
		break;
	case TokenElevationTypeLimited: /* �����������޵�Ȩ������ */
		if (bIsAdmin)  bFullToken = false;//�û��й���ԱȨ�ޣ�������Ȩ������
		else           bFullToken = false;//�û����ǹ���Ա�飬�ҽ���Ȩ������
	}
	// 4. ����Ȩ�޵Ĳ�ͬ���ư�ť����ʾ
	if (!bFullToken)
		Button_SetElevationRequiredState(m_changePrivilege.m_hWnd,
			!bFullToken);
	else
		::ShowWindow(m_changePrivilege.m_hWnd, SW_HIDE);

	return bIsAdmin;
}


void CsafeDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// 1. ���ص�ǰ����
	ShowWindow(SW_HIDE);
	// 2. ��ȡ��ǰ����·��
	CHAR szApplication[MAX_PATH] = { 0 };
	DWORD cchLength = _countof(szApplication);
	QueryFullProcessImageName(GetCurrentProcess(), 0,
		szApplication, &cchLength);
	// 3. �Թ���ԱȨ�����´򿪽���
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = "runas";      // ��������Ȩ��
	sei.lpFile = szApplication; // ��ִ���ļ�·��
	sei.lpParameters = NULL;          // ����Ҫ����
	sei.nShow = SW_SHOWNORMAL; // ������ʾ����
	if (ShellExecuteEx(&sei))
		exit(0);
	else
		ShowWindow(SW_SHOWNORMAL);
}


void CsafeDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);

	if (nHotKeyId == m_HotKey && nKey1 == (MOD_SHIFT | MOD_CONTROL) && nKey2 == 'H')
	{
		ShowWindow(!IsWindowVisible());
		SetForegroundWindow();
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

// �ػ�
void CsafeDlg::OnShutdown()
{
	
	if (MessageBox("ȷ��Ҫ�ػ���\r\n�����ȷ�����ػ�\r\n�����ȡ��������",
		"�ػ�", MB_OKCANCEL) == IDCANCEL)
		return;
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
}

// ����
void CsafeDlg::OnRestart()
{
	if (MessageBox("ȷ��Ҫ������\r\n�����ȷ��������\r\n�����ȡ��������",
		"����", MB_OKCANCEL) == IDCANCEL)
		return;
	ExitWindowsEx(EWX_REBOOT , 0);
}

// ����
void CsafeDlg::OnLockscreen()
{
	if (MessageBox("ȷ��Ҫ������\r\n�����ȷ��������\r\n�����ȡ��������",
		"����", MB_OKCANCEL) == IDCANCEL)
		return;
	LockWorkStation();
}

// ����
void CsafeDlg::OnSleep1()
{
	if (MessageBox("ȷ��Ҫ������\r\n�����ȷ��������\r\n�����ȡ��������",
		"����", MB_OKCANCEL) == IDCANCEL)
		return;
	SetSystemPowerState(FALSE, TRUE);
}

// ע��
void CsafeDlg::OnLogout()
{
	if (MessageBox("ȷ��Ҫע����\r\n�����ȷ����ע��\r\n�����ȡ��������",
		"ע��", MB_OKCANCEL) == IDCANCEL)
		return;
	ExitWindowsEx(EWX_LOGOFF , 0);
}



