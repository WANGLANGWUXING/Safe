// DelayImportDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "DelayImportDlg.h"
#include "afxdialogex.h"


// CDelayImportDlg �Ի���

IMPLEMENT_DYNAMIC(CDelayImportDlg, CDialogEx)

CDelayImportDlg::CDelayImportDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DELAYIMPORT_DIALOG, pParent)
{
	m_pFileBuf = pFileBuf;
	m_pNt = pNt;
}

CDelayImportDlg::~CDelayImportDlg()
{
}

void CDelayImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTDELAYDLL, m_listDelayDLL);
	DDX_Control(pDX, IDC_LISTDELAYFUN, m_listDelayFun);
}


BEGIN_MESSAGE_MAP(CDelayImportDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LISTDELAYDLL, &CDelayImportDlg::OnNMClickListdelaydll)
END_MESSAGE_MAP()


// CDelayImportDlg ��Ϣ�������




BOOL CDelayImportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listDelayDLL.AddColumn(3, "DLL����", 200, "INT(RVA)", 120, "IAT(RVA)", 120);
	m_listDelayFun.AddColumn(3, "��������", 200, "Thunk Value", 120, "HINT", 120);

	//������RVA
	DWORD dwImportRVA =
		m_pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress;
	//��RVAת��FOA
	DWORD dwImportFOA = RVAtoFOA(dwImportRVA, m_pFileBuf);
	//��ȡ���ļ��е�ʵ�ʵ�ַ
	PIMAGE_DELAYLOAD_DESCRIPTOR pImport =
		(PIMAGE_DELAYLOAD_DESCRIPTOR)(dwImportFOA + m_pFileBuf);

	CString strDllName;
	CString strINIRVA;
	CString strIAIRVA;
	while (pImport->DllNameRVA)
	{
		//ģ������
		//printf("ģ������:%s\n", RVAtoFOA(pImport->Name, m_pFileBuf) + m_pFileBuf);
		strDllName.Format("%s", RVAtoFOA(pImport->DllNameRVA, m_pFileBuf) + m_pFileBuf);
		strINIRVA.Format("%08X", pImport->ImportNameTableRVA);
		strIAIRVA.Format("%08X", pImport->ImportAddressTableRVA);
		m_listDelayDLL.AddItem(3, strDllName, strINIRVA, strIAIRVA);
		pImport++;
	}
	return TRUE;  
}


void CDelayImportDlg::OnNMClickListdelaydll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nSel = pNMItemActivate->iItem;
	if (nSel < 0)
	{
		MessageBox("��ѡ������");
		return;
	}

	CString strOriginalFirstThunk = m_listDelayDLL.GetItemText(nSel, 1);
	char szOriginalFirstThunk[10];
	strcpy_s(szOriginalFirstThunk, strOriginalFirstThunk);
	DWORD OriginalFirstThunk = hexToDec(szOriginalFirstThunk);

	CString strThunkValue;
	CString strHint;
	CString strApiName;

	m_listDelayFun.DeleteAllItems();
	//��������ģ���е���Щ����
	//��ȡINT�ĵ�ַ
	PIMAGE_THUNK_DATA pINTAddr = (PIMAGE_THUNK_DATA)
		(RVAtoFOA(OriginalFirstThunk, m_pFileBuf) + m_pFileBuf);
	while (pINTAddr->u1.Ordinal)
	{

		strThunkValue.Format("%08X", pINTAddr->u1.AddressOfData);

		//�жϺ����ĵ��뷽ʽ
		//������ΪΪ1˵������ŵ���,����������Ƶ���
		//IMAGE_SNAP_BY_ORDINAL32
		if (pINTAddr->u1.AddressOfData & 0x80000000)
		{
			//��ŵ��룬��ô���2�ֽھ����亯�����
			/*printf(" -->name:[NULL]  ordinal:[%04X]\n",
			pINTAddr->u1.AddressOfData & 0xFFFF);*/
			strHint.Format("%04X", pINTAddr->u1.AddressOfData & 0xFFFF);
			strApiName.Format("-");
		}
		else
		{
			//���Ƶ���
			DWORD dwNameRVA = pINTAddr->u1.AddressOfData;
			DWORD dwNameFOA = RVAtoFOA(dwNameRVA, m_pFileBuf);
			PIMAGE_IMPORT_BY_NAME pName =
				(PIMAGE_IMPORT_BY_NAME)
				(dwNameFOA + m_pFileBuf);
			/*printf(" -->name:[%s]  ordinal:[%04X]\n",
			pName->Name, pName->Hint);*/
			strHint.Format("%04X", pName->Hint);
			strApiName.Format("%s", pName->Name);

		}
		m_listDelayFun.AddItem(3, strApiName, strThunkValue, strHint);

		OriginalFirstThunk += 4;
		//��һ������
		pINTAddr++;
	}


	*pResult = 0;
}
