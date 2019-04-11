// ServiceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "ServiceDlg.h"
#include "afxdialogex.h"


// CServiceDlg �Ի���

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


// CServiceDlg ��Ϣ�������


BOOL CServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MyRtlAdjustPrivilege();
	m_listServices.AddColumn(5, "����",150, "����",80, "״̬",80, "��������",80, "��������",100);
	SetTimer(1, 1000, NULL);
	GetServicesInfo();
	return TRUE;  
}


// ��ȡϵͳ������Ϣ
void CServiceDlg::GetServicesInfo()
{
	m_listServices.DeleteAllItems();
	m_vecSerInfo.clear();
	//1.��Զ�̼����������ƹ�����
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.��һ�ε��ã���ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		NULL,				//������
		0,					//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL);
	//3.������Ҫ���ڴ棬�ڶ��ε���
	LPENUM_SERVICE_STATUS lpEnumService =
		(LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.�ڶ���ö��
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		lpEnumService,		//������
		dwSize,				//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL);
	//5.������Ϣ
	SC_HANDLE hService = nullptr;
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//��ȡ������Ϣ
		//1.��������
		//2.��������
		//3.����״̬(���ݵõ���ֵ�ֶ�����ַ���)
		//����ֹͣ�� ����������" "������ͣ"...
		//4.��������
		//5.��������
		m_vecSerInfo.push_back(lpEnumService[i]);

		//��ȡ������Ϣ
		//1.�򿪷���
		hService = OpenService(hSCM,
			lpEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		// 2.��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig =
			(LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, dwSize);
		//3.�ڶ��ε���,��ȡ��Ϣ
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//״̬
		CString currentState;
		DWORD dwCurrentState = m_vecSerInfo[i].ServiceStatus.dwCurrentState;
		switch (dwCurrentState)
		{
		case SERVICE_STOPPED:
			currentState = "��ֹͣ";
			break;
		case SERVICE_STOP_PENDING:
			currentState = "����ֹͣ";
			break;
		case SERVICE_START_PENDING:
			currentState = "��������";
			break;
		case SERVICE_RUNNING:
			currentState = "��������";
			break;
		case SERVICE_PAUSED:
			currentState = "����ͣ";
			break;
		case SERVICE_PAUSE_PENDING:
			currentState = "������ͣ";
			break;
		case SERVICE_CONTINUE_PENDING:
			currentState = "׼������";
			break;
		}
		//��������
		CString starType;
		DWORD dwStartType = pServiceConfig->dwStartType;
		switch (dwStartType)
		{
		case SERVICE_AUTO_START:
			starType = "�Զ�";
			break;
		case SERVICE_BOOT_START:
			starType = "SERVICE_BOOT_START";
			break;
		case SERVICE_DEMAND_START:
			starType = "�ֶ�";
			break;
		case SERVICE_DISABLED:
			starType = "����";
			break;
		case SERVICE_SYSTEM_START:
			starType = "SERVICE_SYSTEM_START";
			break;
		}
		//��������
		CString serviceType;
		DWORD dwServiceType = m_vecSerInfo[i].ServiceStatus.dwServiceType;
		switch (dwServiceType)
		{
		case SERVICE_FILE_SYSTEM_DRIVER:
			serviceType = "�ļ�ϵͳ��������";
			break;
		case SERVICE_KERNEL_DRIVER:
			serviceType = "��������";
			break;
		case SERVICE_WIN32_OWN_PROCESS:
			serviceType = "�������̷���";
			break;
		case SERVICE_WIN32_SHARE_PROCESS:
			serviceType = "������̷���";
			break;
		}
		//��ȡ������Ϣ��ϣ���ʼ����List
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

// ���·�����Ϣ
void CServiceDlg::UpdateServiceInfo()
{
	//1.��Զ�̼����������ƹ�����
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.��һ�ε��ã���ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		NULL,				//������
		0,					//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL);
	//3.������Ҫ���ڴ棬�ڶ��ε���
	LPENUM_SERVICE_STATUS lpEnumService =
		(LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.�ڶ���ö��
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		lpEnumService,		//������
		dwSize,				//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL);
	//5.������Ϣ

	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//�����Ҫ���µķ���

		if (!_tcscmp(lpEnumService[i].lpServiceName, m_strServiceName))
		{
			CString state;
			//����list�и÷����״̬��Ϣ
			switch (lpEnumService[i].ServiceStatus.dwCurrentState)
			{
			case SERVICE_STOPPED:

				state="��ֹͣ";
				break;
			case SERVICE_STOP_PENDING:
				state = "����ֹͣ";
				break;
			case SERVICE_START_PENDING:
				state = "��������";
				break;
			case SERVICE_RUNNING:
				state = "��������";
				break;
			case SERVICE_PAUSED:
				state = "����ͣ";
				break;
			case SERVICE_PAUSE_PENDING:
				state = "������ͣ";
				break;
			case SERVICE_CONTINUE_PENDING:
				state = "׼������";
				
				break;
			}

			m_listServices.SetItemText(m_nSelectNum, 2, state);
			break;
		}
	}
	LocalFree(lpEnumService);
	CloseServiceHandle(hSCM);
}
// �б��Ҽ�
void CServiceDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int n = pNMItemActivate->iItem;//ѡ���˵ڼ���
	if (n != -1)
	{
		//������ѡ����� ������
		m_strServiceName = m_listServices.GetItemText(n, 0);
		m_nSelectNum = n;
		//�������δ���, ��������Ӧ��ListCtrl  
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

// ��ʱ��
void CServiceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

// ��������
void CServiceDlg::OnStartService()
{
	//��������
	SC_HANDLE hSCM = OpenSCManager(			//�򿪷�����ƹ�����
		NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM,	//�򿪷���
		m_strServiceName, SERVICE_START);
	StartService(hService, 0, 0);			//��������
											//������Ϣ
	UpdateServiceInfo();
	m_bIsUpDateSer = TRUE;
	m_nUpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}

// �رշ���
void CServiceDlg::OnCloseService()
{
	//ֹͣ����
	SC_HANDLE hSCM = OpenSCManager(			//�򿪷�����ƹ�����
		NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM,	//�򿪷���
		m_strServiceName, SERVICE_STOP);
	SERVICE_STATUS status;
	ControlService(hService,				//��������
		SERVICE_CONTROL_STOP,
		&status);
	//������Ϣ
	UpdateServiceInfo();
	m_bIsUpDateSer = TRUE;
	m_nUpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}
