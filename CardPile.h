#pragma once
//////////////////////////////////////////////////////////////////////////
//牌堆，代表一堆牌
//可排序，可乱序
//////////////////////////////////////////////////////////////////////////
#include "ICard.h"
#include "IDrawable.h"
#include <vector>
using std::vector;

const int g_nDefaultCardGap = 20;

class CardGroup;
class CardPile : public IDrawable
{
protected:
	vector<ICard *> m_vecCard;//卡牌集合
	BOOL m_bShowValue;//是否展示牌面值
	vector<int> m_vecOnHoldCard;//被选中的牌序号集
public:
	CardPile(void);
	CardPile(const vector<ICard *> & vecValue);
	~CardPile(void);
	void ShowValue(BOOL bShowValue = TRUE){m_bShowValue = bShowValue;}//是否显示牌面
	void SortCard(BOOL bDec = TRUE);//对卡牌排序，默认降序
	void RandonSortCard();//对卡牌乱序（洗牌）
	void InsertCard(ICard * pCard, int nPos = 0);//在指定位置插入一张牌
	void PushBackCard(ICard * pCard);//在队尾插入一张牌
	ICard * GetCard(int nIndex);//获得指定位置牌
	vector<ICard *> & GetCardPile();//获得牌堆
	void DrawTo(CDC * pDC);
	ICard * PopCard();//弹出一张牌
	CSize GetCardSize();//计算出当前卡牌所占宽高
	void SetOnHoldCard(vector<int> vecOnHoldCard){m_vecOnHoldCard = vecOnHoldCard;}//设置被选中的牌集合
	vector<int> & GetOnHoldCard(){return m_vecOnHoldCard;}//获取被选中的牌序号集合
	CardGroup GetOnHoldCardPile();//根据选中牌序号获取选中牌
	CardGroup PlayOutCard();//打出选中牌
	BOOL CheckOnHoldCard(int nIndex);//判定一张牌是否被选中
	void HoldOneCard(DWORD dwID);//选中或取消选中一张牌
};
