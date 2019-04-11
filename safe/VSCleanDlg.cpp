// VSCleanDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "VSCleanDlg.h"
#include "afxdialogex.h"


// CVSCleanDlg �Ի���

IMPLEMENT_DYNAMIC(CVSCleanDlg, CDialogEx)

CVSCleanDlg::CVSCleanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VSCLEAN_DIALOG, pParent)
	, m_strVSProjectPath(_T(""))
{

}

CVSCleanDlg::~CVSCleanDlg()
{
}

void CVSCleanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strVSProjectPath);
}


BEGIN_MESSAGE_MAP(CVSCleanDlg, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CVSCleanDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CVSCleanDlg ��Ϣ�������


void CVSCleanDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TCHAR FileName[MAX_PATH];
	DragQueryFile(hDropInfo, 0, FileName, MAX_PATH); //ȡ���ļ���	
	UpdateData(TRUE);
	m_strVSProjectPath = FileName;
	UpdateData(FALSE);
	CDialogEx::OnDropFiles(hDropInfo);
}


void CVSCleanDlg::OnBnClickedButton1()
{
	//��ȡ·��;
	UpdateData(TRUE);
	CFileFind finder;
	CString string;
	string = m_strVSProjectPath + "\\*.*";
	BOOL bwork = finder.FindFile(m_strVSProjectPath);
	finder.FindNextFile();
	//��ȡ�ļ��������ļ�����
	CString str1 = finder.GetFileName();
	if (m_strVSProjectPath.IsEmpty() && (str1.Compare("Desktop") == 0)) {
		MessageBox("��ѡ��Ҫɾ�����ļ�");
		//return;
	}
	else {

		CString string;

		string.Format("��ȷ����ȷ��ɾ��\n��ȡ����ȡ��ɾ��\n");

		//����;
		int mBoxID = ::MessageBox(NULL, string, "ɾ������,�������", MB_OKCANCEL);
		if (mBoxID == IDOK) {

			if (m_strVSProjectPath.IsEmpty() && (str1.Compare("Desktop") != 0)) {
				DelFile(m_strVSProjectPath);
				::MessageBoxA(NULL, "ɾ�����", "ɾ������", 0);
			}

			if (!m_strVSProjectPath.IsEmpty() && (str1.Compare("Desktop") != 0)
				|| !m_strVSProjectPath.IsEmpty() && (str1.Compare("Desktop") == 0)) {

				if (DelFile(m_strVSProjectPath)) {
					MessageBox("�������");
				}
				else {
					MessageBox("����ʧ��");
				}

			}

		}
		//ȡ��
		else if (mBoxID == IDCANCEL) {}

	}
}

const TCHAR* delFiles[] = {
	".obj", ".tlog", ".lastbuildstate", ".idb",
	".pdb", ".pch", ".res", ".ilk",
	".exe", ".sdf", ".ipch",".db" };
int len = _countof(delFiles);
bool CVSCleanDlg::DelFile(CString fileName)
{
	if (fileName.IsEmpty()) {
		MessageBox("·������");
		return false;
	}
	CFileFind finder;
	CString string;
	string = fileName + "\\*.*";

	BOOL bwork = finder.FindFile(string);
	while (bwork)
	{
		bwork = finder.FindNextFile();
		if ((finder.IsDots() == FALSE) && finder.IsDirectory())
		{
			CString name = finder.GetFileName();
			if (strcmp(name, ".vs") == 0 || strcmp(name, "Debug") == 0 ||
				strcmp(name, "ipch") == 0 || strcmp(name, "x64") == 0 ||
				strcmp(name, "Release") == 0)
			{
				//ɾ���ļ��м�������
				DelDirectory(finder.GetFilePath());
			}
			else

				DelFile(finder.GetFilePath());//�ݹ�
		}
		else
		{
			CString name = finder.GetFileName();
			LPCSTR File = PathFindExtension(name);//��ȡ��׺��

			for (int i = 0; i < len; i++)
			{
				if (strcmp(File, delFiles[i]) == 0)
				{
					//ɾ���ļ�
					DeleteFile(finder.GetFilePath());
					break;
				}
			}


		}
	}

	return true;
}


void CVSCleanDlg::DelDirectory(CString strDir)
{
	if (strDir.IsEmpty()) {
		RemoveDirectory(strDir);
		return;
	}

	//����ɾ���ļ������ļ��� 
	CFileFind   ff;

	BOOL bFound = ff.FindFile(strDir + _T("\\*"), 0);
	while (bFound) {
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))
			continue;
		//ȥ���ļ�(��)ֻ�������� 
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory()) {
			//�ݹ�ɾ�����ļ��� 
			DelDirectory(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else {
			DeleteFile(ff.GetFilePath());   //ɾ���ļ� 
		}
	}
	ff.Close();
	//Ȼ��ɾ�����ļ��� 
	RemoveDirectory(strDir);
}
