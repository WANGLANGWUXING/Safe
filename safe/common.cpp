#include "stdafx.h"
#include "common.h"
//获取当前进程已加载模块的文件的完整路径
CString GetProgramDir()
{
	TCHAR exeFullPath[MAX_PATH]; // Full path
	TCHAR *nWeak;

	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	nWeak = _tcsrchr(exeFullPath, '\\');

	nWeak[1] = 0;
	return CString(exeFullPath);
	system("pause");

}


// 删除文件到回收站中
//pszPath  : 待删除的全路径文件名
//bDelete  : TRUE 删除，不移到回收站，FALSE:移到回收站
//返回    : 0 成功    
BOOL Recycle(LPCTSTR pszPath, BOOL bDelete/*=FALSE*/)
{
	SHFILEOPSTRUCT  shDelFile;
	memset(&shDelFile, 0, sizeof(SHFILEOPSTRUCT));
	shDelFile.fFlags |= FOF_SILENT;      // don't report progress
	shDelFile.fFlags |= FOF_NOERRORUI;     // don't report errors
	shDelFile.fFlags |= FOF_NOCONFIRMATION;    // don't confirm delete


											   // Copy pathname to double-NULL-terminated string.
											   //
	TCHAR buf[_MAX_PATH + 1]; // allow one more character
	_tcscpy_s(buf, pszPath);   // copy caller's pathname
	buf[_tcslen(buf) + 1] = 0;   // need two NULLs at end

								 // Set SHFILEOPSTRUCT params for delete operation
	shDelFile.wFunc = FO_DELETE;       // REQUIRED: delete operation
	shDelFile.pFrom = buf;         // REQUIRED: which file(s)
	shDelFile.pTo = NULL;          // MUST be NULL
	if (bDelete)
	{         // if delete requested..
		shDelFile.fFlags &= ~FOF_ALLOWUNDO;    // ..don't use Recycle Bin
	}
	else
	{           // otherwise..
		shDelFile.fFlags |= FOF_ALLOWUNDO;    // ..send to Recycle Bin
	}


	return SHFileOperation(&shDelFile);    // do it!
}

void GetProcessPriority(CString &nState, HANDLE nHandle)
{
	if ((DWORD)nHandle == -1)
	{
		nState = "无权限";
		return;
	}

	DWORD nClass;
	//获取特定进程的优先级别
	nClass = GetPriorityClass(nHandle);

	switch (nClass)
	{
	case 256:
		nState = "实时";
		return;
	case 128:
		nState = "最高";
		return;
	case 32768:
		nState = "高于标准";
		return;
	case 32:
		nState = "标准";
		return;
	case 16384:
		nState = "低于标准";
		return;
	case 64:
		nState = "低";
		return;
	case 0:
		nState = "最高";
		return;
	}
	nState = "无权限";
}


void GetProcessFilePath(HANDLE hProcess, CString& sFilePath)
{
	sFilePath = _T("");
	TCHAR tsFileDosPath[MAX_PATH + 1];
	ZeroMemory(tsFileDosPath, sizeof(TCHAR)*(MAX_PATH + 1));
	//获取进程路径
	if (0 == GetProcessImageFileName(hProcess, tsFileDosPath, MAX_PATH + 1))
	{
		return;
	}

	// 获取Logic Drive String长度
	// 获取主机中所有的逻辑驱动器,以驱动器根路径字符串返回
	SIZE_T uiLen = GetLogicalDriveStrings(0, NULL);
	if (0 == uiLen)
	{
		return;
	}

	PTSTR pLogicDriveString = new TCHAR[uiLen + 1];
	ZeroMemory(pLogicDriveString, uiLen + 1);
	uiLen = GetLogicalDriveStrings((DWORD)uiLen, pLogicDriveString);
	if (0 == uiLen)
	{
		delete[]pLogicDriveString;
		return;
	}

	TCHAR szDrive[3] = TEXT(" :");
	PTSTR pDosDriveName = new TCHAR[MAX_PATH];
	PTSTR pLogicIndex = pLogicDriveString;

	do
	{
		szDrive[0] = *pLogicIndex;
		//判断当前的设备映射情况
		uiLen = QueryDosDevice(szDrive, pDosDriveName, MAX_PATH);
		if (0 == uiLen)
		{
			if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
			{
				break;
			}

			delete[]pDosDriveName;
			pDosDriveName = new TCHAR[uiLen + 1];
			uiLen = QueryDosDevice(szDrive, pDosDriveName, (DWORD)uiLen + 1);
			if (0 == uiLen)
			{
				break;
			}
		}

		uiLen = _tcslen(pDosDriveName);
		if (0 == _tcsnicmp(tsFileDosPath, pDosDriveName, uiLen))
		{
			sFilePath.Format(_T("%s%s"), szDrive, tsFileDosPath + uiLen);
			break;
		}

		while (*pLogicIndex++);
	} while (*pLogicIndex);

	delete[]pLogicDriveString;
	delete[]pDosDriveName;
}

DWORD EnumTaskList(TASKLIST &nTaskList)
{
	BOOL nRet;
	DWORD nIndex = 0;
	PROCESSENTRY32 nPT;
	nPT.dwSize = sizeof(PROCESSENTRY32);
	HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	nRet = Process32First(nSnapShot, &nPT);
	while (nRet)
	{
		nRet = Process32Next(nSnapShot, &nPT);
		nIndex++;
	}

	nTaskList.szExeFile = new CString[nIndex];
	nTaskList.th32ProcessID = new DWORD[nIndex];
	nTaskList.cntThreads = new DWORD[nIndex];
	nTaskList.th32ParentProcessID = new DWORD[nIndex];
	nTaskList.szExeFilePath = new CString[nIndex];
	nTaskList.WorkingSetSize = new CString[nIndex];
	nTaskList.szPriority = new CString[nIndex];

	nIndex = 0;
	nRet = Process32First(nSnapShot, &nPT);
	while (nRet)
	{
		HANDLE nHandle;
		CString nFilePath;
		CString nMenory;
		CString nPriority;
		PROCESS_MEMORY_COUNTERS nMen;
		nHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, nPT.th32ProcessID);
		GetProcessFilePath(nHandle, nFilePath);
		GetProcessMemoryInfo(nHandle, &nMen, sizeof(PROCESS_MEMORY_COUNTERS));
		nMenory.Format(TEXT("%d K"), nMen.WorkingSetSize / 1024);
		GetProcessPriority(nPriority, nHandle);
		CloseHandle(nHandle);

		nTaskList.szExeFile[nIndex] = nPT.szExeFile;
		nTaskList.th32ProcessID[nIndex] = nPT.th32ProcessID;
		nTaskList.cntThreads[nIndex] = nPT.cntThreads;
		nTaskList.th32ParentProcessID[nIndex] = nPT.th32ParentProcessID;
		nTaskList.szExeFilePath[nIndex] = nFilePath;
		nTaskList.WorkingSetSize[nIndex] = nMenory;
		nTaskList.szPriority[nIndex] = nPriority;

		nRet = Process32Next(nSnapShot, &nPT);
		nIndex++;
	}

	CloseHandle(nSnapShot);

	return nIndex;
}





void ClearTaskList(TASKLIST &nTaskList)
{
	delete[]nTaskList.szExeFile;
	delete[]nTaskList.th32ProcessID;
	delete[]nTaskList.cntThreads;
	delete[]nTaskList.th32ParentProcessID;
	delete[]nTaskList.szExeFilePath;
	delete[]nTaskList.WorkingSetSize;
	delete[]nTaskList.szPriority;

}

void EndProcess(DWORD nPid)
{
	HANDLE nHandle = OpenProcess(PROCESS_TERMINATE, FALSE, nPid);

	TerminateProcess(nHandle, 0);

	CloseHandle(nHandle);
}

bool GetKey(int vk_code)
{
	return (GetAsyncKeyState(vk_code) & 0x8000) ? true : false;
}



int getIndexOfSigns(char ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return ch - '0';
	}
	if (ch >= 'A' && ch <= 'F')
	{
		return ch - 'A' + 10;
	}
	if (ch >= 'a' && ch <= 'f')
	{
		return ch - 'a' + 10;
	}
	return -1;
}
long hexToDec(char *source)
{
	long sum = 0;
	long t = 1;
	int i, len;

	len = strlen(source);
	for (i = len - 1; i >= 0; i--)
	{
		sum += t * getIndexOfSigns(*(source + i));
		t *= 16;
	}

	return sum;
}


void MyRtlAdjustPrivilege()
{
	//获取关机重启权限;
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();//伪句柄
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//调用函数提升权限
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}


CString timeTotime(FILETIME time)
{
	// 将文件时间转换成本地的文件时间
	FileTimeToLocalFileTime(&time, &time);
	SYSTEMTIME st = { 0 };
	// 将文件时间转换系统时间
	CString str;
	FileTimeToSystemTime(&time, &st);
	str.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return str;
}

