#pragma once
//////////////////////////////////////////////////////////////////////////
//���飬����һ����
//����ĸ���Ϊһ�����һ��ʹ�õ��ƣ����綷�����еĶ��ӣ��������˶�����ը���ȶ�������Ϊ���飬����Ҳ������Ϊ��һ������
//������ݾ�����������Ͳ�ͬҲ���ڿɱ���
//////////////////////////////////////////////////////////////////////////
#include "CardPile.h"
//���ţ����ӣ�������������һ�����������Ĵ�����˳�ӣ����ԣ��ɻ���ը������ը
enum EMCardGroupType
{
	EM_Single_CardGroup, //����
	EM_Couple_CardGroup, //����
	EM_TripleWithout_CardGroup, //������
	EM_TripleWithOne_CardGroup, //����һ
	EM_TripleWithCouple_CardGroup, //������
	EM_FourWithCouple_CardGroup, //�Ĵ���
	EM_MoveLine_CardGroup, //˳��
	EM_CoupleMoveLine_CardGroup, //����
	EM_PlaneWithout_CardGroup, //��˷ɻ�
	EM_PlaneWithone_CardGroup, //���ŷɻ�
	EM_PlaneWithCouple_CardGroup, //���ӷɻ�
	EM_Bomb_CardGroup, //ը��
	EM_JokingBomb_CardGroup, //��ը
	EM_CardGroup_Illegal,//�Ƿ�����
};

const CString g_strType[] = {_T("����"), _T("����"), _T("������"), _T("����һ"), _T("������"),
	_T("�Ĵ���"), _T("˳��"), _T("����"), _T("��˷ɻ�"), _T("���ŷɻ�"),
	_T("���ӷɻ�"), _T("ը��"), _T("��ը"), _T("���ж����Һ����ˣ��Ų��ţ�"),};

//�˶�������������ƣ�Ĭ��5��3��ã�
const int g_nMoveLineCardCountLimit = 5;
const int g_nMoveCoupleCardCountLimit = 3;

class CardGroup : public CardPile
{
public:
	CardGroup(void);
	virtual ~CardGroup(void);
	//����У�麯����У������type�Լ��Ƿ��ܹ�ס����(�ڲ���Ե�ǰ�����ƽ�������)
	EMCardGroupType CheckPile(CardGroup * pCurPile = NULL);//���������У�飬����Ϊ��ʱ����Ϸ��򷵻����ͣ����򷵻طǷ���������Ϊ��ʱ�������ǰ��ѹס�������飬�򷵻����ͣ����򷵻طǷ�
};
