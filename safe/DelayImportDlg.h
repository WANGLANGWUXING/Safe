#pragma once
#include "MyListCtrl.h"
#include "PeInfo.h"
#include "common.h"

// CDelayImportDlg 对话框

class CDelayImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDelayImportDlg)

public:
	CDelayImportDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDelayImportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELAYIMPORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	MyListCtrl m_listDelayDLL;
	MyListCtrl m_listDelayFun;
	char *m_pFileBuf = NULL;
	PIMAGE_NT_HEADERS m_pNt;
	afx_msg void OnNMClickListdelaydll(NMHDR *pNMHDR, LRESULT *pResult);
};
