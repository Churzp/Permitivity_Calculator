#pragma once
#include "CListCtrlEx.h"
class CMyListCtrl :
	public CListCtrlEx
{
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	INT nCol;
	INT nRow;
};

