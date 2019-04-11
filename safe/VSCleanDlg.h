#pragma once


// CVSCleanDlg 对话框

class CVSCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVSCleanDlg)

public:
	CVSCleanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVSCleanDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VSCLEAN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 文件拖拽
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// 项目目录
	CString m_strVSProjectPath;
	// 删除文件、目录
	bool DelFile(CString fileName);
	void DelDirectory(CString strDir);
	afx_msg void OnBnClickedButton1();
};
