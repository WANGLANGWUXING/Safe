// VSCleanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "VSCleanDlg.h"
#include "afxdialogex.h"


// CVSCleanDlg 对话框

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


// CVSCleanDlg 消息处理程序


void CVSCleanDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR FileName[MAX_PATH];
	DragQueryFile(hDropInfo, 0, FileName, MAX_PATH); //取得文件名	
	UpdateData(TRUE);
	m_strVSProjectPath = FileName;
	UpdateData(FALSE);
	CDialogEx::OnDropFiles(hDropInfo);
}


void CVSCleanDlg::OnBnClickedButton1()
{
	//获取路径;
	UpdateData(TRUE);
	CFileFind finder;
	CString string;
	string = m_strVSProjectPath + "\\*.*";
	BOOL bwork = finder.FindFile(m_strVSProjectPath);
	finder.FindNextFile();
	//获取文件名或者文件夹名
	CString str1 = finder.GetFileName();
	if (m_strVSProjectPath.IsEmpty() && (str1.Compare("Desktop") == 0)) {
		MessageBox("请选择要删除的文件");
		//return;
	}
	else {

		CString string;

		string.Format("【确定】确定删除\n【取消】取消删除\n");

		//弹窗;
		int mBoxID = ::MessageBox(NULL, string, "删除操作,请谨慎！", MB_OKCANCEL);
		if (mBoxID == IDOK) {

			if (m_strVSProjectPath.IsEmpty() && (str1.Compare("Desktop") != 0)) {
				DelFile(m_strVSProjectPath);
				::MessageBoxA(NULL, "删除完成", "删除操作", 0);
			}

			if (!m_strVSProjectPath.IsEmpty() && (str1.Compare("Desktop") != 0)
				|| !m_strVSProjectPath.IsEmpty() && (str1.Compare("Desktop") == 0)) {

				if (DelFile(m_strVSProjectPath)) {
					MessageBox("清理完成");
				}
				else {
					MessageBox("清理失败");
				}

			}

		}
		//取消
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
		MessageBox("路径有误");
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
				//删除文件夹及其内容
				DelDirectory(finder.GetFilePath());
			}
			else

				DelFile(finder.GetFilePath());//递归
		}
		else
		{
			CString name = finder.GetFileName();
			LPCSTR File = PathFindExtension(name);//获取后缀名

			for (int i = 0; i < len; i++)
			{
				if (strcmp(File, delFiles[i]) == 0)
				{
					//删除文件
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

	//首先删除文件及子文件夹 
	CFileFind   ff;

	BOOL bFound = ff.FindFile(strDir + _T("\\*"), 0);
	while (bFound) {
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))
			continue;
		//去掉文件(夹)只读等属性 
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory()) {
			//递归删除子文件夹 
			DelDirectory(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else {
			DeleteFile(ff.GetFilePath());   //删除文件 
		}
	}
	ff.Close();
	//然后删除该文件夹 
	RemoveDirectory(strDir);
}
