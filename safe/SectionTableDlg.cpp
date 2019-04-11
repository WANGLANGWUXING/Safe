// SectionTableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "SectionTableDlg.h"
#include "afxdialogex.h"


// CSectionTableDlg 对话框

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


// CSectionTableDlg 消息处理程序


BOOL CSectionTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_sectionList.InsertColumn(0, "名称", 0, 50);
	m_sectionList.InsertColumn(1, "起始的相对虚拟地址(VOffset)", 0, 180);
	m_sectionList.InsertColumn(2, "区段大小(加载后)(VSize)", 0, 145);
	m_sectionList.InsertColumn(3, "文件偏移(ROffset)", 0, 120);
	m_sectionList.InsertColumn(4, "区段大小(文件中)(RSize)", 0, 145);
	m_sectionList.InsertColumn(5, "区段的属性(标志)", 0, 120);
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
