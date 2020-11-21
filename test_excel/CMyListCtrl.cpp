#include "pch.h"
#include "CMyListCtrl.h"


BOOL CMyListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;
	switch (((NMHDR*)lParam)->code)
	{
	case HDN_BEGINTRACKW://拖动  
	case HDN_BEGINTRACKA:
	case HDN_DIVIDERDBLCLICKA:
	case HDN_DIVIDERDBLCLICKW:
		int n;
		for (n = 0; n < nCol; n++)
		{
			if (pHDN->iItem == n)  // Col—设定为自己不想改变的列值，比如Col=0，就是第1列  
			{
				*pResult = TRUE;
				return TRUE;
			}
		}
		break;
	default:
		break;
	}
	return CListCtrlEx::OnNotify(wParam, lParam, pResult);
}
