#include "pch.h"
#include "CMyListCtrl.h"


BOOL CMyListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;
	switch (((NMHDR*)lParam)->code)
	{
	case HDN_BEGINTRACKW://�϶�  
	case HDN_BEGINTRACKA:
	case HDN_DIVIDERDBLCLICKA:
	case HDN_DIVIDERDBLCLICKW:
		int n;
		for (n = 0; n < nCol; n++)
		{
			if (pHDN->iItem == n)  // Col���趨Ϊ�Լ�����ı����ֵ������Col=0�����ǵ�1��  
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
