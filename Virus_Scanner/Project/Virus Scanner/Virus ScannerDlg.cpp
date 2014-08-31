
// Virus ScannerDlg.cpp : implementation file

#include "stdafx.h"
#include "Virus Scanner.h"
#include "Virus ScannerDlg.h"
#include "afxdialogex.h"

#define _ZSDK_INCLUDE_STATIC
#include "zsdk_def.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About	 
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVirusScannerDlg dialog



CVirusScannerDlg::CVirusScannerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVirusScannerDlg::IDD, pParent)
	, _path(_T(""))
	, _last_scnd(_T(""))
{
	_path = L"Enter path for scan";
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVirusScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _path);
	DDX_Text(pDX, IDC_LAST_SCND, _last_scnd);
	DDX_Control(pDX, IDC_LIST_VIRUS, _list_ctrl);
}

BEGIN_MESSAGE_MAP(CVirusScannerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CVirusScannerDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_START, &CVirusScannerDlg::OnBnClickedStart)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CVirusScannerDlg::OnItemClick)
	ON_EN_UPDATE(IDC_EDIT1, &CVirusScannerDlg::OnEnUpdateEdit1)
	ON_BN_CLICKED(IDC_STOP, &CVirusScannerDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CVirusScannerDlg message handlers

BOOL CVirusScannerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// ##############################################################################
// ##############################################################################
	
	// create colums of list_control
	_list_ctrl.InsertColumn(1, _T("Virus"), LVCFMT_LEFT, 200);
	_list_ctrl.InsertColumn(2, _T("Path"), LVCFMT_LEFT, 200);

	//Disable buttons
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	// ##############################################################################
	// ##############################################################################
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVirusScannerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVirusScannerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVirusScannerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}		

// ##############################################################################
// ##############################################################################

// click on Open button
void CVirusScannerDlg::OnBnClickedOpen()
{
	// select folder or file for scan
	BROWSEINFO   bi;
	ZeroMemory(&bi, sizeof(bi));
	TCHAR   szDisplayName[MAX_PATH];
	szDisplayName[0] = '\0';

	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDisplayName;
	bi.lpszTitle = _T("Please select a folder for storing received files :");
	bi.ulFlags =
		BIF_NEWDIALOGSTYLE | 						  // new style
		BIF_NONEWFOLDERBUTTON | 					  // remove button new folder
		BIF_BROWSEINCLUDEFILES;					      // include files	 
	bi.lParam = NULL;
	bi.iImage = 0;

	LPITEMIDLIST   pidl = SHBrowseForFolder(&bi);
	TCHAR   szPathName[MAX_PATH];
	if (NULL != pidl)
	{
		BOOL bRet = SHGetPathFromIDList(pidl, szPathName);
		if (FALSE == bRet){								   // selected item has not path
			_path = L"";
			UpdateData(FALSE);
			_list_paths.RemoveAll();
			OnEnUpdateEdit1();
			return;
		}
		_path = szPathName;								   // selected item has path
		UpdateData(FALSE);
		_list_paths.RemoveAll();
		_list_paths.AddTail(_path);
		OnEnUpdateEdit1();
		return;
	}
}							   

// click on Start button
void CVirusScannerDlg::OnBnClickedStart()
{	
	// Disable and Enable buttons
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);

	_flag = TRUE;									 // stop thread  (TRUE - work)
	scanInThread(this);								 // run in thread scan()
}


// CLick on Stop Button
void CVirusScannerDlg::OnBnClickedStop()
{
	_flag = FALSE;
}

// run scan() in thread
UINT CVirusScannerDlg::scanInThread(LPVOID pParam)
{
	CVirusScannerDlg *pDlg = reinterpret_cast<CVirusScannerDlg*>(pParam);
	pDlg->scan();
	return 0;
}

void CVirusScannerDlg::scan()
{
	DWORD hScan;          // will contain the handle of the current scan
	DWORD dwRes;          // the result of the scan functions
	zRPCAnswer answer;    // the result of the scan
	virus vr;			  

	
	CString st;
	POSITION head_pos = _list_paths.GetHeadPosition();
	while (head_pos && _flag){
		st = _list_paths.GetNext(head_pos);				   // get next path or scan

		hScan = StartScan(st.GetBuffer());					// start scan
		if (hScan == -1){									// problem with connect
			_last_scnd = L"Problem with AVEngineService";
			UpdateData(FALSE);
			GetDlgItem(IDC_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);
			continue;
		}

		do {																   // while there are files for scan
			if ((dwRes = GetScanData(hScan, answer)) == ZSDK_REQUEST_ERROR){   // some problem
				_last_scnd = L"REQUEST ERROR";
				UpdateData(FALSE);
				GetDlgItem(IDC_START)->EnableWindow(TRUE);
				GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);
				break;
			}
			_last_scnd = answer.szFileName;					  // show name of last scaned file
			UpdateData(FALSE);

			if (wmemcmp(answer.szVirName, L"", 2) != 0){	  // finded virus
				vr._name = answer.szVirName;
				vr._path = answer.szFileName;

				POSITION insert_pos = _list_viruss.AddTail(vr);
				int nItem = _list_ctrl.InsertItem(_list_ctrl.GetItemCount(), vr._name);
				_list_ctrl.SetItemText(nItem, 1, vr._path);
				_list_ctrl.SetItemData(nItem, DWORD_PTR(insert_pos));
			}
		} while (dwRes == ZSDK_REQUEST_MORE_DATA && _flag);	  // more scan
	}	   	

	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);	
}

// event handler:  Click on head of LIST column
void CVirusScannerDlg::OnItemClick(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);	  
	// get column and diraction
	if (_sortColumn != phdr->iItem){
		_sortColumn = phdr->iItem;
		_sortParam = FALSE;
	}
	else
		_sortParam = !_sortParam; 
	//sort
	_list_ctrl.SortItems(CompareFunc, (DWORD_PTR) this);
	*pResult = 0;
}

// compare using ItemDATA
int CVirusScannerDlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort){
	// get Position from ItemDATA
	POSITION pos1 = reinterpret_cast<POSITION>  (lParam1);
	POSITION pos2 = reinterpret_cast<POSITION>  (lParam2);

	// get virus from Clist
	CVirusScannerDlg* pThis = (CVirusScannerDlg*)(lParamSort);
	virus vr1 = pThis->_list_viruss.GetAt(pos1);
	virus vr2 = pThis->_list_viruss.GetAt(pos2);

	// compare
	int nResult = 0;
	switch (pThis->_sortColumn){
	case 0:
		nResult = vr1._name.CompareNoCase(vr2._name);
		break;
	case 1:
		nResult = vr1._path.CompareNoCase(vr2._path);
		break;
	}
	// diraction
	if (pThis->_sortParam == FALSE){
		nResult = nResult  * (-1);
	}
	return nResult;
}	 

// event handler:  write in Edit Field
void CVirusScannerDlg::OnEnUpdateEdit1()
{
	UpdateData(TRUE);	
	CFile theFile;				   
	CFileStatus status;
	if (theFile.GetStatus(_list_paths.GetHead(), status)){
		GetDlgItem(IDC_START)->EnableWindow(TRUE);		
	}
	else
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
}				  

