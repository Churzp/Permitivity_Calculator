// CFuncSel.cpp: 实现文件
//

#include "pch.h"
#include "test_excel.h"
#include "CFuncSelDlg.h"
#include "afxdialogex.h"
#include "CJDCSDlg.h"
#include "CDDLDlg.h"


// CFuncSel 对话框

IMPLEMENT_DYNAMIC(CFuncSelDlg, CDialogEx)

CFuncSelDlg::CFuncSelDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_FUNCSEL, pParent)
{

}

CFuncSelDlg::~CFuncSelDlg()
{
}

void CFuncSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFuncSelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_JDCS, &CFuncSelDlg::OnBnClickedButtonJdcs)
	ON_BN_CLICKED(IDC_BUTTON_DDL, &CFuncSelDlg::OnBnClickedButtonDdl)
END_MESSAGE_MAP()


// CFuncSel 消息处理程序

void CFuncSelDlg::OnBnClickedButtonJdcs()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(FALSE);
	CJDCSDlg dlg;
	dlg.DoModal();
	this->ShowWindow(TRUE);
}


void CFuncSelDlg::OnBnClickedButtonDdl()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(FALSE);
	CDDLDlg dlg;
	dlg.DoModal();
	this->ShowWindow(TRUE);
}


BOOL CFuncSelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//用于调整软件界面大小
	CRect rect;
	GetClientRect(&rect);     //取客户区大小  
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	CRect rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
	ReSize();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CFuncSelDlg::ReSize(void)
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
