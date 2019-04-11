#pragma once
#include "MyListCtrl.h"
#include "PeInfo.h"

typedef struct
{
	WORD Offset : 12; //���VirtualAddress��ҳ��ƫ��
	WORD Type : 4;    //�ض�λ���ͣ�3���ض�λ4���ֽڣ�0�������ض�λ��
}TypeOffset;
// CRelocateDlg �Ի���

class CRelocateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocateDlg)

public:
	CRelocateDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRelocateDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RELOCATE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
