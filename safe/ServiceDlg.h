#pragma once
#include "MyListCtrl.h"
#include <winsvc.h>
#include <vector>
#include "common.h"
using std::vector;
// CServiceDlg 对话框

class CServiceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceDlg)

public:
	CServiceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServiceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	MyListCtrl m_listServices;
	virtual BOOL OnInitDialog();
	//服务信息容器
	vector<ENUM_SERVICE_STATUS> m_vecSerInfo;
	void GetServicesInfo();   //获取系统服务
	void UpdateServiceInfo(); //更新系统服务
	CString m_strServiceName;    //选中的服务名称
	int m_nSelectNum;          //选中第几项
	int m_nUpDataTimes;        //更新次数
	BOOL m_bIsUpDateSer;       //是否需要更新

	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStartService();
	afx_msg void OnCloseService();
};
