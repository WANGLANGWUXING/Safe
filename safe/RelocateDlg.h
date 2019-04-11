#pragma once
#include "MyListCtrl.h"
#include "PeInfo.h"

typedef struct
{
	WORD Offset : 12; //相对VirtualAddress的页内偏移
	WORD Type : 4;    //重定位类型（3：重定位4个字节，0：无需重定位）
}TypeOffset;
// CRelocateDlg 对话框

class CRelocateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocateDlg)

public:
	CRelocateDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRelocateDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RELOCATE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	char *m_pFileBuf = NULL;
	PIMAGE_NT_HEADERS m_pNt;
	MyListCtrl m_listRelocate;
	MyListCtrl m_listRelocateRva;
	virtual BOOL OnInitDialog();
	void ShowRelocateList();
	void ShowRelocateRVAList(int item);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
