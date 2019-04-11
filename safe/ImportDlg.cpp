// ImportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "ImportDlg.h"
#include "afxdialogex.h"


// CImportDlg 对话框

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


// CImportDlg 消息处理程序


BOOL CImportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listImprtDll.AddColumn(6, "DLL名称",100, 
		"OriginalFirstThunk",100, "日期时间标志", 100,
		"ForwarderChain", 100, "名称",80, "FirstThunk",80);
	m_listImprtThunk.AddColumn(5, "ThunkRVA", 100, "Thunk偏移", 100, "Thunk值", 100, "提示", 100, "API名称", 150);

	//导入表的RVA
	DWORD dwImportRVA =
		m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	//将RVA转成FOA
	DWORD dwImportFOA = RVAtoFOA(dwImportRVA, m_pFileBuf);
	//获取在文件中的实际地址
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
	// TODO: 在此添加控件通知处理程序代码	
	int nSel = pNMItemActivate->iItem;
	if (nSel < 0)
	{
		MessageBox("请选中数据");
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
	//遍历导入模块中的哪些函数
	//获取INT的地址
	PIMAGE_THUNK_DATA pINTAddr = (PIMAGE_THUNK_DATA)
		(RVAtoFOA(OriginalFirstThunk, m_pFileBuf) + m_pFileBuf);
	while (pINTAddr->u1.Ordinal)
	{
		DWORD dwThunkRVA = OriginalFirstThunk;
		DWORD dwThunkFOA = RVAtoFOA(dwThunkRVA, m_pFileBuf);
		strThunkRVA.Format("%08X", dwThunkRVA);
		strThunkFOA.Format("%08X", dwThunkFOA);
		strThunkValue.Format("%08X", pINTAddr->u1.AddressOfData);
		
		//判断函数的导入方式
		//如果最高为为1说明是序号导入,否则就是名称导入
		//IMAGE_SNAP_BY_ORDINAL32
		if (pINTAddr->u1.AddressOfData & 0x80000000)
		{
			//序号导入，那么其低2字节就是其函数序号
			/*printf(" -->name:[NULL]  ordinal:[%04X]\n",
				pINTAddr->u1.AddressOfData & 0xFFFF);*/
			strHint.Format("%04X", pINTAddr->u1.AddressOfData & 0xFFFF);
			strApiName.Format("-");
		}
		else
		{
			//名称导入
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
		//下一个函数
		pINTAddr++;
	}
	

	*pResult = 0;
}


void CImportDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//EndDialog(0);
	CDialogEx::OnClose();
}
