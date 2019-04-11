#pragma once
#include "PeInfo.h"

// CTLSDlg �Ի���

class CTLSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTLSDlg)

public:
	CTLSDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTLSDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TLS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �ļ�ָ�룬PEͷ
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
