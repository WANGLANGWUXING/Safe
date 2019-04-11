#pragma once


// MyTabCtrl

class MyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(MyTabCtrl)

public:
	MyTabCtrl();
	virtual ~MyTabCtrl();

protected:

	
	DECLARE_MESSAGE_MAP()
public:
	void MyInserItem(DWORD dwCount, ...);	//插入选项	
	void MyInsertChild(DWORD dwCount, ...);	//插入子对话框
	DWORD m_dwCount;						//保存子对话框个数;
	CDialogEx* m_pWnd[15];					//对话框指针数组 子窗口数组;
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


