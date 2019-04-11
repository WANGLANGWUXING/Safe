#pragma once
#include "afxcmn.h"
#include "PEInfo.h"
#include <vector>
using std::vector;
typedef struct _res_count {
	WORD nameCount;
	WORD idCount;
	CString resName;
}RES_COUNT;

typedef struct _res_info {
	DWORD RVA;
	DWORD Offset;
	DWORD Size;
	CString Name;
}RES_INFO;

// CResourceDlg 对话框

class CResourceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResourceDlg)

public:
	CResourceDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResourceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESOURCE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	char *m_pFileBuf = NULL;
	PIMAGE_NT_HEADERS m_pNt;
	RES_COUNT *pResCount;
	int resCount;
	vector<RES_INFO> vecResInfo;
	CTreeCtrl m_treeResource;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CString m_strRootNameCount;
	CString m_strRootIdCount;
	afx_msg void OnTvnSelchangedTreeResource(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_strChildNameCount;
	CString m_strChildIdCount;
	CString m_strItemRva;
	CString m_strItemOffset;
	CString m_strItemSize;
};
