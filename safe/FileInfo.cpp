// FileInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "FileInfo.h"
#include "afxdialogex.h"


// CFileInfo 对话框

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


// CFileInfo 消息处理程序


BOOL CFileInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listFileInfo.AddColumn(5, "文件名", 100, "创建时间", 
		120, "修改时间", 120, "MD5", 200, "文件大小", 80);
	CString nDir;
	ShowFileList(nDir);
	return TRUE;
}



void CFileInfo::ShowFileList(CString &dir)
{
	m_listFileInfo.DeleteAllItems();


	if (dir == "" || dir == "计算机")
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
			case DRIVE_UNKNOWN: nList = nList + TEXT("未知");				break;
			case DRIVE_NO_ROOT_DIR: nList = nList + TEXT("驱动器的根路径无效");  break;
			case DRIVE_REMOVABLE: nList = nList + TEXT("可移动介质类驱动器");	 	break;
			case DRIVE_FIXED: nList = nList + TEXT("不可移除的驱动器");		  	break;
			case DRIVE_REMOTE: nList = nList + TEXT("网络驱动器");			    break;
			case DRIVE_CDROM: nList = nList + TEXT("CD - ROM驱动器");		 	break;
			case DRIVE_RAMDISK: nList = nList + TEXT("RAM驱动器");			  	break;
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
			m_listFileInfo.InsertItem(0, TEXT("无文件！"));
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
		m_listFileInfo.InsertItem(0, TEXT("计算机"));

	static HIMAGELIST nHiml = { 0 };//图标列表
	static SHFILEINFO nPsfi = { 0 };//文件信息
									//ImageList_Destroy(nHiml);//清除图标列表
	static BOOL nOn = FALSE;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(32, 32, ILC_COLOR32, 0, 0);//订制图标框架
		ImageList_SetBkColor(nHiml, m_listFileInfo.GetBkColor());//设置图标列表底色
		m_listFileInfo.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//设置超级列表显示图标
	}

	DWORD nListNum = m_listFileInfo.GetItemCount();

	ImageList_SetImageCount(nHiml, nListNum);

	for (DWORD i = 0; i < nListNum; i++)
	{
		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//获取文件信息
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
	else if (nDir == "计算机")
	{
		ShowFileList(nDir);
	}
	else
	{
		ShellExecute(nullptr, TEXT("open"), nTemp, nullptr, nullptr, SW_SHOW);
	}
	*pResult = 0;
}
