#include "Dialogue.h"
#include "../Utils/ResourceHelper.h"


Dialogue::Dialogue()
{
}
void Dialogue::loadScript()
{
	int tag = 0;
	dialogueList.clear();
	hintMap.clear();
	//robjectMap.clear();
	dialogIndex = 0;

	//LUAH->processTableFont(Luat_RObjectText, [&](LuaIntf::LuaRef ref, const FontInfo& font) {
	//	//CCASSERT(ref.has(Luaf_Text), "Luat_RObjectText�Ի������ı��ֶ�");
	//	auto tag = LUAH->getRefValue(ref, Luaf_Tag, 0);
	//	if (0 == tag)
	//		return;
	//	if (!ref.has(Luaf_Text))
	//		return;
	//	robjectMap[tag].tag = tag;
	//	robjectMap[tag].font = font;
	//	robjectMap[tag].text = ref[Luaf_Text].value<std::string>();
	//}, false);

	short index = 1;
	LUAH->processTableFont(Luat_Dialogue, [&](LuaIntf::LuaRef ref, const FontInfo& font) {
		if (!ref.has(Luaf_Text))
			return;
		//CCASSERT(ref.has(Luaf_Text), "Luat_Dialogue�Ի������ı��ֶ�");
		tag =  ref.get( Luaf_Tag, 0);
		DialogueText dt;
		dt.index = index++;
		dt.tag = tag;
		dt.font = font;
		dt.text = ref[Luaf_Text].value<std::string>();
		dt.initScaleWith(font.size);
		//if (ref.has(Luaf_Choices))
		//{
		//	dt.hasChoices = true;
		//	dt.choices[0] = ref[Luaf_Choices].value().has(1) ? ref[Luaf_Choices].value()[1].value<std::string>() : "";
		//	dt.choices[1] = ref[Luaf_Choices].value().has(2) ? ref[Luaf_Choices].value()[2].value<std::string>() : "";
		//	dt.choices[2] = ref[Luaf_Choices].value().has(3) ? ref[Luaf_Choices].value()[3].value<std::string>() : "";
		//	dt.choices[3] = ref[Luaf_Choices].value().has(4) ? ref[Luaf_Choices].value()[4].value<std::string>() : "";
		//}
		dialogueList.push_back(dt);
	}, false);
	//��ʼ��segment
	dialogEnd = dialogueList.size() == 0 ? 0 : (dialogueList.size() - 1);
	LUAH->processTableFont(Luat_Hints, [&](LuaIntf::LuaRef ref, const FontInfo& font) {
		tag = ref.get(Luaf_Tag, 0);
		if (0 == tag )
			return;
		if (!ref.has(Luaf_Text))
			return;
		//CCASSERT(ref.has(Luaf_Text), "Luat_Hints�Ի������ı��ֶ�");
		HintText hint;
		hint.tag = tag;
		hint.font = font;
		hint.text = ref[Luaf_Text].value<std::string>();
		hintMap[tag].push_back(hint);
		delayHint[tag].delaySecond = hint.text.length() > 10 ? hint.text.length() * 0.25f : 2.5f;
	}, false);
}

void Dialogue::loadCurtainText()
{
	dialogueList.clear();
	dialogIndex = 0;
	short index = 1;
	LUAH->processTableFont(Luat_CurtainText, [&](LuaIntf::LuaRef ref, const FontInfo& font) {
		if (!ref.has(Luaf_Text))
			return;

		DialogueText dt;
		dt.index = index++;
		dt.font = font;
		dt.text = ref[Luaf_Text].value<std::string>();
		dt.initScaleWith(font.size);

		dialogueList.push_back(dt);
	}, false);
	//��ʼ��segment
	dialogEnd = dialogueList.size() == 0 ? 0 : (dialogueList.size() - 1);
}

DialogueText& Dialogue::getCurrentDialog()
{
	if (dialogIndex < 0 || dialogIndex >= dialogueList.size())
		return defaultDialogue;
	
	return dialogueList[dialogIndex];
}

HintText Dialogue::fetchHint(const int & tag)
{
	auto end = hintMap.end();

	if (hintMap.find(tag) != end)
	{
		//���ֻ��һ��hint��ֱ��
		if (hintMap[tag].size() == 1)
			return hintMap[tag][0];
		else
		{
			if (delayHint[tag].isTimeUp())
			{
				hintIter[tag]++;
				delayHint[tag].reset();
			}

			if (hintIter[tag] > hintMap.size())
			{
				hintIter[tag] = 0;
				delayHint[tag].reset();
			}
			return hintMap[tag][hintIter[tag]];
		}

	}

	return HintText();
}

void HintText::initScaleWith(const float& fontPixel)
{
	int pos = 0;
	//auto length = text.length() < 50 ? 50 : text.length();
	//bool isChs = false;

	//while (pos < length)
	//{
	//	isChs = (text[pos] > -127 && text[pos] < 0);
	//	pos += isChs ? 3 : 1;

	//	scaleWidth += isChs ? fontPixel : fontPixel;
	//}
	if (text.length() >= 250)
		scaleWidth = 250;
	else if (text.length() >= 125)
		scaleWidth = 200;
	else if (text.length() >= 75)
		scaleWidth = 150;
	else if (text.length() >= 50)
		scaleWidth = 100;
	else if (text.length() >= 40)
		scaleWidth = 80;
	else if (text.length() >= 20)
		scaleWidth = 40;
	else
		scaleWidth = 30;

}

Menu* DialogueText::getChoices()
{
	//û��ѡ���򷵻�nullptr
	if (!hasChoices)
		return nullptr;

	using namespace cocos2d;

	auto choiceArray = Vector<MenuItem*>();
	choiceArray.reserve(5);
	float height = 0;
	for (auto c : choices)
	{
		if (c == "")	continue;

		auto itemChoice = MenuItemLabel::create(Label::createWithSystemFont(c, Resh::getFontName(), font.size), CC_CALLBACK_1(DialogueText::choiceCallback, this));

		itemChoice->setColor(font.color);
		itemChoice->setPosition(20, height);
		itemChoice->setName(StringUtils::format("key%d", choiceArray.size()));
		height += itemChoice->getBoundingBox().size.height;
		choiceArray.pushBack(itemChoice);
	}
	choiceArray.reverse();
	auto menu = Menu::createWithArray(choiceArray);
	menu->setName(getName());
	menu->setPosition(Point::ZERO);
	return menu;
}

void DialogueText::choiceCallback(cocos2d::Ref * pSender)
{
	auto selectedChoice = dynamic_cast<MenuItemLabel*>(pSender);
	log("clicked choice:%s", selectedChoice->getName().c_str());
	LUAH->setValue(Luat_Dialogue, index, Luaf_Key, selectedChoice->getName());
	selectedChoice->getParent()->removeFromParentAndCleanup(true);
	selectedChoice->removeFromParentAndCleanup(true);
}
