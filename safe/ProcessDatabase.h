#pragma once
#include "MyListCtrl.h"
#include "afxcmn.h"
#include "common.h"
#include "VirusManage.h"
#include "MD5_FILE.h"
#include "afxwin.h"
// CProcessDatabase �Ի���

class CProcessDatabase : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDatabase)

public:
	CProcessDatabase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcessDatabase();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSDATABASE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �б�ؼ�
	MyListCtrl m_listProc;
	MyListCtrl m_listWhite;
	virtual BOOL OnInitDialog();
	// ���ؽ����б�
	void ShowProcessWhiteList();
	// ���½��̰�����
	void UpdateProcessData();
	// �б�˫��
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	// ȫ�����̰�ť
	CButton m_btnAllProc;
	// ��ӽ��̵�������
	afx_msg void OnBnAddProcessWhite();
	// ������а�����
	afx_msg void OnBnDelAllProcessWhite();
};
