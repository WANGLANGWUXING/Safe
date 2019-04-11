#pragma once
#include "MyListCtrl.h"
#include "afxcmn.h"
#include "common.h"
#include "VirusManage.h"
#include "MD5_FILE.h"
#include "afxwin.h"
// CProcessDatabase 对话框

class CProcessDatabase : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDatabase)

public:
	CProcessDatabase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessDatabase();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSDATABASE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 列表控件
	MyListCtrl m_listProc;
	MyListCtrl m_listWhite;
	virtual BOOL OnInitDialog();
	// 加载进程列表
	void ShowProcessWhiteList();
	// 更新进程白名单
	void UpdateProcessData();
	// 列表双击
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	// 全部进程按钮
	CButton m_btnAllProc;
	// 添加进程到白名单
	afx_msg void OnBnAddProcessWhite();
	// 清空所有白名单
	afx_msg void OnBnDelAllProcessWhite();
};
