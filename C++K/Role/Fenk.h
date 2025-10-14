#pragma once
#include "cocos2d.h"
#include <string>
#include "../Utils/Fand.h"
 
using namespace cocos2d;

enum OppositePopup
{
	popNothing,
	popMoney,
	popDrug,
};

struct Fenk
{
	//���ֻ����һ�Σ����ж��Ƿ��ƹ�
	short KnockMax = 12;
	//�������˶��ٴ�
	short knockedTimes =0;
	//�ƶ�����������
	Vec2 pushSteps = Vec2(1.5f, 1.5f);
	//�Ƿ�����˺�
	bool isHarmful = false;
	//��ײ���Ƿ���ʾ��
	bool isShowThorn = true;
	//�Ƿ�����������
	bool allowPush = false;
	//������ײ�������������ɫֱ�ӹ�ȥ
	bool allowCollision= true;
	//�Ƿ��Ѿ���ײ
	bool isCollided= false;
	//�����������ʲô����
	OppositePopup oppositePopup = OppositePopup::popNothing; 
	//�Ƿ�����һ������ײ,��Ҫ������
	bool getAllowKnock()
	{
		//1 ���������ײ��
		if (!allowCollision)	return false;
		//2 ��ײ����
		if ( knockedTimes >= KnockMax)	return false;
		 
		return true;
	}
};
