#pragma once
#include "MyListCtrl.h"
#include "PEInfo.h"
#include "common.h"
// CImportDlg 对话框

class CImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportDlg)

public:
	CImportDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 文件指针，PE头

	char *m_pFileBuf = NULL;
	PIMAGE_NT_HEADERS m_pNt;
	virtual BOOL OnInitDialog();

	MyListCtrl m_listImprtDll;
	MyListCtrl m_listImprtThunk;
	afx_msg void OnNMClickListImportdll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};
