// CDDLDlg.cpp: 实现文件
//

#include "pch.h"
#include "test_excel.h"
#include "CDDLDlg.h"
#include <string>

using std::string;

static int width_roll = GetSystemMetrics(SM_CXVSCROLL);

// CDDLDlg 对话框

IMPLEMENT_DYNAMIC(CDDLDlg, CDialogEx)

CDDLDlg::CDDLDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_DDL, pParent)
{

}

CDDLDlg::~CDDLDlg()
{
}

void CDDLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

void CDDLDlg::ReSize(void)
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


BEGIN_MESSAGE_MAP(CDDLDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDDLDlg::OnTcnSelchangeTab1)
	ON_COMMAND(ID_MENE_OPEN, &CDDLDlg::OnMeneOpen)
END_MESSAGE_MAP()


// CDDLDlg 消息处理程序


BOOL CDDLDlg::OnInitDialog()
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

	m_Tab.InsertItem(0, _T("计算")); //插入第一个标签
	//m_Tab.InsertItem(1, _T("结果")); //插入第二个标签
	//m_Tab.InsertItem(2, _T("绘制曲线"));//插入第三个标签
	cal_dlg_ddl.Create(IDD_DLG_CAL_DDL, &m_Tab); //创建第一个标签页
	//res_dlg.Create(IDD_DLG_RES, &m_Tab); //创建第二个标签页
	//cht_dlg.Create(IDD_DLG_CHT, &m_Tab); //创建第三个标签页
	m_Tab.GetClientRect(&tabRect); //获取客户区CRect
	tabRect.left += 1;//调整tabRect，使覆盖范围适合放置标签页
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	cal_dlg_ddl.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);//根据调整好的tabRect放置cal_cal子对话框，并设置为显示
	//res_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);//设置res_dlg子对话框，设置为隐藏
	//cht_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);//设置cht_dlg子对话框，设置为隐藏
	//res_dlg.ReSize();
	cal_dlg_ddl.ReSize();
	//cht_dlg.ReSize();

	//菜单栏初始化
	m_Menu.LoadMenuA(IDR_MENU2);//初始化菜单栏
	SetMenu(&m_Menu);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDDLDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CRect tabRect; //获取标签客户区Rect
	m_Tab.GetClientRect(&tabRect);
	//调整tabRect，使覆盖范围适合放置标签页
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	switch (m_Tab.GetCurSel())
	{
	case 0://显示计算界面
		cal_dlg_ddl.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		//res_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		//cht_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	//case 1://显示结果界面
	//	cal_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	//	res_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	//	cht_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	//	break;
	//case 2://显示绘图界面
	//	cal_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	//	res_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	//	cht_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	//	break;
	default:
		break;
	}
	*pResult = 0;
}


void CDDLDlg::OnMeneOpen()
{
	// TODO: 在此添加命令处理程序代码
	//获取文件路径
	CFileDialog* lpszOpenFile = new CFileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, "Excel File(*.xlsx;*.xls;*.csv)|*.xls;*.xlsx;*.csv", NULL);
	if (lpszOpenFile->DoModal() == IDOK)
	{
		szGetName = lpszOpenFile->GetPathName();
		cal_dlg_ddl.m_filepath.SetWindowText(szGetName);
		delete lpszOpenFile;
		lpszOpenFile = NULL;
	}
	else
	{
		delete lpszOpenFile;
		lpszOpenFile = NULL;
		return;
	}

	//打开该路径下的Excel，显示在list中
	excel.initExcel();
	if (!excel.open(CT2A(szGetName)))
	{
		MessageBox(_T("excel文件打开出错！"), "", MB_OK);
	}
	else
	{
		CString strSheetName = excel.getSheetName(1);//获取第一个sheet名  
		bool bLoad = excel.loadSheet(strSheetName);//装载sheet  
		nRow = excel.getRowCount();//获取sheet中行数  
		nCol = excel.getColumnCount();//获取sheet中列数  
		CString cell;//获取一个单元格数据
		string  cells;//转换为string类型

		cal_dlg_ddl.m_list.DeleteAllItems();
		while (cal_dlg_ddl.m_list.DeleteColumn(0));
		//res_dlg.m_list_res.DeleteAllItems();
		//while (res_dlg.m_list_res.DeleteColumn(0));

		cal_dlg_ddl.m_list.nCol = nCol;//listctrl控件列数与excel相同
		//res_dlg.m_list_res.nCol = cal_dlg.m_list.nCol + 2;//多出结果的实部和虚部
		cal_dlg_ddl.m_list.nRow = /*res_dlg.m_list_res.nRow =*/ nRow;//列表和结果数组的行数相同
		//cht_dlg.nRow = nRow - 1;//结果行数比列表少一，少了标题行

		for (int j = 1; j <= cal_dlg_ddl.m_list.nRow; j++)
		{
			if (j == 1)//如果是excel第一行，即标题行
			{
				for (int i = 1; i <= cal_dlg_ddl.m_list.nCol; ++i)//从excel的第一列开始，到最后一列，示例为6
				{
					cell = excel.getCellString(1, i);  //获取列标题
					cal_dlg_ddl.m_list.InsertColumn(i - 1, cell, LVCFMT_LEFT, (cal_dlg_ddl.list_rect.Width() - width_roll) / cal_dlg_ddl.m_list.nCol);//对列表而言，从0列开始插入列
					//res_dlg.m_list_res.InsertColumn(i - 1, cell, LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
				}
			}
			else if (excel.getCellDouble(j, 1) != 0.0)//数据行，从excel第2行开始
			{
				for (int i = 1; i <= cal_dlg_ddl.m_list.nCol; ++i)//excel第一列
				{
					cell.Format(TEXT("%lf"), excel.getCellDouble(j, i)); //获取一个单元格数据
					if (i == 1)//如果是第一列
					{
						cal_dlg_ddl.m_list.InsertItem(j - 2, cell);//从第0行开始插入行
						//res_dlg.m_list_res.InsertItem(j - 2, cell);
					}
					else
					{
						cal_dlg_ddl.m_list.SetItemText(j - 2, i - 1, cell);//插入元素
						//res_dlg.m_list_res.SetItemText(j - 2, i - 1, cell);
					}
				}
			}
			else break;
		}
		//res_dlg.m_list_res.InsertColumn(nCol, _T("Epsilon实部"), LVCFMT_LEFT, (cal_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
		//res_dlg.m_list_res.InsertColumn(nCol + 1, _T("Epsilon虚部"), LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);

		/*int row1temp = res_dlg.m_list_res.GetItemCount();
		int row2temp = cal_dlg.m_list.GetItemCount();*/
		/*for (int i = 0; i < nRow; i++)
		{
			CString a = res_dlg.m_list_res.GetItemText(i, 1);
			CString b;
			b.Format(TEXT("%lf"), excel.getCellDouble(i + 2, 2));
			if (a != b)
			{
				CString str;
				str.Format("%d	%d", i, i + 2);
				MessageBox(str);
			}
		}*/

		//res_dlg.m_list_res.InsertColumn(nCol + 2, _T("前一次gamma虚部"), LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
		//res_dlg.m_list_res.InsertColumn(nCol + 3, _T("gamma虚部"), LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
		//res_dlg.m_list_res.InsertColumn(nCol + 4, _T("修改后gamma虚部"), LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
	}
	excel.close();
	excel.release();
}
