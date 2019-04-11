// ResourceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "safe.h"
#include "ResourceDlg.h"
#include "afxdialogex.h"


// CResourceDlg 对话框

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


// CResourceDlg 消息处理程序



BOOL CResourceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	USES_CONVERSION;
	//导入表的RVA
	DWORD dwResRVA =
		m_pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	//将RVA转成FOA
	DWORD dwResFOA = RVAtoFOA(dwResRVA, m_pFileBuf);
	//获取在文件中的实际地址
	PIMAGE_RESOURCE_DIRECTORY pResHeader =
		(PIMAGE_RESOURCE_DIRECTORY)(dwResFOA + m_pFileBuf);

	//第一层
	//获取资源种类个数
	DWORD dwStringCount = pResHeader->NumberOfNamedEntries;
	DWORD dwNumberCount = pResHeader->NumberOfIdEntries;
	//资源总个数
	DWORD dwTotalCount = dwStringCount + dwNumberCount;
	//获取第一个资源种类的地址
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry1 =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)
		(pResHeader + 1);
	//统计每种资源的数量及名称，在选中事件中使用
	pResCount = new RES_COUNT[dwTotalCount];
	resCount = dwTotalCount;
	// 树节点的根节点和子节点
	HTREEITEM hItem, hSubItem;
	// 用来转换类型的
	CString id;
	for (DWORD i1 = 0; i1 < dwTotalCount; i1++)
	{
		//判断资源命名方式
		//如果pEntry1->NameIsString为1说明是字符串命名，
		//否则是ID
		int temp = i1;
		if (pEntry1->NameIsString)
		{
			//低31为是指向IMAGE_RESOURCE_DIR_STRING_U
			//结构的偏移量
			DWORD dwAddr =
				pEntry1->NameOffset +
				(DWORD)pResHeader;
			PIMAGE_RESOURCE_DIR_STRING_U pString =
				(PIMAGE_RESOURCE_DIR_STRING_U)dwAddr;

			WCHAR* pName = new WCHAR[pString->Length + 1]{};
			memcpy(pName, pString->NameString, pString->Length * 2);

			//添加到树控件
			char * szpName = W2A(pName);
			hItem = m_treeResource.InsertItem(szpName);
			//保存资源名
			pResCount[temp].resName = szpName;
			delete pName;


		}
		else
		{
			//低16位是其ID
			char* arryResType[] = { "", "鼠标指针（Cursor）", "位图（Bitmap）", "图标（Icon）", "菜单（Menu）"
				, "对话框（Dialog）", "字符串列表（String Table）", "字体目录（Font Directory）", "字体（Font）", "快捷键（Accelerators）"
				, "非格式化资源（Unformatted）", "消息列表（Message Table）", "鼠标指针组（Croup Cursor）", "", "图标组（Group Icon）", ""
				, "版本信息（Version Information）" };

			/*printf("+--------------------------+\n");*/
			if (pEntry1->Id < 17)
			{
				/*printf("        资源种类：%s\n",
					arryResType[pEntry1->Id]);*/
				//添加到树控件
				hItem = m_treeResource.InsertItem(arryResType[pEntry1->Id]);
				//保存资源名
				pResCount[temp].resName = new char[strlen(arryResType[pEntry1->Id]) + 1];
				pResCount[temp].resName = arryResType[pEntry1->Id];
			}
			else
			{
				/*printf("        资源种类：[%d]\n",
					pEntry1->Id);*/
				//添加到树控件
				id.Format("%hu", pEntry1->Id);
				hItem = m_treeResource.InsertItem(id);
				//保存资源名
				pResCount[temp].resName.Format("%d", pEntry1->Id);
			}
			printf("+--------------------------+\n");
		}
		
		//判断是否还有下一层
		if (pEntry1->DataIsDirectory)
		{
			//获取下一层的地址
			DWORD dwNext =
				pEntry1->OffsetToDirectory +
				(DWORD)pResHeader;

			PIMAGE_RESOURCE_DIRECTORY pRes2 =
				(PIMAGE_RESOURCE_DIRECTORY)dwNext;

			DWORD dwStringCount = pRes2->NumberOfNamedEntries;
			DWORD dwNumberCount = pRes2->NumberOfIdEntries;
			//资源总个数
			DWORD dwTotalCount = dwStringCount + dwNumberCount;
			// 保存每种资源的数量
			pResCount[temp].idCount = pRes2->NumberOfIdEntries;
			pResCount[temp].nameCount = pRes2->NumberOfNamedEntries;
			//获取第一个资源种类的地址
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry2 =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)
				(pRes2 + 1);

			for (DWORD i1 = 0; i1 < dwTotalCount; i1++)
			{
				RES_INFO resInfo = {};
				//判断资源命名方式
				//如果pEntry2->NameIsString为1说明是字符串命名，
				//否则是ID
				if (pEntry2->NameIsString)
				{
					//低31为是指向IMAGE_RESOURCE_DIR_STRING_U
					//结构的偏移量
					DWORD dwAddr =
						pEntry2->NameOffset +
						(DWORD)pResHeader;
					PIMAGE_RESOURCE_DIR_STRING_U pString =
						(PIMAGE_RESOURCE_DIR_STRING_U)dwAddr;

					WCHAR* pName = new WCHAR[pString->Length + 1]{};
					memcpy(pName, pString->NameString, pString->Length * 2);

					/*printf(" -->资源：%S\n", pName);*/

					char * szpName = W2A(pName);

					hSubItem = m_treeResource.InsertItem(szpName, hItem);
					resInfo.Name = szpName;
					delete pName;
				}
				else
				{
					//低16位是其ID
					printf(" -->资源：[%d]\n",
						pEntry2->Id);
					id.Format("%hu", pEntry2->Id);
					hSubItem = m_treeResource.InsertItem(id, hItem);
					resInfo.Name = id;
				}
				//判断是否还有下一层
				if (pEntry2->DataIsDirectory)
				{
					//获取下一层的地址
					DWORD dwNext =
						pEntry2->OffsetToDirectory +
						(DWORD)pResHeader;

					PIMAGE_RESOURCE_DIRECTORY pRes3 =
						(PIMAGE_RESOURCE_DIRECTORY)dwNext;

					DWORD dwStringCount = pRes3->NumberOfNamedEntries;
					DWORD dwNumberCount = pRes3->NumberOfIdEntries;
					//资源总个数
					DWORD dwTotalCount = dwStringCount + dwNumberCount;
					//获取第一个资源种类的地址
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry3 =
						(PIMAGE_RESOURCE_DIRECTORY_ENTRY)
						(pRes3 + 1);

					//第三层
					DWORD dwDataAddr =
						pEntry3->OffsetToData +
						(DWORD)pResHeader;

					PIMAGE_RESOURCE_DATA_ENTRY pDataEntry =
						(PIMAGE_RESOURCE_DATA_ENTRY)dwDataAddr;

					DWORD dwDataRVA = pDataEntry->OffsetToData;
					DWORD dwDataFOA = RVAtoFOA(dwDataRVA, m_pFileBuf);
					//获取在文件中的地址
					PBYTE pData = (PBYTE)(dwDataFOA + m_pFileBuf);
					//资源数据大小
					DWORD dwDataSize = pDataEntry->Size;

					//保存资源数据
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
				//下一个资源
				pEntry2++;
			}
		}
		//下一种资源
		pEntry1++;
	}
	// 名称，字符串
	UpdateData(TRUE);
	m_strRootNameCount.Format("%04X" , dwStringCount);
	m_strRootIdCount.Format("%04X", dwNumberCount);
	

	UpdateData(FALSE);

	return TRUE;
}


void CResourceDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
