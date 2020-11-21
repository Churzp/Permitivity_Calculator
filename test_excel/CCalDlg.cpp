// CCalDlg.cpp: 实现文件
//

#include "pch.h"
#include "test_excel.h"
#include "CCalDlg.h"
#include "afxdialogex.h"
#include "CJDCSDlg.h"
#include <iostream>
#include "my_gsl.h"

// CCalDlg 对话框

using namespace std;
IMPLEMENT_DYNAMIC(CCalDlg, CDialogEx)

CCalDlg::CCalDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_RES, pParent)
{

}

CCalDlg::~CCalDlg()
{
}

void CCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_list);
	DDX_Control(pDX, IDC_EDIT1, m_filepath);
	DDX_Control(pDX, IDC_COMBO1, m_AlgChoosing);
	DDX_Control(pDX, IDC_SHOW_ALG, m_static_alg);
	DDX_Control(pDX, IDC_SHOW_FILEPATH, m_static_filepath);
	DDX_Control(pDX, IDC_BUTTON_CAL, m_button_caljdcs);
}


BEGIN_MESSAGE_MAP(CCalDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCalDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON_CAL, &CCalDlg::OnBnClickedButtonCal)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCalDlg 消息处理程序

//处理多值化
void CCalDlg::Kz(INT i,INT mode)
{
	//使用switch方法区分一二面结果
	switch (mode)
	{
	case 1:K = (1.0 + S11[i] * S11[i] - S21[i] * S21[i]) / (2.0 * S11[i]); break;
	case 2:K = (1.0 + S22[i] * S22[i] - S12[i] * S12[i]) / (2.0 * S22[i]); break;
	}

	complex<double>temp1 = K + sqrt(K*K - 1.0);
	if (abs(temp1) <= 1.0) tao = temp1;
	else tao = K - sqrt(K*K - 1.0);
	switch (mode)
	{
	case 1:T = (S11[i] + S21[i] - tao) / (1.0 - (S11[i] + S21[i])*tao); break;
	case 2:T = (S22[i] + S12[i] - tao) / (1.0 - (S22[i] + S12[i])*tao); break;
	}

	complex<double>temp(0, 2 * n*Pi);//多值问题2n*pi*i修正
	gam_ex = gam;//前一次传播系数
	gam = (log(1.0 / T) + temp) / d;//以当前n计算传播常数
	if (i == 0)
	{
		while (gam.imag() < 0)
		{
			n += 1;
			temp = complex<double>(0, 2 * n*Pi);
			gam = (log(1.0 / T) + temp) / d;
		}
	}
	else
	{
		while (i > 0 && ((gam.imag() - gam_ex.imag())/ gam_ex.imag()) < -0.05) //如果频率升高虚部变小
		{
			n += 1;
			temp = complex<double>(0, 2 * n*Pi);
			gam = (log(1.0/T) + temp) / d;//n变化后重新计算gam
		}
	}
	switch (mode)
	{
	case 1:g1[i] = gam.imag(); break;
	case 2:g2[i] = gam.imag(); break;
	}
}


complex<double> CCalDlg::NRW2(INT i,INT mode)
{
	DOUBLE namdc = 2 * a;//TE10模对应的截止波长
	DOUBLE namd0 = c / f[i];//空气中波长
	complex<double>temp;
	switch (mode)
	{
	case 1:temp = sqrt(((1.0 + S11[i])*(1.0 + S11[i]) - S21[i] * S21[i]) / ((1.0 - S11[i])*(1.0 - S11[i]) - S21[i] * S21[i])); break;
	case 2:temp = sqrt(((1.0 + S22[i])*(1.0 + S22[i]) - S12[i] * S12[i]) / ((1.0 - S22[i])*(1.0 - S22[i]) - S12[i] * S12[i])); break;
	}
	if (temp.real() > 0) Z = temp;
	else Z = -temp;
	return (1 - (namd0 / namdc)*(namd0 / namdc)) / (Z*Z) + (namd0 / namdc)*(namd0 / namdc);
}


DOUBLE CCalDlg::NRW3(INT i,INT mode)
{
	DOUBLE namdc = 2 * a;//TE10模对应的截止波长
	DOUBLE namd0 = c / f[i];//空气中波长
	complex<double>temp;
	switch (mode)
	{
	case 1:temp = sqrt(((1.0 + S11[i])*(1.0 + S11[i]) - S21[i] * S21[i]) / ((1.0 - S11[i])*(1.0 - S11[i]) - S21[i] * S21[i])); break;
	case 2:temp = sqrt(((1.0 + S22[i])*(1.0 + S22[i]) - S12[i] * S12[i]) / ((1.0 - S22[i])*(1.0 - S22[i]) - S12[i] * S12[i])); break;
	}
	if (temp.real() > 0) Z = temp;
	else Z = -temp;
	complex<double>temp2 = (1 - (namd0 / namdc)*(namd0 / namdc)) / (Z*Z) + (namd0 / namdc)*(namd0 / namdc);
	complex<double>temp3 = sqrt(((namd0 / namdc)*(namd0 / namdc) - temp2) / ((namd0 / (2 * Pi))*((namd0 / (2 * Pi)))));
	return abs(temp3.imag());
}


BOOL CCalDlg::Alg1_T_gam() //算法一，透射系数与传播常数
{	
	//动态数组初始化
	array_f = new DOUBLE[m_list.nRow];
	array_er = new DOUBLE[m_list.nRow];
	array_ei = new DOUBLE[m_list.nRow];
	g1 = new DOUBLE[m_list.nRow];
	g2 = new DOUBLE[m_list.nRow];
	f = new DOUBLE[m_list.nRow];
	S11 = new complex<double>[m_list.nRow];
	S21 = new complex<double>[m_list.nRow];
	S12 = new complex<double>[m_list.nRow];
	S22 = new complex<double>[m_list.nRow];
	if (S11 == NULL || S21 == NULL || S12 == NULL || S22 == NULL || array_f == NULL || array_er == NULL || array_ei == NULL || g1 == NULL || g2 == NULL || f == NULL) return 1;
	else if (!Svalue_BuildandCorrect())
	{
		MessageBox("参数设置有误，请检查后重新计算", "");
		return 1;
	}

	//一面计算g1数组
	for (int i = 0; i <= m_list.nRow - 1; i++)	Kz(i, 1);
	//n重置，计算g2数组
	n = 0;
	for (int i = 0; i <= m_list.nRow - 1; i++)	Kz(i, 2);

	//索引一面i1，计算偏移量bc1
	INT i1 = 0, i2 = m_list.nRow - 1;
	complex<double>temp1 = NRW2(i1,1);
	complex<double>temp2 = NRW2(i2,1);
	while (i2 > i1&&abs(temp1 - temp2) > 0.2)
	{
		if (temp1.real() < temp2.real()) i1 = (i1 + i2) / 2;
		else i2 = (i1 + i2) / 2;
		temp1 = NRW2(i1,1);
		temp2 = NRW2(i2,1);
	}
	DOUBLE bc1 = NRW3(i1, 1) - g1[i1];
	//索引二面i1，计算偏移量bc2
	i1 = 0;i2 = m_list.nRow - 1;
	temp1 = NRW2(i1, 2);
	temp2 = NRW2(i2, 2);
	while (i2 > i1&&abs(temp1 - temp2) > 0.2)
	{
		if (temp1.real() < temp2.real()) i1 = (i1 + i2) / 2;
		else i2 = (i1 + i2) / 2;
		temp1 = NRW2(i1, 2);
		temp2 = NRW2(i2, 2);
	}
	DOUBLE bc2 = NRW3(i1, 2) - g2[i1];

	//准备工作完成，开始计算介电常数
	for (int i = 0; i <= m_list.nRow - 1; i++)
	{
		g1[i] = g1[i] + bc1;
		g2[i] = g2[i] + bc2;

		DOUBLE namdc = 2 * a;//TE10模对应的截止波长
		DOUBLE namd0 = c / f[i];//空气中波长

		K = (1.0 - S11[i] * S11[i] + S21[i] * S21[i]) / (2.0*S21[i]);
		DOUBLE temp3 = abs(K + sqrt(K*K - 1.0));
		if (temp3 <= 1) T = K + sqrt(K*K - 1.0);
		else T = K - sqrt(K*K - 1.0);
		Gamma1 = log(abs(1.0 / T)) / d + complex<double>(0, g1[i]);
		K = (1.0 - S22[i] * S22[i] + S12[i] * S12[i]) / (2.0*S12[i]);
		temp3 = abs(K + sqrt(K*K - 1.0));
		if (temp3 <= 1) T = K + sqrt(K*K - 1.0);
		else T = K - sqrt(K*K - 1.0);
		Gamma2 = log(abs(1.0 / T)) / d + complex<double>(0, g2[i]);
		
		//计算相对介电系数，一二面取平均
		Eps = ((namd0 / namdc)*(namd0 / namdc) - Gamma1 * Gamma1 *(namd0 / (2 * Pi))*(namd0 / (2 * Pi))
			+(namd0 / namdc)*(namd0 / namdc) - Gamma2 * Gamma2 *(namd0 / (2 * Pi))*(namd0 / (2 * Pi))) / 2.0;
		//取结果，用于显示或画图
		Eps_r = Eps.real();
		Eps_i = -Eps.imag();
		array_er[i] = Eps_r;
		array_ei[i] = Eps_i;

		CString eps_r;
		eps_r.Format(TEXT("%.4lf"), Eps_r);
		CString eps_i;
		eps_i.Format(TEXT("%.4lf"), Eps_i);
		//CString gamma_ex_i;
		//gamma_ex_i.Format(TEXT("%.4lf"), g1[i]);
		//CString gamma_i;
		//gamma_i.Format(TEXT("%.4lf"), g2[i]);

		CResDlg* pRes = CResDlg::pResDlg;
		CMyListCtrl &ResList = pRes->m_list_res;

		ResList.SetItemText(i, ResList.nCol - 2, eps_r);
		ResList.SetItemText(i, ResList.nCol - 1, eps_i); 
		//ResList.SetItemText(i, ResList.nCol, gamma_ex_i);
		//ResList.SetItemText(i, ResList.nCol + 1, gamma_i);
	}

	CChtDlg* pCht = CChtDlg::pChtDlg;
	CChartCtrl &pic_r = pCht->m_ChartCtrl_r;
	CChartCtrl &pic_i = pCht->m_ChartCtrl_i;
	pCht->Draw(array_f, array_er, &pic_r);
	pCht->Draw(array_f, array_ei, &pic_i);
	
	//释放资源指针
	delete[] array_f;
	delete[] array_ei;
	delete[] array_er;
	delete[] g1;
	delete[] g2;
	delete[] f;
	delete[] S11;
	delete[] S21;
	delete[] S12;
	delete[] S22;
	array_f = NULL;
	array_er = NULL;
	array_ei = NULL;
	g1 = NULL;
	g2 = NULL;
	f = NULL;
	S11 = NULL;
	S21 = NULL;
	S12 = NULL;
	S22 = NULL;
	//释放窗口指针
	pCht = NULL;
	CString str;
	str.Format("虚部突变%d次", n);
	MessageBox(str,"");

	return 0;
}


BOOL CCalDlg::Alg2_Gamma_Z() //算法二，反射系数与阻抗
{
	////存放结果的动态数组初始化
	//array_f = new DOUBLE[m_list.nRow];
	//array_er = new DOUBLE[m_list.nRow];
	//array_ei = new DOUBLE[m_list.nRow];

	//complex<double>K;
	//complex<double>Gamma;
	//complex<double>Eps;

	//for (int i = 0; i <= m_list.nRow - 1; i++)
	//{
	//	f = atof(m_list.GetItemText(i, 0)) * 1e9;//获取频率
	//	array_f[i] = f / 1e9;
	//	s11pre_r = atof(m_list.GetItemText(i, 1));//获取s11实部
	//	s11pre_i = atof(m_list.GetItemText(i, 2));//获取s11虚部
	//	s21pre_r = atof(m_list.GetItemText(i, 3));//获取s21实部
	//	s21pre_i = atof(m_list.GetItemText(i, 4));//获取s21虚部
	//	s21pre_r = atof(m_list.GetItemText(i, 5));//获取s21实部
	//	s21pre_i = atof(m_list.GetItemText(i, 6));//获取s21虚部
	//	s22pre_r = atof(m_list.GetItemText(i, 7));//获取s22实部
	//	s22pre_i = atof(m_list.GetItemText(i, 8));//获取s22虚部
	//	beta = 2 * Pi*f / c;//空气中传播常数
	//	PhaseCorrect();
	//	
	//	K = (1.0 + S11 * S11 - S21 * S21) / (2.0 * S11);
	//	Gamma = K + sqrt(K*K - 1.0);
	//	if (abs(Gamma) > 1)
	//	{
	//		Gamma = K - sqrt(K*K - 1.0);
	//	}

	//	DOUBLE namdc = 2.0 * a;
	//	DOUBLE namd0 = c / f;
	//	Z = (1.0 + Gamma) / (1.0 - Gamma);
	//	Eps = (1.0 - (namd0 / namdc)*(namd0 / namdc)) / (Z*Z) + (namd0 / namdc)*(namd0 / namdc);
	//	Eps_r = Eps.real();
	//	Eps_i = Eps.imag();
	//	array_er[i] = Eps_r;
	//	array_ei[i] = Eps_i;

	//	CString eps_r;
	//	eps_r.Format(_T("%.4lf"), Eps_r);
	//	CString eps_i;
	//	eps_i.Format(_T("%.4lf"), Eps_i);

	//	CResDlg* pRes = CResDlg::pResDlg;
	//	CMyListCtrl &ResList = pRes->m_list_res;

	//	ResList.SetItemText(i, ResList.nCol - 2, eps_r);
	//	ResList.SetItemText(i, ResList.nCol - 1, eps_i);
	//}

	//CChtDlg* pCht = CChtDlg::pChtDlg;
	//CChartCtrl &pic_r = pCht->m_ChartCtrl_r;
	//CChartCtrl &pic_i = pCht->m_ChartCtrl_i;
	//pCht->Draw(array_f, array_er, &pic_r);
	//pCht->Draw(array_f, array_ei, &pic_i);

	//delete[] array_f;
	//delete[] array_ei;
	//delete[]array_er;
	//array_f = NULL;
	//array_er = NULL;
	//array_ei = NULL;
	return 0;
}


BOOL CCalDlg::Svalue_BuildandCorrect()
{
	DOUBLE beta; //空气中传播常数
	for (int i = 0; i <= m_list.nRow - 1; i++)
	{
		f[i] = atof(m_list.GetItemText(i, 0)) * 1e9;//获取频率
		array_f[i] = f[i] / 1e9;
		s11pre_r = atof(m_list.GetItemText(i, 1));//获取s11实部
		s11pre_i = atof(m_list.GetItemText(i, 2));//获取s11虚部
		s21pre_r = atof(m_list.GetItemText(i, 3));//获取s21实部
		s21pre_i = atof(m_list.GetItemText(i, 4));//获取s21虚部
		s12pre_r = atof(m_list.GetItemText(i, 5));//获取s21实部
		s12pre_i = atof(m_list.GetItemText(i, 6));//获取s21虚部
		s22pre_r = atof(m_list.GetItemText(i, 7));//获取s22实部
		s22pre_i = atof(m_list.GetItemText(i, 8));//获取s22虚部
		DOUBLE temp = (2 * Pi*f[i] / c)*(2 * Pi*f[i] / c) - (Pi / a)*(Pi / a);
		if (temp <= 0) 	return FALSE;
		beta = sqrt(temp);//空气中传播常数
		//以校准前数据构建复数
		complex<double>S11pre(s11pre_r, s11pre_i);
		complex<double>S21pre(s21pre_r, s21pre_i);
		complex<double>S12pre(s12pre_r, s12pre_i);
		complex<double>S22pre(s22pre_r, s22pre_i);
		//根据beta（随f变化），l1，l2构建复数校准因子
		complex<double>Correct11(cos(2 * beta*l1), sin(2 * beta*l1));
		complex<double>Correct21(cos(beta*(l1 + l2)), sin(beta*(l1 + l2)));
		complex<double>Correct12(cos(beta*(l1 + l2)), sin(beta*(l1 + l2)));
		complex<double>Correct22(cos(2 * beta*l2), sin(2 * beta*l2));
		//校准
		S11[i] = S11pre * Correct11;
		S21[i] = S21pre * Correct21;
		S12[i] = S12pre * Correct12;
		S22[i] = S22pre * Correct22;
	}	
	return TRUE;
}


void CCalDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	/*获取算法种类
	1：基于透射系数与传播常数
	2：基于反射系数与阻抗*/
	alg = m_AlgChoosing.GetCurSel() + 1;
}


void CCalDlg::OnBnClickedButtonCal()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_list.GetItemCount())
	{
		MessageBoxA(_T("无数据"), _T("警告"));
	}
	else
	{
		a = a_value * pow(10, -3);
		d = d_value * pow(10, -3);
		l1 = l1_value * pow(10, -3);
		l2 = l2_value * pow(10, -3);
		switch (alg)
		{
		case 1:
			if (!Alg1_T_gam()) MessageBox(_T("计算完成，可在结果界面查看"), "");
			else MessageBox(_T("计算失败"), "");
			break;
		case 2:
			if (!Alg2_Gamma_Z()) MessageBox(_T("计算完成，可在结果界面查看"), "");
			else MessageBox(_T("计算失败"), "");
			break;
		default:
			break;
		}
	}	
}


void CCalDlg::UpdateButton(BOOL value_set)
{
	if (value_set == TRUE)
	{
		m_button_caljdcs.EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VALUEUNSET)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_button_caljdcs.EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_VALUEUNSET)->ShowWindow(SW_SHOW);
	}
}


void CCalDlg::ReSize(void)
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


BOOL CCalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//列表控件初始化
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//风格设置，整行选择，表格线

	//下拉列表初始化
	m_AlgChoosing.AddString(_T("介电常数计算"));
	//m_AlgChoosing.AddString(_T("基于反射系数与阻抗"));
	m_AlgChoosing.SetCurSel(0);

	CRect rect;
	GetClientRect(&rect);     //取客户区大小  
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



HBRUSH CCalDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_VALUEUNSET)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
