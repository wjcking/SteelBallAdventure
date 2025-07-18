#include "AppDelegate.h" 
#include "Scene/MainScene.h"
#include "Utils/Constant.h"
#include "Utils/GameScriptor.h"
#include "Ads/Ads.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{


}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
	AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::end();
#endif
	FirebaseExtend::release();
	LUAH->closeLuaState();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{

	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::createWithRect("Steelball's adventure", Rect(0, 0, ScreenWidth, ScreenHeight));
		director->setOpenGLView(glview);
	}

	director->getOpenGLView()->setDesignResolutionSize(ScreenWidth, ScreenHeight, ResolutionPolicy::SHOW_ALL);

	// turn on display FPS
	//director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	//director->setAnimationInterval(1.0 / 60);
	LUAH->openLuaState();
	LUAH->registerClasses();
	LUAH->loadPackages();
	LUAH->doScene();

    Ads::init();

	auto scene = MainScene::createScene();
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
#if USE_AUDIO_ENGINE
	AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
