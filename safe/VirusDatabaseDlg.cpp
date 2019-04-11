// VirusDatabaseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "VirusDatabaseDlg.h"
#include "afxdialogex.h"


// CVirusDatabaseDlg 对话框

IMPLEMENT_DYNAMIC(CVirusDatabaseDlg, CDialogEx)

CVirusDatabaseDlg::CVirusDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIRUSDATABASE_DIALOG, pParent)
{

}

CVirusDatabaseDlg::~CVirusDatabaseDlg()
{
}

void CVirusDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIRUSDATA, m_listVirusDatabase);
}


BEGIN_MESSAGE_MAP(CVirusDatabaseDlg, CDialogEx)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_32775, &CVirusDatabaseDlg::OnDelVirusDatabaseItem)
	ON_COMMAND(ID_32776, &CVirusDatabaseDlg::OnDelAllVirusDatabase)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_VIRUSDATA, &CVirusDatabaseDlg::OnNMRClickListVirusdata)
END_MESSAGE_MAP()


// CVirusDatabaseDlg 消息处理程序


BOOL CVirusDatabaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listVirusDatabase.AddColumn(2, "病毒文件名",180, "病毒文件MD5",200);
	ShowVirusList();

	return TRUE;  
}
// 显示本地病毒
void CVirusDatabaseDlg::ShowVirusList()
{
	m_listVirusDatabase.DeleteAllItems();
	CVirusManage nLoadVirusLib;
	DWORD nCount = nLoadVirusLib.LoadVirusLibData();
	PVIRUSINFO m_pVirusInfo = new VIRUSINFO[nCount];
	nLoadVirusLib.GetVirusLib(m_pVirusInfo);
	nLoadVirusLib.ClearVirusLibData();
	for (DWORD i = 0; i < nCount; i++)
	{
		m_listVirusDatabase.AddItem(2, CString(m_pVirusInfo[i].FileName), CString(m_pVirusInfo[i].FileMd5));
	}

	delete[]m_pVirusInfo;
}


/* 获取文件名*/
CString GetPathShortFileName(CString nPath)
{
	DWORD nIndex = nPath.ReverseFind('\\');
	nPath = nPath.Right(nPath.GetLength() - nIndex - 1);
	return nPath;
}


void CVirusDatabaseDlg::OnDropFiles(HDROP hDropInfo)
{
	CString buff;
	buff.GetBufferSetLength(MAX_PATH);

	DragQueryFile(hDropInfo, 0, (TCHAR*)(const TCHAR*)buff, MAX_PATH);
	DragFinish(hDropInfo);

	CStringA nFileMd5;
	nFileMd5 = GetFileMd5(buff);

	if (nFileMd5 != "")
	{
		CString nTemp;
		nTemp.Format(TEXT("是否将文件：%s\n文件MD5：%s\n加入病毒库？"), GetPathShortFileName(buff), CString(nFileMd5));

		DWORD nRet = MessageBox(nTemp, TEXT("提示"), MB_YESNO + MB_ICONASTERISK);
		if (nRet == IDNO)return;

		DWORD nIndex = m_listVirusDatabase.GetItemCount();
		m_listVirusDatabase.AddItem(2, GetPathShortFileName(buff), CString(nFileMd5));
		UpDataVirusDatabase();
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

void CVirusDatabaseDlg::UpDataVirusDatabase()
{
	DWORD nCount = m_listVirusDatabase.GetItemCount();
	PVIRUSINFO nVirusInfo = new VIRUSINFO[nCount];
	for (DWORD i = 0; i < nCount; i++)
	{
		strcpy_s(nVirusInfo[i].FileName, MAX_PATH, CStringA(m_listVirusDatabase.GetItemText(i, 0)));
		strcpy_s(nVirusInfo[i].FileMd5, MAX_PATH, CStringA(m_listVirusDatabase.GetItemText(i, 1)));
	}

	CVirusManage nLoadVirusLib;
	nLoadVirusLib.SetVirusLib(nVirusInfo, nCount);
}


void CVirusDatabaseDlg::OnDelVirusDatabaseItem()
{
	CString nFileName;
	nFileName.Format(TEXT("是否将：%s\n移除病毒库？"), m_listVirusDatabase.GetItemText(m_item, 0));
	DWORD nRet = MessageBox(nFileName, TEXT("提示"), MB_ICONASTERISK | MB_YESNO);
	if (nRet == IDNO)return;

	m_listVirusDatabase.DeleteItem(m_item);

	UpDataVirusDatabase();

}


void CVirusDatabaseDlg::OnDelAllVirusDatabase()
{
	DWORD nRet = MessageBox(TEXT("是否要清空病毒库所有信息？"), TEXT("提示"), MB_ICONASTERISK | MB_YESNO);
	if (nRet == IDNO)return;
	m_listVirusDatabase.DeleteAllItems();
	UpDataVirusDatabase();
}


void CVirusDatabaseDlg::OnNMRClickListVirusdata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (pNMItemActivate->iItem >= 0)
	{
		m_item = pNMItemActivate->iItem;

		CMenu* pMenu = new CMenu;
		pMenu->LoadMenu(IDR_MENU1);

		CMenu* pSub = pMenu->GetSubMenu(2);

		//获取鼠标位置;
		CPoint point;
		GetCursorPos(&point);

		//弹出;
		pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	}
	*pResult = 0;
}
