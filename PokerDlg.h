
// PokerDlg.h : ͷ�ļ�
//
#pragma once
#include "GDIButton.h"
#include <map>
using std::map;

const int MSG_REFRESH_VIEW = WM_USER + 1;//ˢ����Ϣ
const int MSG_SHOW_CLO_BTN = WM_USER + 2;//��ʾ�е�����ť,wParamΪ��ʾ����ȡ����ʾ��LOW(lParam)Ϊ�Ƿ���ʾ���У�HIGH(lParam)Ϊ��ǰ���ϵ���߷�
const int MSG_SHOW_CP_BTN = WM_USER + 3;//��ʾ���ư�ť��wParamΪ��ʾ����ȡ����ʾ��lParamΪ�Ƿ���ʾpass

// CPokerDlg �Ի���
class CPokerDlg : public CDialogEx
{
// ����
public:
	CPokerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_POKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CMenu m_Menu;
	map<DWORD, GDIButton> m_mapButton;//��ť����
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	void InitialBtnManager();//��ʼ����ť������
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRestart();
	afx_msg void OnAbout();
	afx_msg LRESULT OnRefreshView(WPARAM wParam, LPARAM lParam);//ˢ����ͼ
	afx_msg LRESULT OnShowCloBtn(WPARAM wParam, LPARAM lParam);//�յ���ʾ�е�����ť��Ϣ
	afx_msg LRESULT OnShowCpBtn(WPARAM wParam, LPARAM lParam);//�յ���ʾ���ư�ť��Ϣ
	static LRESULT OnButtonClicked(LPARAM lParam);//�а�ť������
	static LRESULT OnCardClicked(LPARAM lParam);//���Ʊ�����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//����������
};
