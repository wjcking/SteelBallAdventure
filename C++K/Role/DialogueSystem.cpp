#include "DialogueSystem.h"
#include "Dialogue.h"
#include "Role.h"
#include "RoleManager.h"
#include "../Utils/ResourceHelper.h"
bool DialogueSystem::isDialogueOn = false;
bool DialogueSystem::isWaitingForProcced = false;

DialogueSystem::DialogueSystem()
{
}

DialogueSystem::~DialogueSystem()
{

}
bool DialogueSystem::isSegmentDone()
{
	return DIALOGUE->isDone() && TYPE_DIALOG->isDone(false);
}
void DialogueSystem::update()
{

	auto thread = DIALOGUE->getCurrentDialog();

	if (thread.tag == 0)
		return;
	//�������жԻ���
	close();
	auto role = ROLE_MANAGER->getRoleByTag(thread.tag, false);

	if (nullptr == role)
	{
		close();
		return;
	}
	thread.text = TYPE_DIALOG->getTyped();
	role->openDialogBox(thread);

	//if (isWaitingForProcced)
	//	return;

	//if (isSegmentDone())
	//	isWaitingForProcced = true;

}

void DialogueSystem::proceed()
{
	//[���ֻ�]�����������ж��Ƿ��ƶ�����һ���Ի�
	if (!isDialogueOn)
		return;
	 
	isWaitingForProcced = isSegmentDone() ? false : true;
	
		Resh::playSound("sound/proceed.wav");
	if (TYPE_DIALOG->isDone())
	{
		DIALOGUE->fetchNext();
		TYPE_DIALOG->reset();
		TYPE_DIALOG->setContent(DIALOGUE->getCurrentDialog().text);
	}
}

//�Ի����Ķ���ʼ���������
//����lua enter��������
void DialogueSystem::setIndex(const unsigned int& start, const unsigned int& end)
{
	isWaitingForProcced = false;
	isDialogueOn = true;
	DIALOGUE->setSegment(start, end);
	TYPE_DIALOG->setContent(DIALOGUE->getCurrentDialog().text);
	Resh::playSound("sound/question.mp3");
}
bool DialogueSystem::isDone()
{
	//���һ�仰��ʱ����Ҫͣ�٣�������Ҫ�жϴ����Ƿ����
	if (DIALOGUE->isDone() && !isWaitingForProcced)
	{
		isDialogueOn = false;
		return true;
	}

	return false;
}
//����exit����
void DialogueSystem::close()
{ 
	ROLE_MANAGER->process([](Role& role) {
		role.closeDialog();
	});
}