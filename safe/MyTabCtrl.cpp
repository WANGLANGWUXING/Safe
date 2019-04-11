// MyTabCtrl.cpp : ʵ���ļ�
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

// MyTabCtrl ��Ϣ�������




void MyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//������ǰѡ�ѡ����;
	DWORD nSel = GetCurSel();
	//��������;
	for (size_t i = 0; i < m_dwCount; i++)
	{
		m_pWnd[i]->ShowWindow(SW_HIDE);
	}
	//�������;
	if (nSel >= m_dwCount)
	{
		return;
	}

	//�ƶ����ں�TAB�ͻ���һ����;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 22, 0, 0);		//TOPֵ��С;
	m_pWnd[nSel]->MoveWindow(&rt, 1);
	m_pWnd[nSel]->ShowWindow(SW_SHOW);
}

void MyTabCtrl::MyInserItem(DWORD dwCount, ...)
{
	//������;
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

	//�ƶ����ںͿͻ���һ����;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 23, 0, 0);
	m_pWnd[0]->MoveWindow(&rt, 1);
	m_pWnd[0]->ShowWindow(SW_SHOW);
}