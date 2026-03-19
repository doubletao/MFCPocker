#pragma once
#include "core/interfaces/ICard.h"

const int g_nCardDispModel = 15;
extern const QString g_strCardDisp[];

class PokerCard
    : public ICard
{
public:
    PokerCard(void);
    virtual ~PokerCard(void);
    QString GetDisplayTxt();
    virtual int GetSortFactor() const { return m_nValue; }
    virtual void DrawTo(QPainter *painter);
    static bool ValidValue(int nValue);
    virtual bool HitTest(QPoint pt);
};
