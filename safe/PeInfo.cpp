#include "stdafx.h"
#include "PeInfo.h"

char* GetFileMemPointer(char* pFilePath)
{
	//���ļ���ȡ���
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
		MessageBox(0,"�ļ���ʧ��\n",0,0);
		return NULL;
	}
	//��ȡ�ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//�����ڴ�
	char* pBuf = new char[dwFileSize] {};
	//���ļ�
	DWORD dwRead;
	ReadFile(hFile, pBuf, dwFileSize, &dwRead, NULL);
	//�ر��ļ�
	CloseHandle(hFile);
	//�����ļ����ڴ��е���ʼλ�ã���ַ��
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
//NTͷ
void ShowHeaderOfNt(char* pFileBuf)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + pFileBuf);

	printf("��ڵ㣺%08X\n", pNt->OptionalHeader.AddressOfEntryPoint);
	//...
}

void ShowHeaderOfSectionTable(char* pFileBuf)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + pFileBuf);
	//��ȡ����һ�����α��λ��
	PIMAGE_SECTION_HEADER pSecTable = IMAGE_FIRST_SECTION(pNt);

	for (DWORD i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		char name[9] = {};
		printf("����:%s", pSecTable->Name);
		//...
		pSecTable++;
	}
}
//RVA to FOA
DWORD RVAtoFOA(DWORD dwRVA, char* pFileBuf)
{
	//RVA - �������ڴ��е���ʼ��ַ = FOA - �������ļ��е���ʼ��ַ
	//RVA - VOffset = FOA - ROffset
	//FOA = RVA  - VOffset + ROffset

	//�������α��ȡ�������
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + pFileBuf);

	//��ȡ���θ���
	WORD dwSecCount = pNt->FileHeader.NumberOfSections;
	//��ȡ���α����ʼ��ַ
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNt);
	for (WORD i = 0; i < dwSecCount; i++)
	{
		//�ж��Ƿ��������ĸ�����
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
	//�������α��ȡ�������
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + pFileBuf);

	//��ȡ���θ���
	WORD dwSecCount = pNt->FileHeader.NumberOfSections;
	//��ȡ���α����ʼ��ַ
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNt);
	for (WORD i = 0; i < dwSecCount; i++)
	{
		//�ж��Ƿ��������ĸ�����
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
