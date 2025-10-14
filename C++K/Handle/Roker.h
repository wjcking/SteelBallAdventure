#pragma once
#include "cocos2d.h"
#include "Fand.h"
#include "Constant.h"

class FuncMenu : public Menu
{
private:
	static short nextTag;
	Ref* callbackListener;
	SEL_CallFuncN callback;
	void menuCallback(Ref* pSender);
	const char* table;
public:
	virtual bool init() override;
	CREATE_FUNC(FuncMenu);
	void callbackFunc(Ref* target, SEL_CallFuncN callfun);
	void loadScript(const char* padTable);
};