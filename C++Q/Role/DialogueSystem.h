#pragma once
#include "DialogueSystem.h"
class DialogueSystem
{
private:
	//用来判断是否到最后一句对话
	static bool isWaitingForProcced;
	static bool isSegmentDone();
public:
	DialogueSystem();
	~DialogueSystem();
	static bool isDialogueOn; 
	static void update();
	static void proceed();
	static void close();
	static void setIndex(const unsigned int& start, const unsigned int& end);
	static bool isDone();

};

