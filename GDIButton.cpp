#include "StdAfx.h"
#include "GDIButton.h"

GDIButton::GDIButton( void )
	: m_bShow(FALSE)
{
	SetWidth(g_nButtonWidth);
	SetHeight(g_nButtonHeight);
	ShowBound(TRUE);
	SetBoundColor(EM_Color_Button_Bound);
	SetBoundWidth(2);
	ShowBackground(TRUE);
	SetBackColor(EM_Color_Button_Back);
}

GDIButton::~GDIButton( void )
{

}


BOOL GDIButton::HitTest( CPoint pt )
{
	if (m_bShow)
	{
		CRect rtRegion = GetRegion();
		if (rtRegion.PtInRect(pt))
		{
			m_pCallBackFun(m_dwID);
			return TRUE;
		}
	}
	return FALSE;
}

void GDIButton::DrawTo( CDC * pDC )
{
	if (m_bShow)
	{
		IDrawable::DrawTo(pDC);
		pDC->DrawText(m_strBtnTxt, GetRegion(), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}
