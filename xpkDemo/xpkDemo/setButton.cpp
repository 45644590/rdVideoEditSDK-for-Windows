// setButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "setButton.h"
#include "XPKAddMedia.h"



// CSetButton

IMPLEMENT_DYNAMIC(CSetButton, CButton)

CSetButton::CSetButton()
{

}

CSetButton::~CSetButton()
{
}


BEGIN_MESSAGE_MAP(CSetButton, CButton)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CSetButton ��Ϣ�������



void CSetButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int id = GetDlgCtrlID() - RESOURCE_ID*2;
	
	CXPKAddMedia* dlg = (CXPKAddMedia*)GetParent();
	dlg->showAdvanceDlg(RESOURCE_ID*3+id);

	CButton::OnLButtonDown(nFlags, point);
}
