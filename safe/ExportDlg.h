#pragma once
#include "PeInfo.h"
#include "afxcmn.h"
#include "MyListCtrl.h"
// CExportDlg �Ի���

class CExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDlg)

public:
	CExportDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExportDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �ļ�ָ�룬PEͷ
	char *m_pFileBuf = NULL;
	PIMAGE_NT_HEADERS m_pNt;

	CString m_strExportOffset;
	CString m_strCharacteristic;
	CString m_strBase;
	CString m_strName;
	CString m_strNameStr;
	CString m_strNumberOfFunctions;
	CString m_strNumberOfNames;
	CString m_strAddressOfFunctions;
	CString m_strAddressOfNames;
	CString m_strAddressOfNameOrdinals;
	virtual BOOL OnInitDialog();
	MyListCtrl m_listExportFuns;
	afx_msg void OnClose();
};
