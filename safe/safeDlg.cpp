
// safeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "safeDlg.h"
#include "afxdialogex.h"
#include "psapi.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsafeDlg 对话框



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
			//IsWindowVisible获得指定窗口的可视状态
			ShowWindow(nHwnd, !IsWindowVisible(nHwnd));
			// SetForegroundWindow
			// 函数将创建指定窗口的线程设置到前台，并且激活该窗口。
			// 键盘输入转向该窗口，并为用户改各种可视的记号。
			// 系统给创建前台窗口的线程分配的权限稍高于其他线程
			SetForegroundWindow(nHwnd);
			Sleep(300);
		}
		Sleep(10);
	}
}

// CsafeDlg 消息处理程序

BOOL CsafeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MyRtlAdjustPrivilege();
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	// TODO: 在此添加额外的初始化代码
	MyRtlAdjustPrivilege();
	m_menu.LoadMenu(IDR_MENU2);
	SetMenu(&m_menu);
	// 提权按钮显示
	IsAdmin();
	m_tabs.MyInserItem(8, "进程列表", "窗口信息","垃圾清理","系统服务",
		"杀毒","PE","VS清理","文件信息");
	m_tabs.MyInsertChild(8, new CProcessDlg(), IDD_PROCESS_DIALOG,
		new CWinDlg(), IDD_WIN_DIALOG,
		new CCleanDlg(),IDD_CLEAN_DIALOG,
		new CServiceDlg(),IDD_SERVICE_DIALOG,
		new CKillVirusDlg(),IDD_KILLVIRUS_DIALOG,
		new CPEDlg(),IDD_PE_DIALOG,
		new CVSCleanDlg(),IDD_VSCLEAN_DIALOG,
		new CFileInfo(),IDD_FILEINFO_DIALOG
	);
	// 设置状态栏
	UINT nSatusId[] = { STATUS_0};

	m_status.Create(this);
	m_status.SetIndicators(nSatusId, 1);
	m_status.SetIndicators(nSatusId, _countof(nSatusId));
	m_status.SetPaneInfo(0, STATUS_0, SBPS_NORMAL, 10000);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	m_status.SetPaneText(0, "老板键:F12 | Ctrl+Shift+H");
	
	// 设置老板键
	CreateThread(NULL, NULL, MyHotKey, (LPVOID)m_hWnd, NULL, NULL);
	RegisterHotKey(m_hWnd, m_HotKey, MOD_SHIFT | MOD_CONTROL, 'H');
	m_bClose = true;

	//创建线程刷新CPU使用率和内存占用率;
	hThread = AfxBeginThread(MyThreadFunction, this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsafeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
		//CPU利用率
		Buff1.Format("利用率 %d %%", GetCpuUsage());
		TangDlg->m_cpu.SetWindowText(Buff1);

		//内存占用率
		GlobalMemoryStatus(&memStatus);
		Buff2.Format("占用率 %d %%", (int)memStatus.dwMemoryLoad);
		TangDlg->m_memory.SetWindowText(Buff2);
		Sleep(500);
	}

	return 0;
}

double CsafeDlg::FILETIME2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}
//计算CPU占用率
int CsafeDlg::GetCpuUsage()
{
	//空闲时间   内核时间  用户时间
	_FILETIME idleTime, kernelTime, userTime;
	//获取时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//等待1000毫秒使用内核对象会更精确
	WaitForSingleObject(hEvent, 1000);
	//获取新的时间
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	//将各个时间转换
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);
	//计算出使用率
	return int(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);
}


//清理内存
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
	//获取系统中第一个进程的信息  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//设置进程工作区大小
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//尽可能多的将指定进程的页面从工作区移除
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//获取下一个进程的信息  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}


void CsafeDlg::OnBnClickedBtnMemoryup()
{
	//获取清理前的内存信息
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//开始清理内存
	ClearMemory();
	//获取清理后的内存信息
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//输出清理信息
	CString m_szStatus;
	m_szStatus.Format(_T("内存清理完毕！本次清理 : %.2f MB"), preUsedMem - afterUsedMem);

	MessageBox(m_szStatus, TEXT("提示"), MB_ICONINFORMATION);

}



void CsafeDlg::OnClose()
{
	m_bClose = false;

	HANDLE MyProcess = GetCurrentProcess();
	TerminateProcess(MyProcess, 0);
	CDialogEx::OnClose();
}
// 显示小盾牌
BOOL CsafeDlg::IsAdmin()
{
	// 1. 获得本进程的令牌
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return false;
	// 2. 获取提升类型
	TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeDefault;
	BOOL                 bIsAdmin = false;
	DWORD                dwSize = 0;
	if (GetTokenInformation(hToken, TokenElevationType, &ElevationType,
		sizeof(TOKEN_ELEVATION_TYPE), &dwSize)) {
		// 2.1 创建管理员组的对应SID
		BYTE adminSID[SECURITY_MAX_SID_SIZE];
		dwSize = sizeof(adminSID);
		CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);
		// 2.2 判断当前进程运行用户角色是否为管理员
		if (ElevationType == TokenElevationTypeLimited) {
			// a. 获取连接令牌的句柄
			HANDLE hUnfilteredToken = NULL;
			GetTokenInformation(hToken, TokenLinkedToken, (PVOID)&hUnfilteredToken,
				sizeof(HANDLE), &dwSize);
			// b. 检查这个原始的令牌是否包含管理员的SID
			if (!CheckTokenMembership(hUnfilteredToken, &adminSID, &bIsAdmin))
				return false;
			CloseHandle(hUnfilteredToken);
		}
		else {
			bIsAdmin = IsUserAnAdmin();
		}
		CloseHandle(hToken);
	}

	// 3. 判断具体的权限状况
	BOOL bFullToken = false;
	switch (ElevationType) {
	case TokenElevationTypeDefault: /* 默认的用户或UAC被禁用 */
		if (IsUserAnAdmin())  bFullToken = true; // 默认用户有管理员权限
		else                  bFullToken = false;// 默认用户不是管理员组
		break;
	case TokenElevationTypeFull:    /* 已经成功提高进程权限 */
		if (IsUserAnAdmin())  bFullToken = true; //当前以管理员权限运行
		else                  bFullToken = false;//当前未以管理员权限运行
		break;
	case TokenElevationTypeLimited: /* 进程在以有限的权限运行 */
		if (bIsAdmin)  bFullToken = false;//用户有管理员权限，但进程权限有限
		else           bFullToken = false;//用户不是管理员组，且进程权限有限
	}
	// 4. 根据权限的不同控制按钮的显示
	if (!bFullToken)
		Button_SetElevationRequiredState(m_changePrivilege.m_hWnd,
			!bFullToken);
	else
		::ShowWindow(m_changePrivilege.m_hWnd, SW_HIDE);

	return bIsAdmin;
}


void CsafeDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	// 1. 隐藏当前窗口
	ShowWindow(SW_HIDE);
	// 2. 获取当前程序路径
	CHAR szApplication[MAX_PATH] = { 0 };
	DWORD cchLength = _countof(szApplication);
	QueryFullProcessImageName(GetCurrentProcess(), 0,
		szApplication, &cchLength);
	// 3. 以管理员权限重新打开进程
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = "runas";      // 请求提升权限
	sei.lpFile = szApplication; // 可执行文件路径
	sei.lpParameters = NULL;          // 不需要参数
	sei.nShow = SW_SHOWNORMAL; // 正常显示窗口
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

// 关机
void CsafeDlg::OnShutdown()
{
	
	if (MessageBox("确定要关机吗？\r\n点击【确定】关机\r\n点击【取消】继续",
		"关机", MB_OKCANCEL) == IDCANCEL)
		return;
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
}

// 重启
void CsafeDlg::OnRestart()
{
	if (MessageBox("确定要重启吗？\r\n点击【确定】重启\r\n点击【取消】继续",
		"重启", MB_OKCANCEL) == IDCANCEL)
		return;
	ExitWindowsEx(EWX_REBOOT , 0);
}

// 锁屏
void CsafeDlg::OnLockscreen()
{
	if (MessageBox("确定要锁屏吗？\r\n点击【确定】锁屏\r\n点击【取消】继续",
		"锁屏", MB_OKCANCEL) == IDCANCEL)
		return;
	LockWorkStation();
}

// 休眠
void CsafeDlg::OnSleep1()
{
	if (MessageBox("确定要休眠吗？\r\n点击【确定】休眠\r\n点击【取消】继续",
		"休眠", MB_OKCANCEL) == IDCANCEL)
		return;
	SetSystemPowerState(FALSE, TRUE);
}

// 注销
void CsafeDlg::OnLogout()
{
	if (MessageBox("确定要注销吗？\r\n点击【确定】注销\r\n点击【取消】继续",
		"注销", MB_OKCANCEL) == IDCANCEL)
		return;
	ExitWindowsEx(EWX_LOGOFF , 0);
}



