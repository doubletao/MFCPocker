#include "StdAfx.h"
#include "PokerCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PokerCard::PokerCard(void)
{
	SetWidth(g_nCardWidth);
	SetHeight(g_nCardHeight);
	ShowBound(TRUE);
	SetBoundColor(EM_Color_PokerCard_Bound);
	SetBoundWidth(2);
	ShowBackground(TRUE);
	SetBackColor(EM_Color_PokerCard_Back);
}


PokerCard::~PokerCard(void)
{
}


CString PokerCard::GetDisplayTxt()
{
	ASSERT(ValidValue(m_nValue));
	return g_strCardDisp[m_nValue];
}

void PokerCard::DrawTo( CDC * pDC )
{
	CRect rtRegion = GetRegion();
	//����Ǳ�ѡ�е��ƣ���������һ��
	if (m_bSelect)
	{
		//���Ƶ����겢���ı䣬����������䶯
		rtRegion.OffsetRect(0, -10);
	}
	if (m_bShowValue)
	{
		//��������
		int nBackup = pDC->SaveDC();

		//���û��ʻ�ˢ
		HGDIOBJ hOldPen = 0;
		HGDIOBJ hOldBrush = 0;
		CPen pen;
		CBrush brush;
		if (m_bDrawBound)
		{
			pen.CreatePen(PS_SOLID, 2, EM_Color_PokerCard_Value_Bound);
			hOldPen = pDC->SelectObject(pen);
		}
		if (m_bDrawBackground)
		{
			brush.CreateSolidBrush(EM_Color_PokerCard_Value_Back);
			hOldBrush = pDC->SelectObject(brush);
		}

		//�Ȼ�������
		pDC->Rectangle(rtRegion);
		//�����ϽǺ����½ǻ����Ʒ�
		CString strTxt = GetDisplayTxt();
		int nWidth = 0;
		int nWidthTmp = 0;
		int nHeight = 0;
		for (int i = 0; i < strTxt.GetLength(); i++)
		{
			CSize sizeTxt = pDC->GetTextExtent(CString(strTxt[i]));
			if (CString(strTxt[i]) == _T("\n"))
			{
				nWidth = nWidthTmp > sizeTxt.cx ? nWidthTmp : sizeTxt.cx;
				nWidthTmp = 0;
			}
			else
			{
				nWidthTmp += sizeTxt.cx;
				nHeight += sizeTxt.cy;
			}
			nWidth = nWidthTmp > nWidth ? nWidthTmp : nWidth;
		}

		int nGap = 2;

		CRect rtRightTop(rtRegion.right - nWidth - nGap, rtRegion.top + nGap, rtRegion.right - nGap, rtRegion.top + nHeight + nGap);
		CRect rtLeftBottom(rtRegion.left + nGap, rtRegion.bottom - nHeight - nGap, rtRegion.left + nWidth + nGap, rtRegion.bottom - nGap);
		pDC->DrawText(strTxt, rtRightTop, 0);
		pDC->DrawText(strTxt, rtLeftBottom, 0);

		if (m_bDrawBound)
		{
			pDC->SelectObject(hOldPen);
		}
		if (m_bDrawBackground)
		{
			pDC->SelectObject(hOldBrush);
		}

		pDC->RestoreDC(nBackup);
	}
	//�������ʾֽ�����棬�����Ƹ������͹���
	else
	{
		ICard::DrawTo(pDC);
	}
}

BOOL PokerCard::ValidValue(int nValue)
{
	if (nValue > -1 && nValue < g_nCardDispModel)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL PokerCard::HitTest( CPoint pt )
{
	if (m_bShowValue)
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
