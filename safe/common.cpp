#include "stdafx.h"
#include "common.h"
//��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
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


// ɾ���ļ�������վ��
//pszPath  : ��ɾ����ȫ·���ļ���
//bDelete  : TRUE ɾ�������Ƶ�����վ��FALSE:�Ƶ�����վ
//����    : 0 �ɹ�    
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
		nState = "��Ȩ��";
		return;
	}

	DWORD nClass;
	//��ȡ�ض����̵����ȼ���
	nClass = GetPriorityClass(nHandle);

	switch (nClass)
	{
	case 256:
		nState = "ʵʱ";
		return;
	case 128:
		nState = "���";
		return;
	case 32768:
		nState = "���ڱ�׼";
		return;
	case 32:
		nState = "��׼";
		return;
	case 16384:
		nState = "���ڱ�׼";
		return;
	case 64:
		nState = "��";
		return;
	case 0:
		nState = "���";
		return;
	}
	nState = "��Ȩ��";
}


void GetProcessFilePath(HANDLE hProcess, CString& sFilePath)
{
	sFilePath = _T("");
	TCHAR tsFileDosPath[MAX_PATH + 1];
	ZeroMemory(tsFileDosPath, sizeof(TCHAR)*(MAX_PATH + 1));
	//��ȡ����·��
	if (0 == GetProcessImageFileName(hProcess, tsFileDosPath, MAX_PATH + 1))
	{
		return;
	}

	// ��ȡLogic Drive String����
	// ��ȡ���������е��߼�������,����������·���ַ�������
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
		//�жϵ�ǰ���豸ӳ�����
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
	//��ȡ�ػ�����Ȩ��;
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();//α���
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//���ú�������Ȩ��
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}


CString timeTotime(FILETIME time)
{
	// ���ļ�ʱ��ת���ɱ��ص��ļ�ʱ��
	FileTimeToLocalFileTime(&time, &time);
	SYSTEMTIME st = { 0 };
	// ���ļ�ʱ��ת��ϵͳʱ��
	CString str;
	FileTimeToSystemTime(&time, &st);
	str.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return str;
}

