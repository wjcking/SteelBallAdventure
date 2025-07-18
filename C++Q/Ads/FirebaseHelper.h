#ifndef __FIREBASE_HELPER_H__
#define __FIREBASE_HELPER_H__
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "google_play_services/availability.h"
#endif
#include "firebase/admob/types.h"
#include "firebase/future.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
extern "C" {
#include <objc/objc.h>
}
#endif

#include "firebase/admob/interstitial_ad.h"
#include "firebase/admob/banner_view.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static const char ADMob[100] = "ca-app-pub-4826349760185960~9961520626";
static const char ADMob_Interstitial[100] = "ca-app-pub-4826349760185960/1017468740";
static const char ADMob_Banner[100] = "ca-app-pub-4826349760185960/3424140231";
//static const char ADMob_Reward[100] = "ca-app-pub-4826349760185960/3160825471";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
static const char ADMob[100] = "ca-app-pub-4826349760185960~6042654003";
static const char ADMob_Interstitial[100] = "ca-app-pub-4826349760185960/6397877223";
static const char ADMob_Banner[100] = "ca-app-pub-4826349760185960/5659510622";
//static const char ADMob_Reward[100] = "ca-app-pub-4826349760185960/8449325496";
#else
static const char ADMob[1] = "";
static const char ADMob_Interstitial[1] = "";
static const char ADMob_Banner[100] = "";
#endif
class FirebaseExtend
{
private:
	static firebase::admob::AdRequest my_ad_request;
	static firebase::admob::InterstitialAd* interstitial_ad;
	static firebase::admob::BannerView* banner_view;
    static bool  isInterstitialReady;
	static bool  isBannerReady;
	static void OnCompletionCallback(const firebase::Future<void>& future, void* user_data);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	static   google_play_services::Availability availability;
#endif
public:
	// Returns a variable that describes the ad parent for the app. On Android
// this will be a JObject pointing to the Activity. On iOS, it's an ID pointing
// to the root view of the view controller.
	static firebase::admob::AdParent getAdParent();
	static void init();
	static void updateBanner(const  short& pos = -1, const  int& =0, const int& =0);
    static void updateInterstitial();
    inline static bool isAvailable() { return availability;}
	inline static bool checkInterstitial()
	{
		if (availability == google_play_services::kAvailabilityAvailable )
			return true;

		return false;
	}
	inline static bool checkBanner()
	{
		if (availability == google_play_services::kAvailabilityAvailable )
			return true;

		return false;
	}
	inline static void showBanner()
	{
		if (isBannerReady )
		    banner_view->Show();
	};

	inline static void hideBanner()
	{
        if (nullptr != banner_view )
            banner_view->Hide();
	};
	static void showInterstitial(const bool& = true);
	static void release();
};
#endif // __FIREBASE_HELPER_H__
