// CSetADlg.cpp: 实现文件
//

#include "pch.h"
#include "test_excel.h"
#include "CSetADlg.h"
#include "afxdialogex.h"
#include "CJDCSDlg.h"
#include <afxwin.h>


#define MARGIN 30		//模型在上下两边留出的页边距（像素）

// CSetADlg 对话框

IMPLEMENT_DYNAMIC(CSetADlg, CDialogEx)

CSetADlg::CSetADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SET_A, pParent)
{

}

CSetADlg::~CSetADlg()
{
}

void CSetADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SET_A, m_edit_seta);
	DDX_Control(pDX, IDC_EDIT_SET_D, m_edit_set_d);
	DDX_Control(pDX, IDC_EDIT_SET_L1, m_edit_set_l1);
	DDX_Control(pDX, IDC_EDIT_SET_L2, m_edit_set_l2);
	DDX_Control(pDX, IDC_DRAW_MODEL, m_draw_model);
	DDX_Control(pDX, IDC_LABEL_L1, m_label_l1);
	DDX_Control(pDX, IDC_LABEL_D, m_label_d);
	DDX_Control(pDX, IDC_LABEL_L2, m_label_l2);
}


BEGIN_MESSAGE_MAP(CSetADlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetADlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetADlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CSetADlg::OnBnClickedButtonPreview)
END_MESSAGE_MAP()


// CSetADlg 消息处理程序
BOOL CSetADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设定波导宽度的初始显示值
	m_edit_seta.SetWindowTextA(a_str);
	m_edit_set_d.SetWindowTextA(d_str);
	m_edit_set_l1.SetWindowTextA(l1_str);
	m_edit_set_l2.SetWindowTextA(l2_str);

	//取客户区大小，用于自适应分辨率
	CRect rect;
	GetClientRect(&rect);
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;
	ReSize();

	//获取绘制模型的上下文和绘制区域
	pDC_model = m_draw_model.GetDC();
	m_draw_model.GetClientRect(&drawRect);
	h = drawRect.Height();
	w = drawRect.Width();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSetADlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(PaintCheck())
	{
		CJDCSDlg* parent = (CJDCSDlg*)GetParent();
		CString confirm;
		confirm.Format(_T("a确定设置为	%.3lfmm？\nd确定设置为	%.3lfmm？\nl1确定设置为	%.3lfmm？\nl2确定设置为	%.3lfmm？"), a_temp, d_temp, l1_temp, l2_temp);
		if (IDOK == MessageBox(confirm, _T("参数设定结果"), MB_OKCANCEL))
		{
			parent->cal_dlg.a_value = a_temp;
			parent->cal_dlg.d_value = d_temp;
			parent->cal_dlg.l1_value = l1_temp;
			parent->cal_dlg.l2_value = l2_temp;
			parent->cal_dlg.value_set = TRUE;
			parent->cal_dlg.UpdateButton(value_set);
			parent->m_bar.SetPaneText(0, _T("参数已设定"));
			CDialogEx::OnOK();
		}
		parent = NULL;
	}	
}


void CSetADlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CSetADlg::ReSize(void)
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;
	GetClientRect(&recta);     //取客户区大小  
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角
	CPoint OldBRPoint, BRPoint; //右下角
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件  
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID

		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
}


BOOL CSetADlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		switch (GetFocus()->GetDlgCtrlID())
		{
		case IDC_EDIT_SET_A:
		case IDC_EDIT_SET_D:
		case IDC_EDIT_SET_L1:
		case IDC_EDIT_SET_L2:
		{
			PaintCheck();
		}
			break;
		default:
			break;
		}
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CSetADlg::DrawModel(CDC * pDC, CRect & rect)
{
	INT model_width = h - 2 * MARGIN;
	DOUBLE TOTAL = d_temp + l1_temp + l2_temp;
	DOUBLE unit = w / TOTAL;
	//模型构建
	CPoint material_lt(l1_temp*unit, MARGIN);
	CSize material_sz(d_temp*unit, model_width);
	CRect material_rect(material_lt, material_sz);

	CBrush brush, *oldbrush;
	//绘制背景
	brush.CreateSolidBrush(RGB(255, 255, 255));
	oldbrush = pDC->SelectObject(&brush);//选取白色画笔并返回老的原始画笔
	pDC->Rectangle(rect);//绘制背景
	pDC->SelectObject(oldbrush);
	brush.DeleteObject();
	//绘制材料样本
	brush.CreateSolidBrush(RGB(255, 178, 115));
	oldbrush = pDC->SelectObject(&brush);//选取橙色画笔并返回老的原始画笔
	pDC->Rectangle(material_rect);
	pDC->SelectObject(oldbrush);
	brush.DeleteObject();
	//绘制箭头
	ArrowLine(pDC, 0, material_rect.left, h / 2, CSize(10, 10));
	ArrowLine(pDC, material_rect.right, rect.right, h / 2, CSize(10, 10));
	//动态标注
	CRect label_d_rect, label_l1_rect, label_l2_rect;
	m_label_d.GetWindowRect(&label_d_rect);
	m_label_l1.GetWindowRect(&label_l1_rect);
	m_label_l2.GetWindowRect(&label_l2_rect);
	ClientToScreen(&material_rect);
	label_d_rect = CRect(CPoint(material_rect.left + (label_d_rect.Width() + material_rect.Width()) / 2, label_d_rect.top), label_d_rect.Size());
	label_l1_rect = CRect(CPoint(label_d_rect.left - (material_rect.Width() + l1_temp * unit) / 2, label_l1_rect.top), label_l1_rect.Size());
	label_l2_rect = CRect(CPoint(label_d_rect.left + (material_rect.Width() + l2_temp * unit) / 2, label_l2_rect.top), label_l2_rect.Size());
	ScreenToClient(&label_d_rect);
	ScreenToClient(&label_l1_rect);
	ScreenToClient(&label_l2_rect);
	m_label_d.MoveWindow(&label_d_rect);
	m_label_l1.MoveWindow(&label_l1_rect);
	m_label_l2.MoveWindow(&label_l2_rect);

	return TRUE;
}


BOOL CSetADlg::ArrowLine(CDC * pDC, INT start, INT end, INT yPos,CSize size)
{
	if (end - start > 0)
	{
		//绘制箭头
		CPen pen(PS_ENDCAP_ROUND, 2, RGB(0, 99, 99));
		CPen *oldpen = pDC->SelectObject(&pen);
		//绘制线
		pDC->MoveTo(start, yPos);
		pDC->LineTo(end, yPos);
		//左箭头
		pDC->MoveTo(start + size.cx, yPos - size.cy / 2 - 1);
		pDC->LineTo(start, yPos);
		pDC->LineTo(start + size.cx, yPos + size.cy / 2);
		//右箭头
		pDC->MoveTo(end - size.cx, yPos - size.cy / 2 - 1);
		pDC->LineTo(end - 1, yPos);
		pDC->LineTo(end - size.cx, yPos + size.cy / 2);
		pDC->SelectObject(oldpen);
		pen.DeleteObject();
	}
	return 0;
}


BOOL CSetADlg::PaintCheck()
{
	//做绘图函数的上层封装，对数据进行获取和有效性检查，调用时只调用PaintCheck，不调用DrawModel
	m_edit_seta.GetWindowTextA(a_str);
	a_temp = atof(a_str);
	m_edit_set_d.GetWindowTextA(d_str);
	d_temp = atof(d_str);
	m_edit_set_l1.GetWindowTextA(l1_str);
	l1_temp = atof(l1_str);
	m_edit_set_l2.GetWindowTextA(l2_str);
	l2_temp = atof(l2_str);
	if (a_temp <= 0 || d_temp <= 0 || l1_temp < 0 || l2_temp < 0)
	{
		MessageBox("请输入有效数字", "", MB_OK);
		value_set = FALSE;
		return FALSE;
	}
	else DrawModel(pDC_model, drawRect);
	//对动态标注进行更新
	m_label_l2.ShowWindow(SW_HIDE);
	m_label_d.ShowWindow(SW_HIDE);
	m_label_l1.ShowWindow(SW_HIDE);
	m_label_d.ShowWindow(SW_SHOW);
	m_label_l2.ShowWindow(SW_SHOW);
	m_label_l1.ShowWindow(SW_SHOW);

	value_set = TRUE;
	return TRUE;
}


void CSetADlg::OnBnClickedButtonPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	PaintCheck();
}

