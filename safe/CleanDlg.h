#pragma once
#include "MyListCtrl.h"
//清理的缓冲类型
enum DEL_CACHE_TYPE
{
	FILES = 0, COOKIES
};
#define SWEEP_BUFFER_SIZE			10000
#define WM_MYUPDATEDATA				WM_USER+100 

//清理项ID枚举
enum DEL_RUBBISH_TYPE
{
	CLEAN_SYSTEM_TEMP = 0, CLEAN_RUBBISH_STATION, CLEAN_RUN_HISTORY,
	CLEAN_DOCUMENT_HISTORY, CLEAN_PREVUSER_HISTORY, CLEAN_FILEFIND_HISTORY,
	CLEAN_INTERNET_TEMP, CLEAN_INTERNET_COOKIE, CLEAN_ADDRBAR_HISTORY,
	CLEAN_PASSWORD_HISTORY, CLEAN_BROWSEADDR_HISTORY
};
// CCleanDlg 对话框

class CCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCleanDlg)

public:
	CCleanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCleanDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEAN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	// 全选
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck14();

	//系统垃圾
	VOID CleanSystemTemp();				//清理系统临时文件夹		0  CLEAN_SYSTEM_TEMP
	VOID CleanRubbishStation();			//清理垃圾回收站			1  CLEAN_RUBBISH_STATION
	VOID CleanRunHistory();				//清理运行记录				2  CLEAN_RUN_HISTORY
	VOID CleanDocumentHistory();		//清理最近文档记录			3  CLEAN_DOCUMENT_HISTORY
	VOID CleanPrevUserHistory();		//清理上次用户登录记录		4  CLEAN_PREVUSER_HISTORY
	VOID CleanFileFindHistory();		//清理文件查找记录			5  CLEAN_FILEFIND_HISTORY

										//浏览器垃圾
	VOID CleanInternetTemp();			//清理Interner临时文件		6  CLEAN_INTERNET_TEMP
	VOID CleanInternetCookie();			//清理Internet Cookie	7  CLEAN_INTERNET_COOKIE
	VOID CleanAddrBarHistory();			//清理地址栏历史记录		8  CLEAN_ADDRBAR_HISTORY
	VOID CleanPasswordHistory();		//清理密码记录				9  CLEAN_PASSWORD_HISTORY
	VOID CleanBrowseAddrHistory();		//清理浏览网址记录			10 CLEAN_BROWSEADDR_HISTORY

	//其他
	//清除Url缓存
	BOOL DeleteUrlCache(DEL_CACHE_TYPE type);
	//清空一个目录
	BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE,
		BOOL bWipeIndexDat = FALSE);
	//擦除文件内容
	BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile);

	// 清理文件列表
	MyListCtrl m_listClean;
	// 文件大小
	CString m_szTemp;
	//判断是否是扫描文件
	BOOL m_isScanFile;
	//所有文件的大小
	DOUBLE m_dAllFileSize;
	//获得当前文件转换后要显示的大小
	CString GetShowSize(DOUBLE dwSize);
	//提示信息
	CString m_strStatus;
	//获取清理项的选择状态
	void GetSelect();
	//保存清理项是否被选中的数组
	BOOL m_bIsSelect[11];
	// 扫描按钮
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	VOID CleanRubbish(int cleanType);	//清理调用函数
	BOOL StartCleanThread();			//启动清理线程
protected:
	afx_msg LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);
public:

	
	afx_msg void OnBnClickedCheck12();
};
