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
	void MyInserItem(DWORD dwCount, ...);	//����ѡ��	
	void MyInsertChild(DWORD dwCount, ...);	//�����ӶԻ���
	DWORD m_dwCount;						//�����ӶԻ������;
	CDialogEx* m_pWnd[15];					//�Ի���ָ������ �Ӵ�������;
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


