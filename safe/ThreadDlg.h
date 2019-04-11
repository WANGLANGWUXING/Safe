#pragma once
#include <TlHelp32.h>
#include "MyListCtrl.h"

// CThreadDlg �Ի���

class CThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadDlg)

public:
	CThreadDlg(int pid,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThreadDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREAD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowThread(int nPid);
	int m_pid;
	MyListCtrl m_threadList;
};
