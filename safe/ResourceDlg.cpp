// ResourceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "safe.h"
#include "ResourceDlg.h"
#include "afxdialogex.h"


// CResourceDlg �Ի���

IMPLEMENT_DYNAMIC(CResourceDlg, CDialogEx)

CResourceDlg::CResourceDlg(char *pFileBuf, PIMAGE_NT_HEADERS pNt, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RESOURCE_DIALOG, pParent)
	, m_strRootNameCount(_T(""))
	, m_strRootIdCount(_T(""))
	, m_strChildNameCount(_T(""))
	, m_strChildIdCount(_T(""))
	, m_strItemRva(_T(""))
	, m_strItemOffset(_T(""))
	, m_strItemSize(_T(""))
{
	m_pFileBuf = pFileBuf;
	m_pNt = pNt;
}

CResourceDlg::~CResourceDlg()
{
}

void CResourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_RESOURCE, m_treeResource);
	DDX_Text(pDX, IDC_EDIT1, m_strRootNameCount);
	DDX_Text(pDX, IDC_EDIT2, m_strRootIdCount);
	DDX_Text(pDX, IDC_EDIT4, m_strChildNameCount);
	DDX_Text(pDX, IDC_EDIT5, m_strChildIdCount);
	DDX_Text(pDX, IDC_EDIT6, m_strItemRva);
	DDX_Text(pDX, IDC_EDIT7, m_strItemOffset);
	DDX_Text(pDX, IDC_EDIT8, m_strItemSize);
}


BEGIN_MESSAGE_MAP(CResourceDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_RESOURCE, &CResourceDlg::OnTvnSelchangedTreeResource)
END_MESSAGE_MAP()


// CResourceDlg ��Ϣ�������



BOOL CResourceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	USES_CONVERSION;
	//������RVA
	DWORD dwResRVA =
		m_pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	//��RVAת��FOA
	DWORD dwResFOA = RVAtoFOA(dwResRVA, m_pFileBuf);
	//��ȡ���ļ��е�ʵ�ʵ�ַ
	PIMAGE_RESOURCE_DIRECTORY pResHeader =
		(PIMAGE_RESOURCE_DIRECTORY)(dwResFOA + m_pFileBuf);

	//��һ��
	//��ȡ��Դ�������
	DWORD dwStringCount = pResHeader->NumberOfNamedEntries;
	DWORD dwNumberCount = pResHeader->NumberOfIdEntries;
	//��Դ�ܸ���
	DWORD dwTotalCount = dwStringCount + dwNumberCount;
	//��ȡ��һ����Դ����ĵ�ַ
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry1 =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)
		(pResHeader + 1);
	//ͳ��ÿ����Դ�����������ƣ���ѡ���¼���ʹ��
	pResCount = new RES_COUNT[dwTotalCount];
	resCount = dwTotalCount;
	// ���ڵ�ĸ��ڵ���ӽڵ�
	HTREEITEM hItem, hSubItem;
	// ����ת�����͵�
	CString id;
	for (DWORD i1 = 0; i1 < dwTotalCount; i1++)
	{
		//�ж���Դ������ʽ
		//���pEntry1->NameIsStringΪ1˵�����ַ���������
		//������ID
		int temp = i1;
		if (pEntry1->NameIsString)
		{
			//��31Ϊ��ָ��IMAGE_RESOURCE_DIR_STRING_U
			//�ṹ��ƫ����
			DWORD dwAddr =
				pEntry1->NameOffset +
				(DWORD)pResHeader;
			PIMAGE_RESOURCE_DIR_STRING_U pString =
				(PIMAGE_RESOURCE_DIR_STRING_U)dwAddr;

			WCHAR* pName = new WCHAR[pString->Length + 1]{};
			memcpy(pName, pString->NameString, pString->Length * 2);

			//��ӵ����ؼ�
			char * szpName = W2A(pName);
			hItem = m_treeResource.InsertItem(szpName);
			//������Դ��
			pResCount[temp].resName = szpName;
			delete pName;


		}
		else
		{
			//��16λ����ID
			char* arryResType[] = { "", "���ָ�루Cursor��", "λͼ��Bitmap��", "ͼ�꣨Icon��", "�˵���Menu��"
				, "�Ի���Dialog��", "�ַ����б�String Table��", "����Ŀ¼��Font Directory��", "���壨Font��", "��ݼ���Accelerators��"
				, "�Ǹ�ʽ����Դ��Unformatted��", "��Ϣ�б�Message Table��", "���ָ���飨Croup Cursor��", "", "ͼ���飨Group Icon��", ""
				, "�汾��Ϣ��Version Information��" };

			/*printf("+--------------------------+\n");*/
			if (pEntry1->Id < 17)
			{
				/*printf("        ��Դ���ࣺ%s\n",
					arryResType[pEntry1->Id]);*/
				//��ӵ����ؼ�
				hItem = m_treeResource.InsertItem(arryResType[pEntry1->Id]);
				//������Դ��
				pResCount[temp].resName = new char[strlen(arryResType[pEntry1->Id]) + 1];
				pResCount[temp].resName = arryResType[pEntry1->Id];
			}
			else
			{
				/*printf("        ��Դ���ࣺ[%d]\n",
					pEntry1->Id);*/
				//��ӵ����ؼ�
				id.Format("%hu", pEntry1->Id);
				hItem = m_treeResource.InsertItem(id);
				//������Դ��
				pResCount[temp].resName.Format("%d", pEntry1->Id);
			}
			printf("+--------------------------+\n");
		}
		
		//�ж��Ƿ�����һ��
		if (pEntry1->DataIsDirectory)
		{
			//��ȡ��һ��ĵ�ַ
			DWORD dwNext =
				pEntry1->OffsetToDirectory +
				(DWORD)pResHeader;

			PIMAGE_RESOURCE_DIRECTORY pRes2 =
				(PIMAGE_RESOURCE_DIRECTORY)dwNext;

			DWORD dwStringCount = pRes2->NumberOfNamedEntries;
			DWORD dwNumberCount = pRes2->NumberOfIdEntries;
			//��Դ�ܸ���
			DWORD dwTotalCount = dwStringCount + dwNumberCount;
			// ����ÿ����Դ������
			pResCount[temp].idCount = pRes2->NumberOfIdEntries;
			pResCount[temp].nameCount = pRes2->NumberOfNamedEntries;
			//��ȡ��һ����Դ����ĵ�ַ
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry2 =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)
				(pRes2 + 1);

			for (DWORD i1 = 0; i1 < dwTotalCount; i1++)
			{
				RES_INFO resInfo = {};
				//�ж���Դ������ʽ
				//���pEntry2->NameIsStringΪ1˵�����ַ���������
				//������ID
				if (pEntry2->NameIsString)
				{
					//��31Ϊ��ָ��IMAGE_RESOURCE_DIR_STRING_U
					//�ṹ��ƫ����
					DWORD dwAddr =
						pEntry2->NameOffset +
						(DWORD)pResHeader;
					PIMAGE_RESOURCE_DIR_STRING_U pString =
						(PIMAGE_RESOURCE_DIR_STRING_U)dwAddr;

					WCHAR* pName = new WCHAR[pString->Length + 1]{};
					memcpy(pName, pString->NameString, pString->Length * 2);

					/*printf(" -->��Դ��%S\n", pName);*/

					char * szpName = W2A(pName);

					hSubItem = m_treeResource.InsertItem(szpName, hItem);
					resInfo.Name = szpName;
					delete pName;
				}
				else
				{
					//��16λ����ID
					printf(" -->��Դ��[%d]\n",
						pEntry2->Id);
					id.Format("%hu", pEntry2->Id);
					hSubItem = m_treeResource.InsertItem(id, hItem);
					resInfo.Name = id;
				}
				//�ж��Ƿ�����һ��
				if (pEntry2->DataIsDirectory)
				{
					//��ȡ��һ��ĵ�ַ
					DWORD dwNext =
						pEntry2->OffsetToDirectory +
						(DWORD)pResHeader;

					PIMAGE_RESOURCE_DIRECTORY pRes3 =
						(PIMAGE_RESOURCE_DIRECTORY)dwNext;

					DWORD dwStringCount = pRes3->NumberOfNamedEntries;
					DWORD dwNumberCount = pRes3->NumberOfIdEntries;
					//��Դ�ܸ���
					DWORD dwTotalCount = dwStringCount + dwNumberCount;
					//��ȡ��һ����Դ����ĵ�ַ
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry3 =
						(PIMAGE_RESOURCE_DIRECTORY_ENTRY)
						(pRes3 + 1);

					//������
					DWORD dwDataAddr =
						pEntry3->OffsetToData +
						(DWORD)pResHeader;

					PIMAGE_RESOURCE_DATA_ENTRY pDataEntry =
						(PIMAGE_RESOURCE_DATA_ENTRY)dwDataAddr;

					DWORD dwDataRVA = pDataEntry->OffsetToData;
					DWORD dwDataFOA = RVAtoFOA(dwDataRVA, m_pFileBuf);
					//��ȡ���ļ��еĵ�ַ
					PBYTE pData = (PBYTE)(dwDataFOA + m_pFileBuf);
					//��Դ���ݴ�С
					DWORD dwDataSize = pDataEntry->Size;

					//������Դ����
					resInfo.RVA = dwDataRVA;
					resInfo.Offset = dwDataFOA;
					resInfo.Size = dwDataSize;
					//RES_INFO resInfo = { dwDataRVA ,dwDataFOA ,dwDataSize };
					
					/*printf("\t[ ");
					for (int i = 0; i < 10; i++)
					{
						printf("%02X ", pData[i]);
					}
					printf("]\n");*/
				}
				vecResInfo.push_back(resInfo);
				//��һ����Դ
				pEntry2++;
			}
		}
		//��һ����Դ
		pEntry1++;
	}
	// ���ƣ��ַ���
	UpdateData(TRUE);
	m_strRootNameCount.Format("%04X" , dwStringCount);
	m_strRootIdCount.Format("%04X", dwNumberCount);
	

	UpdateData(FALSE);

	return TRUE;
}


void CResourceDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//EndDialog(0);
	CDialogEx::OnClose();
}


void CResourceDlg::OnTvnSelchangedTreeResource(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM treeItem = m_treeResource.GetSelectedItem();
	CString text = m_treeResource.GetItemText(treeItem);
	UpdateData(TRUE);
	for (int i = 0; i < resCount; i++)
	{
		if (strcmp(text, pResCount[i].resName) == 0)
		{
			m_strChildNameCount.Format("%04X", pResCount[i].nameCount);
			m_strChildIdCount.Format("%04X", pResCount[i].idCount);
			goto end;
		}
	}
	for (size_t i = 0; i < vecResInfo.size(); i++)
	{
		if (strcmp(text, vecResInfo[i].Name) == 0)
		{
			m_strItemRva.Format("%08X", vecResInfo[i].RVA);
			m_strItemOffset.Format("%08X", vecResInfo[i].Offset);
			m_strItemSize.Format("%08X", vecResInfo[i].Size);
		}
	}

end:;
	UpdateData(FALSE);
	//MessageBox(text);
	*pResult = 0;
}
