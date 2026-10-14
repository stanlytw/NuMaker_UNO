// NuLinkVendorUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NuLinkVendorUI.h"
#include "NuLinkVendorUIDlg.h"
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#define MAX_PATH          260

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNuLinkVendorUIDlg dialog

CNuLinkVendorUIDlg::CNuLinkVendorUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNuLinkVendorUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNuLinkVendorUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_OUTPUT_RESULT, m_ButtonOutputResult);
	DDX_Check(pDX, IDC_CHECK_OUTPUT_RESULT,	m_bOutputResult);
}

BEGIN_MESSAGE_MAP(CNuLinkVendorUIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CNuLinkVendorUIDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_LDROM, &CNuLinkVendorUIDlg::OnBnClickedLdrom)
	ON_BN_CLICKED(ID_APROM, &CNuLinkVendorUIDlg::OnBnClickedAprom)
	ON_BN_CLICKED(ID_DATA, &CNuLinkVendorUIDlg::OnBnClickedData)
END_MESSAGE_MAP()


// CNuLinkVendorUIDlg message handlers

BOOL CNuLinkVendorUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNuLinkVendorUIDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNuLinkVendorUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNuLinkVendorUIDlg::OnBnClickedOk()
{
	TCHAR szCurDir[MAX_PATH];
	TCHAR str_command[MAX_PATH];

	//########################### Get All NuLink ID of Connected NuLink ICE Dongle ########################
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
	//clear the content of listbox
	pListBox->ResetContent();

	//use NuLink commad to list all connected NuLink ICE dongles and save all NuLink ID into GetNuLinkID.txt
	if(GetCurrentDirectory(sizeof(szCurDir)/sizeof(szCurDir[0]), szCurDir) == 0)
	{
		strcpy_s(szCurDir, "C:\\Program Files\\Nuvoton Tools\\NuLink Command Tool");
	}

	strcpy_s(str_command, "cd ");
	strcat_s(str_command, szCurDir);
	strcat_s(str_command, " & nulink -l | clip");
	system(str_command);

	char * buffer = NULL;
	//open the clipboard
	CString fromClipboard("");
	if ( OpenClipboard() ) 
	{
		HANDLE hData = GetClipboardData( CF_TEXT );
		char * buffer = (char*)GlobalLock( hData );
		fromClipboard = buffer;
		GlobalUnlock( hData );
		CloseClipboard();
	}

	//get all NuLink ID from Clipboard
	string str_content;
	str_content = fromClipboard.GetBuffer(0);
	istringstream f(str_content);
	CStringArray cstr_array_id; //keep all NuLink ID
	while(getline(f, str_content, '\n'))
	{
		CString cstr_nulink_id = str_content.c_str();
		CString cstr_pattern = "0x";
		int goalIndex = cstr_nulink_id.Find(cstr_pattern);
		if ((goalIndex > 0) && (str_content != ""))
		{
			int pattern_length = cstr_pattern.GetLength();
			int nulink_length = cstr_nulink_id.GetLength();
			//parse string to get the NuLink ID
			CString cstr_result  = cstr_nulink_id.Right(nulink_length -  goalIndex + pattern_length -1);
			cstr_array_id.Add(cstr_result);
			//display NuLink ID to listbox
			pListBox->AddString(cstr_result);
		}
	}

	//########################### Program Each Connected NuLink Device ########################
	CString cstr_ldrom_path = ""; //keep LDROM path
	//get LDROM path from editbox
	CEdit* pEditBox_Ldrom = (CEdit*)GetDlgItem(IDC_EDIT_LDROM);
    pEditBox_Ldrom->GetWindowTextA(cstr_ldrom_path);

	CString cstr_aprom_path = ""; //keep APROM path
	//get APROM path from editbox
	CEdit* pEditBox_Aprom = (CEdit*)GetDlgItem(IDC_EDIT_APROM);
    pEditBox_Aprom->GetWindowTextA(cstr_aprom_path);

	CString cstr_datarom_path = ""; //keep DATAROM path
	//get DATAROM path form editbox
	CEdit* pEditBox_Datarom = (CEdit*)GetDlgItem(IDC_EDIT_DATAROM);
    pEditBox_Datarom->GetWindowTextA(cstr_datarom_path);

//	int size = cstr_array_id.GetSize();
//	for(int i = 0; i < size ; i++)
//	{
//		CString cstr_id = cstr_array_id.GetAt(i);
		CString cstr_ldrom_command = ""; //keep NuLink commands for LDROM
		CString cstr_aprom_command = ""; //keep NuLink commands for APROM
		CString cstr_datarom_command = ""; //keep NuLink commands for DATAROM

		CString cstr_output_command; //output NuLink command result message to file
		UpdateData(TRUE);
		if(m_bOutputResult)
			cstr_output_command = " >> NuLinkVendorUI.log";
		else
			cstr_output_command = "";

		//switch working directory to NuLink Command Tool first
		CString cstr_init_command;
		cstr_init_command.Format(_T("%s"), szCurDir);
		cstr_init_command = "C:\\WINDOWS\\System32\\cmd.exe /c cd \"" + cstr_init_command + "\"";

		if (cstr_ldrom_path != "")
		{ //combine NuLink commands of erase, write, verify LDROM
			cstr_ldrom_command = " & nulink all_nulink -e ldrom" + cstr_output_command + " & nulink all_nulink -w ldrom \"" + cstr_ldrom_path + "\"" + cstr_output_command + " & nulink all_nulink -v ldrom \"" + cstr_ldrom_path + "\"" + cstr_output_command;
		}
		if (cstr_aprom_path != "")
		{ //combine NuLink commands of erase, write, verify APROM
			cstr_aprom_command = " & nulink all_nulink -e aprom" + cstr_output_command + " & nulink all_nulink -w aprom \"" + cstr_aprom_path + "\"" + cstr_output_command + " & nulink all_nulink -v aprom \"" + cstr_aprom_path + "\"" + cstr_output_command;
		}
		if (cstr_datarom_path != "")
		{ //combine NuLink commands of erase, write, verify DATAROM
			cstr_datarom_command = "& nulink all_nulink -e datarom" + cstr_output_command + " & nulink all_nulink -w datarom \"" + cstr_datarom_path + "\"" + cstr_output_command + " & nulink all_nulink -v datarom \"" + cstr_datarom_path + "\"" + cstr_output_command;
		}
		CString cstr_pause_command = " & pause";

		//combine NuLink commands of LDROM, APROM and DATAROM
		CString cstr_command = cstr_init_command + cstr_ldrom_command + cstr_aprom_command + cstr_datarom_command + cstr_pause_command;

		//create a process to program the NuLink device
		LPTSTR lpCommandLine = cstr_command.GetBuffer(0);
		ProcessCmd(lpCommandLine);
//	}
}

BOOL CNuLinkVendorUIDlg::ProcessCmd(LPTSTR cmd)
{
    ASSERT(cmd!=NULL);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !::CreateProcess( NULL,   // No module name (use command line)
        cmd,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        //CREATE_NO_WINDOW,//The process is a console application that is being run without a console window
		NORMAL_PRIORITY_CLASS,
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
        ) 
    {
        ::MessageBox(NULL,TEXT("execute external cmd error!"),NULL,MB_OK);
        return FALSE;
    }
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    return TRUE;
}

void CNuLinkVendorUIDlg::OnBnClickedLdrom()
{
	CString strReturn = "";
	//create file browse dialog to select LDROM
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY);
	if(dlg.DoModal()==IDOK)
	{
		strReturn = dlg.GetPathName();
		CEdit* pEditBox = (CEdit*)GetDlgItem(IDC_EDIT_LDROM);
		//clear the content of LDROM editbox
		pEditBox->Clear();
		//display LDROM path to editbox
		pEditBox->SetWindowTextA(strReturn);
	}
}

void CNuLinkVendorUIDlg::OnBnClickedAprom()
{
	CString strReturn = "";
	//create file browse dialog to select APROM
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY);
	if(dlg.DoModal()==IDOK)
	{
		strReturn = dlg.GetPathName();
		CEdit* pEditBox = (CEdit*)GetDlgItem(IDC_EDIT_APROM);
		//clear the content of APROM editbox
		pEditBox->Clear();
		//display APROM path to editbox
		pEditBox->SetWindowTextA(strReturn);
	}
}

void CNuLinkVendorUIDlg::OnBnClickedData()
{
	CString strReturn = "";
	//create file browse dialog to select DATAROM
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY);
	if(dlg.DoModal()==IDOK)
	{
		strReturn = dlg.GetPathName();
		CEdit* pEditBox = (CEdit*)GetDlgItem(IDC_EDIT_DATAROM);
		//clear the content of DATAROM editbox
		pEditBox->Clear();
		//display DATAROM path to editbox
		pEditBox->SetWindowTextA(strReturn);
	}
}
