// CResDlg.cpp: 实现文件
//

#include "pch.h"
#include "test_excel.h"
#include "CResDlg.h"
#include "afxdialogex.h"
#include "CJDCSDlg.h"

// CResDlg 对话框
CResDlg *CResDlg::pResDlg = NULL;

IMPLEMENT_DYNAMIC(CResDlg, CDialogEx)

CResDlg::CResDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_RES, pParent)
{

}


CResDlg::~CResDlg()
{
}


void CResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RES, m_list_res);
	DDX_Control(pDX, IDC_EDIT_F, m_edit_f);
	DDX_Control(pDX, IDC_EDIT_EPSR, m_edit_epsr);
	DDX_Control(pDX, IDC_EDIT_EPSI, m_edit_epsi);
}


BEGIN_MESSAGE_MAP(CResDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RES, &CResDlg::OnLvnItemchangedListRes)
END_MESSAGE_MAP()


// CResDlg 消息处理程序


void CResDlg::OnLvnItemchangedListRes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//将鼠标选中行显示在下方编辑框中
	CString F_edit;
	CString Eps_r_edit;
	CString Eps_i_edit;

	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)
	{
		INT cur_row = pNMListView->iItem; //鼠标指中行

		F_edit = m_list_res.GetItemText(cur_row, 0);
		SetDlgItemText(IDC_EDIT_F, F_edit);
		Eps_r_edit = m_list_res.GetItemText(cur_row, m_list_res.nCol - 2);
		SetDlgItemText(IDC_EDIT_EPSR, Eps_r_edit);
		Eps_i_edit = m_list_res.GetItemText(cur_row, m_list_res.nCol - 1);
		SetDlgItemText(IDC_EDIT_EPSI, Eps_i_edit);
	}
}


void CResDlg::ReSize(void)
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;
	GetClientRect(&recta);     //取客户区大小  
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	/*CString str;
	str.Format(_T("%d     %d"), Newp.x, Newp.y);
	MessageBox(str);*/
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
	m_list_res.GetClientRect(list_rect);//获取列表宽度
}


BOOL CResDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pResDlg = this;
	//列表控件初始化
	m_list_res.SetExtendedStyle(m_list_res.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//风格设置，整行选择，表格线
	CRect rect;
	GetClientRect(&rect);     //取客户区大小  
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}