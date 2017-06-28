#pragma once
//////////////////////////////////////////////////////////////////////////
//�ƵĻ��࣬����һ����
//////////////////////////////////////////////////////////////////////////
#include "IDrawable.h"
#include "ISortable.h"
#include "IClickable.h"

const int g_nCardWidth = 80;
const int g_nCardHeight = 100;

class ICard
	: public IDrawable
	, public ISortable
	, public IClickable
{
protected:
	BOOL m_bShowValue;
	UINT m_nValue;
	BOOL m_bSelect;
public:
	ICard(void);
	virtual ~ICard(void);
	void SetValue(UINT nValue){m_nValue = nValue;}//������ֵ
	UINT GetValue(){return m_nValue;}//��ȡ��ֵ
	void ShowValue(BOOL bShowValue = TRUE){m_bShowValue = bShowValue;}//�Ƿ���ʾ����
	virtual int GetSortFactor() const = 0;//��ȡ��������
	virtual void DrawTo(CDC * pDC) = 0;//�����߼�
	void SelectCard(BOOL bSet);//�Կ���ȡ��ɫ
};
