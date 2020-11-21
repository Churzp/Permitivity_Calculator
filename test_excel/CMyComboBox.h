#pragma once
#include <afxwin.h>
class CMyComboBox :
	public CComboBox
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCbnDropdown();
};

