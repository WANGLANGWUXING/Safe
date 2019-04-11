// ExportDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "ExportDlg.h"
#include "afxdialogex.h"


// CExportDlg �Ի���

IMPLEMENT_DYNAMIC(CExportDlg, CDialogEx)

CExportDlg::CExportDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXPORT_DIALOG, pParent)
	, m_strExportOffset(_T(""))
	, m_strCharacteristic(_T(""))
	, m_strBase(_T(""))
	, m_strName(_T(""))
	, m_strNameStr(_T(""))
	, m_strNumberOfFunctions(_T(""))
	, m_strNumberOfNames(_T(""))
	, m_strAddressOfFunctions(_T(""))
	, m_strAddressOfNames(_T(""))
	, m_strAddressOfNameOrdinals(_T(""))
{
	m_pFileBuf = pFileBuf;
	m_pNt = pNt;
}

CExportDlg::~CExportDlg()
{
}

void CExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strExportOffset);
	DDX_Text(pDX, IDC_EDIT2, m_strCharacteristic);
	DDX_Text(pDX, IDC_EDIT3, m_strBase);
	DDX_Text(pDX, IDC_EDIT4, m_strName);
	DDX_Text(pDX, IDC_EDIT5, m_strNameStr);
	DDX_Text(pDX, IDC_EDIT6, m_strNumberOfFunctions);
	DDX_Text(pDX, IDC_EDIT7, m_strNumberOfNames);
	DDX_Text(pDX, IDC_EDIT8, m_strAddressOfFunctions);
	DDX_Text(pDX, IDC_EDIT9, m_strAddressOfNames);
	DDX_Text(pDX, IDC_EDIT10, m_strAddressOfNameOrdinals);
	DDX_Control(pDX, IDC_LIST1, m_listExportFuns);
}


BEGIN_MESSAGE_MAP(CExportDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CExportDlg ��Ϣ�������


BOOL CExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//�������RVA
	DWORD dwExprotRVA =
		m_pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	//��RVAת��FOA
	DWORD dwExportFOA = RVAtoFOA(dwExprotRVA, m_pFileBuf);
	//������
	PIMAGE_EXPORT_DIRECTORY pExport =
		(PIMAGE_EXPORT_DIRECTORY)
		(m_pFileBuf + dwExportFOA);
	
	//��ȡ������ַ���е�Ԫ�ظ���
	DWORD dwFunCount = pExport->NumberOfFunctions;
	//��ȡ�������Ʊ��е�Ԫ�ظ���
	DWORD dwNameCount = pExport->NumberOfNames;

	//��ȡ������ַ�����Ʊ���ű��FOA
	DWORD dwFunFOA = RVAtoFOA(pExport->AddressOfFunctions, m_pFileBuf);
	DWORD dwNameFOA = RVAtoFOA(pExport->AddressOfNames, m_pFileBuf);
	DWORD dwOrdinalFOA = RVAtoFOA(pExport->AddressOfNameOrdinals, m_pFileBuf);

	//��ȡ���ű����ļ��е�ʵ�ʵ�ַ
	DWORD* pFunAddr = (DWORD*)(dwFunFOA + m_pFileBuf);
	DWORD* pNameAddr = (DWORD*)(dwNameFOA + m_pFileBuf);
	//��ű��Ԫ�ش�С��2���ֽ�
	WORD* pOrdinalAddr = (WORD*)(dwOrdinalFOA + m_pFileBuf);

	//���ģ�������
	//printf("moduleName:%s\n", RVAtoFOA(pExport->Name, m_pFileBuf) + m_pFileBuf);
	UpdateData(TRUE);

	m_strExportOffset.Format("%08X", dwExportFOA);
	m_strCharacteristic.Format("%08X", pExport->Characteristics);
	m_strBase.Format("%08X", pExport->Base);
	m_strName.Format("%08X", pExport->Name);
	m_strNameStr.Format("%s", RVAtoFOA(pExport->Name, m_pFileBuf) + m_pFileBuf);
	m_strNumberOfFunctions.Format("%08X", pExport->NumberOfFunctions);
	m_strNumberOfNames.Format("%08X", pExport->NumberOfNames);
	m_strAddressOfFunctions.Format("%08X", pExport->AddressOfFunctions);
	m_strAddressOfNames.Format("%08X", pExport->AddressOfNames);
	m_strAddressOfNameOrdinals.Format("%08X", pExport->AddressOfNameOrdinals);
	UpdateData(FALSE);

	m_listExportFuns.AddColumn(4, "���",100, "RVA", 150, "ƫ��", 150, "������",150 );

	//ѭ������
	for (DWORD i = 0; i < dwFunCount; i++)
	{
		//�������Ч��ַ��ֱ������
		if (pFunAddr[i] == NULL)
		{
			continue;
		}
		TCHAR* exportFuns[] = { new TCHAR[9], new TCHAR[9], new TCHAR[9] };

		//�ж��Ƿ�������
		BOOL bRet = TRUE;
		for (DWORD n = 0; n < dwNameCount; n++)
		{
			//������˵���������ֵ�
			if (i == pOrdinalAddr[n])
			{
				//�����Ʊ���ȡ����Ӧ�����Ƶ�ַ��RVA��
				DWORD dwNameRVA = pNameAddr[n];
				DWORD dwNameFOA = RVAtoFOA(dwNameRVA, m_pFileBuf);
				char* pName = m_pFileBuf + dwNameFOA;
				/*printf("name:%s  Address:%08X  "
					"ordinal:%d RealOrdinal:%d\n",
					pName, pFunAddr[i], i, i + pExport->Base);
				*/
				_stprintf_s(exportFuns[0], 9, "%d", i + pExport->Base);
				_stprintf_s(exportFuns[1], 9, "%08X", pFunAddr[i]);
				_stprintf_s(exportFuns[2], 9, "%08X", RVAtoFOA(pFunAddr[i], m_pFileBuf));
				m_listExportFuns.AddItem(4, exportFuns[0], exportFuns[1], exportFuns[2], pName);
				bRet = FALSE;
				break;
			}
		}
		//û������ֻ�����
		if (bRet)
		{
			_stprintf_s(exportFuns[0], 9, "%d", i + pExport->Base);
			_stprintf_s(exportFuns[1], 9, "%08X", pFunAddr[i]);
			_stprintf_s(exportFuns[2], 9, "%08X", RVAtoFOA(pFunAddr[i], m_pFileBuf));
			m_listExportFuns.AddItem(4, exportFuns[0], exportFuns[1], exportFuns[2], "-");

			/*printf("name:[NULL]  Address:[%08X] "
				" ordinal:[%d] RealOrdinal:[%d]\n",
				pFunAddr[i], i, i + pExport->Base);*/
		}

	}
	
	
	return TRUE;  
}


void CExportDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//EndDialog(0);
	CDialogEx::OnClose();
}
