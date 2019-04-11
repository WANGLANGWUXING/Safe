#pragma once
#include "MyListCtrl.h"
#include "MD5_FILE.h"
#include "common.h"
// CFileInfo 对话框

class CFileInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CFileInfo)

public:
	CFileInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEINFO_DIALOG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 文件列表
	MyListCtrl m_listFileInfo;
	virtual BOOL OnInitDialog();
	// 显示参数目录所有文件
	void ShowFileList(CString &dir);
	// 列表控件双击
	afx_msg void OnNMDblclkListfileinfo(NMHDR *pNMHDR, LRESULT *pResult);
};
