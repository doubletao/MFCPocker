#include "StdAfx.h"
#include "IDrawable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IDrawable::IDrawable(void)
	: m_bDrawBackground(FALSE)
	, m_nBackColor(0)
	, m_bDrawBound(FALSE)
	, m_nBoundColor(0)
	, m_nBoundWidth(0)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_ptPosition(0, 0)
{
}


IDrawable::~IDrawable(void)
{
}

void IDrawable::DrawTo(CDC * pDC)
{
	CRect rtRegion = GetRegion();
	int nBackup = pDC->SaveDC();

	//ÉèÖÃ»­±Ê»­Ë¢
	HGDIOBJ hOldPen = 0;
	HGDIOBJ hOldBrush = 0;
	CPen pen;
	CBrush brush;
	if (m_bDrawBound)
	{
		pen.CreatePen(PS_SOLID, m_nBoundWidth, m_nBoundColor);
		hOldPen = pDC->SelectObject(pen);
	}
	if (m_bDrawBackground)
	{
		brush.CreateSolidBrush(m_nBackColor);
		hOldBrush = pDC->SelectObject(brush);
	}

	pDC->Rectangle(rtRegion);

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

void IDrawable::SetBound(CRect rtBound)
{
	m_nWidth = rtBound.Width();
	m_nHeight = rtBound.Height();
	m_ptPosition = rtBound.TopLeft();
}

void IDrawable::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
}

void IDrawable::SetHeight(int nHeight)
{
	m_nHeight = nHeight;
}

void IDrawable::SetPosition(CPoint ptPos)
{
	m_ptPosition = ptPos;
}

int IDrawable::GetWidth()
{
	return m_nWidth;
}

int IDrawable::GetHeight()
{
	return m_nHeight;
}

CPoint IDrawable::GetPosition()
{
	return m_ptPosition;
}

CRect IDrawable::GetRegion()
{
	CRect rtRegion(0, 0, m_nWidth, m_nHeight);
	rtRegion.OffsetRect(m_ptPosition);
	return rtRegion;
}
