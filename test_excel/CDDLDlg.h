#pragma once
#include"Excel.h"
#include"CCalDlgDDL.h"
//#include"CResDlg.h"
//#include"CChtDlg.h"
//#include"CSetADlg.h"
#include"my_gsl.h"

// CDDLDlg 对话框

class CDDLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDLDlg)

public:
	CDDLDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDDLDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DDL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	Excel excel;
	CString szGetName; //数据文件路径
public:
	CCalDlgDDL cal_dlg_ddl;
	CMenu m_Menu;
	CTabCtrl m_Tab;

public:
	int nRow;
	int nCol;
	void ReSize(void);
	POINT old;
	CRect tabRect;  //标签控件客户区大小、位置
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMeneOpen();
};
