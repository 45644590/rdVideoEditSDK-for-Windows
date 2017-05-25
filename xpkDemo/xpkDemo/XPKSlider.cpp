// XPKSlider.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "xpkDemo.h"
#include "XPKSlider.h"


// CXPKSlider

IMPLEMENT_DYNAMIC(CXPKSlider, CSliderCtrl)

CXPKSlider::CXPKSlider()
{
	seek = 0;
}

CXPKSlider::~CXPKSlider()
{
}


BEGIN_MESSAGE_MAP(CXPKSlider, CSliderCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CXPKSlider ��Ϣ�������



void CXPKSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
#if 0
	CRect mChannel;
	int mMinRg,mMaxRg;

	GetChannelRect(&mChannel);
	GetRange(mMinRg, mMaxRg);

	double posRate = (point.x - mChannel.left)/ (double)mChannel.Width();
	int newPos = (mMaxRg - mMinRg) * posRate;
	SetPos(newPos);
	::SendMessage(this->GetParent()->m_hWnd,WM_USER+2,1,NULL);

#else

	CRect tRect;
	GetThumbRect(&tRect);
	if (tRect.PtInRect(point))
	{
		CSliderCtrl::OnLButtonDown(nFlags, point);
		return;
	}
	// ʵ�ֻ����龫ȷ��λ   
	CRect rcSlider;
	GetChannelRect(&rcSlider);
	int nMax = GetRangeMax();
	float fInterval = float(rcSlider.Width())/nMax;
	float fPos = (point.x-rcSlider.left+fInterval-1)/fInterval;

	seek = 1;
	CSliderCtrl::SetPos((int)fPos);
	
#endif
	CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CXPKSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	::SendMessage(this->GetParent()->m_hWnd,WM_USER+1,1,NULL);
	CSliderCtrl::OnLButtonUp(nFlags, point);
}
