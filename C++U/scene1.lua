require("global")

local AchievePosition = Vec2(ScreenWidth/2.9,ScreenHeight/2)
Achievements =
{
  { pos=AchievePosition,text="序幕\n一切都是刚刚开始",},
--stage2
  { pos=AchievePosition,text="棉花糖\n爱上我是你的缘",},
  { pos=AchievePosition,text="你是我的小苹果\n苹果跟随到最后",},
--stage3
  { pos=AchievePosition,text="小苹果\n黄鼠狼给鸡拜年\n没安好心",},
  { pos=AchievePosition,text="迪克的微笑1\n上面不行就下面",},
  { pos=AchievePosition,text="小星星\n一闪一闪亮晶晶\n挂在天上放光明",},
--stage4 ******************
  { pos=AchievePosition,text="小广告\n小广告清理者",},
  { pos=AchievePosition,text="2018\n玩家今年要发",},
  { pos=AchievePosition,text="汪思甜\n狗改不了吃屎\n见屁股就舔",},
--stage5
  { pos=AchievePosition,text="钢蛋跌倒记1\n跌倒没商量",},
--stage6
  { pos=AchievePosition,text="钢蛋跌倒记2\n敞开胸怀，请你过来",},
  { pos=AchievePosition,text="收敛不收手\n该吃吃该喝喝\n啥事别往心里搁",},
--stage7******************
  { pos=AchievePosition,text="三连跪\n连续打错3道题",},
  { pos=AchievePosition,text="别添偶~\n不需要舔我屁股",},
--stage8
  { pos=AchievePosition,text="迪克的微笑2\n翔之地雷",},
--stage9
  { pos=AchievePosition,text="领导者\n不用一枪一炮\n",},
  { pos=AchievePosition,text="推墙\n墙倒众人散",},
--stage10
  { pos=AchievePosition,text="销魂可乐\n喝下销魂可乐\n使您睡得更香",},
--******************
  { pos=AchievePosition,text="耍酷的铁蒺藜\n绊倒你没商量",},
--stage11
  { pos=AchievePosition,text="棉花糖2\n我是天上一片云\n你是地上一摊？",},
  { pos=AchievePosition,text="钢蛋跌倒记3\n心急吃不上热豆腐",},
  { pos=AchievePosition,text="迪克的微笑3\n上面不行就下面",},
--stage12
  { pos=AchievePosition,text="救世主\n拯救所有人",},
  { pos=AchievePosition,text="英雄\n至少拯救一位",},
  --******************
  { pos=AchievePosition,text="坦坦荡荡做人\n光明磊落做事",},
--stage13
  { pos=AchievePosition,text="迷魂阵\n哎呀可算出来了",},
  { pos=AchievePosition,text="迪克的微笑3\n水晶铁蒺藜",},
  { pos=AchievePosition,text="钢蛋跌倒记4\n东边日出西边雨",},
--stage14
  { pos=AchievePosition,text="完美\n你不能阻止我",},
  { pos=AchievePosition,text="小太阳\n受不了迪克的翔",},
--stage15******************
  { pos=AchievePosition,text="史莱姆\n开关",},
  { pos=AchievePosition,text="发疯的史莱姆\n",},

  { pos=AchievePosition,text="合体\n降落",},
--stage16
  { pos=AchievePosition,text="迪克复活\n老王我又活过来了",},
  { pos=AchievePosition,text="好狗不挡道\n狗改不了吃屎",},
  { pos=AchievePosition,text="背后被狗咬\n给我5毛钱\n我就往死里舔",},
  --******************
  { pos=AchievePosition,text="流浪狗\n汪思甜逃离！",},
--stage17
  { pos=AchievePosition,text="十二星\n全部收集完毕",},
--stage18
  { pos=AchievePosition,text="健康100\n收集100个红苹果",},
  { pos=AchievePosition,text="幸福100\n收集100个青苹果",},
  { pos=AchievePosition,text="暴发户\n收集100个金币",},
  { pos=AchievePosition,text="钻石大亨\n收集100个钻石",},

  { pos=AchievePosition,text="全部回答正确\n请接受礼品",},
  { pos=AchievePosition,text="感谢\n观看制作人员\n到最后",},
  { pos=AchievePosition,text="失败者\n失败次数100次",},
  { pos=AchievePosition,text="二百五\n失败次数250次",},
  { pos=AchievePosition,text="迪克五十\n打脸50次",},
  { pos=AchievePosition,text="迪克二百五\n打脸250次",},
}
Chapters =
{
  {text="序幕",count=1},
  {text="踏上征途",count=3},
  {text="水晶碎片1",count=4},
  {text="水晶碎片2",count=3},
  {text="水晶碎片3",count=2}, 
  {text="最后决战",count=3},
  {text="闭幕",count=2},
}

MainMenu =
{
--	{ text= "开始游戏",font={24,"Arial",},pos = Vec2(ScreenWidth/2,ScreenHeight/4.5), name = "startgame", },
--	{ text= "选择关卡",font={24,"Arial",},pos = Vec2(ScreenWidth/2,ScreenHeight/6), name = "selectlevels", },
  --钢蛋
  { file = "control/steelball.png",pos = Vec2(ScreenWidth/2 + 30, ScreenHeight/1.6), name = "steelball", },
  { file = "control/title.png",pos = Vec2(ScreenWidth/2, ScreenHeight/2.8), name = "title", },
  --语言版权
  { file = "control/chinwan.png",pos = Vec2(50, ScreenHeight/7), name = "chinwan", },
  { file = "control/america.png",pos = Vec2(50 * 2.5, ScreenHeight/7), name = "america", },
  { file = "control/copyright.png", font={12,0,false},pos = Vec2(50*2.4 , ScreenHeight/13), name = "language2", },
  --开始游戏
  { file = "control/selectlevels1.png",font={24,"Arial",},pos = Vec2(ScreenWidth/2.4,ScreenHeight/6), name = "selectlevels", },
  { file = "control/startgame1.png",font={24,"Arial",},pos = Vec2(ScreenWidth/1.7,ScreenHeight/6), name = "startgame", },
  --社区
  { file = "control/community.png",pos = Vec2(80,ScreenHeight-50), name = "community" ,url=Urls.Community},
  { file = "control/achieve.png",pos = Vec2(80 * 2.8, ScreenHeight-50), name = "achieve", },
  --交流
  -- { file = "control/donate.png",pos = Vec2(ScreenWidth - 50,ScreenHeight/1.6 + 60), name = "donate", },
  --{ file = "control/facebook.png",pos = Vec2(ScreenWidth - 50,ScreenHeight/1.6), name = "facebook", },
  { file = "control/twitter.png",pos = Vec2(ScreenWidth - 50, ScreenHeight/1.6 - 60 * 2), name = "twitter", url=Urls.Twitter },
  --  { file = "control/share.png",pos = Vec2(ScreenWidth - 50,ScreenHeight/1.6 - 60 * 2), name = "share", },
  --{ file = "control/wechat.png",pos = Vec2(ScreenWidth - 50,ScreenHeight/1.6 - 60 * 2), name = "wechat", },
  { file = "control/info.png",pos = Vec2(ScreenWidth - 50, ScreenHeight/1.6 - 60 * 3), name = "info",  url=Urls.Main},
  { file = "control/bgmusic.png",pos = Vec2(ScreenWidth - 50, ScreenHeight/1.6 - 60 * 4), name = "bgmusic", },
}
LevelMenu=
{
  { file = "control/community.png", pos = Vec2(ScreenWidth-80,      ScreenHeight-50), name = "community",url=Urls.Community},
  { file = "control/achieve.png",   pos = Vec2(ScreenWidth-80 * 2.8, ScreenHeight-50), name = "achieve", },

  -- { file = "control/donate.png", pos = Vec2(120,  ScreenHeight -25 - 58), name = "donate", },
  -- { file = "control/facebook.png",pos = Vec2(120, ScreenHeight -25- 58*2), name = "facebook", },
  { file = "control/twitter.png", pos = Vec2(120, ScreenHeight -25- 58*4), name = "twitter", url=Urls.Twitter },
  -- { file = "control/share.png",   pos = Vec2(120, ScreenHeight -25- 58*4), name = "share", },
--  { file = "control/wechat.png",   pos = Vec2(120, ScreenHeight -25- 58*4), name = "wechat", },
  { file = "control/info.png",    pos = Vec2(120, ScreenHeight -25- 58*5), name = "info", url=Urls.Main},
  { file = "control/bgmusic.png", pos = Vec2(120, ScreenHeight -25- 58*6), name = "bgmusic", },
  { file = "control/goback.png",  pos = Vec2(ScreenWidth - 80,  50), name = "goback", },
}

AchieveMenu =
{

  { text="成就:("..Resh.getAchieveCount(#Achievements).."/"..#Achievements..") 失败最多:"..Resh.getNumber(Constant.User_Trial).." 次",  pos = Vec2(ScreenWidth * 0.3 , ScreenHeight / 1.23), name = "lbAchieve", },

  -- { file="control pos = Vec2(ScreenWidth * 0.5 , ScreenHeight / 1.23), name = "lbAchieve", },
  --{ file = "control/donate.png",  pos = Vec2(ScreenWidth / 1.35 - 58*6, ScreenHeight / 6), name = "donate", },
  -- { file = "control/facebook.png",  pos = Vec2(ScreenWidth / 1.35 - 58*5, ScreenHeight / 6), name = "facebook", },
  { file = "control/reset.png",  pos = Vec2(ScreenWidth / 1.35 - 58*4, ScreenHeight / 6), name = "reset", },
  { file = "control/twitter.png",   pos = Vec2(ScreenWidth / 1.35 - 58*3, ScreenHeight / 6), name = "twitter",  url=Urls.Twitter },
  -- { file = "control/share.png",     pos = Vec2(ScreenWidth / 1.35 - 58*3, ScreenHeight / 6), name = "share", },
  -- { file = "control/wechat.png",     pos = Vec2(ScreenWidth / 1.35 - 58*3, ScreenHeight / 6), name = "wechat", },
  { file = "control/info.png",      pos = Vec2(ScreenWidth / 1.35 - 58*2, ScreenHeight / 6), name = "info",  url=Urls.Main},
  { file = "control/bgmusic.png",   pos = Vec2(ScreenWidth / 1.35 - 58,   ScreenHeight / 6), name = "bgmusic", },
  --返回
  { file = "control/goback.png",    pos = Vec2(ScreenWidth - 80,  50), name = "goback", },
}
