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
	// 病毒库指针
	PVIRUSINFO m_pVirusLib = nullptr;
	// 病毒库数量
	DWORD m_dwVirusCount = 0;
	// 进程白名单指针
	PVIRUSINFO m_pProcessLib = nullptr;
	// 进程白名单数量
	DWORD m_dwProcessCount = 0;
	// 加载病毒库
	DWORD LoadVirusLibData();
	// 获取病毒库
	VOID GetVirusLib(PVIRUSINFO &nVIRUSINFO);
	// 清除病毒库
	VOID ClearVirusLibData();
	// 设置病毒库
	VOID SetVirusLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount);
	// 加载进程白名单
	DWORD LoadProcessLibData();
	// 获取进程白名单
	VOID GetProcessLib(PVIRUSINFO &nVIRUSINFO);
	// 清除进程白名单
	VOID ClearProcessLibData();
	// 设置进程白名单
	VOID SetProcessLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount);
};

