#include "pch.h"
#include "CMyComboBox.h"
BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CMyComboBox::OnCbnDropdown)
END_MESSAGE_MAP()


void CMyComboBox::OnCbnDropdown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRect rc;
	GetClientRect(&rc); //��ȡ��ǰ������Ĵ�С
	SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height() + 100, SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);
}
