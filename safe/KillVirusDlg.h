#pragma once
#include "MyListCtrl.h"
#include "afxwin.h"
#include "VirusDatabaseDlg.h"
#include <vector>
#include "common.h"
#include "ProcessDatabase.h"
using std::vector;

// CKillVirusDlg 对话框

class CKillVirusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKillVirusDlg)

public:
	CKillVirusDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKillVirusDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KILLVIRUS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// list控件绑定
	MyListCtrl m_listFile;
	MyListCtrl m_listProcess;
	BOOL m_radioAllValue;
	// 文件查杀Group框
	CStatic m_staticGroup;
	// 全盘扫描，指定位置扫描Radio
	CButton m_btnAllCtrl;
	CButton m_btnThisCtrl;
	// 点击指定位置
	afx_msg void OnBnClickedRadio3();
	
	// 点击全盘扫描
	afx_msg void OnBnClickedRadio2();
	// 病毒库
	afx_msg void OnBnClickedButton2();
	// 开始扫描
	afx_msg void OnBnClickedButton1();

	// 加载本地病毒库
	void LoadVirus();
	// 图标列表
	CImageList  m_VirusImageList;
	CImageList  m_ProcessImageList;
	BOOL m_bIsScaning = FALSE;			//是否正在扫描
	CString m_strScaningPath;				//正在扫描的路径
	DWORD m_dwScaningNum;					//已扫描文件数量
	DWORD m_dwVirusNum;					//已扫描病毒数量
	CString m_strThisPath;					//指定位置扫描

	PVIRUSINFO m_pVirusInfo = nullptr;	//病毒库结构体
	DWORD m_dwVirusInfoNum = 0;			//病毒库数量

	// 刷新时间
	DWORD REFERSHPATH_TIMER = 0x888;
	// 开始扫描文件
	CButton m_btnStartFile;
	// 扫描文件 
	void ScanFile(CString dir);
	// 扫描进程
	void ScanProcess();
	// 提示文本
	CStatic m_virusMsg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 加载图标
	void LoadImageVirus();
	void LoadImageProcess();
	// 选中病毒文件列表项
	int m_fileItem;
	// 删除病毒
	afx_msg void OnDelVirusItem();
	// 病毒列表右键
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	// 删除所有病毒
	afx_msg void OnDelAllVirus();
	// 白名单
	afx_msg void OnBnClickedButton4();
	// 进程扫描
	afx_msg void OnBnClickedButton3();
	// 开始扫描进程按钮
	CButton m_btnStartProc;
	// 自动结束进程按钮
	CButton m_btnAutoEnd;
};
