#pragma once
//////////////////////////////////////////////////////////////////////////
// 可点击接口：实现点击接口，用于需要响应鼠标点击的对象
//////////////////////////////////////////////////////////////////////////
#include <QPoint>

class IClickable
{
protected:
    quint32 m_dwID;
public:
    IClickable(void);
    virtual ~IClickable(void);
    virtual bool HitTest(QPoint pt) = 0;  // 点击检测接口
    void SetID(quint32 dwID) { m_dwID = dwID; }
    quint32 GetID() { return m_dwID; }
};
