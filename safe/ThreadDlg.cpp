// ThreadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "ThreadDlg.h"
#include "afxdialogex.h"


// CThreadDlg �Ի���

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


// CThreadDlg ��Ϣ�������


BOOL CThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ShowThread(m_pid);

	return TRUE;  
}

void CThreadDlg::ShowThread(int nPid)
{
	//��������;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);

	//���������Ϣ�ṹ��;
	THREADENTRY32 ThreadInfo = { sizeof(THREADENTRY32) };

	//��������;
	if (Thread32First(hSnapShot, &ThreadInfo))
	{
		m_threadList.AddColumn(3, "�߳�ID", 150, "���ȼ�", 150, "״̬", 150);
		do {
			//��ʾ�����������߳�;
			if (ThreadInfo.th32OwnerProcessID == nPid)
			{
				CString PID, Levele, State;
				//�߳�ID
				PID.Format("%d", ThreadInfo.th32ThreadID);
				//���ȼ�
				Levele.Format("%d", ThreadInfo.tpBasePri);

				//��ȡ�߳̾��;
				//DWORD dwState = 0;
				HANDLE m_hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, ThreadInfo.th32ThreadID);

				//�ж��߳�״̬;
				//GetExitCodeThread(m_hThread, &dwState);
				//����ķ�����ȡSTILL_ACTIVE�жϲ��ˣ���Ϊ��������ж���STILL_ACTIVE

				int suspendCount = -1;//�������������ͣ������  �����0��������
				suspendCount = SuspendThread(m_hThread);
				ResumeThread(m_hThread);

				if (suspendCount == 0 || suspendCount == -1)
				{
					State = "������";
				}
				else
				{
					State = "����";
				}
				CloseHandle(m_hThread);
				m_threadList.AddItem(3, PID, Levele, State);
			}

		} while (Thread32Next(hSnapShot, &ThreadInfo));
	}
	else
	{
		MessageBox("��ȡ�߳���Ϣʧ�ܣ�");
		EndDialog(0);
	}
}
