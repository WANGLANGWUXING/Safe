#pragma once
#include "MyListCtrl.h"
#include <vector>
using std::vector;
typedef struct _MYWINDOWINFO
{
	TCHAR hWnd[MAX_PATH + 1];  //���ھ��
	TCHAR WindowName[MAX_PATH + 1];//���ڱ���
	TCHAR ClassName[MAX_PATH + 1];//��������
}MYWINDOWINFO, *PMYWINDOWINFO;


// CWinDlg �Ի���

class CWinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWinDlg)

public:
	CWinDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWinDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	MyListCtrl m_winList;
	virtual BOOL OnInitDialog();
	

};
