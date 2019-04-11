#pragma once


// MyListCtrl

class MyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(MyListCtrl)

public:
	MyListCtrl();
	virtual ~MyListCtrl();
	void AddColumn(DWORD dwCount, ...);					//插入列名;
	void AddItem(DWORD dwCount, ...);					//插入行元素;
protected:
	DECLARE_MESSAGE_MAP()
};


