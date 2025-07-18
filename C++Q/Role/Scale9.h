#pragma once
#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"
#include "Dialogue.h"
USING_NS_CC;
using namespace ui;
class Role;
class DialogBox: Scale9Sprite
{
private:
	Label* label;
	static std::vector<DialogueText> dialogueList;
public:
	static void pushDialogue(const DialogueText&);
	bool display(); 
};