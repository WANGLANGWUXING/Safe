#pragma once
#include "PeInfo.h"

// CTLSDlg 对话框

class CTLSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTLSDlg)

public:
	CTLSDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTLSDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TLS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 文件指针，PE头
	char *m_pFileBuf = NULL;
	PIMAGE_NT_HEADERS m_pNt;
	CString m_strStartAddrVA;
	CString m_strEndAddrVA;
	CString m_strAddrIndexVA;
	CString m_strCallBacksAddrVA;
	CString m_strSizeofZeroFill;
	CString m_strChar;
	virtual BOOL OnInitDialog();
};
