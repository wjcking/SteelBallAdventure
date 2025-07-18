#include "FirebaseHelper.h"
#include "cocos2d.h"
#include "../Utils/GameScriptor.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

#include "firebase/app.h"
#include "firebase/admob.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma comment(lib, "firebase_app.lib")
#pragma comment(lib, "firebase_admob.lib")
#endif


using namespace firebase::admob;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
using namespace google_play_services;
#endif
firebase::admob::AdRequest FirebaseExtend::my_ad_request = {};
InterstitialAd* FirebaseExtend::interstitial_ad = nullptr;
BannerView* FirebaseExtend::banner_view = nullptr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
Availability FirebaseExtend::availability = Availability::kAvailabilityAvailable;
#endif

bool FirebaseExtend::isInterstitialReady= false;
bool FirebaseExtend::isBannerReady= false;

AdParent FirebaseExtend::getAdParent()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// Returns the iOS RootViewController's main view (i.e. the EAGLView).
	return (id)cocos2d::Director::getInstance()->getOpenGLView()->getEAGLView();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// Returns the Android Activity.
	return cocos2d::JniHelper::getActivity();
#else
	// A void* for any other environments.
	return 0;
#endif
}

void FirebaseExtend::init() {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    availability = CheckAvailability(cocos2d::JniHelper::getEnv(),   cocos2d::JniHelper::getActivity());
    if (Availability::kAvailabilityAvailable != availability)
    {
        CCLOG("[FirebaseExtend:: checkGoogle]:%d", availability);
        return;
    }
	// Initialize Firebase for Android.
	firebase::App* app = firebase::App::Create( 	firebase::AppOptions(), cocos2d::JniHelper::getEnv(), cocos2d::JniHelper::getActivity());
	// Initialize AdMob.
	Initialize(*app, ADMob);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// Initialize Firebase for iOS.
	firebase::App* app = firebase::App::Create(firebase::AppOptions());
	// Initialize AdMob.
	Initialize(*app, ADMob);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Initialize();
#endif

	//初始化
	interstitial_ad = new  InterstitialAd();
	interstitial_ad->Initialize(static_cast<AdParent>(getAdParent()), ADMob_Interstitial);
	interstitial_ad->InitializeLastResult().OnCompletion(OnCompletionCallback, interstitial_ad);
	//初始化banner
	banner_view = new  BannerView();
	firebase::admob::AdSize ad_size;
	ad_size.ad_size_type = firebase::admob::kAdSizeStandard;
	auto banner = LUAH->getGlobal("Banner");

	ad_size.width = 	banner.get(Luaf_Width,320);
	ad_size.height = 	banner.get(Luaf_Height,50);
	banner_view->Initialize(getAdParent(), ADMob_Banner, ad_size);
}
void FirebaseExtend::updateBanner(const  short& pos,const  int& x, const int& y)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (availability != Availability::kAvailabilityAvailable)
    {
        hideBanner();
        return;
    }
#endif
//	if (banner_view->InitializeLastResult().status() == firebase::kFutureStatusComplete &&
//		banner_view->InitializeLastResult().error() == firebase::admob::kAdMobErrorNone)
//	{
//
//        banner_view->Show();
//		if (banner_view->ShowLastResult().status() == 	firebase::kFutureStatusComplete &&
//			banner_view->ShowLastResult().error() == firebase::admob::kAdMobErrorNone&&
//                banner_view->LoadAdLastResult().status() == firebase::kFutureStatusInvalid)
//		{
//			banner_view->LoadAd(my_ad_request);
//            if (pos > -1)
//                banner_view->MoveTo(static_cast<firebase::admob::BannerView::Position>(pos));
//            else
//                banner_view->MoveTo( x, y);
//		}
//	}
// Check that the banner has been initialized.
    if (banner_view->InitializeLastResult().status() == firebase::kFutureStatusComplete) {
        // Check that the banner hasn't started loading.
        if (banner_view->LoadAdLastResult().status() == firebase::kFutureStatusInvalid) {
            // Make the banner visible and load an ad.
            banner_view->Show();
            banner_view->LoadAd(my_ad_request);


        if (pos > -1)
            banner_view->MoveTo(static_cast<firebase::admob::BannerView::Position>(pos));
        else
            banner_view->MoveTo( x, y);
            isBannerReady=true;
        }
    //    CCLOG("[FirebaseExtend::update]InitLast:%d,LoadLast:%d", banner_view->InitializeLastResult().status(), banner_view->LoadAdLastResult().status());
    }
		//	CCLOG("[FirebaseExtend::update]InitLast:%d,LoadLast:%d", banner_view->InitializeLastResult().status(), banner_view->LoadAdLastResult().status());

          //  auto screen = cocos2d::Director::getInstance()->getVisibleSize();
}

void FirebaseExtend::updateInterstitial()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if ( availability != Availability::kAvailabilityAvailable )
        return;
#endif
// Once the interstitial_ad::Intitialize() future has completed successfully,
    // enable the interstitial ad buttons.
    if (interstitial_ad->InitializeLastResult().status() == firebase::kFutureStatusComplete &&
        interstitial_ad->InitializeLastResult().error() == firebase::admob::kAdMobErrorNone)
    {
        // If interstitial_ad::LoadAd() method has not been called yet, enable the
        // load interstitial ad button.
        if (interstitial_ad->LoadAdLastResult().status() == firebase::kFutureStatusInvalid)
            interstitial_ad->LoadAdLastResult().OnCompletion(OnCompletionCallback, interstitial_ad);

        // Once the interstitial_ad::Show() future has completed and the interstitial
        // ad has been displayed and dismissed by the user, clean up the existing
        // interstitial ad object and create a new one. Note: interstitial_ad is a
        // single-use object that can load and show a single AdMob interstitial ad.
        if (interstitial_ad->ShowLastResult().status() == firebase::kFutureStatusComplete &&
            interstitial_ad->presentation_state() == InterstitialAd::kPresentationStateHidden) {
            // Invalidate all Futures and enable loadInterstitialAdBtn.

            delete interstitial_ad;
            interstitial_ad = new  InterstitialAd();

            interstitial_ad->Initialize(static_cast<AdParent>(getAdParent()), ADMob_Interstitial);
            interstitial_ad->InitializeLastResult().OnCompletion(OnCompletionCallback, interstitial_ad);
        }
        // If the interstitial_ad::LoadAd() future completed but there was an error,
        // then clean up the existing interstitial ad object and create a new one.
        if (interstitial_ad->InitializeLastResult().status() == firebase::kFutureStatusComplete &&
            interstitial_ad->LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
            interstitial_ad->LoadAdLastResult().error() != firebase::admob::kAdMobErrorNone) {

            delete interstitial_ad;
            interstitial_ad = new InterstitialAd();
            interstitial_ad->Initialize(static_cast<AdParent>(getAdParent()), ADMob_Interstitial);
            interstitial_ad->InitializeLastResult().OnCompletion(OnCompletionCallback, interstitial_ad);
        }
    }
}
// The OnCompletion callback function.
void FirebaseExtend::OnCompletionCallback(const firebase::Future<void>& future, void* user_data)
{
	// Called when the Future is completed for the last call to the InterstitialAd::Initialize()
	// method. If the error code is kAdMobErrorNone, then you're ready to
	// load the interstitial ad.
	if (future.error() == kAdMobErrorNone)
	{
		interstitial_ad->LoadAd(my_ad_request);
        isInterstitialReady = true;
		// Once the interstitial_ad::LoadAd() future has completed successfully,
		// enable the show interstitial ad button.
		//if (interstitial_ad->LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
		//	interstitial_ad->LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone &&
		//	!interstitialAdShown) {
		//	//showInterstitialAdBtn->setEnabled(true);
		//}
	}

	//CCLOG("[FirebaseExtend::OnCompletionCallback] future:%d", future.error());
}

void FirebaseExtend::showInterstitial(const bool& flag)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (!flag ||  availability != Availability::kAvailabilityAvailable )
        return;
#endif
	//必须添加满足initialize否则报错,隐藏界面才能显示否则不重复显示
	if ( nullptr != interstitial_ad&&
            isInterstitialReady&&
            interstitial_ad->InitializeLastResult().status() == firebase::kFutureStatusComplete&&
		  interstitial_ad->LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
//			interstitial_ad->presentation_state() == InterstitialAd::kPresentationStateHidden&&
		  interstitial_ad->LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone)
	{
		interstitial_ad->Show();
        isInterstitialReady = false;
	}

	//	CCLOG("!!NO!![FirebaseExtend::showInterstitial]status:%d error:%d", interstitial_ad->LoadAdLastResult().status(), interstitial_ad->LoadAdLastResult().error());
}

void FirebaseExtend::release()
{
	delete interstitial_ad;
	delete banner_view;
}
