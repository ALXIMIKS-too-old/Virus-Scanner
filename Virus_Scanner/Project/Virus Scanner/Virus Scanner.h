
// Virus Scanner.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CVirusScannerApp:
// See Virus Scanner.cpp for the implementation of this class
//

class CVirusScannerApp : public CWinApp
{
public:
	CVirusScannerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVirusScannerApp theApp;

//run AVEngineService.exe  task
class init_scaner
{
	// run 	AVEngineService.exe  task
	void runInit(){
		// current  path
		CString  folder;
		GetCurrentDirectory(_MAX_PATH, folder.GetBuffer(_MAX_PATH));
		folder.ReleaseBuffer();
		folder = folder + L"\\" + L"AVEngineService.exe";

		//ShellExecute(0, L"open", folder.GetBuffer(), 0, 0, 0);

		STARTUPINFO StartupInfo;
		PROCESS_INFORMATION ProcessInfo;

		memset(&StartupInfo, 0, sizeof(StartupInfo));
		StartupInfo.cb = sizeof(STARTUPINFO);
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_HIDE;

		if (!CreateProcess(NULL, folder.GetBuffer(), NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&StartupInfo,
			&ProcessInfo))
		{
			MessageBox(0, L"Could not open " + folder, L"Error", MB_ICONERROR | MB_OK);
		}
	}	

	// run AVEngineService.exe  task in thread
	static UINT myThread(LPVOID pParam)	{
		init_scaner *pDlg = reinterpret_cast<init_scaner*>(pParam);
		pDlg->runInit();
		return 0;
	}

public:				 	
	init_scaner(){};

	void Run1(){
		myThread(this);
	}		

	// 	 kill AVEngineService.exe  task
	~init_scaner()
	{
		//WinExec("taskkill /f /im AVEngineService.exe", SW_HIDE);	 don't work
		_tsystem(_T("taskkill /F /T /IM AVEngineService.exe"));	  // kill all task
	}
};

