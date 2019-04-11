// ProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "ProcessDlg.h"
#include "afxdialogex.h"


// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESS_DIALOG, pParent)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRO, m_proList);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PRO, &CProcessDlg::OnRclickListPro)
	ON_COMMAND(ID_32771, &CProcessDlg::OnShowThread)
	ON_COMMAND(ID_32772, &CProcessDlg::OnShowModule)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序


BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_proList.AddColumn(5, "名称", 120, "PID", 50, "父进程PID", 80, "线程数量", 80, "路径", 180);
	ShowProcess();
	SetTimer(1, 20000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CProcessDlg::ShowProcess()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox("HELP 函数调用失败！");
		return;
	}

	//初始化快照信息结构体;
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	if (!Process32First(hSnapShot, &pi))
	{
		MessageBox("获取进程信息失败");
		return;
	}
	do
	{
		TCHAR *szBuf[5] = { new TCHAR[MAX_PATH], new TCHAR[6], new TCHAR[6], new TCHAR[6], new TCHAR[MAX_PATH], };
		szBuf[0] = pi.szExeFile;
		if (pi.th32ProcessID > 0 && pi.th32ParentProcessID > 0 && pi.cntThreads >= 0)
		{
			_stprintf_s(szBuf[1], 6, "%d", pi.th32ProcessID);
			_stprintf_s(szBuf[2], 6, "%d", pi.th32ParentProcessID);
			_stprintf_s(szBuf[3], 6, "%d", pi.cntThreads);
		}
		else
		{
			szBuf[1] = "  ";
			szBuf[2] = "  ";
			szBuf[3] = "  ";
		}

		//打开要获取路径的进程
		HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
			| PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pi.th32ProcessID);
		//获取进程完整路径;
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);
		}
		else
		{
			szBuf[4] = "无权访问";
		}
		m_proList.AddItem(5, szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4]);
		CloseHandle(hPro);
	} while (Process32Next(hSnapShot, &pi));

	return;
}



void CProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_proList.DeleteAllItems();
	ShowProcess();
	CDialogEx::OnTimer(nIDEvent);
}


void CProcessDlg::OnRclickListPro(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU1);

	CMenu* pSub = pMenu->GetSubMenu(0);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);
	*pResult = 0;
}


void CProcessDlg::OnShowThread()
{
	int nSel = m_proList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("请选中数据");
		return;
	}
	int PID = _ttoi(m_proList.GetItemText(nSel, 1));
	CThreadDlg m_threadDlg(PID);
	m_threadDlg.DoModal();
}


void CProcessDlg::OnShowModule()
{
	int nSel = m_proList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox("请选中数据");
		return;
	}
	int PID = _ttoi(m_proList.GetItemText(nSel, 1));
	CModuleDlg m_moduleDlg(PID);
	m_moduleDlg.DoModal();
}
