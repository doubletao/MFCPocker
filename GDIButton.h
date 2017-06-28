#pragma once
//////////////////////////////////////////////////////////////////////////
//��GDI��������button
//////////////////////////////////////////////////////////////////////////
#include "IDrawable.h"
#include "IClickable.h"

enum EMButtonID
{
	EM_PASS_SCORE_BUTTON,	//����
	EM_ONE_SCORE_BUTTON,	//һ��
	EM_TWO_SCORE_BUTTON,	//����
	EM_THREE_SCORE_BUTTON,	//����

	EM_PASS_CARD_BUTTON,	//��Ҫ
	EM_SEND_CARD_BUTTON,	//����
};

const int g_nButtonWidth = 70;
const int g_nButtonHeight = 25;
const int g_nButtonGap = 5;
class GDIButton
	: public IDrawable
	, public IClickable
{
private:
	CString m_strBtnTxt;//��ť�ı�
	BOOL m_bShow;//�Ƿ���ʾ
public:
	GDIButton(void);
	~GDIButton(void);
	virtual BOOL HitTest(CPoint pt);//����ӿ�
	virtual void DrawTo(CDC * pDC);//�����߼�
	void SetButtonTxt(CString strBtnTxt){m_strBtnTxt = strBtnTxt;}//���ð�ť�ı�
	void ShowButton(BOOL bShow){m_bShow = bShow;}
	BOOL IsShow(){return m_bShow;}//�����Ƿ�����ʾ״̬
};

