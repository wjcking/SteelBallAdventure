#include "ResourceHelper.h"
#include "SimpleAudioEngine.h"
#include "Constant.h"
#include "GameScriptor.h"
using namespace CocosDenshion;

   const std::string  Resh::Kingsure = "http://www.easyfound.com.cn/";
std::string Resh::musicName = "";
Sprite* Resh::getFrameSprite(const std::string& imageName, const int& index)
{
	char name[5];
	sprintf(name, "%d", index);
	return	Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(imageName + name));
}

void Resh::playMusic(const std::string&name)
{
	if (!UserDefault::getInstance()->getBoolForKey(User_AllowMusic, true))
	{
		log("[Resh:playMusic]background music has been shut down");
		return;
	}
	if (name == musicName)
		return;
	musicName = name;
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.55f);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(name.c_str(), true);
}

void Resh::setMusicVolume(const float & val)
{
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(val);
}

void Resh::playSound(const char * name)
{
	//if (!allowSound)
	//{
	//	log("[Resh:playSound]sound effect has been shut down");
	//	return;
	//}
	SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	SimpleAudioEngine::getInstance()->playEffect(name);
}

int Resh::getAchieveCount(const unsigned short& length)
{
	unsigned short count = 0;
	for (auto i = 1; i <= length; i++)
	{
		if (getAchieve(i))
			count++;
	}
	return count;
}

int Resh::getNumber(const char* key)
{
	return UserDefault::getInstance()->getIntegerForKey(key, 0);
}
void Resh::addNumber(const char* key, const int& value)
{
	auto current = UserDefault::getInstance()->getIntegerForKey(key, 0);
	current += value;
	UserDefault::getInstance()->setIntegerForKey(key, current);
	UserDefault::getInstance()->flush();
}
void Resh::setNumber(const char* key, const int & value)
{
	UserDefault::getInstance()->setIntegerForKey(key, value);
	UserDefault::getInstance()->flush();
}

bool Resh::getAchieve(const unsigned short & id)
{
	//默认为false
	return UserDefault::getInstance()->getBoolForKey(StringUtils::format("%s%d", User_Achieve, id).c_str(), false);
}

void Resh::setAchieve(const unsigned short & id, const bool& value)
{
	UserDefault::getInstance()->setBoolForKey(StringUtils::format("%s%d", User_Achieve, id).c_str(), value);
	UserDefault::getInstance()->flush();
}

void Resh::release()
{
	musicName = "";
	SimpleAudioEngine::getInstance()->stopAllEffects();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

std::string Resh::getFontName()
{
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		return  "Microsoft Sans Serif";
	else if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		return "DroidSansFallback";
	else if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		return "STHeitiSC-Light";
	return "";
}

short Resh::getLanguage()
{
	return UserDefault::getInstance()->getIntegerForKey(User_Language, 1);
}

bool Resh::openURL(const std::string & params)
{
	if (params == "")
		return Application::getInstance()->openURL(Kingsure);

	  return Application::getInstance()->openURL(Kingsure+"request/?params="+params);
}

void Resh::reset(const unsigned short& length)
{
	for (auto i = 1; i <= length; i++)
		setAchieve(i, false);

	UserDefault::getInstance()->setIntegerForKey(User_Trial,0);
	log("[Resh::reset]reset all of achieves, trial times");
}



//unsigned short Resh::getStarCount()
//{
//	unsigned short starCount = 0
//	for (unsigned short i = 1; i <= 12; i++)
//	{
//if  (Resh.getNumber(Keys.stars[i])) > 0
//starCount = starCount + 1
//	}
//	return starCount;
//}
void Resh::preloadFrame(const std::string& filename, const Rect& rect)
{
	auto texture = Director::getInstance()->getTextureCache()->addImage(filename);

	int index = 0;
	char name[5];
	for (int y = 0; y < rect.origin.y; y++)
	{
		for (int x = 0; x < rect.origin.x; x++)
		{
			sprintf(name, "%d", index++);
			auto frame = SpriteFrame::createWithTexture(texture, Rect(x * rect.size.width, y * rect.size.height, rect.size.width, rect.size.height));
			SpriteFrameCache::getInstance()->addSpriteFrame(frame, filename + name);

		}
	}
}

void Resh::resetAchieve()
{
	auto achievements = LUAH->getGlobal(Luat_Achievements);
	assert(achievements.isTable() && "请设置achievements");
	for (auto a = 1; a < achievements.len(); a++)
		setAchieve(a);

}