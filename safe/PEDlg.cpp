// PEDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "PEDlg.h"
#include "afxdialogex.h"


// CPEDlg 对话框

IMPLEMENT_DYNAMIC(CPEDlg, CDialogEx)

CPEDlg::CPEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_DIALOG, pParent)
{

}

CPEDlg::~CPEDlg()
{
}

void CPEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITFilePath, m_strFilePath);
	DDX_Text(pDX, IDC_EDITOEP, m_strOEP);
	DDX_Text(pDX, IDC_EDITSubSystem, m_strSubSystem);
	DDX_Text(pDX, IDC_EDITImageBase, m_strImageBase);
	DDX_Text(pDX, IDC_EDITNumberOfSections, m_strNumberOfSections);
	DDX_Text(pDX, IDC_EDITSizeOfImage, m_strSizeOfImage);
	DDX_Text(pDX, IDC_EDITTimeDateStamp, m_strTimeDateStamp);
	DDX_Text(pDX, IDC_BaseOfCode, m_strBaseOfCode);
	DDX_Text(pDX, IDC_EDITSizeOfHeaders, m_strSizeOfHeaders);
	DDX_Text(pDX, IDC_EDITBaseOfData, m_strBaseOfData);
	DDX_Text(pDX, IDC_EDITCharacteristics, m_strCharacteristics);
	DDX_Text(pDX, IDC_EDITSectionAlignment, m_strSectionAlignment);
	DDX_Text(pDX, IDC_EDITCheckSum, m_strCheckSum);
	DDX_Text(pDX, IDC_EDITFileAlignment, m_strFileAlignment);
	DDX_Text(pDX, IDC_EDITSizeOfOptionalHeader, m_strSizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDITMagic, m_strMagic);
	DDX_Text(pDX, IDC_EDITNumberOfRvaAndSizes, m_strNumberOfRvaAndSizes);

}


BEGIN_MESSAGE_MAP(CPEDlg, CDialogEx)
//	ON_WM_MENUDRAG()
ON_WM_DROPFILES()
ON_BN_CLICKED(IDC_BUTTONSectionHeader, &CPEDlg::OnBnClickedButtonsectionheader)
ON_BN_CLICKED(IDC_BTNDataDirectory, &CPEDlg::OnBnClickedBtndatadirectory)
ON_BN_CLICKED(IDC_BUTTON1, &CPEDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPEDlg 消息处理程序




void CPEDlg::OnDropFiles(HDROP hDropInfo)
{
	char szStr[MAX_PATH];
	DragQueryFileA(hDropInfo, 0, szStr, MAX_PATH);
	DragFinish(hDropInfo);
	pFileBuf = GetFileMemPointer(szStr);
	if (!pFileBuf)
	{
		return;

	}
	if (!IsPeFile(pFileBuf))
	{
		MessageBox("该文件不是PE文件");
		return;
	}
	pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + pFileBuf);

	UpdateData(TRUE);
	m_strFilePath = szStr;
	m_strOEP.Format("%08X", pNt->OptionalHeader.AddressOfEntryPoint);
	m_strSubSystem.Format("%04X", pNt->OptionalHeader.Subsystem);
	m_strImageBase.Format("%08X", pNt->OptionalHeader.ImageBase);
	m_strNumberOfSections.Format("%04X", pNt->FileHeader.NumberOfSections);
	m_strSizeOfImage.Format("%08X", pNt->OptionalHeader.SizeOfImage);
	m_strTimeDateStamp.Format("%08X", pNt->FileHeader.TimeDateStamp);
	m_strBaseOfCode.Format("%08X", pNt->OptionalHeader.BaseOfCode);
	m_strSizeOfHeaders.Format("%08X", pNt->OptionalHeader.SizeOfHeaders);
	m_strBaseOfData.Format("%08X", pNt->OptionalHeader.BaseOfData);
	m_strCharacteristics.Format("%04X", pNt->FileHeader.Characteristics);
	m_strSectionAlignment.Format("%08X", pNt->OptionalHeader.SectionAlignment);
	m_strCheckSum.Format("%08X", pNt->OptionalHeader.CheckSum);
	m_strFileAlignment.Format("%08X", pNt->OptionalHeader.FileAlignment);
	m_strSizeOfOptionalHeader.Format("%04X", pNt->FileHeader.SizeOfOptionalHeader);
	m_strMagic.Format("%04X", pNt->OptionalHeader.Magic);
	m_strNumberOfRvaAndSizes.Format("%08X", pNt->OptionalHeader.NumberOfRvaAndSizes);

	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CPEDlg::OnBnClickedButtonsectionheader()
{
	UpdateData(TRUE);
	if (m_strFilePath == "")
	{
		MessageBox("请拖入有效PE文件");
		return;
	}
	CSectionTableDlg section(pFileBuf, pNt);
	section.DoModal();
}


void CPEDlg::OnBnClickedBtndatadirectory()
{
	UpdateData(TRUE);
	if (m_strFilePath == "")
	{
		MessageBox("请拖入有效PE文件");
		return;
	}
	CDataDirectoryDlg dataDirectory(pFileBuf, pNt);
	dataDirectory.DoModal();
}


BOOL CPEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//MessageBox("test");
	// 在OnInitDialog添加代码
	// 管理员身份运行仍然可接收文件拖拽的方法:
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);

	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);

	return TRUE;  
}


void CPEDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if (m_strFilePath == "")
	{
		MessageBox("请拖入有效PE文件");
		return;
	}
	CROAtoFOA ROAtoFOADlg(pFileBuf);
	ROAtoFOADlg.DoModal();
}
