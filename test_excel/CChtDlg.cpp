// CChtDlg.cpp: 实现文件
//

#include "pch.h"
#include "test_excel.h"
#include "CChtDlg.h"
#include "afxdialogex.h"
#include "CJDCSDlg.h"


// CChtDlg 对话框
CChtDlg *CChtDlg::pChtDlg = NULL;

IMPLEMENT_DYNAMIC(CChtDlg, CDialogEx)

CChtDlg::CChtDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CHT, pParent)
{

}

CChtDlg::~CChtDlg()
{
}

void CChtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ChartCtrl_R, m_ChartCtrl_r);
	DDX_Control(pDX, IDC_ChartCtrl_I, m_ChartCtrl_i);
}

void CChtDlg::ReSize(void)
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
	//m_list_res.GetClientRect(list_rect);//获取列表宽度
}

void CChtDlg::Draw(DOUBLE a[], DOUBLE b[], CChartCtrl* place)
{
	place->EnableRefresh(false);
	CChartLineSerie* pLineSerie;
	place->RemoveAllSeries();//清空
	pLineSerie = place->CreateLineSerie();
	pLineSerie->AddPoints(a, b, nRow);
	place->EnableRefresh(true);
}


BEGIN_MESSAGE_MAP(CChtDlg, CDialogEx)
END_MESSAGE_MAP()


// CChtDlg 消息处理程序


BOOL CChtDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pChtDlg = this;
	//取客户区大小，用于自适应分辨率
	CRect rect;
	GetClientRect(&rect);     
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	//给实部图像创建坐标轴
	CChartAxis *pAxis_r_b = NULL;
	CChartAxis *pAxis_r_l = NULL;
	pAxis_r_b = m_ChartCtrl_r.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis_r_b->SetAutomatic(true);
	pAxis_r_l = m_ChartCtrl_r.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis_r_l->SetAutomatic(true);

	//给虚部图像创建坐标轴
	CChartAxis *pAxis_i_b = NULL;
	CChartAxis *pAxis_i_l = NULL;
	pAxis_i_b = m_ChartCtrl_i.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis_i_b->SetAutomatic(true);
	pAxis_i_l = m_ChartCtrl_i.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis_i_l->SetAutomatic(true);

	//给两幅图像添加标题
	CString title_r, title_i;
	title_r = _T("介电常数实部");
	title_i = _T("介电常数虚部");
	m_ChartCtrl_r.GetTitle()->AddString(TChartString(title_r));
	m_ChartCtrl_i.GetTitle()->AddString(TChartString(title_i));

	//设置坐标轴标题
	CString axistitle_r_x, axistitle_r_y, axistitle_i_x, axistitle_i_y;
	axistitle_r_x = axistitle_i_x = _T("频率f(GHz)");
	axistitle_r_y = _T("Epsilon实部");
	axistitle_i_y = _T("Epsilon虚部");
	m_ChartCtrl_r.GetBottomAxis()->GetLabel()->SetText(TChartString(axistitle_r_x));
	m_ChartCtrl_r.GetLeftAxis()->GetLabel()->SetText(TChartString(axistitle_r_y));
	m_ChartCtrl_i.GetBottomAxis()->GetLabel()->SetText(TChartString(axistitle_i_x));
	m_ChartCtrl_i.GetLeftAxis()->GetLabel()->SetText(TChartString(axistitle_i_y));

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
