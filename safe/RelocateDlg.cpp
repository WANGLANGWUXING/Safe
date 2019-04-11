// RelocateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "RelocateDlg.h"
#include "afxdialogex.h"


// CRelocateDlg 对话框

IMPLEMENT_DYNAMIC(CRelocateDlg, CDialogEx)

CRelocateDlg::CRelocateDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RELOCATE_DIALOG, pParent)
{
	m_pFileBuf = pFileBuf;
	m_pNt = pNt;
}

CRelocateDlg::~CRelocateDlg()
{
}

void CRelocateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listRelocate);
	DDX_Control(pDX, IDC_LIST2, m_listRelocateRva);
}


BEGIN_MESSAGE_MAP(CRelocateDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CRelocateDlg::OnNMClickList1)
END_MESSAGE_MAP()


// CRelocateDlg 消息处理程序


BOOL CRelocateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listRelocate.AddColumn(4, "索引", 80, "区段", 60,
		"RVA", 120,  "数量", 80
		);
	m_listRelocateRva.AddColumn(5, "索引", 40, "RVA", 80, "偏移", 80, "属性", 80,"FAR地址",80);
	ShowRelocateList();
	

	return TRUE;  
}

void CRelocateDlg::ShowRelocateList()
{
	//找到重定位表
	//数据目录表中的第6项（下标为5）
	
	//导入表的RVA
	DWORD dwRelocRVA =
		m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	//将RVA转成FOA
	DWORD dwRelocFOA = RVAtoFOA(dwRelocRVA, m_pFileBuf);
	//获取在文件中的实际地址
	PIMAGE_BASE_RELOCATION pReloc =
		(PIMAGE_BASE_RELOCATION)(dwRelocFOA + m_pFileBuf);
	//重定位项结构体
	struct TypeOffset
	{
		WORD Offset : 12; //相对VirtualAddress的页内偏移
		WORD type : 4;    //重定位类型（3：重定位4个字节，0：无需重定位）
	};
	CString index;
	CString sectionName;
	CString relocRva;
	CString count;
	int i = 0;
	while (pReloc->VirtualAddress)
	{
		i++;
		
		//计算重定位项的个数
		DWORD dwCount =
			(pReloc->SizeOfBlock -
				sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);

		index.Format("%d", i);
		RVAtoFOA(pReloc->VirtualAddress, m_pFileBuf, sectionName);

		relocRva.Format("%08X\n", pReloc->VirtualAddress);
		count.Format("%Xh/%dd", dwCount, dwCount);

		m_listRelocate.AddItem(4, index, sectionName, relocRva, count);
		////第一个重定位项地址
		//TypeOffset* pOffset = (TypeOffset*)(pReloc + 1);

		//for (int i = 0; i < dwCount; i++)
		//{
		//	//type为3说明需要重定位4个字节
		//	if (pOffset->type == 3)
		//	{
		//		//重定位项的RVA
		//		DWORD dwRVA =
		//			pOffset->Offset +
		//			pReloc->VirtualAddress;
		//		//重定位项的FOA
		//		DWORD dwFOA = RVAtoFOA(dwRVA, m_pFileBuf);
		//		//重定位数据在文件中的具体地址
		//		DWORD* pRelocDataAddr =
		//			(DWORD*)(dwFOA + m_pFileBuf);
		//		//取出需要重定位的数据
		//		printf("重定位的数据：%08X\n", *pRelocDataAddr);
		//	}
		//	//下一个重定位项
		//	pOffset++;
		//}
		//下一个重定位块地址
		pReloc =
			(PIMAGE_BASE_RELOCATION)
			((DWORD)pReloc + pReloc->SizeOfBlock);
	}

}


void CRelocateDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem == -1)return;

	ShowRelocateRVAList(pNMItemActivate->iItem);
	*pResult = 0;
}
void CRelocateDlg::ShowRelocateRVAList(int item)
{
	m_listRelocateRva.DeleteAllItems();
	//导入表的RVA
	DWORD dwRelocRVA =
		m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	//将RVA转成FOA
	DWORD dwRelocFOA = RVAtoFOA(dwRelocRVA, m_pFileBuf);
	PIMAGE_BASE_RELOCATION pReloc =
		(PIMAGE_BASE_RELOCATION)(dwRelocFOA + m_pFileBuf);
	//IMAGE_BASE_RELOCATION* pRelTab = (IMAGE_BASE_RELOCATION*)(dwRelTabFoa);

	CString nTemp;

	for (int i = 0; i < item; i++)
	{
		pReloc = (PIMAGE_BASE_RELOCATION)
			((DWORD)pReloc + pReloc->SizeOfBlock);
	}

	DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
	TypeOffset* pTypeOffset = NULL;
	pTypeOffset = (TypeOffset*)(pReloc + 1);

	CString index;
	CString relRva;
	CString relFoa;
	CString relAttr;
	CString redFAR;

	for (DWORD i = 0; i < dwCount; ++i) {

		index.Format("%d", i+1);
		DWORD dwROA = pTypeOffset[i].Offset + pReloc->VirtualAddress;
		relRva.Format("%08X", dwROA);
		DWORD dwFOA = RVAtoFOA(dwROA, m_pFileBuf);
		relFoa.Format("%08X", dwFOA);
		relAttr.Format("HIGHLOW(%d)", pTypeOffset[i].Type);

		DWORD* pRelocDataAddr =
			(DWORD*)(dwFOA + m_pFileBuf);
		redFAR.Format("%08X", *pRelocDataAddr);
		m_listRelocateRva.AddItem(5, index, relRva, relFoa, relAttr, redFAR);
	}
}

