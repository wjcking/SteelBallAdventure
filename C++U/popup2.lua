require("global")
PopupText = { }
PopupText.life = "Life:"
PopupText.tryAgain = "Try again"
PopupText.grumble =
{
  
"Jacky：\n Get over here!!Dick",
"Jacky：\n Prey for yourself,\n this time",
"Dick:\nPlease Stop ！",  
"Dick:\nI surrender please!!",  
"Dick:\nI beg you, \ni'll do what you said.", 
}
PopupText.isDoneInfo = nil
PopupText.getDoneInfo =function() 

  if PopupText.isDoneInfo ~= nil then
    return  PopupText.isDoneInfo
  end
  local info ={
    {text="I'm done~，",font=Font.default} ,
    {text="Holy,mother~",font=Font.default} ,
    {text="Unbelievable，finished~",font=Font.default} ,
    {text= "~ (*+﹏+*)~",font=Font.default} ,
    {text="‘(*>﹏<*)′ ",font=Font.default} ,
    {text="Ouch,ooooo~~~",font=Font.default} ,
  }  
  if math.random(2) == 1 then
    PopupText.isDoneInfo = info[math.random(#info)]
  else
    PopupText.isDoneInfo = {}
  end

  return PopupText.isDoneInfo 
end

local retrySize  =  { width=310,height=357}

 
--对话框信息，注意多语言
local  PopupInfo = { }
PopupInfo.createRetry = function()
  --广告弹出频率
  Ads.popup(Checkpoint.life % 4 ==0)
  Checkpoint.updateTrial()
  local isDick = Checkpoint.life == 0

  local buttonName = "retry"
  --local visible = true
  local buttonText = PopupText.tryAgain
  local titles=
  {
    "You're done","You're dead","Failed","~ (*+﹏+*)~","‘(*>﹏<*)′ "
  }
  local contents=
  {
    "Sorry for the ads~",
    "A burden of one's choice is\n not felt. ",
    "A true hero will never quit\n for his rage",
    "A fall into a pit, a gain \nin your wit",
    "Every door may be shut,\n but death's door.",
    "Failure is the mother of success.",
    "The fox knew too much, \nthat's how he lost his tail.",
    "Quitters never win and \nwinners never quit.",
  }
  if (isDick) then
    titles=
    {
      "Use golden bat","Don't slap me!","Don't hit me!",
    }
    contents=
    {
      "Dick:Is this how you got?",
      "Dick:How about? chick ball?",
      "Dick:You are done! boy",
      "Dick:Lol...,Fancy moment !\n ",
    }
    --buttonText="XXXXX"
    -- name="retry"
    --visible = false
  end
  local function getFrame()
    if   isDick then
      return "dick1.png"
    elseif Checkpoint.life < 9-2-2  then
      return "fail3.png"
    elseif Checkpoint.life < 9-2 then
      return "fail2.png"
    elseif Checkpoint.life < 10  then
      return "fail1.png"
    end
  end
  local function getAnimation()
    if  isDick  then
      return Animate.jumpLaugh
    else
      return nil
    end
  end
  return  
  { 
    isCovered = true,
    autoClose = -1,
    name="retry",
    background =  {  file=Folder.popup.."retry.png", width = 500,height=500, sound=Folder.sound.."gameover1.wav"},
    items =
    {  
      { text= titles[math.random(#titles)], pos = Vec2(retrySize.width/2,retrySize.height - 20), name = "lbTitle",  font=Font.default},
      { text= PopupText.life..Checkpoint.life, pos = Vec2(retrySize.width/2,retrySize.height - 65), name = "lbLife",  font=Font.default},
      {  type=1, pos = Vec2(retrySize.width/2,retrySize.height/2),  name = "face",  frame=getFrame(), animate=getAnimation()},
      { text= contents[math.random(#contents)], pos =  Vec2(retrySize.width   * 0.5,70), name = "lbComment",  font=Font.retry},

      { text= buttonText,font=Font.default, file="control/popuplayer/button.png",pos =  Vec2(retrySize.width/2 ,10), name = buttonName,  },
    }
  }
end
PopupInfo.recover = function()

  return  
  { 
    isCovered = true,
    autoClose = -1,
    name="retry",
    background =  {  file=Folder.popup.."retry.png", width = 500,height=500, sound=Folder.sound.."gameover1.wav"},
    items =
    {  
      { text= "You're dead", pos = Vec2(retrySize.width/2,retrySize.height - 20), name = "lbTitle",  font=Font.default},
      { text= "Life-250", pos = Vec2(retrySize.width/2,retrySize.height - 65), name = "lbLife",  font=Font.default},
      {  type=1, pos = Vec2(retrySize.width/2,retrySize.height/2),  name = "face",  frame="fail1.png",  },
      { text=  "A true hero will never quit\n for his rage", pos =  Vec2(retrySize.width   * 0.5,70), name = "lbComment",  font=Font.retry},

      { text= "Recover",font=Font.default, file="control/popuplayer/button.png",pos =  Vec2(retrySize.width/2 ,10), name = "recover", },
    }
  }
end
--过关
PopupInfo.passed = function()
   Ads.popup(true)
  local titles=
  {
    "Congratulations"," O(∩_∩)O","└(^o^)┘","＝^ω^＝"
  }
  local contents=
  {
    "Great hopes make great man.",
    "God helps those who help themselves",
    "Where these is will, \nthere is a way",
    "Until all is over one's ambition\n never dies. ",
    "Try try try, until you die! ",
  }
  local trialText =  "Victory"

  if (Checkpoint.trial > 0 ) then
    trialText =  "Loser = "..Checkpoint.trial
  end
  return  
  { 
    isCovered = true,
    autoClose = -1,
    name="passed",
    background =  {  file=Folder.popup.."retry.png", width = 500,height=500, sound=Folder.sound.."win.wav"},
    items =
    {  
      { text= titles[math.random(#titles)], pos = Vec2(retrySize.width/2,retrySize.height - 20), name = "lbTitle",  font=Font.medium},
      { text= trialText, pos = Vec2(retrySize.width/2,retrySize.height - 65), name = "lbLife",  font={18,Color.red,false},},
      {  type=1, pos = Vec2(retrySize.width/2,retrySize.height/2),  name = "face", frame="win.png", },
      { text= contents[math.random(#contents)], pos =  Vec2(retrySize.width * 0.5,70), name = "lbComment",  font=Font.retry},

      { text= "Continue",font=Font.default, file="control/popuplayer/button.png",pos =  Vec2(retrySize.width/2 ,10), name = "continue", visible=true},
    }
  }
end
local achieveSize  =  { width=303,height=99}
--
PopupInfo.achieve = function(index)
  local achi = Achievements[index]
  return {
    index = index,
    name="achieve",
    autoClose = 5,
    zorder=10000,
    background =  {  file=Folder.popup.."bgachieve.png",  sound=Folder.sound.."achieved.wav",pos=Vec2(ScreenWidth/2,ScreenHeight/1.2)},
    items =
    {
      { type=1,font=Font.default,pos =  Vec2(achieveSize.width/5 , achieveSize.height/2), name = "image", file=Folder.achieve.."aimg_s.png" }, 
      { text= achi.text,font=Font.defaultWhite,pos =  Vec2(achieveSize.width/1.8 , achieveSize.height/2), name = "lbText",  font=Font.white16}, 
    }
  }
end
local objectSize  =  { width=322,height=222}
PopupInfo.createObject = function (image, desc)
  return  
  { 
    name="object",
    isCovered = true,
    autoClose = -1,
    background =  { file=Folder.popup.."objectacquired.png",  sound=Sound.objectAcquired },
    items =
    {   
      { type=1,font=Font.default,pos =  Vec2(objectSize.width/2 , objectSize.height/2), name = "image", file=image},
      { text=desc,font=Font.default,pos =  Vec2(objectSize.width/2 , objectSize.height/5), name = "lbText", },  
      { text= "Got it",font=Font.default, file=Folder.popup.."button.png",pos =  Vec2(objectSize.width/2 ,objectSize.height), name = "cancel",  },
    }
  }

end
local extendSize = { width=512, height=288}

PopupInfo.createExtend = function()

  return  
  { 
    isCovered = true,
    autoClose = -1,
    name="extend",
    background =  {  file=Folder.popup.."extend.png", width = extendSize.width,height=extendSize.height, sound=Sound.menuItem},
    items =
    {   
      { text= "Donate",font=Font.default, file=Folder.popup.."button.png", pos =  Vec2(extendSize.width * 0.2 ,extendSize.height), name = "btnDonate",  },
   
      { text= "Watch Ads",font=Font.default, file=Folder.popup.."button.png", pos =  Vec2(extendSize.width *0.8 ,50), name = "btnAds",  },
      {  type=1, pos = Vec2(extendSize.width/4.4,extendSize.height * 0.5),  name = "adFace", frame="fail1.png", font=Font.default, },
      { text= "\"Thanks you very much\"", font=Font.large,pos =  Vec2(extendSize.width/1.5,extendSize.height * 0.5),name="lbAds", },
    
      { text= "Cancel",font=Font.default, file=Folder.popup.."button.png", pos =  Vec2(extendSize.width/2 ,10), name = "btnCancel",  },
    }
  }
end
WalkThrough =
{
  { text=[[
[Walk Through]
Area1:
Push up that AD board and step on the green 
button
Area2:
Choose "Little hand",drag down that AD board 
then pick up the baseballbat,Slap dick!
[Achievement]
Prologue Everything is just beginning
]], font=Font.default,},

  { text=[[
[Walk Through]
Ignore the all of the shaking apples, Never jump on the hidden
bricks or you will be killed by the cloud. go straight ahead.
[Achievement]
Marshmallow A sticky friend
Little apple I am following you
Little star Twinkle, twinkle little star
]],font=Font.default,},

  { text=[[
[Walk Through]
Area1:
1) Jump "ON" the last green apple of the apple tree, 
it will fall on the ground
2) Then step on it for fetching the RED apple;
3) Push the green apple to the pipe where you started, 
then use it jump on the pipe then drop 
the RED apple into the pipe.
4) You need to push back the green apple 
for grabbing the bomb, 
when you done it drop the bomb on the bridge.
Area2:
1)knock the SECOND brick from the BOTTOM,
knock the SECOUND from TOP
2)knock the FIRST brick from the BOTTOM, 
knock the FRIST from TOP
3)Never get through from bottom!!, passing through 
the bricks from the TOP!!!!
[Achievement]
Little apple2 I dont' need this
Dick's smile1 Got you!!!
]],font=Font.default,},

  { text=[[
Area1:
1)Tap 3 ad papers on AD board 
2) Jump on the cowboy who said "move along,kid!" 
and then jump again on the AD board for 
dodging the rushing jeep.
3) Go back for touching the Q on the rooftop
Area2:
1)Tap the traffic light.
2)Jump on the green bus then hop on the rooftop, 
knock the NUMBER bricks with the code 2018
3)Push the gift NEAR the bridge then open it.
[Achievement]
Ads Ads cleaner
2018 Players will be rich
Poo licker What a hot dish
]],font=Font.default,},
 
 { text=[[
[Walk Through] 
Area1:
Blue Red Green, then 2 of bricks can be pushed.
one is for pressingthe top left button, 
the other one for the top right one.
Area2:
After the red button stepped on, 
there is one brick where is a white 
pad nearby you can push.then go up and trigger the spike, 
then turn  back
go around into next area.
Area3:
The the advice board, then move back 
into a wall can get through.
[Achievement]
Fallen1 Nothing to talk about it
]],font=Font.default,},

 {text=[[
[Walk Through] 
Area1:
move along at the right side of the blue slime, 
after touched Q then  go around 
at the left side of slime,touch next Q.
Area2: 
combine the middle of Q with the right one 
with your fingers.
Area3:Don't eat the 2 diamonds at the first array, 
don't eat the 2 coins BEFORE diamonds. Eat only 2 diamonds 
at the first column of the top arrary. then eat the rest.
[Achievement]
Fallen2 Get closer,baby!
Enough is never enough

]],font=Font.default,},

{ text=[[
[Walk Through] 
Keys:ADACC, Never answer the last one!, 
then open the 3rd gift
touch the helmet, then knock the 3 bricks
under that dog, then open the rest of the 2 gifts. 
[Achievement]
Wrong answers incorrect 3 times
Ouch Don't kiss my ass
Congratulations Here're the gift
]],font=Font.default,},

{ text=[[
[Walk Through] 
Group 3:watch the balloons,they will be borken 
after you step on.
Group 4:Shoot the slimes FIRST, then shoot the balloons.
Group 5:Touch the balloons with your fingers.
Group 6:Combine the left Q with the right Q,
then eat it.
The last boss: Never face it, It will shoot at you.
[Achievement]
Dick's smile2 Good eat!
]],font=Font.default,},

{ text=[[
[Walk Through] 
After the wall being pushed 70%, 
shoot one bird popped up in the sky.
Eat the twitter icon then shoot the wall.
[Achievement]
Commander Without using guns
Break down The wall collapsed
]],font=Font.default,},

{ text=[[
[Walk Through]
Area1:
use spikes kill the bug, then push it near 
the flying birdthen use the two for jumping on the pipe 
to get the key. 
A missile will be launched, move on it then jump over 
next area.
Area2: 
Jump in then entice the slime,
it will break the brick and drink
the Steelcola, then you can drag the board ahead.
Jump on move from the top.
Area3: 
the SECOND pipe can be drag with your fingers, 
use it for blocking
slimes.
[Achievement]
Steel cola Sleeping soundly
Cool~~ Nice sun-glasses
]],font=Font.default,},

{ text=[[
[Walk Through]
Area2:
Never fetch the crystal in front of it,
get through the bricks
jump on with the red spring,fetch it from behind.
[Achievement]
Marshmallow2 Nice to touch you again
Fallen3 Curiosity kills the cat
Dick's smile3
]],font=Font.default,},

{ text=[[
[Walk Through]
Area1:
You can ether answer all of Questions or
NOT to answer, jump over the NPCs,
the you can go through the wall under Devil Dick.
Area2:
Touch the board with your fingers
it will fall,  then push it back
to the left side area, then use it for jumping
on the hidden wall, shoot dick
from the REAR.
[Achievement]
Savior Save all of people
Hero Rescue at least one in distress
Be open and boardview
]],font=Font.default,},

{ text=[[
[Walk Through]
Area1:
use the brick for pressing the green button, 
then stepped on thered button, at the end step on 
the blue button while the spike is steping on.
Area2:
Go BACK, enther the left spiral, 
go DIRECTLY towards the spikes and slime SLOWLY
for getting the key.
Area3:
Go into the left spiral AGAIN. 
then follow the path but be CAREFUL 
at the end of the path. 
Once you get the key, A secret place will open
at the bottom of stage.
[Achievement]
Finally Out of the trap
Dick's smile4 Crystal caltrop
Fallen4 Smiling caltrop
]],font=Font.default,},

{ text=[[
[Walk Through]
Notice: the red slimes.
Group 1：Don not jump when near them.
Group 2：Jump beforehead
Group 3：Jump over them,then jump AGAIN.
Group 4：Use the last two spike kill dick,
they will jump while you are jumping.
[Achievement]
Perfect!!! You cannot stop me
The sun sets 
]],font=Font.default,},

{ text=[[
[Walk Through]
Area1:
NEVER touch the GREEN switch before the red one. 
a brick can be pushed at right side, 
use it press the red switch, then touch the
green one. then jump over the pit.
Area2:
use the board for touching the right switch, 
then you and slime can touch the rest two.
Area3:
Eat the left Q, shoot the right one, then push
the bomb at the center of platform, then shoot the slime
let it light up the bomb.
Final: Use a brick where a star nearbyfor block the door.
[Achievement]
Switch off A helping hand
Crazy slime! 
Comination Fall together
]],font=Font.default,},

{ text=[[
[Walk Through]
Shoot dick util 40% life point left, then there will
be 2 missle be launched,use them jump on the hidden place.
The entrance at top of the stage where you can find different
bricks.
[Achievement]
Recovered Dick is back again
Go to hell The leopard can't change his spots
The villain Great barkers are no biters!
The dog ran away
]],font=Font.default,},

{ text=[[
[Walk Through]
Hidden stars：
Crystal shards 1 Stage 1：
There is a hidden place at area 3.
Crystal shards 2 Stage 2：
After shooting all of slimes, go back to area 1
Crystal shards 3 Stage 4：
At the top of the toolbar,use the rising board.

Crystal shards 4 Stage1：
The hidden place where you shoot dick form its back,
JUMP for fetch it.
Crystal shards 4 Stage2：
At the bottom of stage, a hidden place with 4 different
bricks on the way of getting crystal.
[Achievement]
12 stars I am going to EU
]],font=Font.default,},

{ text=[[
[Walk Through]
After a cloud getting close to Jacky, then tap it.
[Achievement]
Health+100 Colleted 100 green apples
Happiness+100 Collected 100 red apples
Richer Collected 100 coins
Monopoly Collected 100 diamonds
Final Thank you for playing
[Others]
Loser failed 50 times
Loser250 failed 250 times
Dick Slapping50 hit dick 50 times
Dick Slapping250 hit dick 250 times
]],font=Font.default,},
}--end
return PopupInfo
