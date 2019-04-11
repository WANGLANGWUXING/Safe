#pragma once
#include "MyListCtrl.h"
#include <winsvc.h>
#include <vector>
#include "common.h"
using std::vector;
// CServiceDlg �Ի���

class CServiceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceDlg)

public:
	CServiceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServiceDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	MyListCtrl m_listServices;
	virtual BOOL OnInitDialog();
	//������Ϣ����
	vector<ENUM_SERVICE_STATUS> m_vecSerInfo;
	void GetServicesInfo();   //��ȡϵͳ����
	void UpdateServiceInfo(); //����ϵͳ����
	CString m_strServiceName;    //ѡ�еķ�������
	int m_nSelectNum;          //ѡ�еڼ���
	int m_nUpDataTimes;        //���´���
	BOOL m_bIsUpDateSer;       //�Ƿ���Ҫ����

	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStartService();
	afx_msg void OnCloseService();
};
