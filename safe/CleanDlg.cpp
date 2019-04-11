// CleanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "CleanDlg.h"
#include "afxdialogex.h"
//操作cookie包含的头文件
#include <wininet.h>
#include <UrlHist.h>
#pragma comment(lib,"Wininet.lib")

// CCleanDlg 对话框

IMPLEMENT_DYNAMIC(CCleanDlg, CDialogEx)

CCleanDlg::CCleanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLEAN_DIALOG, pParent)
	, m_strStatus(_T(""))
{
	//初始化m_bIsSelect
	for (int i = 0; i < 11; i++)
	{
		m_bIsSelect[i] = FALSE;
	}

	m_isScanFile = TRUE;
	m_dAllFileSize = 0;
}

CCleanDlg::~CCleanDlg()
{
}

void CCleanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listClean);
	DDX_Text(pDX, IDC_status, m_strStatus);
}


BEGIN_MESSAGE_MAP(CCleanDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK7, &CCleanDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK14, &CCleanDlg::OnBnClickedCheck14)
	ON_BN_CLICKED(IDC_BUTTON1, &CCleanDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCleanDlg::OnBnClickedButton2)
	ON_MESSAGE(WM_MYUPDATEDATA, &CCleanDlg::OnUpdateMyData)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK12, &CCleanDlg::OnBnClickedCheck12)
END_MESSAGE_MAP()


// CCleanDlg 消息处理程序


BOOL CCleanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listClean.AddColumn(2, "文件路径", 250, "文件大小", 80);

	return TRUE;  
}

// 全选系统垃圾
void CCleanDlg::OnBnClickedCheck7()
{
	//获取当前按钮状态
	int nStatus = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck();
	if (nStatus == 1)
	{
		//设置被选中
		
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1);
		//((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(1);
		//((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(1);
		//((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(1);
	}
	if (nStatus == 0)
	{
		//设置未选择
		
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);
		//((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(0);
		//((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(0);
		//((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(0);
	}
}

// 全选浏览器垃圾
void CCleanDlg::OnBnClickedCheck14()
{
	int nStatus = ((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck();;
	if (nStatus == 1)
	{
		//设置被选中
		
		((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(1);
		/*((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(1);*/
	}
	if (nStatus == 0)
	{
		//设置未选择
		
		((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(0);
		/*((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(0);*/
	}
}





#pragma region 清理垃圾
// 	清空一个目录
BOOL CCleanDlg::EmptyDirectory(LPCTSTR szPath,
	BOOL bDeleteDesktopIni /*= FALSE*/, BOOL bWipeIndexDat /*= FALSE*/)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	sFindFilter += TEXT("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
		return FALSE;

	do
	{
		if (_tcscmp(wfd.cFileName, TEXT(".")) == 0 ||
			_tcscmp(wfd.cFileName, TEXT("..")) == 0 ||
			(bDeleteDesktopIni == FALSE && _tcsicmp(wfd.cFileName, TEXT("desktop.ini")) == 0))
		{
			continue;
		}

		sFullPath = szPath;
		sFullPath += TEXT("\\");
		sFullPath += wfd.cFileName;

		//判断是否是扫描文件，如果是 就输出到List
		//if (m_isScanFile)
		//{
		//	//插入到List
		//	if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		//	{
		//		//如果是目录，则递归调用，列举下级目录
		//		EmptyDirectory(sFullPath);
		//		continue;
		//	}
		//	//插入文件路径
		//	m_listClean.InsertItem(0, sFullPath);
		//	//插入文件大小
		//	m_szTemp = GetShowSize(wfd.nFileSizeLow);
		//	m_listClean.SetItemText(0, 1, m_szTemp);
		//	//保存所有文件的大小 用于显示有多少
		//	m_dAllFileSize += wfd.nFileSizeLow;
		//	continue;
		//}

		//去掉只读属性
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}
		//递归文件夹中的文件
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//如果文件大小等于0说明是文件夹 递归查询
			EmptyDirectory(sFullPath);
			//删除完文件夹中的文件，再删除文件夹
			RemoveDirectory(sFullPath);
		}
		else
		{
			//if (bWipeIndexDat && _tcsicmp(wfd.cFileName, TEXT("index.dat")) == 0)
			//{
			//	WipeFile(szPath, wfd.cFileName);
			//}
			////保存所有文件的大小 用于显示有多少
			//m_dAllFileSize += wfd.nFileSizeLow;
			//if (!DeleteFile(sFullPath))
			//{
			//	m_dAllFileSize -= wfd.nFileSizeLow;
			//}

			DeleteFile(sFullPath);
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);

	return TRUE;
}

// 擦除文件内容
BOOL CCleanDlg::WipeFile(LPCTSTR szDir, LPCTSTR szFile)
{
	CString sPath;
	HANDLE	hFile;
	DWORD	dwSize;
	DWORD	dwWrite;
	TCHAR	sZero[SWEEP_BUFFER_SIZE];
	memset(sZero, 0, SWEEP_BUFFER_SIZE);

	sPath = szDir;
	sPath += TEXT("\\");
	sPath += szFile;

	hFile = CreateFile(sPath, GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	dwSize = GetFileSize(hFile, NULL);
	dwSize -= 64;
	SetFilePointer(hFile, 64, NULL, FILE_BEGIN);
	while (dwSize > 0)
	{
		if (dwSize > SWEEP_BUFFER_SIZE)
		{
			WriteFile(hFile, sZero, SWEEP_BUFFER_SIZE, &dwWrite, NULL);
			dwSize -= SWEEP_BUFFER_SIZE;
		}
		else
		{
			WriteFile(hFile, sZero, dwSize, &dwWrite, NULL);
			break;
		}
	}
	CloseHandle(hFile);
	return TRUE;
}
// 文件大小显示方式
CString CCleanDlg::GetShowSize(DOUBLE dwSize)
{
	CString szSize;
	if (dwSize<1024)
	{
		//B
		szSize.Format("%d B", (DWORD)dwSize);
		return szSize;
	}
	else if (dwSize<1024 * 1024)
	{
		//KB
		szSize.Format("%.2f KB", ((DOUBLE)dwSize) / 1024);
		return szSize;
	}
	else if (dwSize<1024 * 1024 * 1024)
	{
		//MB
		szSize.Format("%.2f MB", (DOUBLE)dwSize / 1024 / 1024);
		return szSize;
	}if (dwSize>1024 * 1024 * 1024)
	{
		//GB
		szSize.Format("%.2f GB", (DOUBLE)dwSize / 1024 / 1024 / 1024);
		return szSize;
	}
	return "";
}

// 清除Url缓存

BOOL CCleanDlg::DeleteUrlCache(DEL_CACHE_TYPE type)
{
	BOOL bRet = FALSE;   //返回值
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
	DWORD dwEntrySize;

	//delete the files
	dwEntrySize = 0;
	//获得INTERNET_CACHE_ENTRY_INFO的大小
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	//分配响应大小的缓冲区
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new TCHAR[dwEntrySize];
	//获得进入口的句柄
	hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);

	if (hEntry)
	{
		do
		{
			if (type == FILES &&
				!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
			{
				DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			}
			else if (type == COOKIES &&
				(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
			{
				DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			}
			dwEntrySize = 0;
			FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
			delete[] lpCacheEntry;
			lpCacheEntry = NULL;
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new TCHAR[dwEntrySize];
		} while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));

		bRet = TRUE;
	}

	if (lpCacheEntry)
	{
		delete[] lpCacheEntry;
		lpCacheEntry = NULL;
	}
	return bRet;
}


// 清理系统临时文件夹

VOID CCleanDlg::CleanSystemTemp()
{
	CString strPath;
	BOOL bSuccess = GetTempPath(MAX_PATH, strPath.GetBuffer(MAX_PATH));

	strPath.ReleaseBuffer();
	if (bSuccess)//得到系统临时目录
	{
		EmptyDirectory(strPath, TRUE);
	}
}


// 清理回收站

VOID CCleanDlg::CleanRubbishStation()
{
	//if (m_isScanFile)
	//	return;
	SHEmptyRecycleBin(NULL, NULL,
		SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}


// 清理运行记录

VOID CCleanDlg::CleanRunHistory()
{
	/*if (m_isScanFile)
		return;*/
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"));
}


// 清理最近文档记录

VOID CCleanDlg::CleanDocumentHistory()
{
	CString strPath;

	BOOL bSuccess = SHGetSpecialFolderPath(NULL,
		strPath.GetBuffer(MAX_PATH), CSIDL_RECENT, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{
		EmptyDirectory(strPath);
	}
	/*if (m_isScanFile)
		return;*/
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs"));
}



// 清理上次用户登录记录
VOID CCleanDlg::CleanPrevUserHistory()
{
	if (m_isScanFile)
		return;
	SHDeleteValue(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
		TEXT("DefaultUserName"));
	SHDeleteValue(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
		TEXT("AltDefaultUserName"));
	SHDeleteValue(HKEY_LOCAL_MACHINE,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Winlogon"),
		TEXT("DefaultUserName"));
}


// 清理文件查找记录
VOID CCleanDlg::CleanFileFindHistory()
{
	if (m_isScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Doc Find Spec MRU"));
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\Explorer Bars\\{C4EE31F3-4768-11D2-BE5C-00A0C9A83DA1}\\ContainingTextMRU"));
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\Explorer Bars\\{C4EE31F3-4768-11D2-BE5C-00A0C9A83DA1}\\FilesNamedMRU"));
}


// 清理Interner临时文件

VOID CCleanDlg::CleanInternetTemp()
{
	CString strPath;
	/*if (!m_isScanFile)
	{*/
		DeleteUrlCache(FILES);
	/*}*/
	//SHGetSpecialFolderPath 获取系统指定系统路径
	BOOL bSuccess = SHGetSpecialFolderPath(NULL,
		strPath.GetBuffer(MAX_PATH), CSIDL_INTERNET_CACHE, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{  //得到临时目录，并清空它.
		EmptyDirectory(strPath);
	}
}


// 清理Internet Cookie
VOID CCleanDlg::CleanInternetCookie()
{
	CString strPath;
	DeleteUrlCache(COOKIES);
	BOOL bSuccess = SHGetSpecialFolderPath(NULL,
		strPath.GetBuffer(MAX_PATH), CSIDL_COOKIES, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{//得到目录，并清空
		EmptyDirectory(strPath);
	}
}


// 函数说明:	清理地址栏历史记录

VOID CCleanDlg::CleanAddrBarHistory()
{
	/*if (m_isScanFile)
		return;*/
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\TypedURLs"));
}


// 清理密码记录

VOID CCleanDlg::CleanPasswordHistory()
{
	if (m_isScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\IntelliForms"));
}


// 函数说明:	清理浏览网址记录
VOID CCleanDlg::CleanBrowseAddrHistory()
{
	if (m_isScanFile)
		return;
	HRESULT hr;
	CString strPath;
	IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
	hr = CoCreateInstance(CLSID_CUrlHistory, NULL,
		CLSCTX_INPROC_SERVER, IID_IUrlHistoryStg2,
		(VOID**)&pUrlHistoryStg2);
	if (SUCCEEDED(hr))
	{
		hr = pUrlHistoryStg2->ClearHistory();
		pUrlHistoryStg2->Release();
	}

	delete pUrlHistoryStg2;

	// 如果上面代码不能清
	// 则有下面的，不完美，但能工作.
	GetWindowsDirectory(strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	strPath += TEXT("\\History");
	EmptyDirectory(strPath, FALSE, TRUE);

	BOOL bSuccess = SHGetSpecialFolderPath(NULL, strPath.GetBuffer(MAX_PATH), CSIDL_HISTORY, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{
		EmptyDirectory(strPath, FALSE, TRUE);
	}
}
#pragma endregion






void CCleanDlg::GetSelect()
{
	m_bIsSelect[0] = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	m_bIsSelect[1] = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	m_bIsSelect[2] = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	m_bIsSelect[3] = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();
	m_bIsSelect[4] = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();
	m_bIsSelect[5] = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();
	m_bIsSelect[6] = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();
	m_bIsSelect[7] = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();
	m_bIsSelect[8] = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();
	m_bIsSelect[9] = ((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck();
	m_bIsSelect[10] = ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck();
}

void CCleanDlg::OnBnClickedButton1()
{
	//设置状态为扫描状态
	m_isScanFile = TRUE;
	m_dAllFileSize = 0;
	m_listClean.DeleteAllItems();
	//获取扫描项是否被选中
	GetSelect();
	//判断是否有选项
	int nNum = 0;
	for (int i = 0; i < 11; i++)
	{
		nNum += m_bIsSelect[i];
	}
	if (nNum == 0)
	{
		m_strStatus = _T("请 选 择 要 扫 描 的 项 目 ！");
		UpdateData(FALSE);
		return;
	}
	//启动扫描
	StartCleanThread();
}


void CCleanDlg::OnBnClickedButton2()
{
	//设置状态为清理状态
	m_isScanFile = FALSE;
	m_dAllFileSize = 0;
	//获取扫描项是否被选中
	GetSelect();
	//判断是否有选项
	int nNum = 0;
	for (int i = 0; i < 11; i++)
	{
		nNum += m_bIsSelect[i];
	}
	if (nNum == 0)
	{
		m_strStatus = _T("请 选 择 要 清 理 的 项 目 ！");
		UpdateData(FALSE);
		return;
	}
	//启动清理
	StartCleanThread();
}
// 清理调用函数
VOID CCleanDlg::CleanRubbish(int cleanType)
{
	switch (cleanType)
	{
	case CLEAN_SYSTEM_TEMP:
		CleanSystemTemp();
		break;
	case CLEAN_RUBBISH_STATION:
		CleanRubbishStation();
		break;
	case CLEAN_RUN_HISTORY:
		CleanRunHistory();
		break;
	case CLEAN_DOCUMENT_HISTORY:
		CleanDocumentHistory();
		break;
	case CLEAN_PREVUSER_HISTORY:
		CleanPrevUserHistory();
		break;
	case CLEAN_FILEFIND_HISTORY:
		CleanFileFindHistory();
		break;
	case CLEAN_INTERNET_TEMP:
		CleanInternetTemp();
		break;
	case CLEAN_INTERNET_COOKIE:
		CleanInternetCookie();
		break;
	case CLEAN_ADDRBAR_HISTORY:
		CleanAddrBarHistory();
		break;
	case CLEAN_PASSWORD_HISTORY:
		CleanPasswordHistory();
		break;
	case CLEAN_BROWSEADDR_HISTORY:
		CleanBrowseAddrHistory();
		break;
	}
}
// 函数说明:	启动清理线程
DWORD WINAPI CleanThread(LPVOID lpVoid)
{
	CCleanDlg* pCleanerCtrl = (CCleanDlg*)lpVoid;

	//提示正在扫描或清理
	if (pCleanerCtrl->m_isScanFile)
	{
		pCleanerCtrl->m_strStatus = "正 在 扫 描清理 中 ... 请 耐 心 等 待 ...";
	}
	else
	{
		pCleanerCtrl->m_strStatus = "正 在 清 理 中 ... 请 耐 心 等 待 ...";
	}
	pCleanerCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);

	int nCount = 11;
	for (int i = 0; i < nCount; i++) //要清理的选项ID
	{
		if (pCleanerCtrl->m_bIsSelect[i])
		{
			pCleanerCtrl->CleanRubbish(i);
		}
	}
	pCleanerCtrl->m_strStatus="清理完成";
	pCleanerCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);
	////提示清理或扫描完毕
	//if (pCleanerCtrl->m_isScanFile)
	//{
	//	pCleanerCtrl->m_strStatus.Format(
	//		"扫 描 完 毕 ！\t垃 圾 文 件：%s", pCleanerCtrl->GetShowSize(pCleanerCtrl->m_dAllFileSize));
	//}
	//else
	//{
	//	pCleanerCtrl->m_strStatus.Format(
	//		"清 理 完 毕 ！\t本 次 清 理：%s", pCleanerCtrl->GetShowSize(pCleanerCtrl->m_dAllFileSize));
	//	//再调用一下扫描
	//	//用于刷新列表
	//	pCleanerCtrl->m_isScanFile = TRUE;
	//	pCleanerCtrl->m_listClean.DeleteAllItems();
	//	int nCount = 11;
	//	for (int i = 0; i < nCount; i++) //要清理的选项ID
	//	{
	//		if (pCleanerCtrl->m_bIsSelect[i])
	//		{
	//			pCleanerCtrl->CleanRubbish(i);
	//		}
	//	}
	//}
	//pCleanerCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);

	return 0;
}

BOOL CCleanDlg::StartCleanThread()
{
	CloseHandle(CreateThread(NULL, 0, CleanThread, this, 0, NULL));
	return TRUE;
}


afx_msg LRESULT CCleanDlg::OnUpdateMyData(WPARAM wParam, LPARAM lParam)
{
	return UpdateData(wParam);
}





void CCleanDlg::OnBnClickedCheck12()
{
	// TODO: 在此添加控件通知处理程序代码
}
