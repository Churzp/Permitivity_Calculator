#pragma once
#include"CMyListCtrl.h"
#include<math.h>
#include<complex>
#include"CMyComboBox.h"

using namespace std;

const double Pi = 3.14159265358979323846264338328;
const double c = 3.0e8;

// CCalDlg 对话框

class CCalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalDlg)

public:
	CCalDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCalDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	DOUBLE a;//m为单位的波导宽度
	DOUBLE d;//m为单位的材料厚度
	DOUBLE l1;//m为单位的左夹具间隙
	DOUBLE l2;//m为单位的右夹具间隙

	//修正前，包括了夹具间隙
	DOUBLE s11pre_r;
	DOUBLE s11pre_i;
	DOUBLE s21pre_r;
	DOUBLE s21pre_i;
	DOUBLE s12pre_r;
	DOUBLE s12pre_i;
	DOUBLE s22pre_r;
	DOUBLE s22pre_i;
	//complex<double>S11;//S11修正后
	//complex<double>S21;//S21修正后
	//complex<double>S12;//S12修正后
	//complex<double>S22;//S22修正后

	INT n = 0;//多值问题
	complex<double>gam; //传播常数伽马
	complex<double>gam_ex;//前一次频率较低时传播常数
	complex<double>Gamma1;//成型的Gamma，基于1面
	complex<double>Gamma2;//成型的Gamma，基于2面
	complex<double>tao;//反射系数
	complex<double>K;
	complex<double>T;//透射系数
	complex<double>Eps;//相对介电系数
	complex<double>Z;//阻抗

	DOUBLE Eps_r;//介电系数实部
	DOUBLE Eps_i;//虚部

public:
	CMyListCtrl m_list;
	DOUBLE *array_f = NULL;//用于传递结果的动态数组，频率
	DOUBLE *array_er = NULL;//动态数组，介电系数实部
	DOUBLE *array_ei = NULL;//虚部
	DOUBLE *g1 = NULL;
	DOUBLE *g2 = NULL;//动态数组，无物理含义，中间补偿量，计算中迭代赋值
	DOUBLE *f = NULL;
	complex<double> *S11 = NULL;
	complex<double> *S21 = NULL;
	complex<double> *S12 = NULL;
	complex<double> *S22 = NULL;

	DOUBLE a_value = 10.668;//mm为单位的波导宽度
	DOUBLE d_value;//mm为单位的材料厚度
	DOUBLE l1_value;//mm为单位的左夹具间隙
	DOUBLE l2_value;//mm为单位的右夹具间隙

	void Kz(INT i,INT mode);
	complex<double> NRW2(INT i,INT mode);
	DOUBLE NRW3(INT i,INT mode);
	BOOL Alg1_T_gam();
	BOOL Alg2_Gamma_Z();
	BOOL Svalue_BuildandCorrect();
public:
	CEdit m_filepath;
	CRect list_rect;
	INT alg = 1;
	CMyComboBox m_AlgChoosing;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButtonCal();
public:
	BOOL value_set = 0;
	void UpdateButton(BOOL value_set);

public:
	void ReSize(void);
	POINT old;
	virtual BOOL OnInitDialog();
	CStatic m_static_alg;
	CStatic m_static_filepath;
	CButton m_button_caljdcs;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
