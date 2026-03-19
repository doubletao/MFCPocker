#pragma once
//////////////////////////////////////////////////////////////////////////
// 绘图接口
// 默认实现默认矩形的背景绘制
//////////////////////////////////////////////////////////////////////////

#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QColor>

// 颜色定义
namespace ColorDefine {
    const QColor WholeBound = QColor(0, 90, 0);           // 整体显示边框颜色
    const QColor WholeBack = QColor(0, 90, 0);             // 整体显示背景颜色
    const QColor PlayerBound = QColor(0, 125, 20);         // 玩家显示边框颜色
    const QColor PlayerBack = QColor(0, 50, 50);           // 玩家显示背景颜色
    const QColor PlayerHeadBound = QColor(125, 125, 125);  // 玩家头像边框颜色
    const QColor PlayerHeadBack = QColor(50, 50, 50);      // 玩家头像背景颜色
    const QColor PlayerCardBound = QColor(200, 200, 200);  // 玩家手牌边框颜色
    const QColor PlayerCardBack = QColor(150, 150, 150);   // 玩家手牌背景颜色
    const QColor PokerCardBound = QColor(0, 50, 50);       // 扑克牌边框颜色
    const QColor PokerCardBack = QColor(0, 200, 50);       // 扑克牌背景颜色
    const QColor PokerCardValueBound = QColor(125, 125, 150);  // 牌值边框颜色
    const QColor PokerCardValueBack = QColor(255, 255, 255);   // 牌值背景颜色
    const QColor ButtonBound = QColor(200, 200, 200);      // 按钮边框颜色
    const QColor ButtonBack = QColor(200, 200, 200);       // 按钮背景颜色
};

class IDrawable
{
protected:
    int m_nWidth;      // 矩形宽度
    int m_nHeight;     // 矩形高度
    QPoint m_ptPosition;  // 矩形位置
    // 背景
    bool m_bDrawBackground;
    QColor m_nBackColor;
    // 边框
    bool m_bDrawBound;
    QColor m_nBoundColor;
    int m_nBoundWidth;
public:
    IDrawable(void);
    virtual ~IDrawable(void) = 0;
    virtual void DrawTo(QPainter *painter) = 0;  // 绘制接口
    void SetBound(QRect rtBound);
    void SetWidth(int nWidth);
    void SetHeight(int nHeight);
    void SetPosition(QPoint ptPos);
    int GetWidth();
    int GetHeight();
    QPoint GetPosition();
    QRect GetRegion();
    void ShowBackground(bool bEnabled = true) { m_bDrawBackground = bEnabled; }
    void SetBackColor(QColor nColor) { m_nBackColor = nColor; }
    void ShowBound(bool bEnabled = true) { m_bDrawBound = bEnabled; }
    void SetBoundColor(QColor nColor) { m_nBoundColor = nColor; }
    void SetBoundWidth(int nWidth) { m_nBoundWidth = nWidth; }
};
