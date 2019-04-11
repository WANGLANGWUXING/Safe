#pragma once
#include "MyListCtrl.h"
#include <vector>
using std::vector;
typedef struct _MYWINDOWINFO
{
	TCHAR hWnd[MAX_PATH + 1];  //窗口句柄
	TCHAR WindowName[MAX_PATH + 1];//窗口标题
	TCHAR ClassName[MAX_PATH + 1];//窗口类名
}MYWINDOWINFO, *PMYWINDOWINFO;


// CWinDlg 对话框

class CWinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWinDlg)

public:
	CWinDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWinDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	MyListCtrl m_winList;
	virtual BOOL OnInitDialog();
	

};
