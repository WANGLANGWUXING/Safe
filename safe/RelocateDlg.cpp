// RelocateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "RelocateDlg.h"
#include "afxdialogex.h"


// CRelocateDlg �Ի���

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


// CRelocateDlg ��Ϣ�������


BOOL CRelocateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listRelocate.AddColumn(4, "����", 80, "����", 60,
		"RVA", 120,  "����", 80
		);
	m_listRelocateRva.AddColumn(5, "����", 40, "RVA", 80, "ƫ��", 80, "����", 80,"FAR��ַ",80);
	ShowRelocateList();
	

	return TRUE;  
}

void CRelocateDlg::ShowRelocateList()
{
	//�ҵ��ض�λ��
	//����Ŀ¼���еĵ�6��±�Ϊ5��
	
	//������RVA
	DWORD dwRelocRVA =
		m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	//��RVAת��FOA
	DWORD dwRelocFOA = RVAtoFOA(dwRelocRVA, m_pFileBuf);
	//��ȡ���ļ��е�ʵ�ʵ�ַ
	PIMAGE_BASE_RELOCATION pReloc =
		(PIMAGE_BASE_RELOCATION)(dwRelocFOA + m_pFileBuf);
	//�ض�λ��ṹ��
	struct TypeOffset
	{
		WORD Offset : 12; //���VirtualAddress��ҳ��ƫ��
		WORD type : 4;    //�ض�λ���ͣ�3���ض�λ4���ֽڣ�0�������ض�λ��
	};
	CString index;
	CString sectionName;
	CString relocRva;
	CString count;
	int i = 0;
	while (pReloc->VirtualAddress)
	{
		i++;
		
		//�����ض�λ��ĸ���
		DWORD dwCount =
			(pReloc->SizeOfBlock -
				sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);

		index.Format("%d", i);
		RVAtoFOA(pReloc->VirtualAddress, m_pFileBuf, sectionName);

		relocRva.Format("%08X\n", pReloc->VirtualAddress);
		count.Format("%Xh/%dd", dwCount, dwCount);

		m_listRelocate.AddItem(4, index, sectionName, relocRva, count);
		////��һ���ض�λ���ַ
		//TypeOffset* pOffset = (TypeOffset*)(pReloc + 1);

		//for (int i = 0; i < dwCount; i++)
		//{
		//	//typeΪ3˵����Ҫ�ض�λ4���ֽ�
		//	if (pOffset->type == 3)
		//	{
		//		//�ض�λ���RVA
		//		DWORD dwRVA =
		//			pOffset->Offset +
		//			pReloc->VirtualAddress;
		//		//�ض�λ���FOA
		//		DWORD dwFOA = RVAtoFOA(dwRVA, m_pFileBuf);
		//		//�ض�λ�������ļ��еľ����ַ
		//		DWORD* pRelocDataAddr =
		//			(DWORD*)(dwFOA + m_pFileBuf);
		//		//ȡ����Ҫ�ض�λ������
		//		printf("�ض�λ�����ݣ�%08X\n", *pRelocDataAddr);
		//	}
		//	//��һ���ض�λ��
		//	pOffset++;
		//}
		//��һ���ض�λ���ַ
		pReloc =
			(PIMAGE_BASE_RELOCATION)
			((DWORD)pReloc + pReloc->SizeOfBlock);
	}

}


void CRelocateDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMItemActivate->iItem == -1)return;

	ShowRelocateRVAList(pNMItemActivate->iItem);
	*pResult = 0;
}
void CRelocateDlg::ShowRelocateRVAList(int item)
{
	m_listRelocateRva.DeleteAllItems();
	//������RVA
	DWORD dwRelocRVA =
		m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	//��RVAת��FOA
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

