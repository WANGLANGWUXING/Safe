#pragma once
#include "MyListCtrl.h"
#include "PEInfo.h"
#include "common.h"
// CImportDlg �Ի���

class CImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportDlg)

public:
	CImportDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImportDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �ļ�ָ�룬PEͷ

	char *m_pFileBuf = NULL;
	PIMAGE_NT_HEADERS m_pNt;
	virtual BOOL OnInitDialog();

	MyListCtrl m_listImprtDll;
	MyListCtrl m_listImprtThunk;
	afx_msg void OnNMClickListImportdll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};
