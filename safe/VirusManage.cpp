#include "stdafx.h"
#include "VirusManage.h"



CVirusManage::CVirusManage()
{
}


CVirusManage::~CVirusManage()
{
	ClearVirusLibData();
	ClearProcessLibData();
}

//��ȡ���ز�����
DWORD CVirusManage::LoadVirusLibData()
{

	HANDLE  pFile = nullptr;
	pFile = CreateFile(GetProgramDir() + TEXT("VirusLib.dat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (pFile == (HANDLE)-1)return 0;

	DWORD nSize = GetFileSize((PHANDLE)pFile, 0);
	DWORD nReadSize = 0;

	m_dwVirusCount = nSize / sizeof(VIRUSINFO);
	m_pVirusLib = new VIRUSINFO[m_dwVirusCount];
	ReadFile(pFile, (LPVOID)m_pVirusLib, nSize, &nReadSize, NULL);
	CloseHandle(pFile);
	return m_dwVirusCount;
}
//ȡ����������Ϣ
VOID CVirusManage::GetVirusLib(PVIRUSINFO &nVIRUSINFO)
{
	memcpy_s(nVIRUSINFO, sizeof(VIRUSINFO)*m_dwVirusCount, m_pVirusLib, sizeof(VIRUSINFO)*m_dwVirusCount);
}
//д��������
VOID CVirusManage::SetVirusLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount)
{
	FILE *pFile = nullptr;
	fopen_s(&pFile, GetProgramDir() + "VirusLib.dat", "wb");
	m_dwVirusCount = fwrite(nVIRUSINFO, sizeof(VIRUSINFO), nCount, pFile);
	fclose(pFile);
}
//�رղ�����
VOID CVirusManage::ClearVirusLibData()
{
	if (m_pVirusLib != nullptr)
	{
		delete[] m_pVirusLib;
	}
	m_pVirusLib = nullptr;
	m_dwVirusCount = 0;
}


//��ȡ������
DWORD CVirusManage::LoadProcessLibData()
{

	HANDLE  pFile = nullptr;
	pFile = CreateFile(GetProgramDir() + TEXT("ProcessLib.dat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (pFile == (HANDLE)-1)return 0;

	DWORD nSize = GetFileSize((PHANDLE)pFile, 0);
	DWORD nReadSize = 0;

	m_dwProcessCount = nSize / sizeof(VIRUSINFO);
	m_pProcessLib = new VIRUSINFO[m_dwProcessCount];
	ReadFile(pFile, (LPVOID)m_pProcessLib, nSize, &nReadSize, NULL);
	CloseHandle(pFile);
	return m_dwProcessCount;
}
//ȡ����������Ϣ
VOID CVirusManage::GetProcessLib(PVIRUSINFO &nVIRUSINFO)
{
	memcpy_s(nVIRUSINFO, sizeof(VIRUSINFO)*m_dwProcessCount, m_pProcessLib, sizeof(VIRUSINFO)*m_dwProcessCount);
}
//д��������
VOID CVirusManage::SetProcessLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount)
{
	FILE *pFile = nullptr;
	fopen_s(&pFile, GetProgramDir() + "ProcessLib.dat", "wb");
	m_dwProcessCount = fwrite(nVIRUSINFO, sizeof(VIRUSINFO), nCount, pFile);
	fclose(pFile);
}
//�رհ�����
VOID CVirusManage::ClearProcessLibData()
{
	if (m_pProcessLib != nullptr)
	{
		delete[] m_pProcessLib;
	}
	m_pProcessLib = nullptr;
	m_dwProcessCount = 0;
}
