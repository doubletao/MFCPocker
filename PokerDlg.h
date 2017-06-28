
// PokerDlg.h : 头文件
//
#pragma once
#include "GDIButton.h"
#include <map>
using std::map;

const int MSG_REFRESH_VIEW = WM_USER + 1;//刷新消息
const int MSG_SHOW_CLO_BTN = WM_USER + 2;//显示叫地主按钮,wParam为显示还是取消显示，LOW(lParam)为是否显示不叫，HIGH(lParam)为当前场上的最高分
const int MSG_SHOW_CP_BTN = WM_USER + 3;//显示出牌按钮，wParam为显示还是取消显示，lParam为是否显示pass

// CPokerDlg 对话框
class CPokerDlg : public CDialogEx
{
// 构造
public:
	CPokerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_POKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CMenu m_Menu;
	map<DWORD, GDIButton> m_mapButton;//按钮集合
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void InitialBtnManager();//初始化按钮管理器
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRestart();
	afx_msg void OnAbout();
	afx_msg LRESULT OnRefreshView(WPARAM wParam, LPARAM lParam);//刷新视图
	afx_msg LRESULT OnShowCloBtn(WPARAM wParam, LPARAM lParam);//收到显示叫地主按钮消息
	afx_msg LRESULT OnShowCpBtn(WPARAM wParam, LPARAM lParam);//收到显示出牌按钮消息
	static LRESULT OnButtonClicked(LPARAM lParam);//有按钮被按下
	static LRESULT OnCardClicked(LPARAM lParam);//有牌被点中
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//处理点击动作
};
