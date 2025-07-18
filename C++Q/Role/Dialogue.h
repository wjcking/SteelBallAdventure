#pragma once
#include "../Utils/GameScriptor.h"
#include "../Utils/Constant.h"
#include "../../Common/Misc/FrameCounter.h"
using namespace std;
#define TYPE_DIALOG Typewriter::getInstance()
class Typewriter
{
private:
	DelayPhase delayType;
	unsigned int  pos = 0;
	std::string content;
public:

	//在语言切换的时候会出现打字机速度不一致的bug，先设置一个折中速度
	Typewriter() :delayType(0.065f) {};
	static Typewriter* getInstance()
	{
		static Typewriter typewriter;
		return &typewriter;
	};
	inline void setDelay(const float& delay) { delayType.delaySecond = delay; }
	inline void setContent(const std::string& val) 
	{
		//根据语言不同初始化打印播放速度
		delayType.delaySecond = UserDefault::getInstance()->getIntegerForKey(User_Language, Lang_Chinwan) == 1 ? 0.12f : 0.05f; 
		content = val;
	}
	inline void reset() { pos = 0; }
	inline bool isDone(const bool& isClear = true)
	{
		if (pos < content.length())
		{
			//直接全部读取完毕
			if (isClear)
				pos = content.length();
			return false;
		}

		return true;
	}
	std::string getTyped()
	{
		if (pos >= content.length() || content.length() == 0)
		{
			return content;
		}

		if (delayType.isTimeUp())
		{
			delayType.reset();
			//中英文判断
			pos += (content[pos] > -127 && content[pos] < 0) ? 3 : 1;
			return content.substr(0, pos);
		}

		return content.substr(0, pos);
	}
};
struct HintText
{
	short zorder = Z_ROLE;
	short index = 0;
	int tag = 0;
	float delay = 3.f;
	bool hasChoices = false;
	float scaleWidth = 20.f;
	std::string text;
	FontInfo font;
	void initScaleWith(const float& = 0);
	inline std::string getName() const { return cocos2d::StringUtils::format("dialog%d", index); }
};
 
struct DialogueText : HintText
{
	std::string	choices[4];
	cocos2d::Menu* getChoices();
	void choiceCallback(cocos2d::Ref* pSender);
	inline bool isAnswered() { return LUAH->getGlobal(Luat_Dialogue)[index].value().has(Luaf_Key); }
};

#define DIALOGUE Dialogue::getInstance()
class Dialogue
{
private:
	//RObject
	//unordered_map<int, HintText> robjectMap;
	unordered_map<int, vector<HintText>>  hintMap;
	unordered_map<int, unsigned int> hintIter;
	unordered_map<int, DelayPhase>   delayHint;
	vector<DialogueText> dialogueList;
	DialogueText defaultDialogue;
	unsigned int dialogIndex = 0;
	unsigned int dialogEnd = 0;
	unsigned int previousIndex = 0;
public:
	Dialogue();
	static Dialogue* getInstance()
	{
		static Dialogue instance;
		return &instance;
	}
	void loadScript();
	//单独共荧幕文字使用
	void loadCurtainText();
	//gcc++ 引用编译有问题  error: invalid initialization of reference of type 'DialogueText&' from expression of type 'const value_type {aka const DialogueText}'
	DialogueText& getCurrentDialog();
	HintText fetchHint(const int&);
	//设置分段对话，默认从头到尾
	inline void setSegment(const unsigned int& start, const unsigned int& end)
	{
		dialogIndex = start > end ? end : start;
		dialogEnd = end < start ? start : end;
	}
	inline void fetchNext()
	{
		previousIndex = dialogIndex;
		if (dialogIndex < dialogEnd)
			dialogIndex++;
		else
			dialogIndex = dialogEnd;

	}
	//inline DialogueText& getPreviousDialog() { return dialogueList[previousIndex]; }
	inline void resetHint(const int& tag)
	{
		hintIter[tag] = 0;
		delayHint[tag].reset();
	}
	inline bool isDone() 
	{ 
		return dialogIndex == dialogEnd;
	};

};