// ModuleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "ModuleDlg.h"
#include "afxdialogex.h"


// CModuleDlg 对话框

IMPLEMENT_DYNAMIC(CModuleDlg, CDialogEx)

CModuleDlg::CModuleDlg(int pid,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MODULE_DIALOG, pParent)
{
	m_pid = pid;
}

CModuleDlg::~CModuleDlg()
{
}

void CModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_moduleList);
}


BEGIN_MESSAGE_MAP(CModuleDlg, CDialogEx)
END_MESSAGE_MAP()


// CModuleDlg 消息处理程序


BOOL CModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ShowModule(m_pid);

	return TRUE; 
}

void CModuleDlg::ShowModule(int nPid)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPid);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox("打开进程信息失败！");
		EndDialog(0);
		return;
	}
	MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
	if (!Module32First(hSnapShot, &me))
	{
		MessageBox("获取模块信息失败！");
		EndDialog(0);
		return;
	}
	m_moduleList.AddColumn(3, "模块名称", 100, "模块大小", 100, "模块路径", 290);
	do
	{
		TCHAR* SizeBuf = new TCHAR[11];
		_stprintf_s(SizeBuf, 10, "%d", me.modBaseSize);
		m_moduleList.AddItem(3, me.szModule, SizeBuf, me.szExePath);
	} while (Module32Next(hSnapShot, &me));

	return;
}