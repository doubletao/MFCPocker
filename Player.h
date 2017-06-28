#pragma once
//////////////////////////////////////////////////////////////////////////
//玩家
//////////////////////////////////////////////////////////////////////////
#include "CardPile.h"
#include "IDrawable.h"
#include "PokerCard.h"
#include "CardGroup.h"

enum EMPlayerType
{
	EM_Left_Player, //上家
	EM_Mid_Player,	//玩家
	EM_Right_Player,	//下家
};

const int g_nHeadWidth = 50;
const int g_nHeadHeight = 50;
const int g_nGap = 5;

class Player : public IDrawable
{
private:
	EMPlayerType m_emPlayerType;//玩家类型（决定绘制方位）
	CString m_strID;//玩家的唯一标识符
	CString m_strName;//玩家的名字
	BOOL m_bShowValue;//绘制时展示牌面
	CardPile m_stuHandCard;//玩家手牌
	BOOL m_bLandOwerCardHolder;//地主牌得主（只是拿到了地主牌，叫分询问时第一个问他，并不一定就是地主）
	BOOL m_bLandOwer;//地主（这个才是地主身份标记）
	int m_nLandOwerScore;//叫地主的分数
	CString m_strWord;
protected:
	void DrawPlayerHead(CDC * pDC, CRect rtBound);//绘制头像
	void DrawCard(CDC * pDC, CRect rtBound);//绘制纸牌
public:
	Player(void);
	~Player(void);
	void ShowValue(BOOL bShowValue = TRUE){m_bShowValue = bShowValue;}//是否显示牌面
	void SetName(CString str){m_strName = str;}
	void SetPlayerType(EMPlayerType emPlayerType){m_emPlayerType = emPlayerType;}
	EMPlayerType GetPlayerType(){return m_emPlayerType;}
	virtual void DrawTo(CDC * pDC);
	void PushBackCard(ICard * pCard){m_stuHandCard.PushBackCard(pCard);}//在队尾插入一张牌
	CardPile & GetHandCard(){return m_stuHandCard;}//获取手牌牌堆
	vector<int> & GetOnHoldCard(){return m_stuHandCard.GetOnHoldCard();}//获取选中牌序号
	void SetOnHoldCard(vector<int> vecOnHoldCard){m_stuHandCard.SetOnHoldCard(vecOnHoldCard);}//设置选中牌序号
	CardGroup GetOnHoldCardGroup(){return m_stuHandCard.GetOnHoldCardPile();}//根据选中牌序号获取选中牌
	CardGroup PlayOutCard(){return m_stuHandCard.PlayOutCard();}//打出选中牌
	void SetLandOwer(BOOL bLandOwer){m_bLandOwer = bLandOwer;}
	BOOL IsLandOwer(){return m_bLandOwer;}
	void SetLandOwerCardHolder(BOOL bLandOwerCardHolder){m_bLandOwerCardHolder = bLandOwerCardHolder;}
	BOOL IsLandOwerCardHolder(){return m_bLandOwerCardHolder;}
	CString GetID(){return m_strID;}
	UINT GetCardCount(){return m_stuHandCard.GetCardPile().size();}//返回持有牌的数量
	void ExcuteCallLandOwer();//执行叫地主逻辑
	void ExcuteCallCardPlay();//执行出牌逻辑
	BOOL IsAIPlayer(){return m_emPlayerType != EM_Mid_Player;}
	void Say(CString strWord){m_strWord = strWord;}//玩家说话
	void SetLandOwerScore(int nScore){m_nLandOwerScore = nScore;}
	int GetLandOwerScore(){return m_nLandOwerScore;}
};
