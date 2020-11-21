#pragma once
#include"CMyListCtrl.h"

// CResDlg 对话框

class CResDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResDlg)

public:
	CResDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CResDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_list_res;
	CRect list_rect;
	static CResDlg* pResDlg;
public:
	//BOOL OnInitDialogRes();
	afx_msg void OnLvnItemchangedListRes(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void ReSize(void);
	POINT old;
	virtual BOOL OnInitDialog();
	CEdit m_edit_f;
	CEdit m_edit_epsr;
	CEdit m_edit_epsi;
};
