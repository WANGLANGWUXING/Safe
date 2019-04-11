// MyListCtrl.cpp : 实现文件
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
	//设置列表框属性;
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	va_list vlist;
	va_start(vlist, dwCount);
	//循环获取参数;
	for (DWORD i = 0; i < dwCount; i++)
	{
		TCHAR * pName = va_arg(vlist, TCHAR*);
		int	nWidth = va_arg(vlist, int);

		//插入行;
		InsertColumn(i, pName, NULL, nWidth);
	}
	va_end(vlist);
}
void MyListCtrl::AddItem(DWORD dwCount, ...)
{
	//获取行数;
	int nItemNum = GetItemCount();

	//初始化;
	va_list vlist;
	va_start(vlist, dwCount);

	//添加行;
	TCHAR* pText = va_arg(vlist, TCHAR*);
	InsertItem(nItemNum, pText);

	//设置其他行;
	for (DWORD i = 1; i < dwCount; i++)
	{
		//设置内容;
		pText = va_arg(vlist, TCHAR*);
		SetItemText(nItemNum, i, pText);
	}

	va_end(vlist);
}
BEGIN_MESSAGE_MAP(MyListCtrl, CListCtrl)
END_MESSAGE_MAP()



// MyListCtrl 消息处理程序


