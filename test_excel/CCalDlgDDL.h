#pragma once

#include"CMyListCtrl.h"

// CCalDlgDDL 对话框

class CCalDlgDDL : public CDialogEx
{
	DECLARE_DYNAMIC(CCalDlgDDL)

public:
	CCalDlgDDL(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCalDlgDDL();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CAL_DDL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ReSize(void);
	POINT old;
	CEdit m_filepath;
	CRect list_rect;
	CMyListCtrl m_list;

public:
	afx_msg void OnBnClickedButtonCal();
	afx_msg void OnCbnSelchangeCombo1();
//	CMyComboBox m_AlgChoosing;
	virtual BOOL OnInitDialog();
};
