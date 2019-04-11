// ImportDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "ImportDlg.h"
#include "afxdialogex.h"


// CImportDlg �Ի���

IMPLEMENT_DYNAMIC(CImportDlg, CDialogEx)

CImportDlg::CImportDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMPORT_DIALOG, pParent)
{
	m_pFileBuf = pFileBuf;
	m_pNt = pNt;
}

CImportDlg::~CImportDlg()
{
}

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IMPORTDLL, m_listImprtDll);
	DDX_Control(pDX, IDC_LIST_IMPORTTHUNK, m_listImprtThunk);
}


BEGIN_MESSAGE_MAP(CImportDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IMPORTDLL, &CImportDlg::OnNMClickListImportdll)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CImportDlg ��Ϣ�������


BOOL CImportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listImprtDll.AddColumn(6, "DLL����",100, 
		"OriginalFirstThunk",100, "����ʱ���־", 100,
		"ForwarderChain", 100, "����",80, "FirstThunk",80);
	m_listImprtThunk.AddColumn(5, "ThunkRVA", 100, "Thunkƫ��", 100, "Thunkֵ", 100, "��ʾ", 100, "API����", 150);

	//������RVA
	DWORD dwImportRVA =
		m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	//��RVAת��FOA
	DWORD dwImportFOA = RVAtoFOA(dwImportRVA, m_pFileBuf);
	//��ȡ���ļ��е�ʵ�ʵ�ַ
	PIMAGE_IMPORT_DESCRIPTOR pImport =
		(PIMAGE_IMPORT_DESCRIPTOR)(dwImportFOA + m_pFileBuf);
	TCHAR* importDlls[] = { new TCHAR[MAX_PATH], new TCHAR[9], new TCHAR[9], new TCHAR[9], new TCHAR[9], new TCHAR[9] };
	CString strModuleName;
	CString strOFT;
	CString strTime;
	CString strForwarderChain;
	CString strName;
	CString strFT;
	while (pImport->Name)
	{
		strModuleName.Format("%s", RVAtoFOA(pImport->Name, m_pFileBuf) + m_pFileBuf);
		strOFT.Format("%08X", pImport->OriginalFirstThunk);
		strTime.Format("%08X", pImport->TimeDateStamp);
		strForwarderChain.Format("%08X", pImport->ForwarderChain);
		strName.Format("%08X", pImport->Name);
		strFT.Format("%08X", pImport->FirstThunk);
		m_listImprtDll.AddItem(6, strModuleName, strOFT, strTime, strForwarderChain, strName, strFT);
		pImport++;
	}



	return TRUE;  
}


void CImportDlg::OnNMClickListImportdll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	int nSel = pNMItemActivate->iItem;
	if (nSel < 0)
	{
		MessageBox("��ѡ������");
		return;
	}
	CString strOriginalFirstThunk = m_listImprtDll.GetItemText(nSel, 1);
	char szOriginalFirstThunk[10];
	strcpy_s(szOriginalFirstThunk, strOriginalFirstThunk);
	DWORD OriginalFirstThunk = hexToDec(szOriginalFirstThunk);
	
	CString strThunkRVA;
	CString strThunkFOA;
	CString strThunkValue;
	CString strHint;
	CString strApiName;

	m_listImprtThunk.DeleteAllItems();
	//��������ģ���е���Щ����
	//��ȡINT�ĵ�ַ
	PIMAGE_THUNK_DATA pINTAddr = (PIMAGE_THUNK_DATA)
		(RVAtoFOA(OriginalFirstThunk, m_pFileBuf) + m_pFileBuf);
	while (pINTAddr->u1.Ordinal)
	{
		DWORD dwThunkRVA = OriginalFirstThunk;
		DWORD dwThunkFOA = RVAtoFOA(dwThunkRVA, m_pFileBuf);
		strThunkRVA.Format("%08X", dwThunkRVA);
		strThunkFOA.Format("%08X", dwThunkFOA);
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
		m_listImprtThunk.AddItem(5, strThunkRVA, strThunkFOA, strThunkValue, strHint, strApiName);

		OriginalFirstThunk += 4;
		//��һ������
		pINTAddr++;
	}
	

	*pResult = 0;
}


void CImportDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//EndDialog(0);
	CDialogEx::OnClose();
}
