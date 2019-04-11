#pragma once

#include "PEInfo.h"
#include "SectionTableDlg.h"
#include "DataDirectoryDlg.h"
#include "ROAtoFOA.h"
// CPEDlg �Ի���

class CPEDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPEDlg)

public:
	CPEDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPEDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_strFilePath;
	CString m_strOEP;
	CString m_strSubSystem;
	CString m_strImageBase;
	CString m_strNumberOfSections;
	CString m_strSizeOfImage;
	CString m_strTimeDateStamp;
	CString m_strBaseOfCode;
	CString m_strSizeOfHeaders;
	CString m_strBaseOfData;
	CString m_strCharacteristics;
	CString m_strSectionAlignment;
	CString m_strCheckSum;
	CString m_strFileAlignment;
	CString m_strSizeOfOptionalHeader;
	CString m_strMagic;
	CString m_strNumberOfRvaAndSizes;
	// �ļ�ָ�룬PEͷ
	char *pFileBuf;
	PIMAGE_DOS_HEADER pDos;
	PIMAGE_NT_HEADERS pNt;
	afx_msg void OnBnClickedButtonsectionheader();
	afx_msg void OnBnClickedBtndatadirectory();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
