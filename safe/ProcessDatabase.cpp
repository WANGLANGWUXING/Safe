// ProcessDatabase.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "ProcessDatabase.h"
#include "afxdialogex.h"


// CProcessDatabase 对话框

IMPLEMENT_DYNAMIC(CProcessDatabase, CDialogEx)

CProcessDatabase::CProcessDatabase(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSDATABASE_DIALOG, pParent)
{

}

CProcessDatabase::~CProcessDatabase()
{
}

void CProcessDatabase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listProc);
	DDX_Control(pDX, IDC_LIST2, m_listWhite);
	DDX_Control(pDX, IDC_BUTTON1, m_btnAllProc);
}


BEGIN_MESSAGE_MAP(CProcessDatabase, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CProcessDatabase::OnNMDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CProcessDatabase::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_BUTTON1, &CProcessDatabase::OnBnAddProcessWhite)
	ON_BN_CLICKED(IDC_BUTTON5, &CProcessDatabase::OnBnDelAllProcessWhite)
END_MESSAGE_MAP()


// CProcessDatabase 消息处理程序


BOOL CProcessDatabase::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listProc.AddColumn(2, "进程名", 100, "进程路径", 100);
	m_listWhite.AddColumn(2, "进程名", 100, "文件MD5", 100);
	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);//枚举进程列表
	CStringA nFileMd5;
	DWORD nIndex;

	for (DWORD i = 0; i < nNum; i++)
	{
		if (nTaskList.szExeFilePath[i].IsEmpty())continue;

		nIndex = m_listProc.GetItemCount();
		m_listProc.AddItem(2, nTaskList.szExeFile[i], nTaskList.szExeFilePath[i]);
	}
	ClearTaskList(nTaskList);


	ShowProcessWhiteList();

	return TRUE;  
}
void CProcessDatabase::ShowProcessWhiteList()
{
	m_listWhite.DeleteAllItems();

	CVirusManage nLoadProcessLib;
	DWORD nCount = nLoadProcessLib.LoadProcessLibData();
	PVIRUSINFO m_pVirusInfo = new VIRUSINFO[nCount];
	nLoadProcessLib.GetProcessLib(m_pVirusInfo);
	nLoadProcessLib.ClearProcessLibData();


	for (DWORD i = 0; i < nCount; i++)
	{
		m_listWhite.AddItem(2, CString(m_pVirusInfo[i].FileName), CString(m_pVirusInfo[i].FileMd5));
	}

	delete[]m_pVirusInfo;
}




void CProcessDatabase::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem == -1)return;

	CString nFileName;
	CString nFilePath;
	CString nFileMd5;


	nFileName = m_listProc.GetItemText(pNMItemActivate->iItem, 0);
	nFilePath = m_listProc.GetItemText(pNMItemActivate->iItem, 1);

	nFileMd5 = GetFileMd5(nFilePath);
	if (nFileMd5.IsEmpty())return;

	CString nLibName;
	CString nLibMd5;

	for (INT i = 0; i < m_listWhite.GetItemCount(); i++)
	{
		nLibName = m_listWhite.GetItemText(i, 0);
		nLibMd5 = m_listWhite.GetItemText(i, 1);

		if (strcmp(CStringA(nFileName), CStringA(nLibName)) == 0)return;
	}
	m_listWhite.AddItem(2, nFileName, nFileMd5);

	UpdateProcessData();

	*pResult = 0;
}
void CProcessDatabase::UpdateProcessData()
{
	DWORD nCount = m_listWhite.GetItemCount();
	PVIRUSINFO nVirusInfo = new VIRUSINFO[nCount];
	for (DWORD i = 0; i < nCount; i++)
	{
		strcpy_s(nVirusInfo[i].FileName, MAX_PATH, CStringA(m_listWhite.GetItemText(i, 0)));
		strcpy_s(nVirusInfo[i].FileMd5, MAX_PATH, CStringA(m_listWhite.GetItemText(i, 1)));
	}

	CVirusManage nLoadVirusLib;
	nLoadVirusLib.SetProcessLib(nVirusInfo, nCount);
}

void CProcessDatabase::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (pNMItemActivate->iItem == -1)return;

	m_listWhite.DeleteItem(pNMItemActivate->iItem);

	UpdateProcessData();

	*pResult = 0;
}


void CProcessDatabase::OnBnAddProcessWhite()
{
	m_btnAllProc.EnableWindow(FALSE);
	CString nFileName;
	CString nFilePath;
	CString nFileMd5;
	DWORD nCount;
	BOOL nHave;

	nCount = m_listProc.GetItemCount();
	for (DWORD i = 0; i < nCount; i++)
	{
		nFileName = m_listProc.GetItemText(i, 0);
		nFilePath = m_listProc.GetItemText(i, 1);

		nFileMd5 = GetFileMd5(nFilePath);
		if (nFileMd5.IsEmpty())continue;;

		CString nLibName;
		CString nLibMd5;
		nHave = FALSE;

		for (INT x = 0; x < m_listWhite.GetItemCount(); x++)
		{
			nLibName = m_listWhite.GetItemText(x, 0);
			nLibMd5 = m_listWhite.GetItemText(x, 1);

			if (strcmp(CStringA(nFileName), CStringA(nLibName)) == 0)
			{
				nHave = TRUE;
				break;
			}
		}

		if (!nHave)
		{
			m_listWhite.AddItem(2, nFileName, nFileMd5);
		}
	}

	UpdateProcessData();
	m_btnAllProc.EnableWindow(TRUE);
}


void CProcessDatabase::OnBnDelAllProcessWhite()
{
	m_listWhite.DeleteAllItems();
	UpdateProcessData();
}
