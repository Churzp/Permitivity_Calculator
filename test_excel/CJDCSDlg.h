
// test_excelDlg.h: 头文件
//

#include"Excel.h"
#include"CCalDlg.h"
#include"CResDlg.h"
#include"CChtDlg.h"
#include"CSetADlg.h"
#include"my_gsl.h"

#pragma once

const double GHz = 1e9;

// CtestexcelDlg 对话框
class CJDCSDlg : public CDialogEx
{
// 构造
public:
	CJDCSDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_JDCS };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	Excel excel;
	CString szGetName; //数据文件路径
public:
	CChtDlg cht_dlg;//绘图窗口
	CCalDlg cal_dlg;//计算窗口
	CResDlg res_dlg;//结果显示窗口
	INT title_row;
	CMenu m_Menu;
	CTabCtrl m_Tab;
	CStatusBar m_bar;
	afx_msg void OnMeneOpen();
	afx_msg void OnMenuSave();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuSetA();
public:
	int nRow;
	int nCol;
	void ReSize(void);
	POINT old;
	CRect tabRect;  //标签控件客户区大小、位置
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
