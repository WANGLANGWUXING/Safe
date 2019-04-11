#pragma once
#include "MyListCtrl.h"
#include "MD5_FILE.h"
#include "common.h"
// CFileInfo �Ի���

class CFileInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CFileInfo)

public:
	CFileInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileInfo();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEINFO_DIALOG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �ļ��б�
	MyListCtrl m_listFileInfo;
	virtual BOOL OnInitDialog();
	// ��ʾ����Ŀ¼�����ļ�
	void ShowFileList(CString &dir);
	// �б�ؼ�˫��
	afx_msg void OnNMDblclkListfileinfo(NMHDR *pNMHDR, LRESULT *pResult);
};
