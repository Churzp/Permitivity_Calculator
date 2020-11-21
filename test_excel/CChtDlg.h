#pragma once
#include"ChartCtrl/ChartCtrl.h"
#include"ChartCtrl/ChartTitle.h"
#include"ChartCtrl/ChartAxisLabel.h"
#include"ChartCtrl/ChartLineSerie.h"

// CChtDlg 对话框

class CChtDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChtDlg)

public:
	CChtDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChtDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CHT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	static CChtDlg* pChtDlg;
	void ReSize(void);
	void Draw(DOUBLE a[],DOUBLE b[], CChartCtrl* place);//绘图函数
	POINT old;
	int nRow;
public:
	CChartCtrl m_ChartCtrl_r;//实部绘图框
	CChartCtrl m_ChartCtrl_i;//虚部绘图框
	virtual BOOL OnInitDialog();
};
