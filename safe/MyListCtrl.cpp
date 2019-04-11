// MyListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyListCtrl.h"


// MyListCtrl

IMPLEMENT_DYNAMIC(MyListCtrl, CListCtrl)

MyListCtrl::MyListCtrl()
{

}

MyListCtrl::~MyListCtrl()
{
}

void MyListCtrl::AddColumn(DWORD dwCount, ...)
{
	//�����б������;
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	va_list vlist;
	va_start(vlist, dwCount);
	//ѭ����ȡ����;
	for (DWORD i = 0; i < dwCount; i++)
	{
		TCHAR * pName = va_arg(vlist, TCHAR*);
		int	nWidth = va_arg(vlist, int);

		//������;
		InsertColumn(i, pName, NULL, nWidth);
	}
	va_end(vlist);
}
void MyListCtrl::AddItem(DWORD dwCount, ...)
{
	//��ȡ����;
	int nItemNum = GetItemCount();

	//��ʼ��;
	va_list vlist;
	va_start(vlist, dwCount);

	//�����;
	TCHAR* pText = va_arg(vlist, TCHAR*);
	InsertItem(nItemNum, pText);

	//����������;
	for (DWORD i = 1; i < dwCount; i++)
	{
		//��������;
		pText = va_arg(vlist, TCHAR*);
		SetItemText(nItemNum, i, pText);
	}

	va_end(vlist);
}
BEGIN_MESSAGE_MAP(MyListCtrl, CListCtrl)
END_MESSAGE_MAP()



// MyListCtrl ��Ϣ�������


