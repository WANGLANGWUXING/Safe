#pragma once
#include "common.h"
typedef struct _VIRUSINFO
{
	CHAR FileName[MAX_PATH]{};
	CHAR FileMd5[MAX_PATH]{};
}VIRUSINFO, *PVIRUSINFO;

class CVirusManage
{
public:
	CVirusManage();
	~CVirusManage();
	// ������ָ��
	PVIRUSINFO m_pVirusLib = nullptr;
	// ����������
	DWORD m_dwVirusCount = 0;
	// ���̰�����ָ��
	PVIRUSINFO m_pProcessLib = nullptr;
	// ���̰���������
	DWORD m_dwProcessCount = 0;
	// ���ز�����
	DWORD LoadVirusLibData();
	// ��ȡ������
	VOID GetVirusLib(PVIRUSINFO &nVIRUSINFO);
	// ���������
	VOID ClearVirusLibData();
	// ���ò�����
	VOID SetVirusLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount);
	// ���ؽ��̰�����
	DWORD LoadProcessLibData();
	// ��ȡ���̰�����
	VOID GetProcessLib(PVIRUSINFO &nVIRUSINFO);
	// ������̰�����
	VOID ClearProcessLibData();
	// ���ý��̰�����
	VOID SetProcessLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount);
};

