 #ifndef PROJ_ANDROID_ADS_H
#define PROJ_ANDROID_ADS_H

#include "FirebaseHelper.h"
#include "TGSDKCocos2dxHelper.h"
#include "../Utils/GameScriptor.h"
class Ads
{
public:
    inline static void init()
    {
        FirebaseExtend::init();
  //      yomob::TGSDKCocos2dxHelper::setDebugModel(true);
        yomob::TGSDKCocos2dxHelper::initialize("6360yx00Wst7Xy2jIiP0");
        yomob::TGSDKCocos2dxHelper::preload();
    }

    inline static void showBanner(const LuaRef& bannerInfo)
    {
        if (!bannerInfo.isValid())
            return;
        if (bannerInfo.has(Luaf_IsActive))
            return;
          std::string type = "TGBannerNormal";
        if (bannerInfo.has(Luaf_Type))
            type = bannerInfo.get(Luaf_Type).toValue<std::string>();
            if (yomob::TGSDKCocos2dxHelper::couldShowAd(Yomob_Banner))
            {
                yomob::TGSDKCocos2dxHelper::showAd(Yomob_Banner);
               yomob::TGSDKCocos2dxHelper::setBannerConfig( Yomob_Banner,
                                                             type,
                                                             bannerInfo.get(Luaf_X,0),
                                                             bannerInfo.get(Luaf_Y,0),
                                                             bannerInfo.get(Luaf_Width,320),
                                                             bannerInfo.get(Luaf_Height,50),
                                                             bannerInfo.get(Luaf_Interval,30));
            }

    };
    inline static void hideBanner()
    {
        yomob::TGSDKCocos2dxHelper::closeBanner(Yomob_Banner);
        FirebaseExtend::hideBanner();
    };
    inline static void updateInterstitial()
    {
        FirebaseExtend::updateInterstitial();
    }
    inline static void updateBanner()
    {
        FirebaseExtend::updateBanner();
    }
     inline   static void showInterstitial(const bool& flag = true)
     {
         FirebaseExtend::showInterstitial(true);

              if (flag && yomob::TGSDKCocos2dxHelper::couldShowAd(Yomob_Interstitial))
              {
                  yomob::TGSDKCocos2dxHelper::showAd(Yomob_Interstitial);
              }
     };
    inline   static void showVideo(const bool& flag = true)
    {
        if (flag &&yomob::TGSDKCocos2dxHelper::couldShowAd(Yomob_Video))
        {
            yomob::TGSDKCocos2dxHelper::showAd(Yomob_Video);
        }
    };
    inline   static void showReward(const bool& flag= true)
    {
            if (flag &&yomob::TGSDKCocos2dxHelper::couldShowAd(Yomob_Reward))
            {
                yomob::TGSDKCocos2dxHelper::showAd(Yomob_Reward);
            }
    };
};


#endif //PROJ_ANDROID_ADS_H
