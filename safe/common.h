#pragma once
#include <TlHelp32.h>
#include <Psapi.h>

// ���̽ṹ��
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
// ��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
CString GetProgramDir();
// �ļ�ɾ��������վ
BOOL Recycle(LPCTSTR pszPath, BOOL bDelete/*=FALSE*/);
// ö�ٽ���
DWORD EnumTaskList(TASKLIST &nTaskList);
// ��ȡ����Ȩ��
void GetProcessPriority(CString &nState, HANDLE nHandle);
// ��ȡ����Ŀ¼
void GetProcessFilePath(HANDLE hProcess, CString& sFilePath);
// ���̽ṹ���ڴ�����
void ClearTaskList(TASKLIST &nTaskList);
// ��������
void EndProcess(DWORD nPid);
// ��ȡ���̰���
bool GetKey(int vk_code);
// ʮ�������ַ���ת����
int getIndexOfSigns(char ch);
// ʮ������תʮ����
long hexToDec(char *source);
// ��Ȩ
void MyRtlAdjustPrivilege();
// ���ļ�ʱ��ת���ɱ��ص��ļ�ʱ��
CString timeTotime(FILETIME time);


