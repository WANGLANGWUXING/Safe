#pragma once
#include "afxwin.h"
#include "ExportDlg.h"
#include "ImportDlg.h"
#include "ResourceDlg.h"
#include "RelocateDlg.h"
#include "TLSDlg.h"
#include "DelayImportDlg.h"
// CDataDirectoryDlg 对话框

class CDataDirectoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDirectoryDlg)

public:
	CDataDirectoryDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataDirectoryDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATADIRECTRY_DIALOG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 文件指针
	char *m_pFileBuf = NULL;
	// NT头指针
	PIMAGE_NT_HEADERS m_pNt;
	virtual BOOL OnInitDialog();
	// 绑定变量
	CString m_strExportRva;
	CString m_strExportSize;	
	CString m_strImportRva;
	CString m_strImportSize;
	CString m_strResourceRva;
	CString m_strResourceSize;
	CString m_strExceptionRva;
	CString m_strExceptionSize;
	CString m_strSecurityRva;
	CString m_strSecuritySize;
	CString m_strBaseRelocRva;
	CString m_strBaseRelocSize;
	CString m_strDebugRva;
	CString m_strDebugSize;
	CString m_strArchitectureRva;
	CString m_strArchitectureSize;
	CString m_strGlobalPtrRva;
	CString m_strGlobalPtrSize;
	CString m_strTLSRva;
	CString m_strTLSSize;
	CString m_strLoadConfigRva;
	CString m_strLoadConfigSize;
	CString m_strBoundImportRva;
	CString m_strBoundImportSize;
	CString m_strIAIRva;
	CString m_strIAISize;
	CString m_strDelayImportRva;
	CString m_strDelayImportSize;
	CString m_strComRva;
	CString m_strComSize;
	CString m_strRetainRva;
	CString m_strRetainSize;
	// 按钮点击
	afx_msg void OnBnClickedBtnexportinfo();
	afx_msg void OnBnClickedBtnimportinfo();
	afx_msg void OnBnClickedBtnresourceinfo();
	afx_msg void OnBnClickedBtnbaserelocinfo();
	afx_msg void OnBnClickedBtntlsinfo();
	afx_msg void OnBnClickedBtndelayimportinfo();
	afx_msg void OnClose();
};
