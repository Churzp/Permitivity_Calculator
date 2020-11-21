#include "pch.h"
#include "CMyComboBox.h"
BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CMyComboBox::OnCbnDropdown)
END_MESSAGE_MAP()


void CMyComboBox::OnCbnDropdown()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	GetClientRect(&rc); //获取当前下拉框的大小
	SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height() + 100, SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);
}
