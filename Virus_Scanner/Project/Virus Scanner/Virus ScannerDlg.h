
// Virus ScannerDlg.h : header file
//

#pragma once  
#include "afxcmn.h"
#include "afxwin.h"

// finding virus
struct virus{
	CString _name;								// name of virus
	CString	_path;								// path of virus
};

// CVirusScannerDlg dialog
class CVirusScannerDlg : public CDialogEx
{
// Construction
public:
	CVirusScannerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VIRUSSCANNER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
													
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();							  // click on Open button
	afx_msg void OnBnClickedStart();						  // click on Start button
	afx_msg void OnBnClickedStop();							  // click on Stop button
	afx_msg void OnItemClick(NMHDR *pNMHDR, LRESULT *pResult); // ckick on title in list_control (for sorting)
	afx_msg void OnEnUpdateEdit1();							  // event handler:  write in Edit Field


	void scan();
	static UINT CALLBACK scanInThread(LPVOID pParam);		  // run scan() in thread
	BOOL _flag;												  // stop thread  (TRUE - work)

	int _sortColumn;										  // number of column for sort
	BOOL _sortParam;										  // diraction of sort
	
	
	CString _path;											  // value of edit field
	CString _last_scnd;										  // name of last scaned file
	CListCtrl _list_ctrl;									  // list of path for scan

	CList<CString> _list_paths;                               // list of paths for scan
	CList<virus> _list_viruss;								  // list of finding virus

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);	 // run sort() in thread	
};
