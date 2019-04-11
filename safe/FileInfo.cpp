// FileInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "FileInfo.h"
#include "afxdialogex.h"


// CFileInfo �Ի���

IMPLEMENT_DYNAMIC(CFileInfo, CDialogEx)

CFileInfo::CFileInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILEINFO_DIALOG, pParent)
{

}

CFileInfo::~CFileInfo()
{
}

void CFileInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTFILEINFO, m_listFileInfo);
}


BEGIN_MESSAGE_MAP(CFileInfo, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTFILEINFO, &CFileInfo::OnNMDblclkListfileinfo)
END_MESSAGE_MAP()


// CFileInfo ��Ϣ�������


BOOL CFileInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listFileInfo.AddColumn(5, "�ļ���", 100, "����ʱ��", 
		120, "�޸�ʱ��", 120, "MD5", 200, "�ļ���С", 80);
	CString nDir;
	ShowFileList(nDir);
	return TRUE;
}



void CFileInfo::ShowFileList(CString &dir)
{
	m_listFileInfo.DeleteAllItems();


	if (dir == "" || dir == "�����")
	{
		TCHAR nDrive[MAX_PATH];
		GetLogicalDriveStrings(100, (LPSTR)nDrive);
		TCHAR* pName = nDrive;
		while (*pName != 0)
		{
			DWORD nIndex = m_listFileInfo.GetItemCount();
			CString nList = pName;
			DWORD dwType = GetDriveType(pName);
			/*
			switch (dwType)
			{
			case DRIVE_UNKNOWN: nList = nList + TEXT("δ֪");				break;
			case DRIVE_NO_ROOT_DIR: nList = nList + TEXT("�������ĸ�·����Ч");  break;
			case DRIVE_REMOVABLE: nList = nList + TEXT("���ƶ�������������");	 	break;
			case DRIVE_FIXED: nList = nList + TEXT("�����Ƴ���������");		  	break;
			case DRIVE_REMOTE: nList = nList + TEXT("����������");			    break;
			case DRIVE_CDROM: nList = nList + TEXT("CD - ROM������");		 	break;
			case DRIVE_RAMDISK: nList = nList + TEXT("RAM������");			  	break;
			}*/
			m_listFileInfo.InsertItem(nIndex, nList);
			pName += _tcslen(pName) + 1;
		}
	}

	else {

		CString nFilename;
		WIN32_FIND_DATA fData = { 0 };
		HANDLE hFind = INVALID_HANDLE_VALUE;
		hFind = FindFirstFile(dir + "\\*", &fData);
		if (hFind == INVALID_HANDLE_VALUE) {
			m_listFileInfo.InsertItem(0, TEXT("���ļ���"));
			return;
		}

		do {
			if (CString(fData.cFileName) == TEXT(".."))
			{
				CString nTempName = dir;
				int i = nTempName.ReverseFind('\\');
				nTempName.GetBufferSetLength(i + 1);

				CString nTempName2;
				nTempName2 = m_listFileInfo.GetItemText(0, 0);
				if (nTempName == nTempName2)continue;

				DWORD nIndex = m_listFileInfo.GetItemCount();
				m_listFileInfo.InsertItem(nIndex, nTempName);
				continue;
			}
			else if (CString(fData.cFileName) == TEXT("."))
			{
				CString nTempName = dir;
				int i = nTempName.Find('\\');
				nTempName.GetBufferSetLength(i + 1);
				DWORD nIndex = m_listFileInfo.GetItemCount();
				m_listFileInfo.InsertItem(nIndex, nTempName);
				continue;
			}

			nFilename = dir + TEXT("\\");
			nFilename = nFilename + fData.cFileName;
			if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				nFilename = nFilename + TEXT("\\");


			DWORD nIndex = m_listFileInfo.GetItemCount();
			m_listFileInfo.InsertItem(nIndex, nFilename);

			m_listFileInfo.SetItemText(nIndex, 1, timeTotime(fData.ftCreationTime));
			m_listFileInfo.SetItemText(nIndex, 2, timeTotime(fData.ftLastWriteTime));
			CString nFileSize;
			if (fData.nFileSizeLow < 1024)
				nFileSize.Format(TEXT("%d B"), fData.nFileSizeLow);
			else
				nFileSize.Format(TEXT("%d Kb"), (DWORD)(fData.nFileSizeLow / 1024.0));
			if (fData.nFileSizeLow)
			{
				m_listFileInfo.SetItemText(nIndex, 4, nFileSize);
				CString nFileMd5;
				nFileMd5 = GetFileMd5(nFilename);
				m_listFileInfo.SetItemText(nIndex, 3, nFileMd5);
			}


		} while (FindNextFile(hFind, &fData));

	}

	CString nTempName;
	nTempName = m_listFileInfo.GetItemText(0, 0);
	if (nTempName.GetLength() >= 5)
		m_listFileInfo.InsertItem(0, TEXT("�����"));

	static HIMAGELIST nHiml = { 0 };//ͼ���б�
	static SHFILEINFO nPsfi = { 0 };//�ļ���Ϣ
									//ImageList_Destroy(nHiml);//���ͼ���б�
	static BOOL nOn = FALSE;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(32, 32, ILC_COLOR32, 0, 0);//����ͼ����
		ImageList_SetBkColor(nHiml, m_listFileInfo.GetBkColor());//����ͼ���б��ɫ
		m_listFileInfo.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//���ó����б���ʾͼ��
	}

	DWORD nListNum = m_listFileInfo.GetItemCount();

	ImageList_SetImageCount(nHiml, nListNum);

	for (DWORD i = 0; i < nListNum; i++)
	{
		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//��ȡ�ļ���Ϣ
		if (!SHGetFileInfo(m_listFileInfo.GetItemText(i, 0), nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			SHGetFileInfo(m_listFileInfo.GetItemText(i, 0), nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		ImageList_ReplaceIcon(nHiml, i, nPsfi.hIcon);
		DestroyIcon(nPsfi.hIcon);

		m_listFileInfo.SetItem(i, 0, 2, TEXT(""), i, 0, 0, 0);
	}
}


void CFileInfo::OnNMDblclkListfileinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString nDir;
	DWORD nIndex = pNMItemActivate->iItem;
	if (nIndex == -1)return;
	nDir = m_listFileInfo.GetItemText(nIndex, 0);


	TCHAR nTemp[MAX_PATH] = {};

	_tcscpy_s(nTemp, MAX_PATH, nDir);


	if (nTemp[_tcslen(nTemp) - 1] == '\\')
	{
		nTemp[_tcslen(nTemp) - 1] = 0;
		ShowFileList(CString(nTemp));
	}
	else if (nTemp[_tcslen(nTemp) - 1] == '.')
	{
		ShowFileList(CString(nTemp));
	}
	else if (nDir == "�����")
	{
		ShowFileList(nDir);
	}
	else
	{
		ShellExecute(nullptr, TEXT("open"), nTemp, nullptr, nullptr, SW_SHOW);
	}
	*pResult = 0;
}
