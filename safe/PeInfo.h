#pragma once
#include <stdio.h>
#include <Windows.h>

char* GetFileMemPointer(char* pFilePath);
bool IsPeFile(char* pFileBuf);
void ShowHeaderOfNt(char* pFileBuf);
void ShowHeaderOfSectionTable(char* pFileBuf);
DWORD RVAtoFOA(DWORD dwRVA, char* pFileBuf);
DWORD RVAtoFOA(DWORD dwRVA, char* pFileBuf, CString &pSecName);

