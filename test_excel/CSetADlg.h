#pragma once

// CSetADlg 对话框

class CSetADlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetADlg)

public:
	CSetADlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetADlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SET_A };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit_seta;
	CEdit m_edit_set_d;
	CEdit m_edit_set_l1;
	CEdit m_edit_set_l2;
	CStatic m_draw_model;//绘图控件
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	//用于适应分辨率
	void ReSize(void);
	POINT old;
	CRect tabRect;  //标签控件客户区大小、位置
public:
	//用于绘图
	CRect drawRect;		//绘图区矩形
	INT h;		//绘图区高度
	INT w;		//绘图区长度
	CDC* pDC_model;		//设备上下文
	BOOL DrawModel(CDC * pDC, CRect & rect);
	BOOL ArrowLine(CDC * pDC, INT start, INT end, INT yPos, CSize size);
	BOOL PaintCheck();
	BOOL value_set;//判断是否正确设定参数
public:
	//设定值的获取与格式转换
	CString a_str;
	DOUBLE a_temp;
	CString d_str;
	DOUBLE d_temp;
	CString l1_str;
	DOUBLE l1_temp;
	CString l2_str;
	DOUBLE l2_temp;
	afx_msg void OnBnClickedButtonPreview();
	CStatic m_label_l1;
	CStatic m_label_d;
	CStatic m_label_l2;
};
