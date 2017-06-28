#pragma once
//////////////////////////////////////////////////////////////////////////
//���ƽӿ�
//Ĭ����ʵ���˱��׼��߽�Ļ���
//////////////////////////////////////////////////////////////////////////

//��ɫ����
enum EMColorDefine
{
	EM_Color_Whole_Bound = RGB(0, 90, 0),					//������ʾ���߿�ɫ
	EM_Color_Whole_Back = RGB(0, 90, 0),					//������ʾ������ɫ
	EM_Color_Player_Bound = RGB(0, 125, 20),					//�����ʾ���߿�ɫ
	EM_Color_Player_Back = RGB(0, 50, 50),						//�����ʾ������ɫ
	EM_Color_Player_Head_Bound = RGB(125, 125, 125),			//���ͷ��߿�ɫ
	EM_Color_Player_Head_Back = RGB(50, 50, 50),				//���ͷ�񱳾�ɫ
	EM_Color_Player_Card_Bound = RGB(200, 200, 200),			//���ֽ�����߿�ɫ
	EM_Color_Player_Card_Back = RGB(150, 150, 150),				//���ֽ��������ɫ
	EM_Color_PokerCard_Bound = RGB(0, 50, 50),					//ֽ�Ʊ���߿���ɫ
	EM_Color_PokerCard_Back = RGB(0, 200, 50),					//ֽ�Ʊ��汳����ɫ
	EM_Color_PokerCard_Value_Bound = RGB(125, 125, 150),		//ֽ������߿���ɫ
	EM_Color_PokerCard_Value_Back = RGB(255, 255, 255),			//ֽ�����汳����ɫ
	EM_Color_Button_Bound = RGB(200, 200, 200),					//��ť�߿���ɫ
	EM_Color_Button_Back = RGB(200, 200, 200),					//��ť������ɫ
};

class IDrawable
{
protected:
	int m_nWidth;//��������
	int m_nHeight;//��������
	CPoint m_ptPosition;//������λ��
	//����
	BOOL m_bDrawBackground;
	COLORREF m_nBackColor;
	//�߽�
	BOOL m_bDrawBound;
	COLORREF m_nBoundColor;
	int m_nBoundWidth;
public:
	IDrawable(void);
	virtual ~IDrawable(void) = 0;
	virtual void DrawTo(CDC * pDC) = 0;//�����߼�
	void SetBound(CRect rtBound);
	void SetWidth(int nWidth);
	void SetHeight(int nHeight);
	void SetPosition(CPoint ptPos);
	int GetWidth();
	int GetHeight();
	CPoint GetPosition();
	CRect GetRegion();
	void ShowBackground(BOOL bEnabled = TRUE){m_bDrawBackground = bEnabled;}
	void SetBackColor(COLORREF nColor){m_nBackColor = nColor;}
	void ShowBound(BOOL bEnabled = TRUE){m_bDrawBound = bEnabled;}
	void SetBoundColor(COLORREF nColor){m_nBoundColor = nColor;}
	void SetBoundWidth(int nWidth){m_nBoundWidth = nWidth;}
};
