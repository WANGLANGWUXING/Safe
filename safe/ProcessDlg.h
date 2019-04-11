#pragma once
#include "MyListCtrl.h"
#include <TlHelp32.h>
#include "ThreadDlg.h"
#include "ModuleDlg.h"
// CProcessDlg 对话框

class CProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	MyListCtrl m_proList;
	virtual BOOL OnInitDialog();
	void ShowProcess();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRclickListPro(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowThread();
	afx_msg void OnShowModule();
};
