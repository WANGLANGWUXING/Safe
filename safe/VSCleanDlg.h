#pragma once


// CVSCleanDlg �Ի���

class CVSCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVSCleanDlg)

public:
	CVSCleanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVSCleanDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VSCLEAN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �ļ���ק
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// ��ĿĿ¼
	CString m_strVSProjectPath;
	// ɾ���ļ���Ŀ¼
	bool DelFile(CString fileName);
	void DelDirectory(CString strDir);
	afx_msg void OnBnClickedButton1();
};
