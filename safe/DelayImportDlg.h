#pragma once
#include "MyListCtrl.h"
#include "PeInfo.h"
#include "common.h"

// CDelayImportDlg �Ի���

class CDelayImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDelayImportDlg)

public:
	CDelayImportDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDelayImportDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELAYIMPORT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	MyListCtrl m_listDelayDLL;
	MyListCtrl m_listDelayFun;
	char *m_pFileBuf = NULL;
	PIMAGE_NT_HEADERS m_pNt;
	afx_msg void OnNMClickListdelaydll(NMHDR *pNMHDR, LRESULT *pResult);
};
