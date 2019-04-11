// CleanDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "CleanDlg.h"
#include "afxdialogex.h"
//����cookie������ͷ�ļ�
#include <wininet.h>
#include <UrlHist.h>
#pragma comment(lib,"Wininet.lib")

// CCleanDlg �Ի���

IMPLEMENT_DYNAMIC(CCleanDlg, CDialogEx)

CCleanDlg::CCleanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLEAN_DIALOG, pParent)
	, m_strStatus(_T(""))
{
	//��ʼ��m_bIsSelect
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


// CCleanDlg ��Ϣ�������


BOOL CCleanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listClean.AddColumn(2, "�ļ�·��", 250, "�ļ���С", 80);

	return TRUE;  
}

// ȫѡϵͳ����
void CCleanDlg::OnBnClickedCheck7()
{
	//��ȡ��ǰ��ť״̬
	int nStatus = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck();
	if (nStatus == 1)
	{
		//���ñ�ѡ��
		
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1);
		//((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(1);
		//((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(1);
		//((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(1);
	}
	if (nStatus == 0)
	{
		//����δѡ��
		
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);
		//((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(0);
		//((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(0);
		//((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(0);
	}
}

// ȫѡ���������
void CCleanDlg::OnBnClickedCheck14()
{
	int nStatus = ((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck();;
	if (nStatus == 1)
	{
		//���ñ�ѡ��
		
		((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(1);
		/*((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(1);*/
	}
	if (nStatus == 0)
	{
		//����δѡ��
		
		((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(0);
		/*((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(0);*/
	}
}





#pragma region ��������
// 	���һ��Ŀ¼
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

		//�ж��Ƿ���ɨ���ļ�������� �������List
		//if (m_isScanFile)
		//{
		//	//���뵽List
		//	if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		//	{
		//		//�����Ŀ¼����ݹ���ã��о��¼�Ŀ¼
		//		EmptyDirectory(sFullPath);
		//		continue;
		//	}
		//	//�����ļ�·��
		//	m_listClean.InsertItem(0, sFullPath);
		//	//�����ļ���С
		//	m_szTemp = GetShowSize(wfd.nFileSizeLow);
		//	m_listClean.SetItemText(0, 1, m_szTemp);
		//	//���������ļ��Ĵ�С ������ʾ�ж���
		//	m_dAllFileSize += wfd.nFileSizeLow;
		//	continue;
		//}

		//ȥ��ֻ������
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}
		//�ݹ��ļ����е��ļ�
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//����ļ���С����0˵�����ļ��� �ݹ��ѯ
			EmptyDirectory(sFullPath);
			//ɾ�����ļ����е��ļ�����ɾ���ļ���
			RemoveDirectory(sFullPath);
		}
		else
		{
			//if (bWipeIndexDat && _tcsicmp(wfd.cFileName, TEXT("index.dat")) == 0)
			//{
			//	WipeFile(szPath, wfd.cFileName);
			//}
			////���������ļ��Ĵ�С ������ʾ�ж���
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

// �����ļ�����
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
// �ļ���С��ʾ��ʽ
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

// ���Url����

BOOL CCleanDlg::DeleteUrlCache(DEL_CACHE_TYPE type)
{
	BOOL bRet = FALSE;   //����ֵ
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
	DWORD dwEntrySize;

	//delete the files
	dwEntrySize = 0;
	//���INTERNET_CACHE_ENTRY_INFO�Ĵ�С
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	//������Ӧ��С�Ļ�����
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new TCHAR[dwEntrySize];
	//��ý���ڵľ��
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


// ����ϵͳ��ʱ�ļ���

VOID CCleanDlg::CleanSystemTemp()
{
	CString strPath;
	BOOL bSuccess = GetTempPath(MAX_PATH, strPath.GetBuffer(MAX_PATH));

	strPath.ReleaseBuffer();
	if (bSuccess)//�õ�ϵͳ��ʱĿ¼
	{
		EmptyDirectory(strPath, TRUE);
	}
}


// �������վ

VOID CCleanDlg::CleanRubbishStation()
{
	//if (m_isScanFile)
	//	return;
	SHEmptyRecycleBin(NULL, NULL,
		SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}


// �������м�¼

VOID CCleanDlg::CleanRunHistory()
{
	/*if (m_isScanFile)
		return;*/
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"));
}


// ��������ĵ���¼

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



// �����ϴ��û���¼��¼
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


// �����ļ����Ҽ�¼
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


// ����Interner��ʱ�ļ�

VOID CCleanDlg::CleanInternetTemp()
{
	CString strPath;
	/*if (!m_isScanFile)
	{*/
		DeleteUrlCache(FILES);
	/*}*/
	//SHGetSpecialFolderPath ��ȡϵͳָ��ϵͳ·��
	BOOL bSuccess = SHGetSpecialFolderPath(NULL,
		strPath.GetBuffer(MAX_PATH), CSIDL_INTERNET_CACHE, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{  //�õ���ʱĿ¼���������.
		EmptyDirectory(strPath);
	}
}


// ����Internet Cookie
VOID CCleanDlg::CleanInternetCookie()
{
	CString strPath;
	DeleteUrlCache(COOKIES);
	BOOL bSuccess = SHGetSpecialFolderPath(NULL,
		strPath.GetBuffer(MAX_PATH), CSIDL_COOKIES, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{//�õ�Ŀ¼�������
		EmptyDirectory(strPath);
	}
}


// ����˵��:	�����ַ����ʷ��¼

VOID CCleanDlg::CleanAddrBarHistory()
{
	/*if (m_isScanFile)
		return;*/
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\TypedURLs"));
}


// ���������¼

VOID CCleanDlg::CleanPasswordHistory()
{
	if (m_isScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Internet Explorer\\IntelliForms"));
}


// ����˵��:	���������ַ��¼
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

	// ���������벻����
	// ��������ģ������������ܹ���.
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
	//����״̬Ϊɨ��״̬
	m_isScanFile = TRUE;
	m_dAllFileSize = 0;
	m_listClean.DeleteAllItems();
	//��ȡɨ�����Ƿ�ѡ��
	GetSelect();
	//�ж��Ƿ���ѡ��
	int nNum = 0;
	for (int i = 0; i < 11; i++)
	{
		nNum += m_bIsSelect[i];
	}
	if (nNum == 0)
	{
		m_strStatus = _T("�� ѡ �� Ҫ ɨ �� �� �� Ŀ ��");
		UpdateData(FALSE);
		return;
	}
	//����ɨ��
	StartCleanThread();
}


void CCleanDlg::OnBnClickedButton2()
{
	//����״̬Ϊ����״̬
	m_isScanFile = FALSE;
	m_dAllFileSize = 0;
	//��ȡɨ�����Ƿ�ѡ��
	GetSelect();
	//�ж��Ƿ���ѡ��
	int nNum = 0;
	for (int i = 0; i < 11; i++)
	{
		nNum += m_bIsSelect[i];
	}
	if (nNum == 0)
	{
		m_strStatus = _T("�� ѡ �� Ҫ �� �� �� �� Ŀ ��");
		UpdateData(FALSE);
		return;
	}
	//��������
	StartCleanThread();
}
// ������ú���
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
// ����˵��:	���������߳�
DWORD WINAPI CleanThread(LPVOID lpVoid)
{
	CCleanDlg* pCleanerCtrl = (CCleanDlg*)lpVoid;

	//��ʾ����ɨ�������
	if (pCleanerCtrl->m_isScanFile)
	{
		pCleanerCtrl->m_strStatus = "�� �� ɨ ������ �� ... �� �� �� �� �� ...";
	}
	else
	{
		pCleanerCtrl->m_strStatus = "�� �� �� �� �� ... �� �� �� �� �� ...";
	}
	pCleanerCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);

	int nCount = 11;
	for (int i = 0; i < nCount; i++) //Ҫ�����ѡ��ID
	{
		if (pCleanerCtrl->m_bIsSelect[i])
		{
			pCleanerCtrl->CleanRubbish(i);
		}
	}
	pCleanerCtrl->m_strStatus="�������";
	pCleanerCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);
	////��ʾ�����ɨ�����
	//if (pCleanerCtrl->m_isScanFile)
	//{
	//	pCleanerCtrl->m_strStatus.Format(
	//		"ɨ �� �� �� ��\t�� �� �� ����%s", pCleanerCtrl->GetShowSize(pCleanerCtrl->m_dAllFileSize));
	//}
	//else
	//{
	//	pCleanerCtrl->m_strStatus.Format(
	//		"�� �� �� �� ��\t�� �� �� ��%s", pCleanerCtrl->GetShowSize(pCleanerCtrl->m_dAllFileSize));
	//	//�ٵ���һ��ɨ��
	//	//����ˢ���б�
	//	pCleanerCtrl->m_isScanFile = TRUE;
	//	pCleanerCtrl->m_listClean.DeleteAllItems();
	//	int nCount = 11;
	//	for (int i = 0; i < nCount; i++) //Ҫ�����ѡ��ID
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
