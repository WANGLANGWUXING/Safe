// MyTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTabCtrl.h"


// MyTabCtrl

IMPLEMENT_DYNAMIC(MyTabCtrl, CTabCtrl)

MyTabCtrl::MyTabCtrl()
{

}

MyTabCtrl::~MyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(MyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &MyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()

// MyTabCtrl 消息处理程序




void MyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	//检索当前选项卡选择项;
	DWORD nSel = GetCurSel();
	//隐藏所有;
	for (size_t i = 0; i < m_dwCount; i++)
	{
		m_pWnd[i]->ShowWindow(SW_HIDE);
	}
	//出错结束;
	if (nSel >= m_dwCount)
	{
		return;
	}

	//移动窗口和TAB客户区一样大;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 22, 0, 0);		//TOP值减小;
	m_pWnd[nSel]->MoveWindow(&rt, 1);
	m_pWnd[nSel]->ShowWindow(SW_SHOW);
}

void MyTabCtrl::MyInserItem(DWORD dwCount, ...)
{
	//变参添加;
	va_list va;
	va_start(va, dwCount);
	for (DWORD i = 0; i < dwCount; i++)
	{
		TCHAR *p = va_arg(va, TCHAR*);
		InsertItem(i, p);
	}
	va_end(va);
}
void MyTabCtrl::MyInsertChild(DWORD dwCount, ...)
{
	m_dwCount = dwCount;
	va_list va;
	va_start(va, dwCount);
	for (DWORD i = 0; i < dwCount; i++)
	{
		m_pWnd[i] = va_arg(va, CDialogEx*);
		UINT uID = va_arg(va, UINT);
		m_pWnd[i]->Create(uID, this);
	}
	va_end(va);

	//移动窗口和客户区一样大;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 23, 0, 0);
	m_pWnd[0]->MoveWindow(&rt, 1);
	m_pWnd[0]->ShowWindow(SW_SHOW);
}