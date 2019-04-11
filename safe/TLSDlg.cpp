// TLSDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "TLSDlg.h"
#include "afxdialogex.h"


// CTLSDlg �Ի���

IMPLEMENT_DYNAMIC(CTLSDlg, CDialogEx)

CTLSDlg::CTLSDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TLS_DIALOG, pParent)
	, m_strStartAddrVA(_T(""))
	, m_strEndAddrVA(_T(""))
	, m_strAddrIndexVA(_T(""))
	, m_strCallBacksAddrVA(_T(""))
	, m_strSizeofZeroFill(_T(""))
	, m_strChar(_T(""))
{
	m_pFileBuf = pFileBuf;
	m_pNt = pNt;
}

CTLSDlg::~CTLSDlg()
{
}

void CTLSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strStartAddrVA);
	DDX_Text(pDX, IDC_EDIT2, m_strEndAddrVA);
	DDX_Text(pDX, IDC_EDIT3, m_strAddrIndexVA);
	DDX_Text(pDX, IDC_EDIT4, m_strCallBacksAddrVA);
	DDX_Text(pDX, IDC_EDIT5, m_strSizeofZeroFill);
	DDX_Text(pDX, IDC_EDIT6, m_strChar);
}


BEGIN_MESSAGE_MAP(CTLSDlg, CDialogEx)
END_MESSAGE_MAP()


// CTLSDlg ��Ϣ�������


BOOL CTLSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//������RVA
	DWORD dwTLSRVA =
		m_pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress;
	//��RVAת��FOA
	DWORD dwTLSFOA = RVAtoFOA(dwTLSRVA, m_pFileBuf);
	//��ȡ���ļ��е�ʵ�ʵ�ַ
	PIMAGE_TLS_DIRECTORY pTLS =
		(PIMAGE_TLS_DIRECTORY)(dwTLSFOA + m_pFileBuf);
	UpdateData(TRUE);


	m_strStartAddrVA.Format("%08X",pTLS->StartAddressOfRawData);
	m_strEndAddrVA.Format("%08X", pTLS->EndAddressOfRawData);
	m_strAddrIndexVA.Format("%08X", pTLS->AddressOfIndex);
	m_strCallBacksAddrVA.Format("%08X", pTLS->AddressOfCallBacks);
	m_strSizeofZeroFill.Format("%08X", pTLS->SizeOfZeroFill);
	m_strChar.Format("%08X", pTLS->Characteristics);
	UpdateData(FALSE);



	return TRUE;  
}
