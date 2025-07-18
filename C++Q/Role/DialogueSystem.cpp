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
	//隐藏所有对话框
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
	//[打字机]触摸结束后判断是否移动到下一个对话
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

//对话从哪儿开始从哪里结束
//放在lua enter函数里面
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
	//最后一句话的时候需要停顿，但不需要判断打字是否完成
	if (DIALOGUE->isDone() && !isWaitingForProcced)
	{
		isDialogueOn = false;
		return true;
	}

	return false;
}
//放在exit里面
void DialogueSystem::close()
{ 
	ROLE_MANAGER->process([](Role& role) {
		role.closeDialog();
	});
}