// WinDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "WinDlg.h"
#include "afxdialogex.h"
//������Ϣ����;
vector<MYWINDOWINFO> vecWindowInfo;

// CWinDlg �Ի���

IMPLEMENT_DYNAMIC(CWinDlg, CDialogEx)

CWinDlg::CWinDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WIN_DIALOG, pParent)
{

}

CWinDlg::~CWinDlg()
{
}

void CWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WIN_LIST, m_winList);
}


BEGIN_MESSAGE_MAP(CWinDlg, CDialogEx)
END_MESSAGE_MAP()


// CWinDlg ��Ϣ�������

BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	MYWINDOWINFO* WindowInfo = new MYWINDOWINFO;
	_stprintf_s(WindowInfo->hWnd, "%p", hWnd);
	::GetWindowText(hWnd, WindowInfo->WindowName, MAX_PATH);
	::GetClassName(hWnd, WindowInfo->ClassName, MAX_PATH);
	if (::GetWindowLong(hWnd, GWL_STYLE)&WS_VISIBLE&&WindowInfo->WindowName[0])
	{
		vecWindowInfo.push_back(*WindowInfo);
	}
	return true;
}


BOOL CWinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_winList.AddColumn(3, "���ھ��", 100, "���ڱ���", 200, "��������", 210);
	::EnumWindows(EnumWindowProc, 0); //  ö��������Ļ�ϵĶ��㴰��
	for (size_t i = 0; i < vecWindowInfo.size(); i++)
	{
		m_winList.AddItem(3,
			vecWindowInfo[i].hWnd,
			vecWindowInfo[i].WindowName,
			vecWindowInfo[i].ClassName);
	}

	return TRUE;  
}


