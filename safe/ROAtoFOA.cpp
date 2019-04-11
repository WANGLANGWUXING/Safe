// ROAtoFOA.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "ROAtoFOA.h"
#include "afxdialogex.h"
#include "PeInfo.h"
#include "common.h"
#include <time.h>
// CROAtoFOA 对话框

IMPLEMENT_DYNAMIC(CROAtoFOA, CDialogEx)

CROAtoFOA::CROAtoFOA(char *pFileBuf,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_strROA(_T(""))
	, m_strFOA(_T(""))
	, m_strTimeBefore(_T(""))
	, m_strTimeAfter(_T(""))
{
	m_pFileBuf = pFileBuf;
}

CROAtoFOA::~CROAtoFOA()
{
}

void CROAtoFOA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITRVA, m_strROA);
	DDX_Text(pDX, IDC_EDITFOA, m_strFOA);
	DDX_Text(pDX, IDC_EDIT2, m_strTimeBefore);
	DDX_Text(pDX, IDC_EDIT3, m_strTimeAfter);
}


BEGIN_MESSAGE_MAP(CROAtoFOA, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CROAtoFOA::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CROAtoFOA::OnBnClickedButton2)
END_MESSAGE_MAP()


// CROAtoFOA 消息处理程序


void CROAtoFOA::OnBnClickedButton1()
{
	UpdateData(TRUE);
	char szRVA[10];
	strcpy_s(szRVA, m_strROA);
	DWORD dwRVA = hexToDec(szRVA);
	DWORD dwFOA = RVAtoFOA(dwRVA, m_pFileBuf);
	m_strFOA.Format("%08X", dwFOA);
	UpdateData(FALSE);
}


void CROAtoFOA::OnBnClickedButton2()
{
	UpdateData(TRUE);
	char szBeforeTime[20];
	strcpy_s(szBeforeTime, m_strTimeBefore);
	time_t dwBeforeTime = hexToDec(szBeforeTime);
	time_t *llTime = new time_t();
	llTime = &dwBeforeTime;
	tm* fileTime = gmtime(llTime);
	
	m_strTimeAfter.Format("%d年%d月%d日 %2d:%2d:%2d", 
		fileTime->tm_year + 1900, fileTime->tm_mon + 1, fileTime->tm_mday,
		fileTime->tm_hour, fileTime->tm_min, fileTime->tm_sec);
	UpdateData(FALSE);
}
