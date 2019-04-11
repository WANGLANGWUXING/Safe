// DataDirectoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "DataDirectoryDlg.h"
#include "afxdialogex.h"


// CDataDirectoryDlg 对话框

IMPLEMENT_DYNAMIC(CDataDirectoryDlg, CDialogEx)

CDataDirectoryDlg::CDataDirectoryDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DATADIRECTRY_DIALOG, pParent)
	, m_strExportRva(_T(""))
	, m_strExportSize(_T(""))
	, m_strImportRva(_T(""))
	, m_strImportSize(_T(""))
	, m_strResourceRva(_T(""))
	, m_strResourceSize(_T(""))
	, m_strExceptionRva(_T(""))
	, m_strExceptionSize(_T(""))
	, m_strSecurityRva(_T(""))
	, m_strSecuritySize(_T(""))
	, m_strBaseRelocRva(_T(""))
	, m_strBaseRelocSize(_T(""))
	, m_strDebugRva(_T(""))
	, m_strDebugSize(_T(""))
	, m_strArchitectureRva(_T(""))
	, m_strArchitectureSize(_T(""))
	, m_strGlobalPtrRva(_T(""))
	, m_strGlobalPtrSize(_T(""))
	, m_strTLSRva(_T(""))
	, m_strTLSSize(_T(""))
	, m_strLoadConfigRva(_T(""))
	, m_strLoadConfigSize(_T(""))
	, m_strBoundImportRva(_T(""))
	, m_strBoundImportSize(_T(""))
	, m_strIAIRva(_T(""))
	, m_strIAISize(_T(""))
	, m_strDelayImportRva(_T(""))
	, m_strDelayImportSize(_T(""))
	, m_strComRva(_T(""))
	, m_strComSize(_T(""))
	, m_strRetainRva(_T(""))
	, m_strRetainSize(_T(""))
{
	m_pFileBuf = pFileBuf;
	m_pNt = pNt;
}

CDataDirectoryDlg::~CDataDirectoryDlg()
{
}

void CDataDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITEXPORTRVA, m_strExportRva);
	DDX_Text(pDX, IDC_EDITEXPORTSIZE, m_strExportSize);
	DDX_Text(pDX, IDC_EDITIMPORTRVA, m_strImportRva);
	DDX_Text(pDX, IDC_EDITIMPORTSIZE, m_strImportSize);
	DDX_Text(pDX, IDC_EDITRESOURCERVA, m_strResourceRva);
	DDX_Text(pDX, IDC_EDITRESOURCESIZE, m_strResourceSize);
	DDX_Text(pDX, IDC_EDITEXECPTIONRVA, m_strExceptionRva);
	DDX_Text(pDX, IDC_EDITEXECPTIONSIZE, m_strExceptionSize);
	DDX_Text(pDX, IDC_EDITSECURITYRVA, m_strSecurityRva);
	DDX_Text(pDX, IDC_EDITSECURITYSIZE, m_strSecuritySize);
	DDX_Text(pDX, IDC_EDITBASERELOCRVA, m_strBaseRelocRva);
	DDX_Text(pDX, IDC_EDITBASERELOCSIZE, m_strBaseRelocSize);
	DDX_Text(pDX, IDC_EDITDEBUGRVA, m_strDebugRva);
	DDX_Text(pDX, IDC_EDITDEBUDSIZE, m_strDebugSize);
	DDX_Text(pDX, IDC_EDITARCHITECTURERVA, m_strArchitectureRva);
	DDX_Text(pDX, IDC_EDITARCHITECTURESIZE, m_strArchitectureSize);
	DDX_Text(pDX, IDC_EDITGLOBAKPTRRVA, m_strGlobalPtrRva);
	DDX_Text(pDX, IDC_EDITGLOBAKPTRSIZE, m_strGlobalPtrSize);
	DDX_Text(pDX, IDC_EDITTLSRVA, m_strTLSRva);
	DDX_Text(pDX, IDC_EDITTLSSIZE, m_strTLSSize);
	DDX_Text(pDX, IDC_EDITLOAD_CONFIGRVA, m_strLoadConfigRva);
	DDX_Text(pDX, IDC_EDITLOAD_CONFIGSIZE, m_strLoadConfigSize);
	DDX_Text(pDX, IDC_EDITBOUND_IMPORTRVA, m_strBoundImportRva);
	DDX_Text(pDX, IDC_EDITBOUND_IMPORTSIZE, m_strBoundImportSize);
	DDX_Text(pDX, IDC_EDITIAIRVA, m_strIAIRva);
	DDX_Text(pDX, IDC_EDITIAISIZE, m_strIAISize);
	DDX_Text(pDX, IDC_EDITDELAY_IMPORTRVA, m_strDelayImportRva);
	DDX_Text(pDX, IDC_EDITDELAY_IMPORTSIZE, m_strDelayImportSize);
	DDX_Text(pDX, IDC_COMRVA, m_strComRva);
	DDX_Text(pDX, IDC_EDITCOMSIZE, m_strComSize);
	DDX_Text(pDX, IDC_EDITRETAINRVA, m_strRetainRva);
	DDX_Text(pDX, IDC_EDITRETAINSIZE, m_strRetainSize);
}


BEGIN_MESSAGE_MAP(CDataDirectoryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNExportInfo, &CDataDirectoryDlg::OnBnClickedBtnexportinfo)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTNImportInfo, &CDataDirectoryDlg::OnBnClickedBtnimportinfo)
	ON_BN_CLICKED(IDC_BTNResourceInfo, &CDataDirectoryDlg::OnBnClickedBtnresourceinfo)
	ON_BN_CLICKED(IDC_BTNBaseRelocInfo, &CDataDirectoryDlg::OnBnClickedBtnbaserelocinfo)
	ON_BN_CLICKED(IDC_BTNTLSInfo, &CDataDirectoryDlg::OnBnClickedBtntlsinfo)
	ON_BN_CLICKED(IDC_BTNDelayImportInfo, &CDataDirectoryDlg::OnBnClickedBtndelayimportinfo)
END_MESSAGE_MAP()


// CDataDirectoryDlg 消息处理程序


BOOL CDataDirectoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(TRUE);
	
	
	 
	m_strExportRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[0].VirtualAddress);
	m_strExportSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[0].Size);
	m_strImportRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress);
	m_strImportSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[1].Size);
	m_strResourceRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[2].VirtualAddress);
	m_strResourceSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[2].Size);
	m_strExceptionRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[3].VirtualAddress);
	m_strExceptionSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[3].Size);
	m_strSecurityRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[4].VirtualAddress);
	m_strSecuritySize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[4].Size);
	m_strBaseRelocRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress);
	m_strBaseRelocSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[5].Size);
	m_strDebugRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[6].VirtualAddress);
	m_strDebugSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[6].Size);
	m_strArchitectureRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[7].VirtualAddress);
	m_strArchitectureSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[7].Size);
	m_strGlobalPtrRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[8].VirtualAddress);
	m_strGlobalPtrSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[8].Size);
	m_strTLSRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[9].VirtualAddress);
	m_strTLSSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[9].Size);
	m_strLoadConfigRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[10].VirtualAddress);
	m_strLoadConfigSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[10].Size);
	m_strBoundImportRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[11].VirtualAddress);
	m_strBoundImportSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[11].Size);
	m_strIAIRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[12].VirtualAddress);
	m_strIAISize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[12].Size);
	m_strDelayImportRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[13].VirtualAddress);
	m_strDelayImportSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[13].Size);
	m_strComRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[14].VirtualAddress);
	m_strComSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[14].Size);
	m_strRetainRva.Format("%08X", m_pNt->OptionalHeader.DataDirectory[15].VirtualAddress);
	m_strRetainSize.Format("%08X", m_pNt->OptionalHeader.DataDirectory[15].Size);
	UpdateData(FALSE);

	return TRUE; 
}


void CDataDirectoryDlg::OnBnClickedBtnexportinfo()
{
	CExportDlg exportTable(m_pFileBuf, m_pNt);
	exportTable.DoModal();
}


void CDataDirectoryDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//EndDialog(0);
	CDialogEx::OnClose();
}


void CDataDirectoryDlg::OnBnClickedBtnimportinfo()
{
	CImportDlg importTable(m_pFileBuf, m_pNt);
	importTable.DoModal();
}


void CDataDirectoryDlg::OnBnClickedBtnresourceinfo()
{
	CResourceDlg resourceTable(m_pFileBuf, m_pNt);
	resourceTable.DoModal();
}


void CDataDirectoryDlg::OnBnClickedBtnbaserelocinfo()
{
	CRelocateDlg relocateTable(m_pFileBuf, m_pNt);
	relocateTable.DoModal();
}


void CDataDirectoryDlg::OnBnClickedBtntlsinfo()
{
	UpdateData(TRUE);
	if (_ttoi(m_strTLSSize) == 0)
	{
		MessageBox("无TLS表");
		return;
	}
	CTLSDlg tlsTable(m_pFileBuf, m_pNt);
	tlsTable.DoModal();
}


void CDataDirectoryDlg::OnBnClickedBtndelayimportinfo()
{
	UpdateData(TRUE);
	if (_ttoi(m_strDelayImportSize) == 0)
	{
		MessageBox("无延迟加载表");
		return;
	}
	CDelayImportDlg delayImportTable(m_pFileBuf, m_pNt);
	delayImportTable.DoModal();
}
