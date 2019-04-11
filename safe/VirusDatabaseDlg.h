#pragma once
#include "MyListCtrl.h"
#include "VirusManage.h"
#include "MD5_FILE.h"
// CVirusDatabaseDlg 对话框

class CVirusDatabaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVirusDatabaseDlg)

public:
	CVirusDatabaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVirusDatabaseDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRUSDATABASE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 病毒库列表
	MyListCtrl m_listVirusDatabase;
	// 选中项
	int m_item;
	// 显示病毒列表
	void ShowVirusList();
	// 更新病毒列表
	void UpDataVirusDatabase();
	// 文件拖拽
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// 删除病毒库项
	afx_msg void OnDelVirusDatabaseItem();
	// 删除所有病毒项
	afx_msg void OnDelAllVirusDatabase();
	// 列表右键
	afx_msg void OnNMRClickListVirusdata(NMHDR *pNMHDR, LRESULT *pResult);
};
