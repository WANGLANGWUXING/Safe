#pragma once


// MyListCtrl

class MyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(MyListCtrl)

public:
	MyListCtrl();
	virtual ~MyListCtrl();
	void AddColumn(DWORD dwCount, ...);					//��������;
	void AddItem(DWORD dwCount, ...);					//������Ԫ��;
protected:
	DECLARE_MESSAGE_MAP()
};


