require("global")
local AchievePosition = Vec2(ScreenWidth/2.9,ScreenHeight/2)
Achievements =
{
  { pos=AchievePosition,text="Prologue\nEverything is\n just beginning",},
--stage2
  { pos=AchievePosition,text="Marshmallow\nA sticky friend",},
  { pos=AchievePosition,text="Little apple\nI am\n following you",},
--stage3
  { pos=AchievePosition,text="Little apple2\nI dont' need\n this",},
  { pos=AchievePosition,text="Dick's smile1\nGot you!!!",},
  { pos=AchievePosition,text="Little star\n Twinkle, twinkle\nlittle star",},
--stage4 ******************
  { pos=AchievePosition,text="Ads\nAds cleaner",},
  { pos=AchievePosition,text="2018\nPlayers will\nbe rich",},
  { pos=AchievePosition,text="Poo licker\n What a \nhot dish",},
--stage5
  { pos=AchievePosition,text="Fallen1\nNothing to\ntalk about it",},
--stage6
  { pos=AchievePosition,text="Fallen2\nGet closer,baby!",},
  { pos=AchievePosition,text="Enough is \nnever enough",},
--stage7******************
  { pos=AchievePosition,text="Wrong answers\n incorrect 3 times",},
  { pos=AchievePosition,text="Ouch\nDon't kiss\nmy ass",},
--stage8
  { pos=AchievePosition,text="Dick's smile2\nGood eat!",},
--stage9
  { pos=AchievePosition,text="Commander\n Without using\nguns",},
  { pos=AchievePosition,text="Break down\nThe wall collapsed",},
--stage10
  { pos=AchievePosition,text="Steel cola\nSleeping soundly",},
--******************
  { pos=AchievePosition,text="Cool~~\nNice sun-glasses",},
--stage11
  { pos=AchievePosition,text="Marshmallow2\nNice to\ntouch you again",},
  { pos=AchievePosition,text="Fallen3\n Curiosity kills\n the cat",},
  { pos=AchievePosition,text="Dick's smile3\n ",},
--stage12
  { pos=AchievePosition,text="Savior\nSave all of\n people",},
  { pos=AchievePosition,text="Hero\nRescue at least one\nin distress",},
  --******************
  { pos=AchievePosition,text="Be open\n and boardview",},
--stage13
  { pos=AchievePosition,text="Finally\n Out of the trap",},
  { pos=AchievePosition,text="Dick's smile4\nCrystal caltrop",},
  { pos=AchievePosition,text="Fallen4\nSmiling caltrop",},
--stage14
  { pos=AchievePosition,text="Perfect!!!\nYou cannot\nstop me",},
  { pos=AchievePosition,text="The sun sets\n",},
--stage15******************
  { pos=AchievePosition,text="Switch off\nA helping hand",},
  { pos=AchievePosition,text="Crazy slime!\n",},

  { pos=AchievePosition,text="Comination\nFall together",},
--stage16
  { pos=AchievePosition,text="Recovered\nDick is back\nagain",},
  { pos=AchievePosition,text="Go to hell\nThe leopard can't\n change his spots",},
  { pos=AchievePosition,text="The villain\n Great barkers \nare no biters!",},
  --******************
  { pos=AchievePosition,text="The dog\n ran away",},
--stage17
  { pos=AchievePosition,text="12 stars\n I am going to\n EU",},
--stage18
  { pos=AchievePosition,text="Health+100\nColleted 100 \n green apples",},
  { pos=AchievePosition,text="Happiness+100\nCollected 100\n red apples",},
  { pos=AchievePosition,text="Richer\nCollected 100\n coins",},
  { pos=AchievePosition,text="Monopoly\nCollected 100\n diamonds",},
  
  { pos=AchievePosition,text="Congratulations\n Here're the gift",},
  { pos=AchievePosition,text="Final\nThank you \nfor playing",},
  { pos=AchievePosition,text="Loser\n failed 100 times",},
  { pos=AchievePosition,text="Loser250\n failed 250 times",},
  { pos=AchievePosition,text="Dick Slapping50\nhit dick 50 times",},
  { pos=AchievePosition,text="Dick Slapping250\nhit dick 250 times",},
}

Chapters =
{
  {text="Prologue",count=1},
  {text="A journey awaits",count=3},
  {text="Crystal shard 1",count=4},
  {text="Crystal shard 2",count=3},
  {text="Crystal shard 3",count=2}, 
  {text="Final battle",count=3},
  {text="Epilogue",count=2},
}

MainMenu =
{
  --钢蛋
  { file = "control/steelball.png",pos = Vec2(ScreenWidth/2 + 30, ScreenHeight/1.6), name = "steelball", },
  { file = "control/title2.png",pos = Vec2(ScreenWidth/2, ScreenHeight/2.8), name = "title", },
  --语言版权
  { file = "control/chinwan.png",pos = Vec2(50, ScreenHeight/7), name = "chinwan", },
  { file = "control/america.png",pos = Vec2(50 * 2.5, ScreenHeight/7), name = "america", },
  { file = "control/copyright.png", font={12,0,false},pos = Vec2(50*2.4 , ScreenHeight/13), name = "language2", },
  --开始游戏
  { file = "control/selectlevels2.png",   font={24,"Arial",},pos = Vec2(ScreenWidth/2.4,ScreenHeight/6), name = "selectlevels", },
  { file = "control/startgame2.png",font={24,"Arial",},pos = Vec2(ScreenWidth/1.6,ScreenHeight/6), name = "startgame", },
  --推特 
  { file = "control/community2.png",pos = Vec2(80,ScreenHeight-50), name = "community", url=Urls.Community},
  { file = "control/achieve2.png",pos = Vec2(80 * 2.8, ScreenHeight-50), name = "achieve", },
  --交流
  --{ file = "control/donate.png",pos = Vec2(ScreenWidth - 50,ScreenHeight/1.6 + 60), name = "donate", },
  --{ file = "control/facebook.png",pos = Vec2(ScreenWidth - 50,ScreenHeight/1.6), name = "facebook", },
  { file = "control/twitter.png",pos = Vec2(ScreenWidth - 50, ScreenHeight/1.6 - 60*2), name = "twitter",  url=Urls.Twitter,},
--  { file = "control/share.png",pos = Vec2(ScreenWidth - 50,ScreenHeight/1.6 - 60 * 2), name = "share", },
  { file = "control/info.png",pos = Vec2(ScreenWidth - 50, ScreenHeight/1.6 - 60 * 3), name = "info",  url=Urls.Main},
  { file = "control/bgmusic.png",pos = Vec2(ScreenWidth - 50, ScreenHeight/1.6 - 60 * 4), name = "bgmusic", },
}

LevelMenu=
{
  { file = "control/community2.png", pos = Vec2(ScreenWidth-80,      ScreenHeight-50), name = "community",url=Urls.Community },
  { file = "control/achieve2.png",   pos = Vec2(ScreenWidth-80 * 2.8, ScreenHeight-50), name = "achieve", },


 -- { file = "control/donate.png", pos = Vec2(120,  ScreenHeight -25 - 58), name = "donate", },
 -- { file = "control/facebook.png",pos = Vec2(120, ScreenHeight -25- 58*2), name = "facebook", },
  { file = "control/twitter.png", pos = Vec2(120, ScreenHeight -25- 58*4), name = "twitter",  url=Urls.Twitter, },
--  { file = "control/share.png",   pos = Vec2(120, ScreenHeight -25- 58*4), name = "share", },
  { file = "control/info.png",    pos = Vec2(120, ScreenHeight -25- 58*5), name = "info",  url=Urls.Main},
  { file = "control/bgmusic.png", pos = Vec2(120, ScreenHeight -25- 58*6), name = "bgmusic", },
  { file = "control/goback.png",  pos = Vec2(ScreenWidth - 80,  50), name = "goback", },
}
AchieveMenu =
{
  { text="Achieved:("..Resh.getAchieveCount(#Achievements).."/"..#Achievements..") Failed:"..Resh.getNumber("Trial").." times",  pos = Vec2(ScreenWidth * 0.35 , ScreenHeight / 1.23), name = "lbAchieve", },
  { file = "control/reset.png",  pos = Vec2(ScreenWidth / 1.35 - 58*4, ScreenHeight / 6), name = "reset", },
--  { file = "control/donate.png",  pos = Vec2(ScreenWidth / 1.35 - 58*6, ScreenHeight / 6), name = "donate", },
 -- { file = "control/facebook.png",  pos = Vec2(ScreenWidth / 1.35 - 58*5, ScreenHeight / 6), name = "facebook", },
  { file = "control/twitter.png",   pos = Vec2(ScreenWidth / 1.35 - 58*3, ScreenHeight / 6), name = "twitter", url=Urls.Twitter, },
--  { file = "control/share.png",     pos = Vec2(ScreenWidth / 1.35 - 58*3, ScreenHeight / 6), name = "share", },
   { file = "control/info.png",      pos = Vec2(ScreenWidth / 1.35 - 58*2, ScreenHeight / 6), name = "info", url=Urls.Main,},
  { file = "control/bgmusic.png",   pos = Vec2(ScreenWidth / 1.35 - 58,   ScreenHeight / 6), name = "bgmusic", },
  --返回
  { file = "control/goback.png",    pos = Vec2(ScreenWidth - 80,  50), name = "goback", },
}
