
// test_excelDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "test_excel.h"
#include "CJDCSDlg.h"
#include "afxdialogex.h"
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int width_roll = GetSystemMetrics(SM_CXVSCROLL);

// CtestexcelDlg 对话框

static UINT BASED_CODE indicators[]=
{
	ID_INDICATOR_STATUS,
	ID_INDICATOR_COPYRIGHT,
};

CJDCSDlg::CJDCSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_JDCS, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MYICON);
}

void CJDCSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CJDCSDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENE_OPEN, &CJDCSDlg::OnMeneOpen)
	ON_COMMAND(ID_MENU_SAVE, &CJDCSDlg::OnMenuSave)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CJDCSDlg::OnTcnSelchangeTab1)
	ON_COMMAND(ID_MENU_SET_A, &CJDCSDlg::OnMenuSetA)
	ON_COMMAND_RANGE(ID_INDICATOR_STATUS, ID_INDICATOR_COPYRIGHT, NULL)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CtestexcelDlg 消息处理程序

BOOL CJDCSDlg::OnInitDialog()
{	
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

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

	//标签控件初始化
	m_Tab.InsertItem(0, _T("计算")); //插入第一个标签
	m_Tab.InsertItem(1, _T("结果")); //插入第二个标签
	m_Tab.InsertItem(2, _T("绘制曲线"));//插入第三个标签
	cal_dlg.Create(IDD_DLG_CAL, &m_Tab); //创建第一个标签页
	res_dlg.Create(IDD_DLG_RES, &m_Tab); //创建第二个标签页
	cht_dlg.Create(IDD_DLG_CHT, &m_Tab); //创建第三个标签页
	m_Tab.GetClientRect(&tabRect); //获取客户区CRect
	tabRect.left += 1;//调整tabRect，使覆盖范围适合放置标签页
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	cal_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);//根据调整好的tabRect放置cal_cal子对话框，并设置为显示
	res_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);//设置res_dlg子对话框，设置为隐藏
	cht_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);//设置cht_dlg子对话框，设置为隐藏
	res_dlg.ReSize();
	cal_dlg.ReSize();
	cht_dlg.ReSize();

	//菜单栏初始化
	m_Menu.LoadMenuA(IDR_MENU1);//初始化菜单栏
	SetMenu(&m_Menu);

	//状态栏初始化
	if (!m_bar.Create(this) || !m_bar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	//设置各栏长度
	m_bar.SetPaneInfo(0, ID_INDICATOR_STATUS, SBPS_NORMAL, rect.Width() / 2);
	m_bar.SetPaneInfo(1, ID_INDICATOR_COPYRIGHT, SBPS_NORMAL, rect.Width() / 2);
	//m_bar.SetPaneInfo(2, ID_INDICATOR_TIME, SBPS_NORMAL, rect.Width() / 4);
	m_bar.SetPaneText(0, _T("状态栏"));
	m_bar.SetPaneText(1, _T("版权所有 V1.0 东南大学（2020）"));
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_TIME);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJDCSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CJDCSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CJDCSDlg::OnMeneOpen()
{
	title_row = -1;
	// TODO: 在此添加命令处理程序代码
	//获取文件路径
	CFileDialog* lpszOpenFile = new CFileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, "Excel File(*.xlsx;*.xls;*.csv)|*.xls;*.xlsx;*.csv", NULL);
	if (lpszOpenFile->DoModal() == IDOK)
	{
		szGetName = lpszOpenFile->GetPathName();
		cal_dlg.m_filepath.SetWindowTextA(szGetName);
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
		m_bar.SetPaneText(0, _T("文件加载中"));
		CString strSheetName = excel.getSheetName(1);//获取第一个sheet名  
		bool bLoad = excel.loadSheet(strSheetName);//装载sheet  
		nRow = excel.getRowCount();//获取sheet中行数  
		nCol = excel.getColumnCount();//获取sheet中列数  
		CString cell;//获取一个单元格数据
		string cells;//转换为string类型

		cal_dlg.m_list.DeleteAllItems();
		while (cal_dlg.m_list.DeleteColumn(0));
		res_dlg.m_list_res.DeleteAllItems();
		while (res_dlg.m_list_res.DeleteColumn(0));

		cal_dlg.m_list.nCol = nCol;//listctrl控件列数与excel相同
		res_dlg.m_list_res.nCol = cal_dlg.m_list.nCol + 2;//多出结果的实部和虚部
		
		//跳过参数数据行，读取测量行
		for (int j = 1; j <= nRow; j++)
		{
			cell = excel.getCellString(j, 1);
			if (cell == _T("frequency"))
			{
				title_row = j;
				cal_dlg.m_list.nRow = res_dlg.m_list_res.nRow = nRow - title_row; //列表和结果数组的行数相同
				cht_dlg.nRow = res_dlg.m_list_res.nRow = nRow - title_row;//绘图所用结果行数比列表少一，少了标题行
			}
			else if (title_row == -1) continue;
			if (j == title_row)//如果是标题行
			{
				for (int i = 1; i <= nCol; ++i)//从excel的第一列开始，到最后一列，示例为6
				{
					cell = excel.getCellString(j, i);  //获取列标题
					cal_dlg.m_list.InsertColumn(i - 1, cell, LVCFMT_LEFT, (cal_dlg.list_rect.Width() - width_roll) / cal_dlg.m_list.nCol);//对列表而言，从0列开始插入列
					res_dlg.m_list_res.InsertColumn(i - 1, cell, LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
				}
			}
			else if (excel.getCellDouble(j, 1) != 0.0) //数据行，从excel第2行开始
			{
				for (int i = 1; i <= nCol; ++i)//excel第一列
				{
					if (i == 1)//如果是第一列
					{
						cell.Format(_T("%.4lf"), excel.getCellDouble(j, i)/GHz); //获取一个单元格数据
						cal_dlg.m_list.InsertItem(j - title_row - 1, cell);//从第0行开始插入行
						res_dlg.m_list_res.InsertItem(j - title_row - 1, cell);
					}
					else
					{
						cell.Format(_T("%.4lf"), excel.getCellDouble(j, i)); //获取一个单元格数据
						cal_dlg.m_list.SetItemText(j - title_row - 1, i - 1, cell);//插入元素
						res_dlg.m_list_res.SetItemText(j - title_row - 1, i - 1, cell);
					}
				}
			}
			else break;
		}
		res_dlg.m_list_res.InsertColumn(nCol, _T("Epsilon实部"), LVCFMT_LEFT, (cal_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
		res_dlg.m_list_res.InsertColumn(nCol + 1, _T("Epsilon虚部"), LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
		
		//res_dlg.m_list_res.InsertColumn(nCol + 2, _T("前一次gamma虚部"), LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
		//res_dlg.m_list_res.InsertColumn(nCol + 3, _T("gamma虚部"), LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
		//res_dlg.m_list_res.InsertColumn(nCol + 4, _T("修改后gamma虚部"), LVCFMT_LEFT, (res_dlg.list_rect.Width() - width_roll) / res_dlg.m_list_res.nCol);
		if (cal_dlg.value_set == TRUE)
		{
			if (IDYES == MessageBox(_T("使用已设定参数？"), "", MB_YESNO));
			else cal_dlg.value_set = FALSE;
		}
		cal_dlg.UpdateButton(cal_dlg.value_set);
		m_bar.SetPaneText(0, _T("文件加载完成"));
	}
	excel.close();
	excel.release();
}


void CJDCSDlg::OnMenuSave()
{
	// TODO: 在此添加命令处理程序代码
	
	//打开原路径下的Excel
	excel.initExcel();
	if (szGetName == _T(""))
	{
		MessageBox(_T("无数据文件"),"");
	}
	else if(!excel.open(CT2A(szGetName)))
	{
		MessageBox(_T("excel文件打开出错！"),"");
		excel.close();
	}
	else
	{
		CString strSheetName = excel.getSheetName(1);//获取第一个sheet名  
		bool bLoad = excel.loadSheet(strSheetName);//装载sheet  
		nRow = excel.getRowCount();//获取sheet中行数  
		nCol = excel.getColumnCount();//获取sheet中列数

		CString a_result_excel, d_result_excel, l1_result_excel, l2_result_excel;
		a_result_excel.Format(_T("%.4lf mm"), cal_dlg.a_value);
		d_result_excel.Format(_T("%.4lf mm"), cal_dlg.d_value);
		l1_result_excel.Format(_T("%.4lf mm"), cal_dlg.l1_value);
		l2_result_excel.Format(_T("%.4lf mm"), cal_dlg.l2_value);

		for (int j = 1; j <= nRow; j++)
		{
			if (j == 1) { excel.setCellString(1, nCol + 1, _T("波导宽边长度")); excel.setCellString(1, nCol + 2, a_result_excel); }
			if (j == 2) { excel.setCellString(2, nCol + 1, _T("材料厚度"));  excel.setCellString(2, nCol + 2, d_result_excel); }
			if (j == 3) { excel.setCellString(3, nCol + 1, _T("夹具1面间隙"));  excel.setCellString(3, nCol + 2, l1_result_excel); }
			if (j == 4) { excel.setCellString(4, nCol + 1, _T("夹具2面间隙"));  excel.setCellString(4, nCol + 2, l2_result_excel); }
			if (j == title_row)
			{
				excel.setCellString(title_row, nCol + 1, _T("Epsilon实部"));
				excel.setCellString(title_row, nCol + 2, _T("Epsilon虚部"));
			}
			else if (j > title_row)
			{
				for (int i = nCol + 1; i <= nCol + 2; ++i)
				{
					excel.setCellString(j, i, res_dlg.m_list_res.GetItemText(j - title_row - 1, i - 1));
				}
			}
			else continue;
		}
		excel.close(TRUE);
	}
	excel.release();
}


void CJDCSDlg::OnMenuSetA()
{
	// TODO: 在此添加命令处理程序代码
	//参数编辑对话款
	CSetADlg seta_dlg;
	//显示在对话框中的默认值或上次编辑值
	seta_dlg.a_str.Format(_T("%.3lf"), cal_dlg.a_value);
	seta_dlg.d_str.Format(_T("%.3lf"), cal_dlg.d_value);
	seta_dlg.l1_str.Format(_T("%.3lf"), cal_dlg.l1_value);
	seta_dlg.l2_str.Format(_T("%.3lf"), cal_dlg.l2_value);
	seta_dlg.DoModal();
}


void CJDCSDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
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
		cal_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		res_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		cht_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 1://显示结果界面
		cal_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		res_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		cht_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 2://显示绘图界面
		cal_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		res_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		cht_dlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}

	//
	*pResult = 0;
}


void CJDCSDlg::ReSize(void)
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


//void CJDCSDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	switch (nIDEvent)
//	{
//	case 1:
//	{
//		CTime t1;
//		t1 = CTime::GetCurrentTime();
//		m_bar.SetPaneText(2, t1.Format("%Y-%m-%d  %H:%M:%S"));
//		break;
//	}
//	}
//	CDialogEx::OnTimer(nIDEvent);
//}
