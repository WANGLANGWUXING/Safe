// KillVirusDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "KillVirusDlg.h"
#include "afxdialogex.h"


// CKillVirusDlg �Ի���

IMPLEMENT_DYNAMIC(CKillVirusDlg, CDialogEx)

CKillVirusDlg::CKillVirusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KILLVIRUS_DIALOG, pParent)
	, m_radioAllValue(FALSE)
{

}

CKillVirusDlg::~CKillVirusDlg()
{
}

void CKillVirusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listFile);
	DDX_Control(pDX, IDC_LIST2, m_listProcess);
	DDX_Radio(pDX, IDC_RADIO2, m_radioAllValue);
	DDX_Control(pDX, IDC_STATIC_FILE, m_staticGroup);
	DDX_Control(pDX, IDC_RADIO2, m_btnAllCtrl);
	DDX_Control(pDX, IDC_RADIO_THIS, m_btnThisCtrl);
	DDX_Control(pDX, IDC_BUTTON1, m_btnStartFile);
	DDX_Control(pDX, IDC_STATIC_MSG, m_virusMsg);
	DDX_Control(pDX, IDC_BUTTON3, m_btnStartProc);
	DDX_Control(pDX, IDC_CHECK1, m_btnAutoEnd);
}


BEGIN_MESSAGE_MAP(CKillVirusDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_THIS, &CKillVirusDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO2, &CKillVirusDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON2, &CKillVirusDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CKillVirusDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_COMMAND(ID_32777, &CKillVirusDlg::OnDelVirusItem)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CKillVirusDlg::OnNMRClickList1)
	ON_COMMAND(ID_32778, &CKillVirusDlg::OnDelAllVirus)
	ON_BN_CLICKED(IDC_BUTTON4, &CKillVirusDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CKillVirusDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CKillVirusDlg ��Ϣ�������


BOOL CKillVirusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listFile.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listFile.AddColumn(3, "���",50, "�ļ�·��",200, "�ļ�MD5",150);
	m_listProcess.AddColumn(3, "������",150 ,"�ļ�·��",200, "�ļ�MD5",150);
	LoadImageVirus();
	LoadImageProcess();

	return TRUE;  
}

CString GetPathFrom()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("������ѡ�ļ���Ŀ¼:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return CString("");
	}
	SHGetPathFromIDList(idl, szBuffer);
	return szBuffer;
}
// ���ָ��λ��
void CKillVirusDlg::OnBnClickedRadio3()
{
	m_strThisPath = GetPathFrom();
	if (m_strThisPath == TEXT(""))
	{
		m_btnAllCtrl.SetCheck(TRUE);
		m_btnThisCtrl.SetCheck(FALSE);
		return;
	}
	m_staticGroup.SetWindowText(TEXT("ָ��λ�� - ") + m_strThisPath);
}

// ���ȫ��ɨ��
void CKillVirusDlg::OnBnClickedRadio2()
{
	m_staticGroup.SetWindowText(TEXT("ȫ��ɨ��"));
}


void CKillVirusDlg::OnBnClickedButton2()
{
	CVirusDatabaseDlg virusDatabaseDlg;
	virusDatabaseDlg.DoModal();
}
// ��ȡ�������б�
vector<CString> GetDriverList()
{
	vector<CString>nDiverList;
	TCHAR nDrive[MAX_PATH];
	GetLogicalDriveStrings(100, (LPSTR)nDrive);
	TCHAR* pName = nDrive;

	while (*pName != 0) {
		nDiverList.push_back(pName);
		pName += _tcslen(pName) + 1;
	}
	return nDiverList;
}


//ɨ���߳�
DWORD WINAPI ScanFileThread(LPVOID lpThreadParameter)
{
	CKillVirusDlg *nDlg = (CKillVirusDlg*)lpThreadParameter;


	if (nDlg->m_btnAllCtrl.GetCheck())
	{
		vector<CString>nDiverList;
		nDiverList = GetDriverList();

		for (auto &val : nDiverList) nDlg->ScanFile(val);

	}
	else
	{
		nDlg->ScanFile(nDlg->m_strThisPath);

	}


	nDlg->m_btnStartFile.EnableWindow(TRUE);
	nDlg->SetTimer(nDlg->REFERSHPATH_TIMER, -1, NULL);//ֹͣ��ʱˢ��
	nDlg->m_virusMsg.SetWindowText(TEXT("ɨ�����"));
	nDlg->m_btnStartFile.SetWindowText(TEXT("��ʼɨ��"));
	nDlg->m_bIsScaning = FALSE;

	return 0;
}


void CKillVirusDlg::OnBnClickedButton1()
{
	LoadVirus();
	if (!m_bIsScaning)
	{
		m_dwScaningNum = 0;
		m_dwVirusNum = 0;
		m_btnStartFile.SetWindowText(TEXT("ֹͣɨ��"));
		m_listFile.DeleteAllItems();

		SetTimer(REFERSHPATH_TIMER, 300, NULL);//���ö�ʱˢ��
		CreateThread(NULL, NULL, ScanFileThread, (LPVOID)this, NULL, NULL);
		m_bIsScaning = TRUE;
	}
	else
	{
		m_btnStartFile.SetWindowText(TEXT("����ֹͣ"));
		m_btnStartFile.EnableWindow(FALSE);
		m_bIsScaning = FALSE;

	}
}


void CKillVirusDlg::LoadVirus()
{
	if (m_pVirusInfo != nullptr)delete[]m_pVirusInfo;

	CVirusManage nLoadVirusLib;

	m_dwVirusInfoNum = nLoadVirusLib.LoadVirusLibData();

	m_pVirusInfo = new VIRUSINFO[m_dwVirusInfoNum];

	nLoadVirusLib.GetVirusLib(m_pVirusInfo);
	nLoadVirusLib.ClearVirusLibData();
}


//�����ļ���ɱ��
void CKillVirusDlg::ScanFile(CString dir)
{

	CString nTemp;
	WIN32_FIND_DATA fData = { 0 };
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(dir + "\\*", &fData);
	if (hFind == INVALID_HANDLE_VALUE || m_bIsScaning == FALSE) return;

	do {
		if (m_bIsScaning == FALSE)return;

		if (CString(fData.cFileName) == "." || CString(fData.cFileName) == "..")continue;
		//�ж��ļ�����Ŀ¼
		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	
			ScanFile(dir + "\\" + fData.cFileName);
		else
		{
			//ʵ������ԭ���Լ�
			InterlockedIncrement(&m_dwScaningNum);
			m_strScaningPath = dir + "\\" + fData.cFileName;
			CStringA nFileMd5 = GetFileMd5(m_strScaningPath);//��ȡ�ļ�MD5

			for (DWORD i = 0; i < m_dwVirusInfoNum; i++)//ѭ���벡����MD5�Ƚ�
			{
				if (strcmp(m_pVirusInfo[i].FileMd5, nFileMd5) == 0)//���MD5�Ǻ�
				{
					//ʵ������ԭ���Լ�
					InterlockedIncrement(&m_dwVirusNum);//��������+1
					DWORD nIndex = m_listFile.GetItemCount();
					nTemp.Format(TEXT("%d"), nIndex + 1);
					//��������Ϣ������б��
					m_listFile.AddItem(3, nTemp, dir + "\\" + fData.cFileName, nFileMd5);
					break;
				}
			}
		}
	} while (FindNextFile(hFind, &fData));

}

void CKillVirusDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == REFERSHPATH_TIMER)
	{
		CString nTemp;
		nTemp.Format(TEXT("�ļ�������%d ����ɨ�裺"), m_dwScaningNum);
		m_virusMsg.SetWindowText(nTemp + m_strScaningPath);
	}

	CDialogEx::OnTimer(nIDEvent);
}


//����ͼ��
void CKillVirusDlg::LoadImageVirus()
{
	m_VirusImageList.Create(20, 20, ILC_COLOR32 | ILC_MASK, 2, 2);

	HICON hSmallIco;

	CString nDir = GetProgramDir();

	hSmallIco = ExtractIcon(AfxGetInstanceHandle(), nDir + TEXT("res\\virus.ico"), 0);
	m_VirusImageList.Add(hSmallIco);
	DestroyIcon(hSmallIco);

	m_listFile.SetImageList(&m_VirusImageList, LVSIL_SMALL);
}
//����ͼ��
void CKillVirusDlg::LoadImageProcess()
{
	m_ProcessImageList.Create(20, 20, ILC_COLOR32 | ILC_MASK, 2, 2);

	HICON hSmallIco;
	CString nDir = GetProgramDir();

	hSmallIco = ExtractIcon(AfxGetInstanceHandle(), nDir + TEXT("res\\safe2.ico"), 0);
	m_ProcessImageList.Add(hSmallIco);
	DestroyIcon(hSmallIco);

	hSmallIco = ExtractIcon(AfxGetInstanceHandle(), nDir + TEXT("res\\virus.ico"), 0);
	m_ProcessImageList.Add(hSmallIco);
	DestroyIcon(hSmallIco);


	m_listProcess.SetImageList(&m_ProcessImageList, LVSIL_SMALL);
}


void CKillVirusDlg::OnDelVirusItem()
{
	CString nFileName;
	nFileName = m_listFile.GetItemText(m_fileItem, 1);
	if (Recycle(nFileName, FALSE)==0)
	{
		m_listFile.DeleteItem(m_fileItem);
	}
}


void CKillVirusDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMItemActivate->iItem >= 0)
	{
		m_fileItem = pNMItemActivate->iItem;

		CMenu* pMenu = new CMenu;
		pMenu->LoadMenu(IDR_MENU1);

		CMenu* pSub = pMenu->GetSubMenu(3);

		//��ȡ���λ��;
		CPoint point;
		GetCursorPos(&point);

		//����;
		pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	}

	*pResult = 0;
}


void CKillVirusDlg::OnDelAllVirus()
{
	CString nFileName;
	DWORD nCount;
	nCount = m_listFile.GetItemCount();
	for (DWORD i = 0; i < nCount; i++)
	{
		nFileName = m_listFile.GetItemText(i, 1);
		if (Recycle(nFileName, FALSE)==0)
		{
			m_listFile.DeleteItem(i);
		}
		
	}
}


void CKillVirusDlg::OnBnClickedButton4()
{
	CProcessDatabase processDatabase;
	processDatabase.DoModal();
}

//ɨ������߳�
DWORD WINAPI ScanProcessThread(LPVOID lpThreadParameter)
{
	CKillVirusDlg *nDlg = (CKillVirusDlg*)lpThreadParameter;

	nDlg->ScanProcess();

	nDlg->m_btnStartProc.EnableWindow(TRUE);
	return 0;
}

//ɨ���߳�
void CKillVirusDlg::ScanProcess()
{
	m_listProcess.DeleteAllItems();

	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);//ö�ٽ����б�
	CStringA nFileMd5;
	DWORD nIndex;


	CVirusManage nLoadProcessLib;
	DWORD nCount = nLoadProcessLib.LoadProcessLibData();
	PVIRUSINFO m_pVirusInfo = new VIRUSINFO[nCount];
	nLoadProcessLib.GetProcessLib(m_pVirusInfo);
	nLoadProcessLib.ClearProcessLibData();


	for (DWORD i = 0; i < nNum; i++)
	{
		if (nTaskList.szExeFilePath[i].IsEmpty())continue;

		nFileMd5 = GetFileMd5(nTaskList.szExeFilePath[i]);

		if (nFileMd5.IsEmpty())continue;


		BOOL nSafe = FALSE;
		for (DWORD x = 0; x <nCount; x++)//ѭ���жϽ������Ƿ��ڰ�������
		{
			if (strcmp(m_pVirusInfo[x].FileName, CStringA(nTaskList.szExeFile[i])) == 0)
			{	//����������ڰ������ڣ�����Ϊ��ȫ
				nSafe = TRUE; break;
			}
		}
		//������̲���ȫ���ҹ������Զ��������̣����Զ�������ǰ����
		if (m_btnAutoEnd.GetCheck() && !nSafe)
			EndProcess(nTaskList.th32ProcessID[i]);
		//��������Ϣ������б��
		nIndex = m_listProcess.GetItemCount();

		m_listProcess.InsertItem(nIndex, nTaskList.szExeFile[i], nSafe ? 0 : 1);
		m_listProcess.SetItemText(nIndex, 1, nTaskList.szExeFilePath[i]);
		m_listProcess.SetItemText(nIndex, 2, nFileMd5);
		m_listProcess.SendMessage(LVM_SCROLL, 0, 100);
	}
	ClearTaskList(nTaskList);

	delete[]m_pVirusInfo;

}
void CKillVirusDlg::OnBnClickedButton3()
{
	m_btnStartProc.EnableWindow(FALSE);
	CreateThread(NULL, NULL, ScanProcessThread, (LPVOID)this, NULL, NULL);

}
