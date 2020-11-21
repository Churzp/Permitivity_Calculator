#pragma once


// CFuncSel 对话框

class CFuncSelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFuncSelDlg)

public:
	CFuncSelDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFuncSelDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FUNCSEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonJdcs();
	afx_msg void OnBnClickedButtonDdl();
	virtual BOOL OnInitDialog();

public:
	POINT old;

public:
	void ReSize(void);
};
