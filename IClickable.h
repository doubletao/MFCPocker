#pragma once
//////////////////////////////////////////////////////////////////////////
//����ӿڣ�ʵ�ֱ��ӿڿ����ڱ���������ָ���ص�
//////////////////////////////////////////////////////////////////////////
typedef LRESULT (* ClickCallBackFun)(LPARAM lParam);

class IClickable
{
protected:
	DWORD m_dwID;
	ClickCallBackFun m_pCallBackFun;
public:
	IClickable(void);
	virtual ~IClickable(void);
	virtual BOOL HitTest(CPoint pt) = 0;//����ӿ�
	void SetCallBackFunc(ClickCallBackFun pCallBackFun){m_pCallBackFun = pCallBackFun;}
	void SetID(DWORD dwID){m_dwID = dwID;}
	DWORD GetID(){return m_dwID;}
};

