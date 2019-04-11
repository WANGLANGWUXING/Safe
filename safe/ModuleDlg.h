#pragma once
#include "MyListCtrl.h"
#include <TlHelp32.h>

// CModuleDlg �Ի���

class CModuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModuleDlg)

public:
	CModuleDlg(int pid,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModuleDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODULE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	MyListCtrl m_moduleList;
	virtual BOOL OnInitDialog();
	void ShowModule(int nPid);
	int m_pid;
};
