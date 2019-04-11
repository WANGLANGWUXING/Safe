#include "common.h"
// safeDlg.h : ͷ�ļ�
//

#pragma once
#include <powrprof.h>
#include "MyTabCtrl.h"
#include "ProcessDlg.h"
#include "WinDlg.h"
#include "CleanDlg.h"
#include "ServiceDlg.h"
#include "KillVirusDlg.h"
#include "afxwin.h"
#include "PEDlg.h"
#include "VSCleanDlg.h"
#include "FileInfo.h"
// CsafeDlg �Ի���
class CsafeDlg : public CDialogEx
{
	// ����
public:
	CsafeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAFE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;



	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	MyTabCtrl m_tabs;
	static UINT MyThreadFunction(LPVOID lpParam);
	static double FILETIME2Double(const _FILETIME& fileTime);
	static int GetCpuUsage();
	CString m_strCPU;
	CString m_strMemory;
	CStatic m_cpu;
	CStatic m_memory;
	CStatusBar m_status;
	DWORD m_HotKey = 5235;
	HANDLE hThread;
	bool m_bClose;
	afx_msg void OnBnClickedBtnMemoryup();
	afx_msg void OnClose();
	BOOL IsAdmin();
	CButton m_changePrivilege;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);

	const DWORD STATUS_0 = 0x10001;
	CMenu m_menu;
	afx_msg void OnShutdown();
	afx_msg void OnRestart();
	afx_msg void OnLockscreen();
	afx_msg void OnSleep1();
	afx_msg void OnLogout();

};
