#pragma once
#include "afxcmn.h"


// CSectionTableDlg �Ի���

class CSectionTableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionTableDlg)

public:
	CSectionTableDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSectionTableDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECTIONTABLE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	char *m_pFileBuf = NULL;
	PIMAGE_NT_HEADERS m_pNt;
	virtual BOOL OnInitDialog();
	CListCtrl m_sectionList;
};
