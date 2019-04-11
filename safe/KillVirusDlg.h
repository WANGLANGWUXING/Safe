#pragma once
#include "MyListCtrl.h"
#include "afxwin.h"
#include "VirusDatabaseDlg.h"
#include <vector>
#include "common.h"
#include "ProcessDatabase.h"
using std::vector;

// CKillVirusDlg �Ի���

class CKillVirusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKillVirusDlg)

public:
	CKillVirusDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKillVirusDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KILLVIRUS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// list�ؼ���
	MyListCtrl m_listFile;
	MyListCtrl m_listProcess;
	BOOL m_radioAllValue;
	// �ļ���ɱGroup��
	CStatic m_staticGroup;
	// ȫ��ɨ�裬ָ��λ��ɨ��Radio
	CButton m_btnAllCtrl;
	CButton m_btnThisCtrl;
	// ���ָ��λ��
	afx_msg void OnBnClickedRadio3();
	
	// ���ȫ��ɨ��
	afx_msg void OnBnClickedRadio2();
	// ������
	afx_msg void OnBnClickedButton2();
	// ��ʼɨ��
	afx_msg void OnBnClickedButton1();

	// ���ر��ز�����
	void LoadVirus();
	// ͼ���б�
	CImageList  m_VirusImageList;
	CImageList  m_ProcessImageList;
	BOOL m_bIsScaning = FALSE;			//�Ƿ�����ɨ��
	CString m_strScaningPath;				//����ɨ���·��
	DWORD m_dwScaningNum;					//��ɨ���ļ�����
	DWORD m_dwVirusNum;					//��ɨ�財������
	CString m_strThisPath;					//ָ��λ��ɨ��

	PVIRUSINFO m_pVirusInfo = nullptr;	//������ṹ��
	DWORD m_dwVirusInfoNum = 0;			//����������

	// ˢ��ʱ��
	DWORD REFERSHPATH_TIMER = 0x888;
	// ��ʼɨ���ļ�
	CButton m_btnStartFile;
	// ɨ���ļ� 
	void ScanFile(CString dir);
	// ɨ�����
	void ScanProcess();
	// ��ʾ�ı�
	CStatic m_virusMsg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ����ͼ��
	void LoadImageVirus();
	void LoadImageProcess();
	// ѡ�в����ļ��б���
	int m_fileItem;
	// ɾ������
	afx_msg void OnDelVirusItem();
	// �����б��Ҽ�
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	// ɾ�����в���
	afx_msg void OnDelAllVirus();
	// ������
	afx_msg void OnBnClickedButton4();
	// ����ɨ��
	afx_msg void OnBnClickedButton3();
	// ��ʼɨ����̰�ť
	CButton m_btnStartProc;
	// �Զ��������̰�ť
	CButton m_btnAutoEnd;
};
