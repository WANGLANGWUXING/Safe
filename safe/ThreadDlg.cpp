// ThreadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "ThreadDlg.h"
#include "afxdialogex.h"


// CThreadDlg 对话框

IMPLEMENT_DYNAMIC(CThreadDlg, CDialogEx)

CThreadDlg::CThreadDlg(int pid,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_THREAD_DIALOG, pParent)
{
	m_pid = pid;
}

CThreadDlg::~CThreadDlg()
{
}

void CThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THREAD_LIST, m_threadList);
}


BEGIN_MESSAGE_MAP(CThreadDlg, CDialogEx)
END_MESSAGE_MAP()


// CThreadDlg 消息处理程序


BOOL CThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ShowThread(m_pid);

	return TRUE;  
}

void CThreadDlg::ShowThread(int nPid)
{
	//创建快照;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);

	//保存进程信息结构体;
	THREADENTRY32 ThreadInfo = { sizeof(THREADENTRY32) };

	//遍历进程;
	if (Thread32First(hSnapShot, &ThreadInfo))
	{
		m_threadList.AddColumn(3, "线程ID", 150, "优先级", 150, "状态", 150);
		do {
			//显示符合条件的线程;
			if (ThreadInfo.th32OwnerProcessID == nPid)
			{
				CString PID, Levele, State;
				//线程ID
				PID.Format("%d", ThreadInfo.th32ThreadID);
				//优先级
				Levele.Format("%d", ThreadInfo.tpBasePri);

				//获取线程句柄;
				//DWORD dwState = 0;
				HANDLE m_hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, ThreadInfo.th32ThreadID);

				//判断线程状态;
				//GetExitCodeThread(m_hThread, &dwState);
				//上面的方法获取STILL_ACTIVE判断不了，因为挂起和运行都是STILL_ACTIVE

				int suspendCount = -1;//挂起计数器（暂停次数）  如果是0则是运行
				suspendCount = SuspendThread(m_hThread);
				ResumeThread(m_hThread);

				if (suspendCount == 0 || suspendCount == -1)
				{
					State = "运行中";
				}
				else
				{
					State = "挂起";
				}
				CloseHandle(m_hThread);
				m_threadList.AddItem(3, PID, Levele, State);
			}

		} while (Thread32Next(hSnapShot, &ThreadInfo));
	}
	else
	{
		MessageBox("获取线程信息失败！");
		EndDialog(0);
	}
}
