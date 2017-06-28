#pragma once
//////////////////////////////////////////////////////////////////////////
//点击接口，实现本接口可以在被点击后调用指定回调
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
	virtual BOOL HitTest(CPoint pt) = 0;//点击接口
	void SetCallBackFunc(ClickCallBackFun pCallBackFun){m_pCallBackFun = pCallBackFun;}
	void SetID(DWORD dwID){m_dwID = dwID;}
	DWORD GetID(){return m_dwID;}
};

