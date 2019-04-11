#pragma once
#include "MyListCtrl.h"
//����Ļ�������
enum DEL_CACHE_TYPE
{
	FILES = 0, COOKIES
};
#define SWEEP_BUFFER_SIZE			10000
#define WM_MYUPDATEDATA				WM_USER+100 

//������IDö��
enum DEL_RUBBISH_TYPE
{
	CLEAN_SYSTEM_TEMP = 0, CLEAN_RUBBISH_STATION, CLEAN_RUN_HISTORY,
	CLEAN_DOCUMENT_HISTORY, CLEAN_PREVUSER_HISTORY, CLEAN_FILEFIND_HISTORY,
	CLEAN_INTERNET_TEMP, CLEAN_INTERNET_COOKIE, CLEAN_ADDRBAR_HISTORY,
	CLEAN_PASSWORD_HISTORY, CLEAN_BROWSEADDR_HISTORY
};
// CCleanDlg �Ի���

class CCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCleanDlg)

public:
	CCleanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCleanDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEAN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	// ȫѡ
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck14();

	//ϵͳ����
	VOID CleanSystemTemp();				//����ϵͳ��ʱ�ļ���		0  CLEAN_SYSTEM_TEMP
	VOID CleanRubbishStation();			//������������վ			1  CLEAN_RUBBISH_STATION
	VOID CleanRunHistory();				//�������м�¼				2  CLEAN_RUN_HISTORY
	VOID CleanDocumentHistory();		//��������ĵ���¼			3  CLEAN_DOCUMENT_HISTORY
	VOID CleanPrevUserHistory();		//�����ϴ��û���¼��¼		4  CLEAN_PREVUSER_HISTORY
	VOID CleanFileFindHistory();		//�����ļ����Ҽ�¼			5  CLEAN_FILEFIND_HISTORY

										//���������
	VOID CleanInternetTemp();			//����Interner��ʱ�ļ�		6  CLEAN_INTERNET_TEMP
	VOID CleanInternetCookie();			//����Internet Cookie	7  CLEAN_INTERNET_COOKIE
	VOID CleanAddrBarHistory();			//�����ַ����ʷ��¼		8  CLEAN_ADDRBAR_HISTORY
	VOID CleanPasswordHistory();		//���������¼				9  CLEAN_PASSWORD_HISTORY
	VOID CleanBrowseAddrHistory();		//���������ַ��¼			10 CLEAN_BROWSEADDR_HISTORY

	//����
	//���Url����
	BOOL DeleteUrlCache(DEL_CACHE_TYPE type);
	//���һ��Ŀ¼
	BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE,
		BOOL bWipeIndexDat = FALSE);
	//�����ļ�����
	BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile);

	// �����ļ��б�
	MyListCtrl m_listClean;
	// �ļ���С
	CString m_szTemp;
	//�ж��Ƿ���ɨ���ļ�
	BOOL m_isScanFile;
	//�����ļ��Ĵ�С
	DOUBLE m_dAllFileSize;
	//��õ�ǰ�ļ�ת����Ҫ��ʾ�Ĵ�С
	CString GetShowSize(DOUBLE dwSize);
	//��ʾ��Ϣ
	CString m_strStatus;
	//��ȡ�������ѡ��״̬
	void GetSelect();
	//�����������Ƿ�ѡ�е�����
	BOOL m_bIsSelect[11];
	// ɨ�谴ť
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	VOID CleanRubbish(int cleanType);	//������ú���
	BOOL StartCleanThread();			//���������߳�
protected:
	afx_msg LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);
public:

	
	afx_msg void OnBnClickedCheck12();
};
