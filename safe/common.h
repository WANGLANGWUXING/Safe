#pragma once
#include <TlHelp32.h>
#include <Psapi.h>

// 进程结构体
typedef struct _TASKLIST
{
	CString *szExeFile;
	DWORD *th32ProcessID;
	DWORD *cntThreads;
	DWORD *th32ParentProcessID;
	CString *szExeFilePath;
	CString *WorkingSetSize;
	CString *szPriority;

}TASKLIST, *PTASKLIST;
// 获取当前进程已加载模块的文件的完整路径
CString GetProgramDir();
// 文件删除到回收站
BOOL Recycle(LPCTSTR pszPath, BOOL bDelete/*=FALSE*/);
// 枚举进程
DWORD EnumTaskList(TASKLIST &nTaskList);
// 获取进程权限
void GetProcessPriority(CString &nState, HANDLE nHandle);
// 获取进程目录
void GetProcessFilePath(HANDLE hProcess, CString& sFilePath);
// 进程结构体内存清理
void ClearTaskList(TASKLIST &nTaskList);
// 结束进程
void EndProcess(DWORD nPid);
// 获取键盘按键
bool GetKey(int vk_code);
// 十六进程字符串转数字
int getIndexOfSigns(char ch);
// 十六进制转十进制
long hexToDec(char *source);
// 提权
void MyRtlAdjustPrivilege();
// 将文件时间转换成本地的文件时间
CString timeTotime(FILETIME time);


