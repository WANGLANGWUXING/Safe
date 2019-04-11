// VirusDatabaseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "VirusDatabaseDlg.h"
#include "afxdialogex.h"


// CVirusDatabaseDlg �Ի���

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


// CVirusDatabaseDlg ��Ϣ�������


BOOL CVirusDatabaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listVirusDatabase.AddColumn(2, "�����ļ���",180, "�����ļ�MD5",200);
	ShowVirusList();

	return TRUE;  
}
// ��ʾ���ز���
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


/* ��ȡ�ļ���*/
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
		nTemp.Format(TEXT("�Ƿ��ļ���%s\n�ļ�MD5��%s\n���벡���⣿"), GetPathShortFileName(buff), CString(nFileMd5));

		DWORD nRet = MessageBox(nTemp, TEXT("��ʾ"), MB_YESNO + MB_ICONASTERISK);
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
	nFileName.Format(TEXT("�Ƿ񽫣�%s\n�Ƴ������⣿"), m_listVirusDatabase.GetItemText(m_item, 0));
	DWORD nRet = MessageBox(nFileName, TEXT("��ʾ"), MB_ICONASTERISK | MB_YESNO);
	if (nRet == IDNO)return;

	m_listVirusDatabase.DeleteItem(m_item);

	UpDataVirusDatabase();

}


void CVirusDatabaseDlg::OnDelAllVirusDatabase()
{
	DWORD nRet = MessageBox(TEXT("�Ƿ�Ҫ��ղ�����������Ϣ��"), TEXT("��ʾ"), MB_ICONASTERISK | MB_YESNO);
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

		//��ȡ���λ��;
		CPoint point;
		GetCursorPos(&point);

		//����;
		pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	}
	*pResult = 0;
}
