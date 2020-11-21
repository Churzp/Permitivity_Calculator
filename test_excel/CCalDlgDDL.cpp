// CCalDlgDDL.cpp: 实现文件
//

#include "pch.h"
#include "test_excel.h"
#include "CCalDlgDDL.h"
#include "afxdialogex.h"
#include "CDDLDlg.h"
#include <iostream>


// CCalDlgDDL 对话框

IMPLEMENT_DYNAMIC(CCalDlgDDL, CDialogEx)

CCalDlgDDL::CCalDlgDDL(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CAL_DDL, pParent)
{

}

CCalDlgDDL::~CCalDlgDDL()
{
}

void CCalDlgDDL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_filepath);
	//  DDX_Control(pDX, IDC_COMBO1, m_AlgChoosing);
	DDX_Control(pDX, IDC_LIST_SHOW, m_list);
}


BEGIN_MESSAGE_MAP(CCalDlgDDL, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CAL, &CCalDlgDDL::OnBnClickedButtonCal)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCalDlgDDL::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CCalDlgDDL 消息处理程序

void CCalDlgDDL::ReSize(void)
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
	m_list.GetClientRect(list_rect);//获取列表宽度
}


void CCalDlgDDL::OnBnClickedButtonCal()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CCalDlgDDL::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CCalDlgDDL::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//列表控件初始化
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//风格设置，整行选择，表格线

	//下拉列表初始化
	/*m_AlgChoosing.AddString(_T("基于透射系数与传播常数"));
	m_AlgChoosing.AddString(_T("基于反射系数与阻抗"));
	m_AlgChoosing.SetCurSel(0);*/
	
	CRect rect;
	GetClientRect(&rect);     //取客户区大小  
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
