#pragma once


// CROAtoFOA �Ի���

class CROAtoFOA : public CDialogEx
{
	DECLARE_DYNAMIC(CROAtoFOA)

public:
	CROAtoFOA(char *pFileBuf,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CROAtoFOA();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	char *m_pFileBuf;
	CString m_strROA;
	CString m_strFOA;
	afx_msg void OnBnClickedButton1();
	CString m_strTimeBefore;
	CString m_strTimeAfter;
	afx_msg void OnBnClickedButton2();
};
