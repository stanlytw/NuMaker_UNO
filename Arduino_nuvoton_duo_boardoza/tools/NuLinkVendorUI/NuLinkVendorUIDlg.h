// NuLinkVendorUIDlg.h : header file
//

#pragma once


// CNuLinkVendorUIDlg dialog
class CNuLinkVendorUIDlg : public CDialog
{
// Construction
public:
	CNuLinkVendorUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NULINK_VENDOR_UI };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL ProcessCmd(LPTSTR cmd);
	afx_msg void OnBnClickedLdrom();
	afx_msg void OnBnClickedAprom();
	afx_msg void OnBnClickedData();

	CButton m_ButtonOutputResult;
	BOOL m_bOutputResult;
};
