#pragma once
#include "MyListCtrl.h"
#include "VirusManage.h"
#include "MD5_FILE.h"
// CVirusDatabaseDlg �Ի���

class CVirusDatabaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVirusDatabaseDlg)

public:
	CVirusDatabaseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVirusDatabaseDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRUSDATABASE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// �������б�
	MyListCtrl m_listVirusDatabase;
	// ѡ����
	int m_item;
	// ��ʾ�����б�
	void ShowVirusList();
	// ���²����б�
	void UpDataVirusDatabase();
	// �ļ���ק
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// ɾ����������
	afx_msg void OnDelVirusDatabaseItem();
	// ɾ�����в�����
	afx_msg void OnDelAllVirusDatabase();
	// �б��Ҽ�
	afx_msg void OnNMRClickListVirusdata(NMHDR *pNMHDR, LRESULT *pResult);
};
