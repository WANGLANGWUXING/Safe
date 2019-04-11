// DelayImportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "DelayImportDlg.h"
#include "afxdialogex.h"


// CDelayImportDlg 对话框

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


// CDelayImportDlg 消息处理程序




BOOL CDelayImportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listDelayDLL.AddColumn(3, "DLL名字", 200, "INT(RVA)", 120, "IAT(RVA)", 120);
	m_listDelayFun.AddColumn(3, "函数名字", 200, "Thunk Value", 120, "HINT", 120);

	//导入表的RVA
	DWORD dwImportRVA =
		m_pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress;
	//将RVA转成FOA
	DWORD dwImportFOA = RVAtoFOA(dwImportRVA, m_pFileBuf);
	//获取在文件中的实际地址
	PIMAGE_DELAYLOAD_DESCRIPTOR pImport =
		(PIMAGE_DELAYLOAD_DESCRIPTOR)(dwImportFOA + m_pFileBuf);

	CString strDllName;
	CString strINIRVA;
	CString strIAIRVA;
	while (pImport->DllNameRVA)
	{
		//模块名字
		//printf("模块名字:%s\n", RVAtoFOA(pImport->Name, m_pFileBuf) + m_pFileBuf);
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
	// TODO: 在此添加控件通知处理程序代码

	int nSel = pNMItemActivate->iItem;
	if (nSel < 0)
	{
		MessageBox("请选中数据");
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
	//遍历导入模块中的哪些函数
	//获取INT的地址
	PIMAGE_THUNK_DATA pINTAddr = (PIMAGE_THUNK_DATA)
		(RVAtoFOA(OriginalFirstThunk, m_pFileBuf) + m_pFileBuf);
	while (pINTAddr->u1.Ordinal)
	{

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
		m_listDelayFun.AddItem(3, strApiName, strThunkValue, strHint);

		OriginalFirstThunk += 4;
		//下一个函数
		pINTAddr++;
	}


	*pResult = 0;
}
