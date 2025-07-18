#ifndef  AtlasLoader_H
#define AtlasLoader_H

#include "cocos2d.h"
using namespace cocos2d;

static const char Texture_Debris[50] = "texture/debris.png";

class Resh
{
private:
	static std::string musicName;
public:
	Resh() {};
	//读取后放到变量里面而不是每次都IO读取User xml

	static const std::string Kingsure;
	static void preloadFrame(const std::string& filename, const Rect&);
	static void resetAchieve();
	inline static unsigned short getPlatform() { return CC_TARGET_PLATFORM; }
	static Sprite* getFrameSprite(const std::string& imageName, const int& index);
	static void playMusic(const std::string& name);
	static void setMusicVolume(const float& val);
	static void playSound(const char* name);
	static int getAchieveCount( const unsigned short& length);
	static int getNumber(const char* key);
	static void setNumber(const char* key, const int& value);
	static void addNumber(const char* key, const int& value);
	static bool getAchieve(const unsigned short&);
	static void setAchieve(const unsigned short&,const bool& =false );
	static void release();
	static std::string getFontName();
	static short getLanguage();
	static int getRand(const int& min, const int& max) { return random(min, max); }
	 static bool openURL(const std::string &url);
	//  static unsigned short getStarCount();
	static void reset(const unsigned short& length);
	static inline  Size getVisibleSize() { return cocos2d::Director::getInstance()->getVisibleSize(); }
	
 
};

#endif 
