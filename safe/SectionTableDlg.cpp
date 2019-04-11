// SectionTableDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "SectionTableDlg.h"
#include "afxdialogex.h"


// CSectionTableDlg �Ի���

IMPLEMENT_DYNAMIC(CSectionTableDlg, CDialogEx)

CSectionTableDlg::CSectionTableDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SECTIONTABLE_DIALOG, pParent)
{
	m_pFileBuf = pFileBuf;
	m_pNt = pNt;

}

CSectionTableDlg::~CSectionTableDlg()
{
}

void CSectionTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_sectionList);
}


BEGIN_MESSAGE_MAP(CSectionTableDlg, CDialogEx)
END_MESSAGE_MAP()


// CSectionTableDlg ��Ϣ�������


BOOL CSectionTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_sectionList.InsertColumn(0, "����", 0, 50);
	m_sectionList.InsertColumn(1, "��ʼ����������ַ(VOffset)", 0, 180);
	m_sectionList.InsertColumn(2, "���δ�С(���غ�)(VSize)", 0, 145);
	m_sectionList.InsertColumn(3, "�ļ�ƫ��(ROffset)", 0, 120);
	m_sectionList.InsertColumn(4, "���δ�С(�ļ���)(RSize)", 0, 145);
	m_sectionList.InsertColumn(5, "���ε�����(��־)", 0, 120);
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNt);
	CString strName;
	CString strVirtualAddress;
	CString strSizeOfRawData;
	CString strPointerToRawData;
	CString strVirtualSize;
	CString strCharacteristics;

	for (DWORD i = 0; i < m_pNt->FileHeader.NumberOfSections; i++)
	{
		strVirtualAddress.Format("%08X", pSectionHeader->VirtualAddress);
		strSizeOfRawData.Format("%08X", pSectionHeader->SizeOfRawData);
		strPointerToRawData.Format("%08X", pSectionHeader->PointerToRawData);
		strVirtualSize.Format("%08X", pSectionHeader->Misc.VirtualSize);
		strCharacteristics.Format("%08X", pSectionHeader->Characteristics);
		LVITEM *listItem = new LVITEM();
		listItem->mask = LVIF_TEXT;
		listItem->iItem = i;

		listItem->pszText = (LPSTR)pSectionHeader->Name;
		m_sectionList.InsertItem(listItem);
		m_sectionList.SetItemText(i, 1, (LPCTSTR)strVirtualAddress);
		m_sectionList.SetItemText(i, 2, (LPCTSTR)strVirtualSize);
		m_sectionList.SetItemText(i, 3, (LPCTSTR)strPointerToRawData);
		m_sectionList.SetItemText(i, 4, (LPCTSTR)strSizeOfRawData);
		m_sectionList.SetItemText(i, 5, (LPCTSTR)strCharacteristics);


		pSectionHeader++;
	}
	

	return TRUE;  
}
