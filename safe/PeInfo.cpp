#include "stdafx.h"
#include "PeInfo.h"

char* GetFileMemPointer(char* pFilePath)
{
	//打开文件获取句柄
	HANDLE hFile = CreateFileA(
		pFilePath,
		GENERIC_READ,
		FALSE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(0,"文件打开失败\n",0,0);
		return NULL;
	}
	//获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//申请内存
	char* pBuf = new char[dwFileSize] {};
	//读文件
	DWORD dwRead;
	ReadFile(hFile, pBuf, dwFileSize, &dwRead, NULL);
	//关闭文件
	CloseHandle(hFile);
	//返回文件在内存中的起始位置（基址）
	return pBuf;
}

bool IsPeFile(char* pFileBuf)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + pFileBuf);
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}
	return true;

}
//NT头
void ShowHeaderOfNt(char* pFileBuf)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + pFileBuf);

	printf("入口点：%08X\n", pNt->OptionalHeader.AddressOfEntryPoint);
	//...
}

void ShowHeaderOfSectionTable(char* pFileBuf)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + pFileBuf);
	//获取到第一个区段表的位置
	PIMAGE_SECTION_HEADER pSecTable = IMAGE_FIRST_SECTION(pNt);

	for (DWORD i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		char name[9] = {};
		printf("名字:%s", pSecTable->Name);
		//...
		pSecTable++;
	}
}
//RVA to FOA
DWORD RVAtoFOA(DWORD dwRVA, char* pFileBuf)
{
	//RVA - 区段在内存中的起始地址 = FOA - 区段在文件中的起始地址
	//RVA - VOffset = FOA - ROffset
	//FOA = RVA  - VOffset + ROffset

	//遍历区段表获取相关数据
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + pFileBuf);

	//获取区段个数
	WORD dwSecCount = pNt->FileHeader.NumberOfSections;
	//获取区段表的起始地址
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNt);
	for (WORD i = 0; i < dwSecCount; i++)
	{
		//判断是否落在了哪个区段
		if (dwRVA >= pSec->VirtualAddress &&
			dwRVA < pSec->VirtualAddress + pSec->SizeOfRawData)
		{
			//pSecName = pSec->Name;
			return
				dwRVA -
				pSec->VirtualAddress +
				pSec->PointerToRawData;
		}
		pSec++;
	}
	return 0;
}

DWORD RVAtoFOA(DWORD dwRVA, char* pFileBuf, CString &pSecName)
{
	//遍历区段表获取相关数据
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + pFileBuf);

	//获取区段个数
	WORD dwSecCount = pNt->FileHeader.NumberOfSections;
	//获取区段表的起始地址
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNt);
	for (WORD i = 0; i < dwSecCount; i++)
	{
		//判断是否落在了哪个区段
		if (dwRVA >= pSec->VirtualAddress &&
			dwRVA < pSec->VirtualAddress + pSec->SizeOfRawData)
		{
			pSecName= pSec->Name;
			//pSecName = pSec->Name;
			return
				dwRVA -
				pSec->VirtualAddress +
				pSec->PointerToRawData;
		}
		pSec++;
	}
	return 0;
}
