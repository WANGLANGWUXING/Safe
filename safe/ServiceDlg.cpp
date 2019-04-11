// ServiceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "ServiceDlg.h"
#include "afxdialogex.h"


// CServiceDlg 对话框

IMPLEMENT_DYNAMIC(CServiceDlg, CDialogEx)

CServiceDlg::CServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVICE_DIALOG, pParent)
{

}

CServiceDlg::~CServiceDlg()
{
}

void CServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listServices);
}


BEGIN_MESSAGE_MAP(CServiceDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CServiceDlg::OnNMRClickList1)
	ON_WM_TIMER()
	ON_COMMAND(ID_32773, &CServiceDlg::OnStartService)
	ON_COMMAND(ID_32774, &CServiceDlg::OnCloseService)
END_MESSAGE_MAP()


// CServiceDlg 消息处理程序


BOOL CServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MyRtlAdjustPrivilege();
	m_listServices.AddColumn(5, "名称",150, "描述",80, "状态",80, "启动类型",80, "服务类型",100);
	SetTimer(1, 1000, NULL);
	GetServicesInfo();
	return TRUE;  
}


// 获取系统服务信息
void CServiceDlg::GetServicesInfo()
{
	m_listServices.DeleteAllItems();
	m_vecSerInfo.clear();
	//1.打开远程计算机服务控制管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		NULL,				//缓冲区
		0,					//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//3.申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS lpEnumService =
		(LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.第二次枚举
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		lpEnumService,		//缓冲区
		dwSize,				//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//5.遍历信息
	SC_HANDLE hService = nullptr;
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//获取基础信息
		//1.服务名称
		//2.服务描述
		//3.服务状态(根据得到的值手动输出字符串)
		//“已停止” “正在运行" "正在暂停"...
		//4.启动类型
		//5.服务类型
		m_vecSerInfo.push_back(lpEnumService[i]);

		//获取更多信息
		//1.打开服务
		hService = OpenService(hSCM,
			lpEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		// 2.第一次调用获取需要的缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig =
			(LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, dwSize);
		//3.第二次调用,获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//状态
		CString currentState;
		DWORD dwCurrentState = m_vecSerInfo[i].ServiceStatus.dwCurrentState;
		switch (dwCurrentState)
		{
		case SERVICE_STOPPED:
			currentState = "已停止";
			break;
		case SERVICE_STOP_PENDING:
			currentState = "正在停止";
			break;
		case SERVICE_START_PENDING:
			currentState = "正在启动";
			break;
		case SERVICE_RUNNING:
			currentState = "正在运行";
			break;
		case SERVICE_PAUSED:
			currentState = "已暂停";
			break;
		case SERVICE_PAUSE_PENDING:
			currentState = "正在暂停";
			break;
		case SERVICE_CONTINUE_PENDING:
			currentState = "准备继续";
			break;
		}
		//启动类型
		CString starType;
		DWORD dwStartType = pServiceConfig->dwStartType;
		switch (dwStartType)
		{
		case SERVICE_AUTO_START:
			starType = "自动";
			break;
		case SERVICE_BOOT_START:
			starType = "SERVICE_BOOT_START";
			break;
		case SERVICE_DEMAND_START:
			starType = "手动";
			break;
		case SERVICE_DISABLED:
			starType = "禁用";
			break;
		case SERVICE_SYSTEM_START:
			starType = "SERVICE_SYSTEM_START";
			break;
		}
		//服务类型
		CString serviceType;
		DWORD dwServiceType = m_vecSerInfo[i].ServiceStatus.dwServiceType;
		switch (dwServiceType)
		{
		case SERVICE_FILE_SYSTEM_DRIVER:
			serviceType = "文件系统驱动服务";
			break;
		case SERVICE_KERNEL_DRIVER:
			serviceType = "驱动服务";
			break;
		case SERVICE_WIN32_OWN_PROCESS:
			serviceType = "独立进程服务";
			break;
		case SERVICE_WIN32_SHARE_PROCESS:
			serviceType = "共享进程服务";
			break;
		}
		//获取服务信息完毕，开始插入List
		m_listServices.AddItem(5,
			m_vecSerInfo[i].lpServiceName,
			m_vecSerInfo[i].lpDisplayName,
			currentState,
			starType,
			serviceType);

		LocalFree(pServiceConfig);
	}
	LocalFree(lpEnumService);
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}

// 更新服务信息
void CServiceDlg::UpdateServiceInfo()
{
	//1.打开远程计算机服务控制管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		NULL,				//缓冲区
		0,					//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//3.申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS lpEnumService =
		(LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.第二次枚举
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		lpEnumService,		//缓冲区
		dwSize,				//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//5.遍历信息

	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//如果是要更新的服务

		if (!_tcscmp(lpEnumService[i].lpServiceName, m_strServiceName))
		{
			CString state;
			//更新list中该服务的状态信息
			switch (lpEnumService[i].ServiceStatus.dwCurrentState)
			{
			case SERVICE_STOPPED:

				state="已停止";
				break;
			case SERVICE_STOP_PENDING:
				state = "正在停止";
				break;
			case SERVICE_START_PENDING:
				state = "正在启动";
				break;
			case SERVICE_RUNNING:
				state = "正在运行";
				break;
			case SERVICE_PAUSED:
				state = "已暂停";
				break;
			case SERVICE_PAUSE_PENDING:
				state = "正在暂停";
				break;
			case SERVICE_CONTINUE_PENDING:
				state = "准备继续";
				
				break;
			}

			m_listServices.SetItemText(m_nSelectNum, 2, state);
			break;
		}
	}
	LocalFree(lpEnumService);
	CloseServiceHandle(hSCM);
}
// 列表右键
void CServiceDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int n = pNMItemActivate->iItem;//选择了第几行
	if (n != -1)
	{
		//保存所选服务的 服务名
		m_strServiceName = m_listServices.GetItemText(n, 0);
		m_nSelectNum = n;
		//下面的这段代码, 不单单适应于ListCtrl  
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_MENU1);
		pPopup = menu.GetSubMenu(1);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}

	*pResult = 0;
}

// 计时器
void CServiceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent&&m_nUpDataTimes != 0 && m_bIsUpDateSer)
	{
		UpdateServiceInfo();
		if (m_nUpDataTimes == 1)
		{
			m_bIsUpDateSer = FALSE;
		}
		m_nUpDataTimes--;
	}
	CDialogEx::OnTimer(nIDEvent);
}

// 启动服务
void CServiceDlg::OnStartService()
{
	//启动服务
	SC_HANDLE hSCM = OpenSCManager(			//打开服务控制管理器
		NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM,	//打开服务
		m_strServiceName, SERVICE_START);
	StartService(hService, 0, 0);			//启动服务
											//更新信息
	UpdateServiceInfo();
	m_bIsUpDateSer = TRUE;
	m_nUpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}

// 关闭服务
void CServiceDlg::OnCloseService()
{
	//停止服务
	SC_HANDLE hSCM = OpenSCManager(			//打开服务控制管理器
		NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM,	//打开服务
		m_strServiceName, SERVICE_STOP);
	SERVICE_STATUS status;
	ControlService(hService,				//结束服务
		SERVICE_CONTROL_STOP,
		&status);
	//更新信息
	UpdateServiceInfo();
	m_bIsUpDateSer = TRUE;
	m_nUpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}
